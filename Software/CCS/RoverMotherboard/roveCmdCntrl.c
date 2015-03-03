//	TODO: Port To Fresh Build (using TI example:			This version educational practice not for distro)
//
// roveCmdCtrl.c
//
// first created:
//
// 01_22_2015_Owen_Chiaventone
//
// last edited:
//
//02_24_2015_Judah Schad_jrs6w7@mst.edu
//
// 	this implements a single function BIOS thread that acts as the RoverMotherboard.cfg roveCmdCtrlTask handle
//
//	recieves commands from roveTCPHandler in roveCom protocol using TI.Mailbox.from object and
//
//	sends requests to roveTelemCntrl for device telemetry in roveCom protocol using using TI.Mailbox.to object

#include "roveIncludes/roveWareHeaders/roveCmdCntrl.h"

Void roveCmdCntrl(UArg arg0, UArg arg1){
/*
	//external ref the scope for global uart handles

	extern UART_Handle uart0;
	extern UART_Handle uart1;
	extern UART_Handle uart2;
	extern UART_Handle uart3;
	extern UART_Handle uart4;
	extern UART_Handle uart5;
	extern UART_Handle uart6;
	extern UART_Handle uart7;

	//init and clean RoveCom msg recieve struct

	base_station_msg_struct fromBaseMsg;

	fromBaseMsg.id = onenull_device;

	memset(&fromBaseMsg.value, 1, sizeof(MAX_COMMAND_SIZE) );


	//init and clean RoveCom uart send struct

	struct motor_control_struct motor_control_struct;

	motor_control_struct.value = 0;

    //Task loops for ever

    //It awakes on full Mailbox_pend frome roveTCPHandler Task

    //It only exits on error

    ms_delay( 1 );

	System_printf("roveCmdCntrlr		init! \n");
	System_flush();

	ms_delay( 1 );

	while(1){

TODO-> Mailbox Init

				//read cmd from roveTCPHandler
				//The following call also opens implicitly task_sleeping, roveCmdCntrlTask
				//BIOS will next reawake roveCmdCntrlTask here, when roveTCPHandler Thread posts telem on this Mail.Semaphore.Pend
				//This is a RoverMotherboard.cfg object::		fromBaseMsg::		1024, max msg = 10

				Mailbox_pend(fromBaseStationMailbox, &fromBaseMsg, BIOS_WAIT_FOREVER);

				ms_delay( 1 );

				System_printf("2:	 Cmd Cntrl Just RECIEVED PENDED MAIL! ID: %d \n", fromBaseMsg.id);
				System_flush();

				ms_delay( 1 );

				switch(fromBaseMsg.id){

					//////////////
					// Drive Left
					//////////////

					//roveCom::	enum motor_left::	id = 100

					case motor_left:

						memset(&motor_control_struct.value, &fromBaseMsg.value, sizeof(struct motor_control_struct) );

						mux_1( 8 );
						mux_2( 7 );
						mux_3( 6 );

						send_struct(uart1, &motor_control_struct, motor_controller);
						send_struct(uart2, &motor_control_struct, motor_controller);
						send_struct(uart3, &motor_control_struct, motor_controller);

						ms_delay( 1 );
						System_printf("3:	Cmd Cntrl Just SENT UART 1, 2, 3! VALUE: %d \n", motor_control_struct.value);
						System_flush();
						ms_delay( 1 );

					break;

					//////////////
					// Drive Right
					//////////////

					//roveCom::	enum motor_right::	id = 101

					case motor_right:

						memset(&motor_control_struct.value, &fromBaseMsg.value, sizeof(struct motor_control_struct) );

						mux_1( 1 );
						mux_2( 2 );
						mux_3( 3 );

						send_struct(uart1, &motor_control_struct, motor_controller);
						send_struct(uart2, &motor_control_struct, motor_controller);
						send_struct(uart3, &motor_control_struct, motor_controller);

						ms_delay( 1 );
						System_printf("3:	Cmd Cntrl Just SENT UART 1, 2, 3! VALUE: %d \n", motor_control_struct.value);
						System_flush();
						ms_delay( 1 );

					break;

				}//endswitch::		(fromBaseMsg.id)

	}//endwhile:		(1)

	//postcondition: execution will not reach this state unless a serious error occurs

	System_printf("Rove Cmd Cntrl Task Error: Forced Exit\n");
	System_flush();

	//exit Task

	Task_exit();
*/
}//endfnct:		roveCmdCntrl() Task Thread