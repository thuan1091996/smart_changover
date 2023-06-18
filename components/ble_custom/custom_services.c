/*
 * app_custom_services.c
 *
 */

#include "stdbool.h"
#include "stdint.h"

#include "common_blesvc.h"

#include "custom_stm.h"


/* Overwrite */
void SVCCTL_InitCustomSvc( void )
{
	Custom_STM_Service_Init();
}

