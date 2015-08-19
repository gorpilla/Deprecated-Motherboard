/*
 ===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

#include <stdint.h>
#include "LSM9DS1.h"
#include "i2c.h"

void SysTick_Handler(void);
void I2C_IRQHandler(void);
static void i2c_state_handling(I2C_ID_T id);

static volatile uint32_t tick_cnt;

// Create an instance of the LSM9DS1 library
// Parameters: [accel/gyro I2C address],[mag I2C address]
LSM9DS1 dof(I2C_SLAVE_AG_ADDR, I2C_SLAVE_M_ADDR);

/**
 * @brief	SysTick Interrupt Handler
 * @return	Nothing
 * @note	Systick interrupt handler updates the button status
 */
void SysTick_Handler(void) {
	dof.i2c_ag_update_regs(2);
	dof.i2c_m_update_regs(2);
	tick_cnt++;
}

/**
 * @brief	I2C Interrupt Handler
 * @return	None
 */
void I2C_IRQHandler(void) {
	i2c_state_handling(I2C0);
}

// State machine handler for I2C0 and I2C1
static void i2c_state_handling(I2C_ID_T id) {
	if (Chip_I2C_IsMasterActive(id)) {
		Chip_I2C_MasterStateHandler(id);
	} else {
		Chip_I2C_SlaveStateHandler(id);
	}
}

int main(void) {
	// Read clock settings and update SystemCoreClock variable
	SystemCoreClockUpdate();
	// Set up and initialize all required blocks and
	// functions related to the board hardware
	Board_Init();
	// Set the LED to the state of "On"
	Board_LED_Set(0, true);

	float gx, gy, gz, ax, ay, az, mx, my, mz;

	// Initialize LSM9DS1 object
	uint16_t status = dof.begin(dof.G_SCALE_245DPS, dof.A_SCALE_2G, dof.M_SCALE_4GS,
			dof.G_ODR_119_BW_14, dof.A_ODR_50, dof.M_ODR_40);

	// Wait until successful communication
	while (status != WHO_AM_I) {
	};

	// Calibrate sensors
	dof.calibrate();

	while (1) { // do stuff
		dof.update(gx, gy, gz, ax, ay, az, mx, my, mz);
	}

	Chip_I2C_DeInit(I2C0);

	// Force the counter to be placed into memory
	volatile static int i = 0;
	// Enter an infinite loop, just incrementing a counter
	while (1) {
		i++;
	}
	return 0;
}