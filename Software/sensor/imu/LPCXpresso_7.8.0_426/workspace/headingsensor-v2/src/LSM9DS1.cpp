/******************************************************************************
 LSM9DS1.cpp
 Mike Miles
 Original Creation Date: June 29, 2015

 This file implements all functions of the LSM9DS1 class. Functions here range
 from higher level stuff, like reading/writing LSM9DS1 registers to low-level,
 hardware reads and writes. I2C handler functions can be found
 towards the bottom of this file.

 Development environment specifics:
 IDE: LPCXpresso v7.8.0
 Hardware Platform: NXP LPC11C24

 Distributed as-is; no warranty is given.
 ******************************************************************************/

#include "LSM9DS1.h"

LSM9DS1::LSM9DS1(unsigned char agAddr, unsigned char mAddr) {
	// mAddress and agAddress will store the 7-bit I2C address
	mAddress = mAddr;
	agAddress = agAddr;
}

unsigned short LSM9DS1::begin(gyro_scale gScl, accel_scale aScl, mag_scale mScl,
		gyro_odr gODR, accel_odr aODR, mag_odr mODR, mag_xy_om mXYOM,
		mag_z_om mZOM) {

	// Store the given scales in class variables. These scale variables
	// are used throughout to calculate the actual g's, DPS,and Gs's.
	gScale = gScl;
	aScale = aScl;
	mScale = mScl;

	// Once we have the scale values, we can calculate the resolution
	// of each sensor. That's what these functions are for. One for each sensor
	calcgRes(); // Calculate DPS / ADC tick, stored in gRes variable
	calcmRes(); // Calculate Gs / ADC tick, stored in mRes variable
	calcaRes(); // Calculate g / ADC tick, stored in aRes variable

	// Now, initialize our hardware interface.
	//initI2C();          // Initialize I2C

	// To verify communication, we can read from the WHO_AM_I register of
	// each device. Store those in a variable so we can return them.
	unsigned char agTest = agReadByte(WHO_AM_I_AG); // Read the accel/gyro WHO_AM_I - should be 0x68
	unsigned char mTest = mReadByte(WHO_AM_I_M); // Read the mag WHO_AM_I - should be 0x3D

	// Gyro initialization stuff:
	initAccelGyro(); // This will "turn on" the gyro. Setting up interrupts, etc.
	setGyroODR(gODR); // Set the gyro output data rate and bandwidth.
	setGyroScale(gScale); // Set the gyro range

	// Accelerometer initialization stuff:
//  initAccel(); // "Turn on" all axes of the accel. Set up interrupts, etc.
	setAccelODR(aODR); // Set the accel data rate.
	setAccelScale(aScale); // Set the accel range.

	// Magnetometer initialization stuff:
	initMag(); // "Turn on" all axes of the mag. Set up interrupts, etc.
	setMagODR(mODR); // Set the magnetometer output data rate.
	setMagScale(mScale); // Set the magnetometer's range.
	setMagOM(mXYOM, mZOM); // Set the magnetometer's operating modes.

	// Once everything is initialized, return the WHO_AM_I registers we read:
	return (mTest << 8) | agTest;
}

void LSM9DS1::update() {

}

