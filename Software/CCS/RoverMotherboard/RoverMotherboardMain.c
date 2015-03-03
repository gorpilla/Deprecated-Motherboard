//	TODO: Port To Fresh Build (using TI example:			This version educational practice not for distro)

// RoverMotherboardMain.c
//
// first created:
//
// 01_22_2015_Owen_Chiaventone
//
// last edited:
//
//02_24_2015_Judah Schad_jrs6w7@mst.edu
//
//	this instatiates global handles, initializes TI modules and then calls the TI BIOS operating system
//
//	TI BIOS operating system instatiates the roveTCPHandler, roveCmdCntrl, and roveTelemCntrl threads

//includes globally scoped Texas Instruments (TI) header files

#include "roveIncludes/RoverMotherboardMain.h"

//globally init UART handles

	UART_Handle uart0;
    UART_Handle uart1;
    UART_Handle uart2;
    UART_Handle uart3;
    UART_Handle uart4;
    UART_Handle uart5;
    UART_Handle uart6;
    UART_Handle uart7;

//TODO Mailbox Init

//
//  ======== init main  ========
//
int main(void)
{

//init TI board driver routines

    Board_initGeneral();
    Board_initGPIO();
    Board_initEMAC();
    Board_initUART();

//init UARTS

	uart0 = (UART_Handle)init_uart( 0, 115200 );
    uart1 = (UART_Handle)init_uart( 1, 115200 );
	uart2 = (UART_Handle)init_uart( 2, 115200 );
	uart3 = (UART_Handle)init_uart( 3, 115200 );
	uart4 = (UART_Handle)init_uart( 4, 115200 );
	uart5 = (UART_Handle)init_uart( 5, 115200 );
	uart7 = (UART_Handle)init_uart( 7, 115200 );

//start TI BIOS

	ms_delay( 1 );

	System_printf("roveMotherboardMain init \n");
	System_printf("\n");
	System_printf("\n");
	System_flush();

	ms_delay( 1 );

    BIOS_start();

//exit BIOS

    return (0);

}//endmain: 		RoverMotherboardMain