/*******************************************************************************
* Title                 :   command handler
* Filename              :   cmd_handler.c
* Author                :   ItachiVN
* Origin Date           :   2023/06/24
* Version               :   0.0.0
* Compiler              :   STM32CubeIDE
* Target                :   STM32WB55
* Notes                 :   None
*******************************************************************************/

/*************** MODULE REVISION LOG ******************************************
*
*    Date       Software Version	Initials	Description
*  2023/06/24       0.0.0	         ItachiVN      Module Created.
*
*******************************************************************************/

/** \file cmd_handler.c
 *  \brief This module contains the
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "cmd_handler.h"

#include "custom_app.h"

#include "FreeRTOS.h"
/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define FIELD_SIZEOF(t, f) (sizeof(((t*)0)->f))


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Function Prototypes
*******************************************************************************/
uint8_t Priority_Switching_L1_Handler(uint8_t* p_param, uint16_t* p_param_len);
uint8_t Priority_Switching_L2_Handler(uint8_t* p_param, uint16_t* p_param_len);
uint8_t Priority_Switching_L3_Handler(uint8_t* p_param, uint16_t* p_param_len);
uint8_t Inverter_Switching_Handler(uint8_t* p_param, uint16_t* p_param_len);
uint8_t Generator_Switching_Handler(uint8_t* p_param, uint16_t* p_param_len);
uint8_t House_Output_Switching_Handler(uint8_t* p_param, uint16_t* p_param_len);
uint8_t Set_Time_Generator_Switching_Handler(uint8_t* p_param, uint16_t* p_param_len);
uint8_t Set_Time_Inverter_Switching_Handler(uint8_t* p_param, uint16_t* p_param_len);
uint8_t Shutdown_SAC_Handler(uint8_t* p_param, uint16_t* p_param_len);
uint8_t Restart_SAC_Handler(uint8_t* p_param, uint16_t* p_param_len);


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static cmd_t app_cmd_table[] = 
{
    {.cmd_id = CMD_PRIO_SWTG_L1,    .executer = (cmd_handler)&Priority_Switching_L1_Handler},
    {.cmd_id = CMD_PRIO_SWTG_L2,    .executer = (cmd_handler)&Priority_Switching_L2_Handler},
    {.cmd_id = CMD_PRIO_SWTG_L3,    .executer = (cmd_handler)&Priority_Switching_L3_Handler},
    {.cmd_id = CMD_INV_SWTG,        .executer = (cmd_handler)&Inverter_Switching_Handler},
    {.cmd_id = CMD_GEN_SWTG,        .executer = (cmd_handler)&Generator_Switching_Handler},
    {.cmd_id = CMD_HS_SWTG,         .executer = (cmd_handler)&House_Output_Switching_Handler},
    {.cmd_id = CMD_GEN_SWTG_TIME,   .executer = (cmd_handler)&Set_Time_Generator_Switching_Handler},
    {.cmd_id = CMD_INV_SWTG_TIME,   .executer = (cmd_handler)&Set_Time_Inverter_Switching_Handler},
    {.cmd_id = CMD_SHUTDOWN,        .executer = (cmd_handler)&Shutdown_SAC_Handler},
    {.cmd_id = CMD_RESTART,         .executer = (cmd_handler)&Restart_SAC_Handler},
};


/******************************************************************************
* Function Definitions
*******************************************************************************/
error_code_t cmd_process(uint8_t* p_payload, uint16_t* p_len)
{
    assert(p_payload != NULL);
    assert(p_len != NULL);
    if(*p_len < 1)
        return ERR_INVALID_LEN;
    /* Lookup command */
    for(uint8_t idx=0; idx < (sizeof(app_cmd_table) / sizeof(app_cmd_table[0])); idx++)
    { 
        /* First byte as cmd id */
        if(app_cmd_table[idx].cmd_id == *((uint8_t*)p_payload))
        {
            /* Found -> execute command */
            uint8_t cmd_param_idx = FIELD_SIZEOF(cmd_t, cmd_id); // Get param idx
            *p_len -= cmd_param_idx;                             // Get param length (exclude cmd id)
            error_code_t status = app_cmd_table[idx].executer(&p_payload[cmd_param_idx], p_len); // @note: update p_len if cmd have output data
            return status;
        }
    }
    return ERR_INVALID_CMD;
}

error_code_t app_packet_handler(uint8_t* p_payload, uint16_t* p_len)
{
    assert(p_payload != NULL);
    assert(p_len != NULL);
    if(*p_len < 1)
        return ERR_INVALID_LEN;
    if(p_payload[0] != DEV_ID)
        return ERR_INVALID_PARAM;
        
    /* Process command */
    *p_len -= 1; // Exclude dev id
    error_code_t status = cmd_process(&p_payload[1], p_len);
    if(status != ERR_OK)
        APP_DBG_MSG("CMD PROCESS ERR: %d \n", status);
    return status;
} 