// Initialize sensors
void LSM9DS1::initAccelGyro() {
	/* CTRL_REG1_G sets output data rate, full-scale selection, and bandwidth
	 Bits[7:0]: ODR_G2 ODR_G1 ODR_G0 FS_G1 FS_G0 0 BW_G1 BW_G0
	 ODR[2:0] - Output data rate selection
	 *See Table 46 in datasheet
	 FS[1:0] - Full-scale selection
	 00: 245 dps, 01: 500 dps, 10: N/A, 11: 2000 dps
	 BW[1:0] - Bandwidth selection (sets cutoff frequency)
	 *See Table 47 in datasheet. Depends on ODR. */
	agWriteByte(CTRL_REG1_G, 0B01100000); // ODR=119Hz, Scale=245dps, BW=14Hz

	/* CTRL_REG2_G sets up INT/OUT config
	 Bits[7:0]: 0 0 0 0 INT_SEL1 INT_SEL0 OUT_SEL1 OUT_SEL0
	 INT_SEL[1:0]: INT selection config
	 OUT_SEL[1:0]: OUT selection config */
	agWriteByte(CTRL_REG2_G, 0x00); // Default mode

	/* CTRL_REG3_G sets up low power mode and HPF
	 Bits[7:0]: LP_mode HP_EN 0 0 HPCF3_G HPCF2_G HPCF1_G HPCF0_G
	 LP_mode - low-power mode enable
	 0: LPM disabled, 1: LPM enabled.
	 HP_EN - high-pass filter enable
	 0: HPF disable, 1: HPF enabled.
	 HPCF[3:0] - HPF cutoff frequency selection
	 *See Table 52 in datasheet. */
	agWriteByte(CTRL_REG3_G, 0B01000000); // LPM=disabled, HPF=enabled, HPFC=8Hz (ODR=119Hz)

	/* ORIENT_CFG_G sets up sensor sign and orientation
	 Bits[7:0]: 0 0 SignX_G SignY_G SignZ_G Orient_2 Orient_1 Orient_0
	 SignX_G - Pitch axis angular rate sign; 0: positive sign, 1: negative sign
	 SignY_G - Roll axis angular rate sign; 0: positive sign, 1: negative sign
	 SignZ_G - Yaw axis angular rate sign; 0: positive sign, 1: negative sign
	 Orient[2:0] - Directional user orientation selection */
	agWriteByte(ORIENT_CFG_G, 0x00); // all axis signs positive

	/* CTRL_REG4 sets up gyro axis output enables and accel latched interrupts
	 Bits[7:0] - 0 0 Zen_G Yen_G Xen_G 0 LIR_XL1 4D_XL1
	 Zen_G - Yaw axis output enable; 0: output disabled, 1: output enabled
	 Yen_G - Roll axis output enable; 0: output disabled, 1: output enabled
	 Xen_G - Pitch axis output enable; 0: output disabled, 1: output enabled
	 LIR_XL1 - Latched interrupt; 0: interrupt request not latched, 1: interupt request latched
	 4D_XL1 - 4D opetion enabled on Interrupt
	 0: interrupt generator uses 6D for position recognition; 1: interrupt generator uses 4D for position recognition. */
	agWriteByte(CTRL_REG4, 0B00111000); // all gyro axes enabled, interrupt not latched, interrupt generator uses 6D

	// Temporary !!! For testing !!! Remove !!! Or make useful !!!
	//configGyroInt(0x2A, 0, 0, 0, 0); // Trigger interrupt when above 0 DPS...

	/* CTRL_REG5_XL sets up decimation of accel data and accel axes output enables
	 Bits[7:0] - DEC_1 DEC_0 Zen_XL Yen_XL Xen_XL 0 0 0
	 DEC[1:0] - decimation of accel data on OUT REG and FIFO
	 Zen_XL - Accel Z-axis output enable; 0: output disabled, 1: output enabled
	 Yen_XL - Accel Y-axis output enable; 0: output disabled, 1: output enabled
	 Xen_XL - Accel X-axis output enable; 0: output disabled, 1: output enabled */
	agWriteByte(CTRL_REG5_XL, 0B00111000); // no decimation, all Z axes enabled

	/* CTRL_REG6_XL sets up output data rate, power mode selection, accel full-scale selection, and BW selection
	 Bits[7:0] - ODR_XL2 ORD_XL1 ODR_XL0 FS1_XL FS0_XL BW_SCAL_ODR BW_XL1 BW_XL0
	 ODR_XL[2:0] - output data rate and power mode selection
	 *See Table 68 in datasheet
	 FS[1:0] - accel full-scale selection; 00: 2g, 01: 16g, 10: 4g, 11: 8g
	 BW_SCAL_ODR - BW selection; 0: BW determined by ODR selection (See Table 67), 1: BW selected by BW_XL[1:0]
	 BW_XL[1:0] - anti-aliasing filter BW selection; 00: 408Hz, 01: 211Hz, 10: 105Hz, 11: 50Hz */
	agWriteByte(CTRL_REG6_XL, 0B01000000); // ODR=50Hz, FS=2g, BW=50Hz (ODR=119Hz)

	/* CTRL_REG7_XL sets up high res mode, digital filter, HPF enable
	 Bits[7:0] - HR DCF1 DCF0 0 0 FDS 0 HPIS1
	 HR - high resolution mode; 0: disabled, 1: enabled
	 DCF[1:0] - accel digital filter cutoff frequency selection
	 *See Table 71 in datasheet
	 FDS - filtered data selection
	 HPIS1 - HPF enabled for accel sensor interrupt funciton on Interrupt */
	agWriteByte(CTRL_REG7_XL, 0x00); // default

	/* CTRL_REG8
	 Bits[7:0] - BOOT BDU H_LACTIVE PP_OD SIM IF_ADD_INC BLE SW_RESET
	 BOOT - Reboot memory content (0: normal, 1: reboot)
	 BDU - block data update for accel AND gyro
	 0: Continuous update
	 1: Output registers aren't updated until MSB and LSB have been read
	 H_LACTIVE - interrupt activation level
	 0: interrupt output pins active high
	 1: interrupt output pins active low
	 PP_OD - Push-pull/open-drain selection on INT1_A/G and INT2_A/G (0=push-pull, 1=open-drain)
	 SIM - SPI mode selection
	 0=4-wire, 1=3-wire
	 IF_ADD_INC - register address auto incremented during a multiple byte access with a serial interface
	 0: disabled, 1: enabled
	 BLE - Big/Little Endian data selection
	 0: data LSB @ lower address, 1: data MSB @ lower address
	 SW_RESET - software reset (cleared by hardware after next flash boot) */
	agWriteByte(CTRL_REG8, 0B0000100); //normal boot content, continuous data update, interrupt active high,
									   //push-pull int, 4-wire SPI, IF_ADD_INC enabled, data LSB @ lower address

	/* CTRL_REG9
	 Bits[7:0] - 0 SLEEP_G 0 FIFO_TEMP_EN DRDY_mask_bit I2C_DISABLE FIFO_EN STOP_ON_FTH
	 SLEEP_G - gyro sleep mode enable; 0: disabled, 1: enabled
	 FIFO_TEMP_EN - temp data storage in FIFO enable; 0: data not stored in FIFO, 1: data stored in FIFO
	 DRDY_mask_bit - data available enable bit; 0: DA timer disabled, 1: DA timer enabled
	 I2C_DISABLE - disable I2C interface; 0: both I2C and SPI enabled, 1: I2C disabled, SPI only
	 FIFO_EN - FIFO memory enable; 0: disabled, 1: enabled
	 STOP_ON_FTH - enable FIFO threshold level use; 0: FIFO depth not limited, 1: FIFO depth limited to threshold level */
	agWriteByte(CTRL_REG9, 0x00); // gyro sleep mode disabled, temp data not stored in FIFO, DA timer disabled, I2C and SPI enabled

	/* CTRL_REG10 sets up self-tests
	 Bits[7:0] - 0 0 0 0 0 ST_G 0 ST_XL
	 ST_G - gyro sensor self-test enable; 0: disabled, 1: enabled
	 ST_XL - accel sensor self-test enable; 0: disabled, 1: enabled */
	agWriteByte(CTRL_REG10, 0x00); // self-tests disabled
}
void LSM9DS1::initMag() {
	/* CTRL_REG1_M sets up temp compensation, X/Y axes operative mode, output data rate, self-test
	 Bits[7:0] - TEMP_COMP OM1 OM0 DO2 DO1 DO0 0 ST
	 TEMP_COMP - temperature compensation enable; 0: disabled, 1: enabled
	 OM[1:0] - X and Y axes operative mode selection
	 00: low-power
	 01: medium-performance
	 10: high-performance
	 11: ultra-high performance
	 DO[2:0] - output data rate selection
	 *See Table 111 in datasheet
	 ST - self test enable; 0: disabled, 1: enabled */
	mWriteByte(CTRL_REG1_M, 0B00110000); // no temp comp, medium performance, 10Hz ODR, no self test

	/* CTRL_REG2_M sets up full-scale selection, reboot, sw reset
	 Bits[7:0] - 0 FS1 FS0 0 REBOOT SOFT_RST 0 0
	 FS[1:0] - full-scale selection; 00: 4g, 01: 8g, 10: 12g, 11: 16g
	 REBOOT - reboot memory content; 0: normal mode, 1: reboot memory content
	 SOFT_RST - config regs and user reg reset function; 0: default value, 1: reset operation */
	mWriteByte(CTRL_REG2_M, 0B00000000);

	/* CTRL_REG3_M sets up I2C disable, low power mode, SPI mode select, operating mode
	 Bits[7:0] - I2C_DISABLE 0 LP 0 0 SIM MD1 MD0
	 I2C_DISABLE - disable I2C interface; 0: I2C enable, 1: I2C disable
	 LP - low-power mode config
	 If this bit is ‘1’, the DO[2:0] is set to 0.625 Hz and the system performs, for each
	 channel, the minimum number of averages. Once the bit is set to ‘0’, the magnetic
	 data rate is configured by the DO bits in the CTRL_REG1_M (20h) register.
	 SIM - SPI Serial Interface mode selection
	 0: SPI only write operations enabled
	 1: SPI read and write operations enabled
	 MD[1:0] - operating mode selection
	 00: Continuous-conversion mode
	 01: Single-conversion mode
	 10/11: Power-down mode */
	mWriteByte(CTRL_REG3_M, 0B00000000); // I2C enable, LP disable, SPI r/w operations enable, cont.-conv. mode (default)

	/* CTRL_REG4_M sets up Z axis operative mode, Endian data selection
	 Bits[7:0] - 0 0 0 0 OMZ1 OMZ0 BLE 0
	 OMZ[1:0] - Z-axis operative mode selection
	 00: low-power
	 01: medium-performance
	 10: high-performance
	 11: ultra-high performance
	 BLE - Big/Little Endian data selection
	 0: data LSB @ lower address, 1: data MSB @ lower address */
	mWriteByte(CTRL_REG4_M, 0B00000000); // low-power mode, data LSB @ lower address

	/* CTRL_REG5_M sets up block data for magnetic data
	 Bits[7:0] - 0 BDU 0 0 0 0 0 0
	 BDU - block data update for magnetic data
	 0: continuous update
	 1: output regs not updated until MSB adn LSB have been read */
	mWriteByte(CTRL_REG5_M, 0B00000000); // continuous update
}

