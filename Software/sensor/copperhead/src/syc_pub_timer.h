/*********************************************************************
*
*   HEADER NAME:
*       SYC_pub_timer.h - System Controller Timer Public Declarations
*
*   NOTES:
*       This file is where public timers interface are listed.
*
*   Copyright 2002-2008 by Garmin Ltd. or its subsidiaries.
*---------------------------------------------------------------------
* $Log$
* $NoKeywords$
*********************************************************************/

#if !defined _SYC_PUB_TIMER_
#define      _SYC_PUB_TIMER_

/*--------------------------------------------------------------------
                            GENERAL INCLUDES
--------------------------------------------------------------------*/

//#include "SYC_pub_timer_prj_s.h"

/*--------------------------------------------------------------------
                            LITERAL CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                 TYPES
--------------------------------------------------------------------*/

/*----------------------------------------------------------
This type defines how timer count values are stored.
----------------------------------------------------------*/
typedef uint16 SYC_timer_tick_type;

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
void syc_init_timer
    (
    void
    );

SYC_timer_tick_type SYC_get_tick_count
    (
    void
    );
#endif  /* _SYC_PUB_TIMER_ */
