/*
 * roveComTest.c
 *
 *  Created on: Mar 23, 2015
 *      Author: Connor Walsh
 */

#include "roveIncludes/roveWareHeaders/roveComTest.h"

void roveComTest(UArg arg0, UArg arg1)
{
	base_station_msg_struct baseStationMsg;

	struct mobo_robo_arm_command robotArmStruct;

	while (1)
	{
		Task_sleep(5000);

		robotArmStruct.struct_id = mobo_robo_arm_command;
		robotArmStruct.reset = 0;
		robotArmStruct.wristUp = 1;
		robotArmStruct.wristDown = 2;
		robotArmStruct.wristClockWise = 3;
		robotArmStruct.wristCounterClockWise = 4;
		robotArmStruct.elbowUp = 5;
		robotArmStruct.elbowDown = 6;
		robotArmStruct.elbowClockWise = 7;
		robotArmStruct.elbowCounterClockWise = 8;
		robotArmStruct.actuatorForward = 9;
		robotArmStruct.actuatorReverse = 10;
		robotArmStruct.baseClockWise = 11;
		robotArmStruct.baseCounterClockWise = 12;

		memcpy(&(baseStationMsg.value), &robotArmStruct, sizeof(robotArmStruct));
		baseStationMsg.id = robot_arm;

		Mailbox_post(fromBaseStationMailbox, &baseStationMsg, BIOS_WAIT_FOREVER);
	}
}