/* Calibrate sensors
 This is a function that uses the FIFO to accumulate sample of accelerometer and gyro data, average
 them, scales them to  gs and deg/s, respectively, and then passes the biases to the main sketch
 for subtraction from all subsequent data. There are no gyro and accelerometer bias registers to store
 the data as there are in the ADXL345, a precursor to the LSM9DS1, or the MPU-9150, so we have to
 subtract the biases ourselves. This results in a more accurate measurement in general and can
 remove errors due to imprecise or varying initial placement. Calibration of sensor data in this manner
 is good practice. */
//void LSM9DS1::agCal(float * gbias, float * abias) {
//  unsigned char data[6] = {0, 0, 0, 0, 0, 0};
//  signed short gyro_bias[3] = {0, 0, 0}, accel_bias[3] = {0, 0, 0};
//  int samples, ii;
//  
//  // First get accel/gyro biases
//  byte c = agReadByte(CTRL_REG9);
//  agWriteByte(CTRL_REG9, c | 0x02);         // Enable gyro FIFO  
//  delay(20);                                 // Wait for change to take effect
//  agWriteByte(FIFO_CTRL, 0x20 | 0x1F);  // Enable gyro FIFO stream mode and set watermark at 32 samples
//  delay(1000);  // delay 1000 milliseconds to collect FIFO samples
//  
//  // do gyro stuff
//  samples = (agReadByte(FIFO_SRC) & 0x3F); // Read number of stored samples
//
//  for(ii = 0; ii < samples ; ii++) {            // Read the gyro data stored in the FIFO
//    agReadBytes(OUT_X_L_G,  &data[0], 6);
//    gyro_bias[0] += (((signed short)data[1] << 8) | data[0]);
//    gyro_bias[1] += (((signed short)data[3] << 8) | data[2]);
//    gyro_bias[2] += (((signed short)data[5] << 8) | data[4]);
//  }  
//
//  gyro_bias[0] /= samples; // average the data
//  gyro_bias[1] /= samples; 
//  gyro_bias[2] /= samples; 
//  
//  gbias[0] = (float)gyro_bias[0]*gRes;  // Properly scale the data to get deg/s
//  gbias[1] = (float)gyro_bias[1]*gRes;
//  gbias[2] = (float)gyro_bias[2]*gRes;
//  
//  // do accel stuff
//  samples = (agReadByte(FIFO_SRC) & 0x3F); // Read number of stored accelerometer samples
//  Serial.println(samples);
//
//   for(ii = 0; ii < samples ; ii++) {          // Read the accelerometer data stored in the FIFO
//    agReadBytes(OUT_X_L_XL, &data[0], 6);
//    accel_bias[0] += (((signed short)data[1] << 8) | data[0]);
//    accel_bias[1] += (((signed short)data[3] << 8) | data[2]);
//    accel_bias[2] += (((signed short)data[5] << 8) | data[4]) - (signed short)(1./aRes); // Assumes sensor facing up!
//  }  
//
//  accel_bias[0] /= samples; // average the data
//  accel_bias[1] /= samples; 
//  accel_bias[2] /= samples; 
//  
//  abias[0] = (float)accel_bias[0]*aRes; // Properly scale data to get gs
//  abias[1] = (float)accel_bias[1]*aRes;
//  abias[2] = (float)accel_bias[2]*aRes;
//
//  // disable FIFO and re-enable bypass mode
//  c = agReadByte(CTRL_REG9);
//  agWriteByte(CTRL_REG9, c & ~0x02);  // Disable gyro FIFO  
//  delay(20);
//  agWriteByte(FIFO_CTRL, 0x00);   // Enable gyro bypass mode
//}
//void LSM9DS1::mCal(float * mbias) {
//  unsigned char data[6] = {0, 0, 0, 0, 0, 0};
//  signed short mag_bias[3] = {0, 0, 0};
//  int samples = 32, ii;
//  
//  // First get mag biases
//  //byte c = mReadByte(CTRL_REG9);
//  //mWriteByte(CTRL_REG9, c | 0x02);         // Enable mag FIFO  
//  //delay(20);                                 // Wait for change to take effect
//  //mWriteByte(FIFO_CTRL, 0x20 | 0x1F);  // Enable mag FIFO stream mode and set watermark at 32 samples
//  //delay(1000);  // delay 1000 milliseconds to collect FIFO samples
//  
//  // do mag stuff
//  for(ii = 0; ii < samples ; ii++) {            // Read the mag data stored in the FIFO
//    mReadBytes(OUT_X_L_M,  &data[0], 6);
//    mag_bias[0] += (((signed short)data[1] << 8) | data[0]);
//    mag_bias[1] += (((signed short)data[3] << 8) | data[2]);
//    mag_bias[2] += (((signed short)data[5] << 8) | data[4]);
//  }  
//
//  mag_bias[0] /= samples; // average the data
//  mag_bias[1] /= samples; 
//  mag_bias[2] /= samples; 
//  
//  mbias[0] = (float)mag_bias[0]*mRes;  // Properly scale the data to get deg/s
//  mbias[1] = (float)mag_bias[1]*mRes;
//  mbias[2] = (float)mag_bias[2]*mRes;
//
//  // disable FIFO and re-enable bypass mode
//  //c = mReadByte(CTRL_REG9);
//  //mWriteByte(CTRL_REG9, c & ~0x02);  // Disable mag FIFO  
//  //delay(20);
//  //mWriteByte(FIFO_CTRL, 0x00);   // Enable mag bypass mode
//}
// Read sensor regs
void LSM9DS1::readAccel() {
	unsigned char temp[6]; // We'll read six bytes from the accelerometer into temp
	agReadBytes(OUT_X_L_XL, temp, 6); // Read 6 bytes, beginning at OUT_X_L_A
	ax = (temp[1] << 8) | temp[0]; // Store x-axis values into ax
	ay = (temp[3] << 8) | temp[2]; // Store y-axis values into ay
	az = (temp[5] << 8) | temp[4]; // Store z-axis values into az
}
void LSM9DS1::readMag() {
	unsigned char temp[6]; // We'll read six bytes from the mag into temp
	mReadBytes(OUT_X_L_M, temp, 6); // Read 6 bytes, beginning at OUT_X_L_M
	mx = (temp[1] << 8) | temp[0]; // Store x-axis values into mx
	my = (temp[3] << 8) | temp[2]; // Store y-axis values into my
	mz = (temp[5] << 8) | temp[4]; // Store z-axis values into mz
}
void LSM9DS1::readTemp() {
	unsigned char temp[2]; // We'll read two bytes from the temperature sensor into temp
	agReadBytes(OUT_TEMP_L, temp, 2); // Read 2 bytes, beginning at OUT_TEMP_L_M
	temperature = (((signed short) temp[1] << 12) | temp[0] << 4) >> 4; // Temperature is a 12-bit signed integer
}
void LSM9DS1::readGyro() {
	unsigned char temp[6]; // We'll read six bytes from the gyro into temp
	agReadBytes(OUT_X_L_G, temp, 6); // Read 6 bytes, beginning at OUT_X_L_G
	gx = (temp[1] << 8) | temp[0]; // Store x-axis values into gx
	gy = (temp[3] << 8) | temp[2]; // Store y-axis values into gy
	gz = (temp[5] << 8) | temp[4]; // Store z-axis values into gz
}

