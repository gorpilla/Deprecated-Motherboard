/*
===============================================================================
 Name        : headingsensor.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include <board_api.h>
#include <chip.h>

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

//#include "i2c.c"
#include "LSM9DS1.h"

// Create an instance of the LSM9DS1 library
// Parameters: [accel/gyro I2C address],[mag I2C address]
LSM9DS1 dof(LSM9DS1_AG, LSM9DS1_M);

int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif

    // Initialize LSM9DS1 object
	uint16_t status = dof.begin(dof.G_SCALE_245DPS, dof.A_SCALE_2G, dof.M_SCALE_4GS,
								dof.G_ODR_119_BW_14, dof.A_ODR_50, dof.M_ODR_40);

	// Wait until successful communication
	while( status != WHO_AM_I );

	// If not calibrated, calibrate sensors
	if( !dof.calibrated );// dof.calibrate();

	while(1) { // do stuff
		dof.update();

	}

    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;
    }
    return 0 ;
}