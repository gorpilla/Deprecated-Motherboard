/*
 * roveTelemController.h
 *
 *  Created on: Feb 14, 2015
 *      Author: mrdtdev
 */

#ifndef ROVETELEMCONTROLLER_H_
#define ROVETELEMCONTROLLER_H_

#include <string.h>
#include <stdbool.h>
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/GPIO.h>

/* NDK BSD support */
#include <sys/socket.h>

/* Example/Board Header file */
#include "Board.h"

/* Mailbox Support */
#include <ti/sysbios/knl/Mailbox.h>

/* Defines communication interface with Network Task */
#include "roveTcpHandler.h"
#include "structs.h"
#include "struct_xfer.h"

Void roveTelemController(UArg arg0, UArg arg1);

#endif /* ROVETELEMCONTROLLER_H_ */