// Calculate sensor values
float LSM9DS1::convGyro(signed short gyro) {
	// Return the gyro raw reading times our pre-calculated DPS / (ADC tick):
	return gRes * gyro;
}
float LSM9DS1::convAccel(signed short accel) {
	// Return the accel raw reading times our pre-calculated g's / (ADC tick):
	return aRes * accel;
}
float LSM9DS1::convMag(signed short mag) {
	// Return the mag raw reading times our pre-calculated Gs / (ADC tick):
	return mRes * mag;
}

// Set sensor scales
void LSM9DS1::setGyroScale(gyro_scale gScl) {
	// We need to preserve the other bytes in CTRL_REG4_G. So, first read it:
	unsigned char temp = agReadByte(CTRL_REG1_G);
	// Then mask out the gyro scale bits:
	temp &= 0xFF ^ (0x3 << 3);
	// Then shift in our new scale bits:
	temp |= gScl << 3;
	// And write the new register value back into CTRL_REG4_G:
	agWriteByte(CTRL_REG1_G, temp);

	// We've updated the sensor, but we also need to update our class variables
	// First update gScale:
	gScale = gScl;
	// Then calculate a new gRes, which relies on gScale being set correctly:
	calcgRes();
}
void LSM9DS1::setAccelScale(accel_scale aScl) {
	// We need to preserve the other bytes in CTRL_REG2_XM. So, first read it:
	unsigned char temp = agReadByte(CTRL_REG6_XL);
	// Then mask out the accel scale bits:
	temp &= 0xFF ^ (0x3 << 3);
	// Then shift in our new scale bits:
	temp |= aScl << 3;
	// And write the new register value back into CTRL_REG2_XM:
	agWriteByte(CTRL_REG6_XL, temp);

	// We've updated the sensor, but we also need to update our class variables
	// First update aScale:
	aScale = aScl;
	// Then calculate a new aRes, which relies on aScale being set correctly:
	calcaRes();
}
void LSM9DS1::setMagScale(mag_scale mScl) {
	// We need to preserve the other bytes in CTRL_REG6_XM. So, first read it:
	unsigned char temp = mReadByte(CTRL_REG2_M);
	// Then mask out the mag scale bits:
	temp &= 0xFF ^ (0x3 << 5);
	// Then shift in our new scale bits:
	temp |= mScl << 5;
	// And write the new register value back into CTRL_REG6_XM:
	mWriteByte(CTRL_REG2_M, temp);

	// We've updated the sensor, but we also need to update our class variables
	// First update mScale:
	mScale = mScl;
	// Then calculate a new mRes, which relies on mScale being set correctly:
	calcmRes();
}

