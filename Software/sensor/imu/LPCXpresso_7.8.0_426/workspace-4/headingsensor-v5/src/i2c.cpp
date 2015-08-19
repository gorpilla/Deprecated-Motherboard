#include "i2c.h"

int Wire::mode_poll;
uint8_t Wire::buffer[2][256];

// Constructors ////////////////////////////////////////////////////////////////

Wire::Wire() {
}

// Public Methods //////////////////////////////////////////////////////////////

void Wire::init(I2C_ID_T id, int speed) {
	// Init I2C Pin Mux (Init_I2C_PinMux)
#if (defined(BOARD_NXP_XPRESSO_11U14) || defined(BOARD_NGX_BLUEBOARD_11U24))
	Chip_SYSCTL_PeriphReset(RESET_I2C0);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 4, IOCON_FUNC1 | I2C_FASTPLUS_BIT);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 5, IOCON_FUNC1 | I2C_FASTPLUS_BIT);
#elif (defined(BOARD_NXP_XPRESSO_11C24) || defined(BOARD_MCORE48_1125))
	Chip_SYSCTL_PeriphReset(RESET_I2C0);
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_4, IOCON_FUNC1 | I2C_FASTPLUS_BIT);
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_5, IOCON_FUNC1 | I2C_FASTPLUS_BIT);
#else
#error "No Pin Muxing defined for I2C operation"
#endif

	// Initialize I2C
	Chip_I2C_Init(id);
	Chip_I2C_SetClockRate(id, speed);

	// Set I2C mode to polling/interrupt (i2c_set_mode)
	setMode(id, 1); // Set default mode to interrupt
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t Wire::write(I2C_XFER_T *xfer, uint8_t slaveAddr, uint8_t data, int length) {
	xfer->slaveAddr = slaveAddr;
	xfer->rxBuff = 0;
	xfer->txBuff = 0;
	xfer->txSz = 0;
	xfer->rxSz = 0;

	buffer[0][0] = data;

	xfer->txSz = length;
	xfer->txBuff = buffer[0];

	if (xfer->txSz == 0) {
		return 0;
	}

	return Chip_I2C_MasterSend(I2C0, xfer->slaveAddr, xfer->txBuff, xfer->txSz);
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int Wire::read(I2C_XFER_T *xfer, uint8_t slaveAddr, int length) {
	xfer->slaveAddr = slaveAddr;
	xfer->rxBuff = 0;
	xfer->txBuff = 0;
	xfer->txSz = 0;
	xfer->rxSz = 0;

	xfer->rxSz = length;
	xfer->rxBuff = buffer[1];

	length = Chip_I2C_MasterRead(I2C0, xfer->slaveAddr, xfer->rxBuff, xfer->rxSz);
	return length;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
I2C_STATUS_T Wire::status(I2C_XFER_T *xfer) {
	return xfer->status;
}

void Wire::setMode(I2C_ID_T id, bool polling) {
	if (!polling) { // Set mode to interrupt
		mode_poll &= ~(1 << id);
		Chip_I2C_SetMasterEventHandler(id, Chip_I2C_EventHandler);
		NVIC_EnableIRQ(I2C0_IRQn);
	} else { // Set mode to polling
		mode_poll |= 1 << id;
		NVIC_DisableIRQ(I2C0_IRQn);
		Chip_I2C_SetMasterEventHandler(id, Chip_I2C_EventHandlerPolling);
	}
}

// Preinstantiate Objects //////////////////////////////////////////////////////
Wire i2c = Wire();
