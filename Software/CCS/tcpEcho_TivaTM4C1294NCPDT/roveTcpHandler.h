/*
 * roveTcpHandler.h
 *
 *  Created on: Jan 22, 2015
 *      Author: owen
 */

#ifndef ROVETCPHANDLER_H_
#define ROVETCPHANDLER_H_


#include <string.h>

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

#define TCPPACKETSIZE 1024
#define NUMTCPWORKERS 3
#define TCPPORT 4500
#define NOT_CONNECTED 0
#define CONNECTED 1

typedef struct MsgObj {
	char message_body[TCPPACKETSIZE];
} MsgObj, *Msg;

Void roveTcpHandler(UArg arg0, UArg arg1);



#endif /* ROVETCPHANDLER_H_ */