// Set sensor ODRs
void LSM9DS1::setGyroODR(gyro_odr gRate) {
	// We need to preserve the other bytes in CTRL_REG1_G. So, first read it:
	unsigned char temp = agReadByte(CTRL_REG1_G);
	// Then mask out the gyro ODR bits (ODR and BW):
	temp &= 0B00011100;
	// Then put in our new ODR bits:
	temp |= gRate;
	// And write the new register value back into CTRL_REG1_G:
	agWriteByte(CTRL_REG1_G, temp);
}
void LSM9DS1::setAccelODR(accel_odr aRate) { // Only used when in accel-only mode, o/w ODR determined by setGyroODR
	// We need to preserve the other bytes in CTRL_REG1_G. So, first read it:
	unsigned char temp = agReadByte(CTRL_REG6_XL);
	// Then mask out the accel ODR bits (ODR and BW):
	temp &= 0B00011111;
	// Then put in our new ODR bits:
	temp |= (aRate << 5);
	// And write the new register value back into CTRL_REG1_G:
	agWriteByte(CTRL_REG6_XL, temp);
}
void LSM9DS1::setAccelABW(accel_abw abwRate) {
	// We need to preserve the other bytes in CTRL_REG1_G. So, first read it:
	unsigned char temp = agReadByte(CTRL_REG6_XL);
	// Then mask out the gyro ODR bits (ODR and BW):
	temp &= 0B11111100;
	// Then put in our new ODR bits:
	temp |= abwRate;
	// And write the new register value back into CTRL_REG1_G:
	agWriteByte(CTRL_REG6_XL, temp);
}
void LSM9DS1::setMagODR(mag_odr mRate) {
	// We need to preserve the other bytes in CTRL_REG1_G. So, first read it:
	unsigned char temp = mReadByte(CTRL_REG1_M);
	// Then mask out the gyro ODR bits (ODR and BW):
	temp &= 0B11100011;
	// Then put in our new ODR bits:
	temp |= (mRate << 2);
	// And write the new register value back into CTRL_REG1_G:
	mWriteByte(CTRL_REG1_M, temp);
}

