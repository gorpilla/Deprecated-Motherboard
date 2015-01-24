/*
 * roveTcpHandler.c
 *
 *  Created on: Jan 22, 2015
 *      Author: Owen Chiaventone, Judah Schad, Connor Walsh
 *
 *  Deals with maintaining a connection to the base station
 *  more detailed documentation is available on the motherboard wiki
 */

#include "roveTcpHandler.h"

Void roveTcpHandler(UArg arg0, UArg arg1)
{

	System_printf("Tcp Handler Task Created\n");
	System_flush();

	//---- This command creates
	//     a file descriptor environment
	//     This is required to have a place to put the sockets
	//     The socket descriptors are integers that reference objects
	//     In this file descriptor environment
	//
	//     By default, this environment has space to hold up to 16 file
	//     descriptors concurrently. Calls to socket() and accept() will
	//     allocate one of these spaces to a socket object.
	//     Close(int socket) can be used to free one of these spaces
	fdOpenSession(TaskSelf());

	//----Allocate variables
    int                status;

    //These are going to be references to sockets
    int                clientfd;
    int                listen_socket;

    //These will be used to configure the sockets.
    //This configuration is very similar to BSD sockets.
    struct sockaddr_in localAddr;
    struct sockaddr_in clientAddr;
    int                optval;
    int                optlen = sizeof(optval);
    socklen_t          addrlen = sizeof(clientAddr);
    int                connectedFlag = NOT_CONNECTED; //Used to indicate if the system is connected to RED
    int                bytesReceived = 0; //Used for detecting errors in receiving
    int                bytesSent; //Used for echoing data
    MsgObj             fromBaseMsg;

    //char fromBaseMsg.message_body[TCPPACKETSIZE]; //Where we hold incoming data

    //----Create socket that will be used to listen
    listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listen_socket == -1) {
            System_printf("Error: socket not created.\n");
        }

    //----Define Socket options
	memset(&localAddr, 0, sizeof(localAddr));
	//Use IPv4
	localAddr.sin_family = AF_INET;
	//Allow any IP to connect
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//Connect on port TCPPORT
	localAddr.sin_port = htons(TCPPORT);

	//---- Bind and Listen on listen_socket. This will allow the socket to
	//     see when something tries to connect
    status = bind(listen_socket, (struct sockaddr *)&localAddr, sizeof(localAddr));
    if (status == -1) {
        System_printf("Error: bind failed.\n");
    }

    status = listen(listen_socket, NUMTCPWORKERS);
    if (status == -1) {
        System_printf("Error: listen failed.\n");
    }

    //---- Apply the socket options set up above to the listening socket
    if (setsockopt(listen_socket, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
        System_printf("Error: setsockopt failed\n");
    }

    //Preset the connection status for the first time through the loop
    connectedFlag = CONNECTED;

    System_printf("Network Setup Completed\n\n");
    System_flush();
    //---- The main loop
    //---- precondition: listen_socket is configured and listening on port TCPPORT
    while(1)
    {
    	//Block until a connection is available
    	//When it is, pass it off to the socket clientfd
    	System_printf("Waiting to Accept() new connection\n");
    	System_flush();
    	clientfd = accept(listen_socket, (struct sockaddr *)&clientAddr, &addrlen);
    	if(clientfd < 0)
    	{
    		connectedFlag = NOT_CONNECTED;
    		System_printf("Error: accept() failed\n");
    		System_flush();
    	} else
    	{
    		connectedFlag = CONNECTED;
    		System_printf("Connected to RED\n");
    		System_flush();
    	}

    	//At this point, we know we have a valid connection
    	while(connectedFlag == CONNECTED)
    	{
    		//Pend on a new event being available on the socket
    		System_printf("Connected. Waiting for data\n");
    		System_flush();
    		bytesReceived = recv(clientfd, fromBaseMsg.message_body, TCPPACKETSIZE, 0);

    		//Check if the connection broke
    		if(bytesReceived<0)
    		{
    			connectedFlag = NOT_CONNECTED;
    			System_printf("Connection lost. Reconnecting (src = recv)\n");
    			System_flush();
    		} else
    		{
    			//At this point, we know that the connection is valid and we have data waiting

    			//Dump data to debugging console
    			System_printf("Received data: %s\n", fromBaseMsg.message_body);
    			System_flush();

    			//Put the data in a mailbox
    			Mailbox_post(fromBaseStationMailbox, &fromBaseMsg, BIOS_WAIT_FOREVER);

    			//Echo the data back
    	        bytesSent = send(clientfd, fromBaseMsg.message_body, bytesReceived, 0);
    	        if (bytesSent < 0 || bytesSent != bytesReceived) {
    	            System_printf("Connection lost. Reconnecting (src = send)\n");
    	            connectedFlag = NOT_CONNECTED;
    	        } // endif

    		} // endelse

    	} // end while(connectedFlag == CONNECTED)

    	//If execution reaches this point, then the connection has broken
    	close(clientfd);
    } //end while(1)
    //---- postcondition: Execution will not reach this state unless a serious error occurs

    //Close the file descriptor environment opened at the start of the task
    fdCloseSession(TaskSelf());

	System_printf("Tcp Handler Task Exit\n");
	System_flush();
}


