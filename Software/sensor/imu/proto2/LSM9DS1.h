/******************************************************************************
LSM9DS1.h
Mike Miles
Original Creation Date: June 29, 2015

This file implements all functions of the LSM9DS1 class. Functions here range
from higher level stuff, like reading/writing LSM9DS1 registers to low-level,
hardware reads and writes. Both SPI and I2C handler functions can be found
towards the bottom of this file.

Development environment specifics:
  IDE: LPCXpresso v7.8.0
  Hardware Platform: NXP LPC11C24

Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef __LSM9DS1_H__
#define __LSM9DS1_H__

//////////////////////
// Global Variables //
//////////////////////
const float PI = 3.14159265;
const float RAD2DEG = 57.29577951; // RAD2DEG = 180.0/PI

///////////////////
// Code Settings //
///////////////////
#define WHO_AM_I			0x3D68 // WHO_AM_I value of mag (0x3D) and accel/gyro (0x68), used to check successful communication
#define LSM9DS1_AG			0x6A // I2C address of Accel/Gyro (LSB high)
#define LSM9DS1_M			0x1C // I2C address of Mag (LSB high)
#define PRINT_CALCULATED // if defined, prints calculated data instead of direct ADC output

//////////////////////////////////
// LSM9DS1 Accel/Gyro Registers //
//////////////////////////////////
#define ACT_THS				0x04
#define ACT_DUR             0x05
#define INT_GEN_CFG_XL      0x06
#define INT_GEN_THS_X_XL    0x07
#define INT_GEN_THS_Y_XL    0x08
#define INT_GEN_THS_Z_XL    0x09
#define INT_GEN_DUR_XL      0x0A
#define REFERENCE_G         0x0B
#define INT1_CTRL           0x0C
#define INT2_CTRL           0x0D
#define WHO_AM_I_AG         0x0F
#define CTRL_REG1_G         0x10
#define CTRL_REG2_G         0x11
#define CTRL_REG3_G         0x12
#define ORIENT_CFG_G        0x13
#define INT_GEN_SRC_G       0x14
#define OUT_TEMP_L          0x15
#define OUT_TEMP_H          0x16
#define STATUS_REG_G        0x17
#define OUT_X_L_G           0x18
#define OUT_X_H_G           0x19
#define OUT_Y_L_G           0x1A
#define OUT_Y_H_G           0x1B
#define OUT_Z_L_G           0x1C
#define OUT_Z_H_G           0x1D
#define CTRL_REG4           0x1E
#define CTRL_REG5_XL        0x1F
#define CTRL_REG6_XL        0x20
#define CTRL_REG7_XL        0x21
#define CTRL_REG8           0x22
#define CTRL_REG9           0x23
#define CTRL_REG10          0x24
#define INT_GEN_SRC_XL      0x26
#define STATUS_REG_XL       0x27
#define OUT_X_L_XL          0x28
#define OUT_X_H_XL          0x29
#define OUT_Y_L_XL          0x2A
#define OUT_Y_H_XL          0x2B
#define OUT_Z_L_XL          0x2C
#define OUT_Z_H_XL          0x2D
#define FIFO_CTRL           0x2E
#define FIFO_SRC            0x2F
#define INT_GEN_CFG_G       0x30
#define INT_GEN_THS_XH_G    0x31
#define INT_GEN_THS_XL_G    0x32
#define INT_GEN_THS_YH_G    0x33
#define INT_GEN_THS_YL_G    0x34
#define INT_GEN_THS_ZH_G    0x35
#define INT_GEN_THS_ZL_G    0x36
#define INT_GEN_DUR_G       0x37

///////////////////////////
// LSM9DS1 Mag Registers //
///////////////////////////
#define OFFSET_X_REG_L_M    0x05
#define OFFSET_X_REG_H_M    0x06
#define OFFSET_Y_REG_L_M    0x07
#define OFFSET_Y_REG_H_M    0x08
#define OFFSET_Z_REG_L_M    0x09
#define OFFSET_Z_REG_H_M    0x0A
#define WHO_AM_I_M          0x0F
#define CTRL_REG1_M         0x20
#define CTRL_REG2_M         0x21
#define CTRL_REG3_M         0x22
#define CTRL_REG4_M         0x23
#define CTRL_REG5_M         0x24
#define STATUS_REG_M        0x27
#define OUT_X_L_M           0x28
#define OUT_X_H_M           0x29
#define OUT_Y_L_M           0x2A
#define OUT_Y_H_M           0x2B
#define OUT_Z_L_M           0x2C
#define OUT_Z_H_M           0x2D
#define INT_CFG_M           0x30
#define INT_SRC_M           0x31
#define INT_THS_L_M         0x32
#define INT_THS_H_M         0x33

class LSM9DS1 {

public:

  // gyro_scale defines the possible full-scale ranges of the gyroscope:
  enum gyro_scale {
    G_SCALE_245DPS,   // 00:  245 degrees per second
    G_SCALE_500DPS,   // 01:  500 dps
    G_SCALE_2000DPS,  // 11: 2000 dps
  };

  // accel_scale defines all possible FSR's of the accelerometer:
  enum accel_scale {
    A_SCALE_2G,   // 00:  2g
    A_SCALE_16G,  // 01: 16g
    A_SCALE_4G,   // 10:  4g
    A_SCALE_8G,   // 11:  8g
  };

  // mag_scale defines all possible FSR's of the magnetometer:
  enum mag_scale {
    M_SCALE_4GS,   // 00:  4 Gs
    M_SCALE_8GS,   // 01:  8 Gs
    M_SCALE_12GS,  // 10: 12 Gs
    M_SCALE_16GS,  // 11: 16 Gs
  };

  // gyro_odr defines all possible data rate/bandwidth combos of the gyro (and accel if not accel-only mode):
  enum gyro_odr {							 // ODR (Hz) --- Cutoff
    G_ODR_POWER_DOWN = 0x00, // power-down	 N/A
	G_ODR_149_BW_NA = 0x20,  // 14.9		 N/A
	G_ODR_595_BW_16 = 0x40,  // 59.5		 N/A
	G_ODR_119_BW_14 = 0x60,  // 119			 14
	G_ODR_119_BW_31 = 0x61,  // 119		 	 31
	G_ODR_238_BW_14 = 0x80,  // 238		 	 14
	G_ODR_238_BW_29 = 0x81,  // 238		 	 29
	G_ODR_238_BW_63 = 0x82,  // 238		 	 63
	G_ODR_238_BW_78 = 0x83,  // 238		 	 78
	G_ODR_476_BW_21 = 0xA0,  // 479		 	 21
	G_ODR_476_BW_28 = 0xA1,  // 479		 	 28
	G_ODR_476_BW_57 = 0xA2,  // 479		 	 57
	G_ODR_476_BW_100 = 0xA3, // 479		 	 100
	G_ODR_952_BW_33 = 0xC0,  // 952		 	 33
	G_ODR_952_BW_40 = 0xC1,  // 952		 	 40
	G_ODR_952_BW_58 = 0xC2,  // 952		 	 58
	G_ODR_952_BW_100 = 0xC3, // 952		 	 100
	G_ODR_NA_BW_NA = 0xE0,   // N/A		 	 N/A
  };

  // accel_oder defines all possible output data rates of the accelerometer (accel-only mode, else odr defined by gyro_odr):
  enum accel_odr {
    A_POWER_DOWN = 0x00, // Power-down mode (0x00)
    A_ODR_10 = 0x01,	 //  10 Hz (0x01)
    A_ODR_50 = 0x02,	 //  50 Hz (0x02)
    A_ODR_119 = 0x03,	 // 119 Hz (0x03)
    A_ODR_238 = 0x04,	 // 238 Hz (0x04)
    A_ODR_476 = 0x05,	 // 476 Hz (0x05)
    A_ODR_952 = 0x06,	 // 952 Hz (0x06)
    A_ODR_NA = 0x07,	 // N/A Hz (0x07)
  };

  // accel_abw defines all possible anti-aliasing filter rates of the accelerometer:
  enum accel_abw {
    A_ABW_408 = 0x00, // 408 Hz (0x00)
    A_ABW_211 = 0x01, // 211 Hz (0x01)
    A_ABW_105 = 0x02, // 105 Hz (0x02)
    A_ABW_50 = 0x03,  //  50 Hz (0x03)
  };

  // mag_oder defines all possible output data rates of the magnetometer:
  enum mag_odr {
    M_ODR_625 = 0x00, //  0.625 Hz (0x00)
    M_ODR_125 = 0x01, //  1.25  Hz (0x01)
    M_ODR_25 = 0x02,  //  2.5 Hz   (0x02)
    M_ODR_5	= 0x03,   //  5 Hz     (0x03)
    M_ODR_10 = 0x04,  // 10 Hz     (0x04)
    M_ODR_20 = 0x05,  // 20 Hz     (0x05)
	M_ODR_40 = 0x06,  // 40 Hz     (0x06)
	M_ODR_80 = 0x07,  // 80 Hz     (0x07)
  };

  // mag_xy_om defines the X and Y axes operative mode selection of the magnetometer:
  enum mag_xy_om {
	M_XY_OM_L = 0x00,  // low-power mode (0x00)
	M_XY_OM_M = 0x01,  // medium-performance mode (0x01)
	M_XY_OM_H = 0x02,  // high-performance mode (0x02)
	M_XY_OM_UH = 0x03, // ultra-high performance mode (0x03)
  };

  // mag_z_om defines the Z axis operative mode selection of the magnetometer:
  enum mag_z_om {
	M_Z_OM_L = 0x00,   // low-power mode (0x00)
	M_Z_OM_M = 0x01,   // medium-performance mode (0x01)
	M_Z_OM_H = 0x02,   // high-performance mode (0x02)
	M_Z_OM_UH = 0x03, // ultra-high performance mode (0x03)
  };

  // We'll store the gyro, accel, and magnetometer readings in a series of
  // public class variables. Each sensor gets three variables -- one for each
  // axis. Call readGyro(), readAccel(), and readMag() first, before using
  // these variables!
  // These values are the RAW signed 16-bit readings from the sensors.
  int16_t gx, gy, gz; // x, y, and z axis readings of the gyroscope
  int16_t ax, ay, az; // x, y, and z axis readings of the accelerometer
  int16_t mx, my, mz; // x, y, and z axis readings of the magnetometer
  int16_t temperature; // temperature reading
  // Variable to keep track of calibration status
  bool calibrated = false;

  //int16_t gx_calc, gy_calc, gz_calc; // calculated x, y, and z axis readings of the gyroscope
  //int16_t ax_calc, ay_calc, az_calc; // calculated x, y, and z axis readings of the accelerometer
  //int16_t mx_calc, my_calc, mz_calc; // calculated x, y, and z axis readings of the magnetometer
  float pitch, roll, yaw; // calculated pitch, roll, and yaw readings

  // DC and Scale biases of accel values
  int16_t ax_dcbias = 0,
		  ay_dcbias = 0,
		  az_dcbias = 0;
  float	ax_scalebias = 1,
		ay_scalebias = -1,
		az_scalebias = 1;

  // DC and Scale biases of mag values
  int16_t mx_dcbias = 13,
		  my_dcbias = -41,
		  mz_dcbias = 25;
  float mx_scalebias = 2.62,
		my_scalebias = 2.44,
		mz_scalebias = -3.60;

  // LSM9DS1 -- LSM9DS1 class constructor
  // The constructor will set up a handful of private variables, and set the
  // communication mode as well.
  // Input:
  //  - agAddr = If MODE_I2C, this is the I2C address of the gyroscope.
  //        If MODE_SPI, this is the chip select pin of the gyro (CSG)
  //  - mAddr = If MODE_I2C, this is the I2C address of the accel/mag.
  //        If MODE_SPI, this is the cs pin of the accel/mag (CSXM)
  // SPI functionality removed.
  LSM9DS1(uint8_t agAddr, uint8_t mAddr);

  // begin() -- Initialize the gyro, accelerometer, and magnetometer.
  // This will set up the scale and output rate of each sensor. It'll also
  // "turn on" every sensor and every axis of every sensor.
  // Input:
  //  - gScl = The scale of the gyroscope. This should be a gyro_scale value.
  //  - aScl = The scale of the accelerometer. Should be a accel_scale value.
  //  - mScl = The scale of the magnetometer. Should be a mag_scale value.
  //  - gODR = Output data rate of the gyroscope. gyro_odr value.
  //  - aODR = Output data rate of the accelerometer. accel_odr value.
  //  - mODR = Output data rate of the magnetometer. mag_odr value.
  // Output: The function will return an unsigned 16-bit value. The most-sig
  //    bytes of the output are the WHO_AM_I reading of the accel. The
  //    least significant two bytes are the WHO_AM_I reading of the gyro.
  // All parameters have a defaulted value, so you can call just "begin()".
  // Default values are FSR's of:  245DPS, 2g, 2Gs; ODRs of 95 Hz for
  // gyro, 100 Hz for accelerometer, 100 Hz for magnetometer.
  // Use the return value of this function to verify communication.
  uint16_t begin( gyro_scale gScl = G_SCALE_245DPS,
                  accel_scale aScl = A_SCALE_2G,
                  mag_scale mScl = M_SCALE_4GS,
                  gyro_odr gODR = G_ODR_119_BW_14,
                  accel_odr aODR = A_ODR_50,
                  mag_odr mODR = M_ODR_40,
				  mag_xy_om = M_XY_OM_M,
				  mag_z_om = M_Z_OM_M );
  
  // update() -- Updates values of LSM9DS1 object.
  // This function will do the following:
  //  - Get sensor values from raw ADC registers.
  //  - 
  void update();

  // readGyro() -- Read the gyroscope output registers.
  // This function will read all six gyroscope output registers.
  // The readings are stored in the class' gx, gy, and gz variables. Read
  // those _after_ calling readGyro().
  void readGyro();

  // readAccel() -- Read the accelerometer output registers.
  // This function will read all six accelerometer output registers.
  // The readings are stored in the class' ax, ay, and az variables. Read
  // those _after_ calling readAccel().
  void readAccel();

  // readMag() -- Read the magnetometer output registers.
  // This function will read all six magnetometer output registers.
  // The readings are stored in the class' mx, my, and mz variables. Read
  // those _after_ calling readMag().
  void readMag();

  // readTemp() -- Read the temperature output register.
  // This function will read two temperature output registers.
  // The combined readings are stored in the class' temperature variables. Read
  // those _after_ calling readTemp().
  void readTemp();

  // convGyro() -- Convert from RAW signed 16-bit value to degrees per second
  // This function reads in a signed 16-bit value and returns the scaled
  // DPS. This function relies on gScale and gRes being correct.
  // Input:
  //  - gyro = A signed 16-bit raw reading from the gyroscope.
  float convGyro(int16_t gyro);

  // convAccel() -- Convert from RAW signed 16-bit value to gravity (g's).
  // This function reads in a signed 16-bit value and returns the scaled
  // g's. This function relies on aScale and aRes being correct.
  // Input:
  //  - accel = A signed 16-bit raw reading from the accelerometer.
  float convAccel(int16_t accel);

  // convMag() -- Convert from RAW signed 16-bit value to Gauss (Gs)
  // This function reads in a signed 16-bit value and returns the scaled
  // Gs. This function relies on mScale and mRes being correct.
  // Input:
  //  - mag = A signed 16-bit raw reading from the magnetometer.
  float convMag(int16_t mag);

  // setGyroScale() -- Set the full-scale range of the gyroscope.
  // This function can be called to set the scale of the gyroscope to
  // 245, 500, or 200 degrees per second.
  // Input:
  //  - gScl = The desired gyroscope scale. Must be one of three possible
  //    values from the gyro_scale enum.
  void setGyroScale(gyro_scale gScl);

  // setAccelScale() -- Set the full-scale range of the accelerometer.
  // This function can be called to set the scale of the accelerometer to
  // 2, 4, 6, 8, or 16 g's.
  // Input:
  //  - aScl = The desired accelerometer scale. Must be one of five possible
  //    values from the accel_scale enum.
  void setAccelScale(accel_scale aScl);

  // setMagScale() -- Set the full-scale range of the magnetometer.
  // This function can be called to set the scale of the magnetometer to
  // 2, 4, 8, or 12 Gs.
  // Input:
  //  - mScl = The desired magnetometer scale. Must be one of four possible
  //    values from the mag_scale enum.
  void setMagScale(mag_scale mScl);

  // setGyroODR() -- Set the output data rate and bandwidth of the gyroscope
  // Input:
  //  - gRate = The desired output rate and cutoff frequency of the gyro.
  //    Must be a value from the gyro_odr enum (check above, there're 14).
  void setGyroODR(gyro_odr gRate);

  // setAccelODR() -- Set the output data rate of the accelerometer
  // Input:
  //  - aRate = The desired output rate of the accel.
  //    Must be a value from the accel_odr enum (check above, there're 11).
  void setAccelODR(accel_odr aRate);

  // setAccelABW() -- Set the anti-aliasing filter rate of the accelerometer
  // Input:
  //  - abwRate = The desired anti-aliasing filter rate of the accel.
  //    Must be a value from the accel_abw enum (check above, there're 4).
  void setAccelABW(accel_abw abwRate);

  // setMagODR() -- Set the output data rate of the magnetometer
  // Input:
  //  - mRate = The desired output rate of the mag.
  //    Must be a value from the mag_odr enum (check above, there're 6).
  void setMagODR(mag_odr mRate);

  // setMagOM() -- Set the operating modes for the xy-axes and z-axis
  // Input:
  //  - mXYOM = The desired operating mode for the mag xy-axes
  //  - mZOM = The desired operating mode for the mag z-axis
  //    Must be values from the mag_xy_om and mag_z_om enums, respectively (there're 4 each).
  void setMagOM(mag_xy_om mXYOM, mag_z_om mZOM);

  // calcGyro() -- Filters and scales RAW member variables of LSM9DS1 gyro values
  // Alters RAW member variables of LSM9DS1 gyro values, 
  // so be sure variables are updated before using elsewhere.
  void calcGyro();
  
  // calcAccel() -- Filters and scales RAW member variables of LSM9DS1 accel values
  // Alters RAW member variables of LSM9DS1 accel values, 
  // so be sure variables are updated before using elsewhere.
  void calcAccel();

  // calcMag() -- Filters and scales RAW member variables of LSM9DS1 mag values
  // Alters RAW member variables of LSM9DS1 mag values, 
  // so be sure variables are updated before using elsewhere.
  void calcMag();
   
  // printGyro() -- Prints status of gyro sensor in formatted manner.
  void printGyro();

  // printAccel() -- Prints status of accel sensor in formatted manner.
  void printAccel();

  // printMag() -- Prints status of mag sensor in formatted manner.
  void printMag();

  // calcPitch() -- Calculates pitch based on accel values
  float calcPitch();

  // calcRoll() -- Calculates roll based on accel values
  float calcRoll();

  // calcYaw() -- Calculates yaw based on accel and mag values (tilt-compensated heading)
  float calcYaw();

  // configGyroInt() -- Configure the gyro interrupt output.
  // Triggers can be set to either rising above or falling below a specified
  // threshold. This function helps setup the interrupt configuration and
  // threshold values for all axes.
  // Input:
  //  - int1Cfg = A 8-bit value that is sent directly to the INT1_CFG_G
  //    register. This sets AND/OR and high/low interrupt gen for each axis
  //  - int1ThsX = 16-bit interrupt threshold value for x-axis
  //  - int1ThsY = 16-bit interrupt threshold value for y-axis
  //  - int1ThsZ = 16-bit interrupt threshold value for z-axis
  //  - duration = Duration an interrupt holds after triggered. This value
  //    is copied directly into the INT1_DURATION_G register.
  // Before using this function, read about the INT1_CFG_G register and
  // the related INT1* registers in the LMS9DS0 datasheet.
  void configGyroInt( uint8_t int1Cfg, uint16_t int1ThsX = 0,
					  uint16_t int1ThsY = 0, uint16_t int1ThsZ = 0,
					  uint8_t duration = 0 );

  void calibrate();
 
private:

  // mAddress and agAddress store the I2C address or SPI chip select pin
  // for each sensor.
  uint8_t mAddress, agAddress;
  // interfaceMode keeps track of whether we're using SPI or I2C to talk
  interface_mode interfaceMode;

  // gScale, aScale, and mScale store the current scale range for each
  // sensor. Should be updated whenever that value changes.
  gyro_scale gScale;
  accel_scale aScale;
  mag_scale mScale;

  // gRes, aRes, and mRes store the current resolution for each sensor.
  // Units of these values would be DPS (or g's or Gs's) per ADC tick.
  // This value is calculated as (sensor scale) / (2^15).
  float gRes, aRes, mRes;

  // initGyro() -- Sets up the gyroscope to begin reading.
  // This function steps through all five gyroscope control registers.
  // Upon exit, the following parameters will be set:
  //  - CTRL_REG1_G = 0x0F: Normal operation mode, all axes enabled.
  //    95 Hz ODR, 12.5 Hz cutoff frequency.
  //  - CTRL_REG2_G = 0x00: HPF set to normal mode, cutoff frequency
  //    set to 7.2 Hz (depends on ODR).
  //  - CTRL_REG3_G = 0x88: Interrupt enabled on INT_G (set to push-pull and
  //    active high). Data-ready output enabled on DRDY_G.
  //  - CTRL_REG4_G = 0x00: Continuous update mode. Data LSB stored in lower
  //    address. Scale set to 245 DPS. SPI mode set to 4-wire.
  //  - CTRL_REG5_G = 0x00: FIFO disabled. HPF disabled.
  void initAccelGyro();

  // initAccel() -- Sets up the accelerometer to begin reading.
  // This function steps through all accelerometer related control registers.
  // Upon exit these registers will be set as:
  //  - CTRL_REG0_XM = 0x00: FIFO disabled. HPF bypassed. Normal mode.
  //  - CTRL_REG1_XM = 0x57: 100 Hz data rate. Continuous update.
  //    all axes enabled.
  //  - CTRL_REG2_XM = 0x00:  2g scale. 773 Hz anti-alias filter BW.
  //  - CTRL_REG3_XM = 0x04: Accel data ready signal on INT1_XM pin.
//  void initAccel();

  // initMag() -- Sets up the magnetometer to begin reading.
  // This function steps through all magnetometer-related control registers.
  // Upon exit these registers will be set as:
  //  - CTRL_REG4_XM = 0x04: Mag data ready signal on INT2_XM pin.
  //  - CTRL_REG5_XM = 0x14: 100 Hz update rate. Low resolution. Interrupt
  //    requests don't latch. Temperature sensor disabled.
  //  - CTRL_REG6_XM = 0x00:  2 Gs scale.
  //  - CTRL_REG7_XM = 0x00: Continuous conversion mode. Normal HPF mode.
  //  - INT_CTRL_REG_M = 0x09: Interrupt active-high. Enable interrupts.
  void initMag();

  // gReadByte() -- Reads a byte from a specified gyroscope register.
  // Input:
  //  - subAddress = Register to be read from.
  // Output:
  //  - An 8-bit value read from the requested address.
  uint8_t agReadByte(uint8_t subAddress);

  // gReadBytes() -- Reads a number of bytes -- beginning at an address
  // and incrementing from there -- from the gyroscope.
  // Input:
  //  - subAddress = Register to be read from.
  //  - * dest = A pointer to an array of uint8_t's. Values read will be
  //    stored in here on return.
  //  - count = The number of bytes to be read.
  // Output: No value is returned, but the `dest` array will store
  //  the data read upon exit.
  void agReadBytes(uint8_t subAddress, uint8_t * dest, uint8_t count);

  // gWriteByte() -- Write a byte to a register in the gyroscope.
  // Input:
  //  - subAddress = Register to be written to.
  //  - data = data to be written to the register.
  void agWriteByte(uint8_t subAddress, uint8_t data);

  // xmReadByte() -- Read a byte from a register in the accel/mag sensor
  // Input:
  //  - subAddress = Register to be read from.
  // Output:
  //  - An 8-bit value read from the requested register.
  uint8_t mReadByte(uint8_t subAddress);

  // xmReadBytes() -- Reads a number of bytes -- beginning at an address
  // and incrementing from there -- from the accelerometer/magnetometer.
  // Input:
  //  - subAddress = Register to be read from.
  //  - * dest = A pointer to an array of uint8_t's. Values read will be
  //    stored in here on return.
  //  - count = The number of bytes to be read.
  // Output: No value is returned, but the `dest` array will store
  //  the data read upon exit.
  void mReadBytes(uint8_t subAddress, uint8_t * dest, uint8_t count);

  // xmWriteByte() -- Write a byte to a register in the accel/mag sensor.
  // Input:
  //  - subAddress = Register to be written to.
  //  - data = data to be written to the register.
  void mWriteByte(uint8_t subAddress, uint8_t data);

  // calcgRes() -- Calculate the resolution of the gyroscope.
  // This function will set the value of the gRes variable. gScale must
  // be set prior to calling this function.
  void calcgRes();

  // calcmRes() -- Calculate the resolution of the magnetometer.
  // This function will set the value of the mRes variable. mScale must
  // be set prior to calling this function.
  void calcmRes();

  // calcaRes() -- Calculate the resolution of the accelerometer.
  // This function will set the value of the aRes variable. aScale must
  // be set prior to calling this function.
  void calcaRes();

  ///////////////////
  // I2C Functions //
  ///////////////////
  // initI2C() -- Initialize the I2C hardware.
  // This function will setup all I2C pins and related hardware.
  void initI2C();

  // I2CwriteByte() -- Write a byte out of I2C to a register in the device
  // Input:
  //  - address = The 7-bit I2C address of the slave device.
  //  - subAddress = The register to be written to.
  //  - data = Byte to be written to the register.
  void I2CwriteByte(uint8_t address, uint8_t subAddress, uint8_t data);

  // I2CreadByte() -- Read a single byte from a register over I2C.
  // Input:
  //  - address = The 7-bit I2C address of the slave device.
  //  - subAddress = The register to be read from.
  // Output:
  //  - The byte read from the requested address.
  uint8_t I2CreadByte(uint8_t address, uint8_t subAddress);

  // I2CreadBytes() -- Read a series of bytes, starting at a register via SPI
  // Input:
  //  - address = The 7-bit I2C address of the slave device.
  //  - subAddress = The register to begin reading.
  //  - * dest = Pointer to an array where we'll store the readings.
  //  - count = Number of registers to be read.
  // Output: No value is returned by the function, but the registers read are
  //    all stored in the *dest array given.
  void I2CreadBytes(uint8_t address, uint8_t subAddress, uint8_t * dest, uint8_t count);
};

#endif // LSM9DS1_H //