uint8_t Priority_Switching_L1_Handler(uint8_t* p_param, uint16_t* p_param_len)
{
    assert(p_param != NULL);
    assert(p_param_len != NULL);
    uint8_t status = ERR_OK;
    APP_DBG_MSG("on Priority_Switching_L1_Handler \n");
    uint16_t len_input = *p_param_len;
    if(len_input != 1)
        return ERR_INVALID_LEN;
    // TODO: IMPLEMENT
    return status;
}

uint8_t Priority_Switching_L2_Handler(uint8_t* p_param, uint16_t* p_param_len)
{
    assert(p_param != NULL);
    assert(p_param_len != NULL);
    uint8_t status = ERR_OK;
    APP_DBG_MSG("on Priority_Switching_L2_Handler \n");
    uint16_t len_input = *p_param_len;
    if(len_input != 1)
        return ERR_INVALID_LEN;
    // TODO: IMPLEMENT
    return status;
}

uint8_t Priority_Switching_L3_Handler(uint8_t* p_param, uint16_t* p_param_len)
{
    assert(p_param != NULL);
    assert(p_param_len != NULL);
    uint8_t status = ERR_OK;
    APP_DBG_MSG("on Priority_Switching_L3_Handler \n");
    uint16_t len_input = *p_param_len;
    if(len_input != 1)
        return ERR_INVALID_LEN;
    // TODO: IMPLEMENT
    return status;
}

uint8_t Inverter_Switching_Handler(uint8_t* p_param, uint16_t* p_param_len)
{
    assert(p_param != NULL);
    assert(p_param_len != NULL);
    uint8_t status = ERR_OK;
    APP_DBG_MSG("on Inverter_Switching_Handler \n");
    uint16_t len_input = *p_param_len;
    if(len_input != 1)
        return ERR_INVALID_LEN;
    // TODO: IMPLEMENT
    return status;
}

uint8_t Generator_Switching_Handler(uint8_t* p_param, uint16_t* p_param_len)
{
    assert(p_param != NULL);
    assert(p_param_len != NULL);
    uint8_t status = ERR_OK;
    APP_DBG_MSG("on Generator_Switching_Handler \n");
    uint16_t len_input = *p_param_len;
    if(len_input != 1)
        return ERR_INVALID_LEN;
    // TODO: IMPLEMENT
    return status;
}

uint8_t House_Output_Switching_Handler(uint8_t* p_param, uint16_t* p_param_len)
{
    assert(p_param != NULL);
    assert(p_param_len != NULL);
    uint8_t status = ERR_OK;
    APP_DBG_MSG("on House_Output_Switching_Handler \n");
    uint16_t len_input = *p_param_len;
    if(len_input != 1)
        return ERR_INVALID_LEN;
    // TODO: IMPLEMENT
    return status;
}

uint8_t Shutdown_SAC_Handler(uint8_t* p_param, uint16_t* p_param_len)
{
    assert(p_param != NULL);
    assert(p_param_len != NULL);
    uint8_t status = ERR_OK;
    APP_DBG_MSG("on Shutdown_SAC_Handler \n");
    uint16_t len_input = *p_param_len;
    if(len_input != 1)
        return ERR_INVALID_LEN;
    // TODO: IMPLEMENT
    return status;
}

uint8_t Restart_SAC_Handler(uint8_t* p_param, uint16_t* p_param_len)
{
    assert(p_param != NULL);
    assert(p_param_len != NULL);
    uint8_t status = ERR_OK;
    APP_DBG_MSG("on Restart_SAC_Handler \n");
    uint16_t len_input = *p_param_len;
    if(len_input != 1)
        return ERR_INVALID_LEN;
    // TODO: IMPLEMENT
    return status;
}

uint8_t Set_Time_Generator_Switching_Handler(uint8_t* p_param, uint16_t* p_param_len)
{
    assert(p_param != NULL);
    assert(p_param_len != NULL);
    uint8_t status = ERR_OK;
    APP_DBG_MSG("on Set_Time_Generator_Switching_Handler \n");
    uint16_t len_input = *p_param_len;
    if(len_input != 4)
        return ERR_INVALID_LEN;
    // TODO: IMPLEMENT
    return status;
}

uint8_t Set_Time_Inverter_Switching_Handler(uint8_t* p_param, uint16_t* p_param_len)
{
    assert(p_param != NULL);
    assert(p_param_len != NULL);
    uint8_t status = ERR_OK;
    APP_DBG_MSG("on Set_Time_Inverter_Switching_Handler \n");
    uint16_t len_input = *p_param_len;
    if(len_input != 4)
        return ERR_INVALID_LEN;
    // TODO: IMPLEMENT
    return status;
}
