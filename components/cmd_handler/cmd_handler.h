/****************************************************************************
* Title                 :   command handler header file
* Filename              :   cmd_handler.h
* Author                :   ItachiVN
* Origin Date           :   2023/06/24
* Version               :   v0.0.0
* Compiler              :   STM32CubeIDE
* Target                :   STM32WB55
* Notes                 :   None
*****************************************************************************/

/*************** INTERFACE CHANGE LIST **************************************
*
*    Date    	Software Version    Initials   	Description
*  2023/06/24    v0.0.0         	ItachiVN      Interface Created.
*
*****************************************************************************/

/** \file cmd_handler.h
 *  \brief This module contains .
 *
 *  This is the header file for 
 */
#ifndef CMD_HANDLER_H_
#define CMD_HANDLER_H_

/******************************************************************************
* Includes
*******************************************************************************/
#include "app_common.h"
#include "dbg_trace.h"


/******************************************************************************
* Preprocessor Constants
*******************************************************************************/


/******************************************************************************
* Configuration Constants
*******************************************************************************/


/******************************************************************************
* Macros
*******************************************************************************/


/******************************************************************************
* Typedefs
*******************************************************************************/
typedef uint8_t (*cmd_handler)(void* p_param, void* p_param_len);

typedef enum 
{
    CMD_PRIO_SWTG_L1 = 1,
    CMD_PRIO_SWTG_L2,
    CMD_PRIO_SWTG_L3,
    CMD_INV_SWTG,
    CMD_GEN_SWTG,
    CMD_HS_SWTG,
    CMD_GEN_SWTG_TIME,
    CMD_INV_SWTG_TIME,
    CMD_SHUTDOWN,
    CMD_RESTART
} cmd_id_t;

typedef struct
{
    cmd_id_t    cmd_id;
    cmd_handler executer;
}cmd_t;

typedef enum 
{
	ERR_OK = 0,
	ERR_UNKNOWN, // Common error
	ERR_INVALID_PARAM,
	ERR_INVALID_CMD,
	ERR_INVALID_LEN,
	ERR_CMD_EXEC_FAIL,
	ERR_CMD_TIMEOUT,
}error_code_t; // User error codes
/******************************************************************************
* Variables
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/
error_code_t app_packet_handler(uint8_t* p_payload, uint16_t* p_len);

#ifdef __cplusplus
extern "C"{
#endif

void Function(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // CMD_HANDLER_H_

/*** End of File **************************************************************/
