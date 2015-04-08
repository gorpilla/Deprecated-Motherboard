// roveCmdCtrl.c
//
// first created:
//
// 01_22_2015_Owen_Chiaventone omc8db@mst.edu
//
// last edited:
//
// 03_08_2015 Connor Walsh cwd8d@mst.edu
//
// 04_07_2015_Judah Schad_jrs6w7@mst.edu
//
// this implements a single function BIOS thread that acts as the RoverMotherboard.cfg roveCmdCtrlTask handle
//
// recieves commands from roveTCPHandler in roveCom protocol using TI.Mailbox.from object
//
// sends pwm commands to motors, and uart commands to robot arm
//
// BIOS_start in main inits this as the roveCmdCntrlTask Thread
//
// this is a RoverMotherboard.cfg object::roveCmdCntrlTask::		priority 3, vital_flag = t, 2048 persistent private stack

#include "roveIncludes/roveWareHeaders/roveCmdCntrl.h"

Void roveCmdCntrl(UArg arg0, UArg arg1){

	//const FOREVER hack to kill the 'unreachable statement' compiler warning

	extern const uint8_t FOREVER;

	//initialized in main

	extern PWM_Handle motor_0;
	extern PWM_Handle motor_1;
	extern PWM_Handle motor_2;
	extern PWM_Handle motor_3;
	extern PWM_Handle motor_4;
	extern PWM_Handle motor_5;

	base_station_msg_struct fromBaseMsg;

	struct robot_arm_command robotArmCommand;

	char commandBuffer[MAX_COMMAND_SIZE + 4];

	int messageSize;

	int speed = 0;

					System_printf("roveCmdCntrlr		init! \n");
					System_flush();

	while (FOREVER){

		Mailbox_pend(fromBaseStationMailbox, &fromBaseMsg, BIOS_WAIT_FOREVER);

					//System_printf("2:	 Cmd Cntrl Just RECIEVED PENDED MAIL! ID: %d \n", fromBaseMsg.id);
					//System_flush();

		switch(fromBaseMsg.id){

			// case 0 hack to make a happy switch
			case 0:
			break;

/* This is for ASCII control only
 *
			case motor_left_id:

				// TODO implement correct Jack for Motor Comm Board

				//the left motors must be the negative of the right motors. Their phase is backwards

				speed = -((struct motor_control_struct*)(&fromBaseMsg))->speed;

				//protect from the max and min for the motorcontroller

				if (speed > 999){

					speed = 999;
				}//endif

				if (speed < -999){

						speed = -999;
				}//endif

				messageSize = generateMotorCommand(speed, commandBuffer);
				deviceWrite(ONBOARD_ROVECOMM, commandBuffer, (messageSize-1));

							System_printf("commandBuffer holds %s \n", commandBuffer);
							System_flush();

							System_printf("messageSize holds %d \n", messageSize);
							System_flush();

							System_printf("speed holds %d \n", speed);
							System_flush();

			break;

			//end drive motor_left_id with ASCII strings

			//case motor_right_id:

				speed = ((struct motor_control_struct*)(&fromBaseMsg))->speed;

				//protect from the max and min for the motorcontroller

				if (speed > 999){

					speed = 999;
				}//endif

				if (speed < -999){

						speed = -999;
				}//endif

				messageSize = generateMotorCommand(speed, commandBuffer);
				deviceWrite(ONBOARD_ROVECOMM, commandBuffer, (messageSize-1));

							System_printf("commandBuffer holds %s \n", commandBuffer);
							System_flush();

							System_printf("messageSize holds %d \n", messageSize);
							System_flush();

							System_printf("speed holds %d \n", speed);
							System_flush();

			break;

			// end drive motor_right_id with ASCII strings
*/

			case motor_left_id:

				//the left motors must be the negative of the right motors. Their phase is backwards

				speed = -( ((struct motor_control_struct*)(&fromBaseMsg))->speed );

							//System_printf("left speed before conversion holds %d \n", speed);

				//protect from the max and min for the motorcontroller

				if(speed > 999){

					speed = 999;
				}//endif

				if(speed < -999){

						speed = -999;
				}//endif

				pwmWrite(motor_0, speed);
				pwmWrite(motor_1, speed);
				pwmWrite(motor_2, speed);

			break;

			// end drive motor_left_id

			case motor_right_id:

				speed = ((struct motor_control_struct*)(&fromBaseMsg))->speed;

								//System_printf("right speed before conversion holds %d \n", speed);

				//protect from the max and min for the motorcontroller

				if(speed > 999){

					speed = 999;
				}//endif

				if(speed < -999){

						speed = -999;
				}//endif

				pwmWrite(motor_0, speed);
				pwmWrite(motor_1, speed);
				pwmWrite(motor_2, speed);

			break;

			//end drive motor_right_id

			case robot_arm_id:

				//base sends a single value int that we switch on to build the arm struct

				//zero out the struct

				robotArmCommand.wristCounterClockWise = 	0;
				robotArmCommand.wristClockWise = 			0;
				robotArmCommand.wristDown = 				0;
				robotArmCommand.wristUp = 					0;
				robotArmCommand.elbowDown = 				0;
				robotArmCommand.elbowUp =					0;
				robotArmCommand.elbowCounterClockWise = 	0;
				robotArmCommand.elbowClockWise = 			0;
				robotArmCommand.actuatorReverse = 			0;
				robotArmCommand.actuatorForward = 			0;
				robotArmCommand.baseCounterClockWise = 		0;
				robotArmCommand.baseClockWise = 			0;
				robotArmCommand.reset =						0;

				switch( (int)fromBaseMsg.value ){

					// defined 0
					case wrist_counter_clock_wise:

						robotArmCommand.wristCounterClockWise = 1;
					break;

					// defined 1
					case wrist_clock_wise:

						robotArmCommand.wristClockWise = 1;
					break;

					// defined 2
					case wrist_down:

						robotArmCommand.wristDown = 1;
					break;

					//defined 3
					case wrist_up:

						robotArmCommand.wristUp = 1;
					break;

					// defined 4
					case elbow_counter_clock_wise:

						robotArmCommand.elbowCounterClockWise = 1;
					break;

					// defined 5
					case elbow_clock_wise:

						robotArmCommand.elbowClockWise = 1;
					break;

					// defined 6
					case elbow_down:

						robotArmCommand.elbowDown = 1;
					break;

					// defined 7
					case elbow_up:

						robotArmCommand.elbowUp = 1;
					break;

					// defined 8
					case actuator_reverse:

						robotArmCommand.actuatorReverse = 1;
					break;

					// defined 9
					case actuator_forward:

						robotArmCommand.actuatorForward = 1;
					break;

					// defined 10
					case base_counter_clock_wise:

						robotArmCommand.baseCounterClockWise = 1;
					break;

					// defined 11
					case base_clock_wise:

						robotArmCommand.baseClockWise = 1;
					break;

					// defined 12
					case reset_arm:

						robotArmCommand.reset =	1;
					break;

					// end robot arm

				}//endswitch(fromBaseMsg.value)

				// using case fallthrough

				// might want to change this to fallthrough cases and save default for error case

				//case gripper_id:

				//case drill_id:

								//System_printf("\nDefault case reached in CmdCnt\n");
								//System_flush();

				// adds the start bytes, size byte, and checksum based on what struct id

				messageSize = buildSerialStructMessage((void *)&fromBaseMsg, commandBuffer);

								//System_printf("Message Size: %d\n", messageSize);
								//System_flush();

				// TODO change deviceJack = getDeviceJack(fromBaseMsg.id);

				// getDeviceJack(fromBaseMsg.id);

				int deviceJack = 1;

				deviceWrite(deviceJack, commandBuffer, messageSize);

			break;

			default:

				System_printf("Error: StructID cannot be handled");
				System_flush();

			break;

		}//endswitch(fromBaseMsg.id)

	}//endwhile(FOREVER)

	// postcondition: execution will not reach this state unless a serious error occurs

	System_printf("Rove Cmd Cntrl Task Error: Forced Exit\n");
	System_flush();

	// exit Task

	Task_exit();

}//endfnct:		roveCmdCntrl() Task Thread
