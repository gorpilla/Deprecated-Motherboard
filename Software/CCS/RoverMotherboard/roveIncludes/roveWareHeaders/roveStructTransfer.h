/*
 * roveStructTransfer.h
 *
 *  Created on: Mar 7, 2015
 *      Author: Connor Walsh
 */

#pragma once

#ifndef ROVESTRUCTTRANSFER_H_
#define ROVESTRUCTTRANSFER_H_

// globally scoped Texas Instruments (TI) headers

#include "../RoverMotherboardMain.h"

// MRDesign Team::roveWare::		roveCom and RoveNet services headers

#include "../mrdtRoveWare.h"

/*enum peripheral_devices{motor_controller, bms, robotic_arm, tcp_cmd, drill, gripper, science_payload, gps, lighting_board, camera, power_board, test};

enum peripheral_struct_ids{
		blank_id_zero = 0,
		mobo_identify_req = 1,
		dev_identify_reply = 2,
		mobo_begin_op_req,
		dev_begin_op_reply,
		mobo_telem_req,
		dev_command_reply = 6,
		blank_id_seven,
		mobo_robo_arm_command,
		mobo_gripper_command,
		mobo_drill_command
};*/


// used in Hardware tester, probably need to change to lowercase version
uint8_t CalcCheckSum(const void* my_struct, uint8_t size);

// XOR checksum calculation for a struct
uint8_t calcCheckSum(const void* my_struct, uint8_t size);

// Pre: buffer must be of size(my_struct) + 4 bytes (start bytes, size, and checksum)
int buildSerialStructMessage(void* my_struct, char* buffer);

// Pre: is a buffer containing a message in easyTransfer form

#endif /* ROVESTRUCTTRANSFER_H_ */
