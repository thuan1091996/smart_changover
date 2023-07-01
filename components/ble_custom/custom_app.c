/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_app.c
  * @author  MCD Application Team
  * @brief   Custom Example Application (Server)
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "custom_app.h"
#include "custom_stm.h"
#include "cmd_handler.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  /* CUSTOM_SVC */
  uint8_t               Char_read_notify_Notification_Status;
  /* USER CODE BEGIN CUSTOM_APP_Context_t */

  /* USER CODE END CUSTOM_APP_Context_t */

  uint16_t              ConnectionHandle;
} Custom_App_Context_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

static Custom_App_Context_t Custom_App_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */

uint8_t UpdateCharData[247];
uint8_t NotifyCharData[247];

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* CUSTOM_SVC */
static void Custom_Char_read_notify_Update_Char(void);
static void Custom_Char_read_notify_Send_Notification(void);

/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_1 */

  /* USER CODE END CUSTOM_STM_App_Notification_1 */
  switch (pNotification->Custom_Evt_Opcode)
  {
    /* USER CODE BEGIN CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

    /* USER CODE END CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

    /* CUSTOM_SVC */
    case CUSTOM_STM_CHAR_READ_NOTIFY_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_CHAR_READ_NOTIFY_READ_EVT */
    	APP_DBG_MSG(">>== READ EVENT FROM GATT CLIENT \n");

      /* USER CODE END CUSTOM_STM_CHAR_READ_NOTIFY_READ_EVT */
      break;

    case CUSTOM_STM_CHAR_READ_NOTIFY_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_CHAR_READ_NOTIFY_NOTIFY_ENABLED_EVT */
    	APP_DBG_MSG(">>== Notification Enable \n");

    	Custom_App_Context.Char_read_notify_Notification_Status = 1;
      /* USER CODE END CUSTOM_STM_CHAR_READ_NOTIFY_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_CHAR_READ_NOTIFY_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_CHAR_READ_NOTIFY_NOTIFY_DISABLED_EVT */
    	APP_DBG_MSG(">>== Notification disabled \n");

    	Custom_App_Context.Char_read_notify_Notification_Status = 0;
      /* USER CODE END CUSTOM_STM_CHAR_READ_NOTIFY_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_CHAR_WRITE_WRITE_EVT:
      /* USER CODE BEGIN CUSTOM_STM_CHAR_WRITE_WRITE_EVT */


#if (CFG_DEBUG_APP_TRACE != 0)
    {
		#define MSG_DEBUG_MAX_LEN	100
        char debug_msg_buffer[MSG_DEBUG_MAX_LEN]={0};
		APP_DBG_MSG("APP NOTIFICATION WRITE EVT: Received %d B \n", pNotification->DataTransfered.Length);
        for(uint8_t i=0; (i < pNotification->DataTransfered.Length) && (i < MSG_DEBUG_MAX_LEN/2); i++)
          sprintf(&debug_msg_buffer[i*2], "%02X", pNotification->DataTransfered.pPayload[i]);
        APP_DBG_MSG("Payload: %s \n", (char*)debug_msg_buffer);
    }
#endif  /* End of CFG_DEBUG_APP_TRACE */
		app_packet_handler(pNotification->DataTransfered.pPayload, &pNotification->DataTransfered.Length);



      /* USER CODE END CUSTOM_STM_CHAR_WRITE_WRITE_EVT */
      break;

    default:
      /* USER CODE BEGIN CUSTOM_STM_App_Notification_default */

      /* USER CODE END CUSTOM_STM_App_Notification_default */
      break;
  }
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_2 */

  /* USER CODE END CUSTOM_STM_App_Notification_2 */
  return;
}

void Custom_APP_Notification(Custom_App_ConnHandle_Not_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_APP_Notification_1 */

  /* USER CODE END CUSTOM_APP_Notification_1 */

  switch (pNotification->Custom_Evt_Opcode)
  {
    /* USER CODE BEGIN CUSTOM_APP_Notification_Custom_Evt_Opcode */

    /* USER CODE END P2PS_CUSTOM_Notification_Custom_Evt_Opcode */
    case CUSTOM_CONN_HANDLE_EVT :
      /* USER CODE BEGIN CUSTOM_CONN_HANDLE_EVT */

      /* USER CODE END CUSTOM_CONN_HANDLE_EVT */
      break;

    case CUSTOM_DISCON_HANDLE_EVT :
      /* USER CODE BEGIN CUSTOM_DISCON_HANDLE_EVT */

      /* USER CODE END CUSTOM_DISCON_HANDLE_EVT */
      break;

    default:
      /* USER CODE BEGIN CUSTOM_APP_Notification_default */

      /* USER CODE END CUSTOM_APP_Notification_default */
      break;
  }

  /* USER CODE BEGIN CUSTOM_APP_Notification_2 */

  /* USER CODE END CUSTOM_APP_Notification_2 */

  return;
}

void Custom_APP_Init(void)
{
  /* USER CODE BEGIN CUSTOM_APP_Init */

  /* USER CODE END CUSTOM_APP_Init */
  return;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

/* CUSTOM_SVC */
void Custom_Char_read_notify_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Char_read_notify_UC_1*/

  /* USER CODE END Char_read_notify_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_CHAR_READ_NOTIFY, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Char_read_notify_UC_Last*/

  /* USER CODE END Char_read_notify_UC_Last*/
  return;
}

void Custom_Char_read_notify_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Char_read_notify_NS_1*/

  /* USER CODE END Char_read_notify_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_CHAR_READ_NOTIFY, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Char_read_notify_NS_Last*/

  /* USER CODE END Char_read_notify_NS_Last*/

  return;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/

/* USER CODE END FD_LOCAL_FUNCTIONS*/
