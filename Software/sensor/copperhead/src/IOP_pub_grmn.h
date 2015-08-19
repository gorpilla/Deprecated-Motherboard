/*********************************************************************
*
*   HEADER NAME:
*       IOP_pub_grmn.h - Garmin Format Definitions
*
* Copyright 2007 by Garmin Ltd. or its subsidiaries.
*---------------------------------------------------------------------
* $Log$
* $NoKeywords$
*********************************************************************/

#if !defined _IOP_PUB_GRMN_H
#define _IOP_PUB_GRMN_H

/*--------------------------------------------------------------------
                           GENERAL INCLUDES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/

/*----------------------------------------------------------
Garmin port definations
----------------------------------------------------------*/
typedef uint8 IOP_grmn_port_type;

typedef struct
    {
    uint8                   source_addr;
    IOP_grmn_port_type      source_port;
    IOP_grmn_port_type      dest_port;
    } IOP_route_type;

/*--------------------------------------------------------------------
                           PROJECT INCLUDES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                           MEMORY CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              VARIABLES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                MACROS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              PROCEDURES
--------------------------------------------------------------------*/

#endif /* _IOP_PUB_GRMN_H */