// Set magnetometer operating modes
void LSM9DS1::setMagOM(mag_xy_om mXYOM, mag_z_om mZOM) {
	unsigned char temp;
	// Set XY operating mode
	temp = mReadByte(CTRL_REG1_M);
	temp &= 0B10011111;
	temp |= (mXYOM << 5);
	mWriteByte(CTRL_REG1_M, temp);
	// Set Z operating mode
	temp = mReadByte(CTRL_REG4_M);
	temp &= 0B11110011;
	temp |= (mZOM << 2);
	mWriteByte(CTRL_REG4_M, temp);
}

// Load LSM9DS1 object with sensor values
void LSM9DS1::calcGyro() {
	float deadzone_dps = 3.0;
	signed short deadzone_raw = deadzone_dps / gRes; // gyro offset dep on full scale range - make this dep. upon deadzone_dps or % of raw reading?

	// To read from the gyroscope, you must first call the
	// readGyro() function. When this exits, it'll update the
	// gx, gy, and gz variables with the most current data.
	readGyro();

#ifdef PRINT_CALCULATED
	// If you want to print calculated values, you can use the
	// convGyro helper function to convert a raw ADC value to
	// DPS. Give the function the value that you want to convert.
	/*gx = convGyro(gx);
	 gy = convGyro(gy);
	 gz = convGyro(gz);*/
	if (gx > -deadzone_raw && gx < deadzone_raw)
		gx = 0;
	if (gy > -deadzone_raw && gy < deadzone_raw)
		gy = 0;
	if (gz > -deadzone_raw && gz < deadzone_raw)
		gz = 0;
#endif
}
void LSM9DS1::calcAccel() {
	// To read from the accelerometer, you must first call the
	// readAccel() function. When this exits, it'll update the
	// ax, ay, and az variables with the most current data.
	readAccel();

#ifdef PRINT_CALCULATED
	// If you want to print calculated values, you can use the
	// convAccel helper function to convert a raw ADC value to
	// g's. Give the function the value that you want to convert.
	/*ax = convAccel(ax);
	 ay = convAccel(ay);
	 az = convAccel(az);*/
	// Apply calibration
	ax = (ax + ax_dcbias) * ax_scalebias;
	ay = (ay + ay_dcbias) * ay_scalebias;
	az = (az + az_dcbias) * az_scalebias;
#endif
}
void LSM9DS1::calcMag() {
	// To read from the accelerometer, you must first call the
	// readAccel() function. When this exits, it'll update the
	// mx, my, and mz variables with the most current data.
	readMag();

#ifdef PRINT_CALCULATED
	// If you want to print calculated values, you can use the
	// convMag helper function to convert a raw ADC value to
	// Gauss. Give the function the value that you want to convert.
	/*mx = convMag(mx);
	 my = convMag(my);
	 mz = convMag(mz);*/
	// Apply calibration
	mx = (mx + mx_dcbias) * mx_scalebias;
	my = (my + my_dcbias) * my_scalebias;
	mz = (mz + mz_dcbias) * mz_scalebias;
#endif
}

// Print formatted values of LSM9DS1 object
//void LSM9DS1::printGyro() {
//  Serial.print("G: ");
//  Serial.print(gx);
//  Serial.print(", ");
//  Serial.print(gy);
//  Serial.print(", ");
//  Serial.println(gz);
//}
//void LSM9DS1::printAccel() {
//  Serial.print("A: ");
//  Serial.print(ax);
//  Serial.print(", ");
//  Serial.print(ay);
//  Serial.print(", ");
//  Serial.println(az);
//}
//void LSM9DS1::printMag() {
//  Serial.print("M: ");
//  Serial.print(mx);
//  Serial.print(", ");
//  Serial.print(my);
//  Serial.print(", ");
//  Serial.println(mz);
//}

// Calculate derived sensor values
void LSM9DS1::calcPitch() {
	if( pitch == -1000 ) {
		pitch = atan2(ay, -az);
	}
}
void LSM9DS1::calcRoll() {
	if (roll == -1000 ) {
		calcPitch();
		roll = atan2(ax, -ay * sin(pitch) + az * cos(pitch));
	}
}
void LSM9DS1::calcYaw() {
	if (yaw == -1000) {
		calcPitch();
		calcRoll();

		float bfx = (mx) * cos(roll) + (my) * sin(pitch) * sin(roll) - (mz) * cos(pitch) * sin(roll);
		float bfy = (my) * cos(pitch) + (mz) * sin(pitch);
		yaw = atan2(bfx, -bfy);
	}
}

// Configure sensor interrupts
void LSM9DS1::configGyroInt(unsigned char int1Cfg, unsigned short int1ThsX,
		unsigned short int1ThsY, unsigned short int1ThsZ,
		unsigned char duration) {
	agWriteByte(INT_GEN_CFG_G, int1Cfg);
	agWriteByte(INT_GEN_THS_XH_G, (int1ThsX & 0xFF00) >> 8);
	agWriteByte(INT_GEN_THS_XL_G, (int1ThsX & 0xFF));
	agWriteByte(INT_GEN_THS_YH_G, (int1ThsY & 0xFF00) >> 8);
	agWriteByte(INT_GEN_THS_YL_G, (int1ThsY & 0xFF));
	agWriteByte(INT_GEN_THS_ZH_G, (int1ThsZ & 0xFF00) >> 8);
	agWriteByte(INT_GEN_THS_ZL_G, (int1ThsZ & 0xFF));
	if (duration)
		agWriteByte(INT_GEN_DUR_G, 0x80 | duration);
	else
		agWriteByte(INT_GEN_DUR_G, 0x00);
}

bool LSM9DS1::isCalibrated() {
	return calibrated;
}
// Calibrate sensors
//void LSM9DS1::calibrate() {
//  bool print = false;
//
//  // Calibrate magnetometer x/y-axes
//  float delta = .001; // time delta in milliseconds
//  float intg = 0;
//  float calDeg = 360; // Amount of rotation required to complete calibration in degrees
//  float calLim = (calDeg/3.0)*(1000.0*delta);
//  float gz;
//  float deadzone = 3.0;
//
////  if(print) {
////  Serial.println("MagXY Calibration Started."); delay(1000);
////  Serial.println("Set the device in mounting position."); delay(1000);
////  Serial.print("Slowly move the device counter-clockwise "); Serial.print((int)calDeg); Serial.println(" degs to calibrate.");
////  }
//
//  mathMag(mx, my, mz);
//  float mx_max = mx, my_max = my, mx_min = mx, my_min = my;
//
//  while(intg < calLim) {
//    // set up mag
//    mathMag(mx, my, mz);
//    if(mx > mx_max) mx_max = mx;
//    if(my > my_max) my_max = my;
//    if(mx < mx_min) mx_min = mx;
//    if(my < my_min) my_min = my;
//
//    // set up gz
//    readGyro();
//    gz = gz;
//    if(gz>-deadzone && gz<deadzone) gz = 0;
//
//    // calc integral of gz
//    intg += gz*delta;
//
//    delay(delta);
//  }
//
//  // apply calibration biases
//  mx_dcbias = -((mx_max - mx_min)/2.0 + mx_min);
//  mx_scalebias = -sign(mz)*1.0/(mx_max + mx_dcbias);
//  my_dcbias = -((my_max - my_min)/2.0 + my_min);
//  my_scalebias = 1.0/(my_max + my_dcbias);
//
//  if(print) {
//  Serial.println();
//  Serial.println("MagXY Calibrated!");
//  Serial.println();
//  }
//
//  // Calibrate magnetometer z-axis
//  float mz_max, mz_min;
//
//  if(print) {
//  Serial.println("MagZ Calibration Started."); delay(1000);
//  Serial.println("Set the device rightside-up and do not touch it."); delay(1000);
//  Serial.print("Calibrating in 3... "); delay(1000);
//  Serial.print("2... "); delay(1000);
//  Serial.println("1... "); delay(1000);
//  }
//
//  readMag();
//  mz_min = mz;
//
//  if(print) {
//  Serial.println();
//  Serial.println("MagZ Part 1 Calibration Complete!");
//  delay(1000);
//  Serial.println();
//  Serial.println("Set the device upside-down and do not touch it."); delay(1000);
//  Serial.print("Calibrating in 3... "); delay(1000);
//  Serial.print("2... "); delay(1000);
//  Serial.println("1... "); delay(1000);
//  }
//
//  readMag();
//  mz_max = mz;
//
//  if(print) {
//  Serial.println();
//  Serial.println("MagZ Part 2 Calibration Complete!");
//  }
//
//  mz_dcbias = -((mz_max - mz_min)/2.0 + mz_min);
//  mz_scalebias = -1.0/(mz_max + mz_dcbias);
//
//  if(print) {
//  Serial.println();
//  Serial.println("MagZ Calibrated!");
//  Serial.println();
//  }
//}

// Calculate sensor resolutions
void LSM9DS1::calcgRes() {
	// Possible gyro scales (and their register bit settings) are:
	// 245 DPS (00), 500 DPS (01), 2000 DPS (11). Here's a bit of an algorithm
	// to calculate DPS/(ADC tick) based on that 2-bit value:
	switch (gScale) {
	case G_SCALE_245DPS:
		gRes = 245.0 / 32768.0;
		break;
	case G_SCALE_500DPS:
		gRes = 500.0 / 32768.0;
		break;
	case G_SCALE_2000DPS:
		gRes = 2000.0 / 32768.0;
		break;
	}
}
void LSM9DS1::calcaRes() {
	// Possible accelerometer scales (and their register bit settings) are:
	// 2 g (00), 4g (10), 8g (11), 16g (01). Here's a bit of an
	// algorithm to calculate g/(ADC tick) based on that 2-bit value:
//  aRes = aScale == A_SCALE_16G ? 16.0 / 32768.0 : 
//       (((float) aScale + 1.0) * 2.0) / 32768.0;
	switch (aScale) {
	case A_SCALE_2G:
		aRes = 2.0 / 32768.0;
		break;
	case A_SCALE_4G:
		aRes = 4.0 / 32768.0;
		break;
	case A_SCALE_8G:
		aRes = 8.0 / 32768.0;
		break;
	case A_SCALE_16G:
		aRes = 16.0 / 32768.0;
		break;
	}
}
void LSM9DS1::calcmRes() {
	// Possible magnetometer scales (and their register bit settings) are:
	// 4 Gs (00), 8 Gs (01), 12 Gs (10) 16 Gs (11). Here's a bit of an algorithm
	// to calculate Gs/(ADC tick) based on that 2-bit value:
//  mRes = mScale == M_SCALE_4GS ? 4.0 / 32768.0 : 
//         (float) (mScale << 2) / 32768.0;
	switch (mScale) {
	case M_SCALE_4GS:
		mRes = 4.0 / 32768.0;
		break;
	case M_SCALE_8GS:
		mRes = 8.0 / 32768.0;
		break;
	case M_SCALE_12GS:
		mRes = 12.0 / 32768.0;
		break;
	case M_SCALE_16GS:
		mRes = 16.0 / 32768.0;
		break;
	}
}

// Write byte to sensor regs
void LSM9DS1::agWriteByte(unsigned char subAddress, unsigned char data) {
//	I2CwriteByte(agAddress, subAddress, data);
}
void LSM9DS1::mWriteByte(unsigned char subAddress, unsigned char data) {
//	return I2CwriteByte(mAddress, subAddress, data);
}

// Read byte(s) from sensor regs
unsigned char LSM9DS1::agReadByte(unsigned char subAddress) {
//	return I2CreadByte(agAddress, subAddress);
}
void LSM9DS1::agReadBytes(unsigned char subAddress, unsigned char * dest,
		unsigned char count) {
//	I2CreadBytes(agAddress, subAddress, dest, count);
}
unsigned char LSM9DS1::mReadByte(unsigned char subAddress) {
//	return I2CreadByte(mAddress, subAddress);
}
void LSM9DS1::mReadBytes(unsigned char subAddress, unsigned char * dest,
		unsigned char count) {
//	I2CreadBytes(mAddress, subAddress, dest, count);
}

// I2C (Wire) init, read, write
//void LSM9DS1::initI2C() {
//  Wire.begin(); // Initialize I2C library
//
//  // disable internal pullups on SDA and SCL /*
//  #ifndef cbi
//  #define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
//  #endif
//  #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega8__) || defined(__AVR_ATmega328P__)
//    // deactivate internal pull-ups for twi
//    // as per note from atmega8 manual pg167
//    cbi(PORTC, 4);
//    cbi(PORTC, 5);
//  #else
//    // deactivate internal pull-ups for twi
//    // as per note from atmega128 manual pg204
//    cbi(PORTD, 0);
//    cbi(PORTD, 1);
//  #endif */
//}
//void LSM9DS1::I2CwriteByte(unsigned char address, unsigned char subAddress, unsigned char data) {
//  Wire.beginTransmission(address);  // Initialize the Tx buffer
//  Wire.write(subAddress);           // Put slave register address in Tx buffer
//  Wire.write(data);                 // Put data in Tx buffer
//  Wire.endTransmission();           // Send the Tx buffer
//}
//unsigned char LSM9DS1::I2CreadByte(unsigned char address, unsigned char subAddress) {
//  unsigned char data; // `data` will store the register data
//  Wire.beginTransmission(address);         // Initialize the Tx buffer
//  Wire.write(subAddress);                  // Put slave register address in Tx buffer
//  Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
//  Wire.requestFrom(address, (unsigned char) 1);  // Read one byte from slave register address
//  data = Wire.read();                      // Fill Rx buffer with result
//  return data;                             // Return data read from slave register
//}
//void LSM9DS1::I2CreadBytes(unsigned char address, unsigned char subAddress, unsigned char * dest, unsigned char count) {
//  Wire.beginTransmission(address);   // Initialize the Tx buffer
//  // Next send the register to be read. OR with 0x80 to indicate multi-read.
//  Wire.write(subAddress | 0x80);     // Put slave register address in Tx buffer
//  Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
//  unsigned char i = 0;
//  Wire.requestFrom(address, count);  // Read bytes from slave register address
//  while (Wire.available()) {
//    dest[i++] = Wire.read(); // Put read results in the Rx buffer
//  }
//}