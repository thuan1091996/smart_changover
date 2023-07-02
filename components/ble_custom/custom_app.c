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

uint8_t UpdateCharData[BLE_WRITE_CHAR_ATT_MAX_LEN];
uint8_t NotifyCharData[BLE_READ_NOTIFY_CHAR_ATT_MAX_LEN];

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
/**
 * @brief Send device packet frame to APP
 */
void Device_PacketSend()
{
#if (BLE_TEST_NOTIFY == 1)
  sac_data_t sac_data =
  {
	.CMD_SL1 = 0x01,
	.CDL1 = 0x0203,
	.VDL1 = 0x0405,
	.PDL1 = 0x0607,

	.CMD_SL2 = 0x08,
	.CDL2 = 0x090a,
	.VDL2 = 0x0b0c,
	.PDL2 = 0x0d0e,

	.CMD_SL3 = 0x0f,
	.CDL3 = 0x1011,
	.VDL3 = 0x1213,
	.PDL3 = 0x1415,
  };
  uint8_t CMD_SYS = 0x16;
  uint8_t ASTA = 0x17;
  uint8_t BSTA = 0x18;
  uint16_t BLVL = 0x1920;
  uint16_t DTMP = 0x2122;
#endif  /* End of (BLE_TEST_NOTIFY == 1) */

  /* Format buffer with structure
   * CMD_SL1 | CDL1 | VDL1 | PDL1 | CMD_SL2 | CDL2 | VDL2 | PDL2 | CMD_SL3 | CDL3 | VDL3 | PDL3 
   * CMD_SYS | ASTA | BSTA | BLVL | DTMP
   */

  NotifyCharData[0] = sac_data.CMD_SL1;
  NotifyCharData[1] = sac_data.CDL1 >> 8;
  NotifyCharData[2] = sac_data.CDL1 & 0xFF;
  NotifyCharData[3] = sac_data.VDL1 >> 8;
  NotifyCharData[4] = sac_data.VDL1 & 0xFF;
  NotifyCharData[5] = sac_data.PDL1 >> 8;
  NotifyCharData[6] = sac_data.PDL1 & 0xFF;

  NotifyCharData[7] = sac_data.CMD_SL2;
  NotifyCharData[8] = sac_data.CDL2 >> 8;
  NotifyCharData[9] = sac_data.CDL2 & 0xFF;
  NotifyCharData[10] = sac_data.VDL2 >> 8;
  NotifyCharData[11] = sac_data.VDL2 & 0xFF;
  NotifyCharData[12] = sac_data.PDL2 >> 8;
  NotifyCharData[13] = sac_data.PDL2 & 0xFF;

  NotifyCharData[14] = sac_data.CMD_SL3;
  NotifyCharData[15] = sac_data.CDL3 >> 8;
  NotifyCharData[16] = sac_data.CDL3 & 0xFF;
  NotifyCharData[17] = sac_data.VDL3 >> 8;
  NotifyCharData[18] = sac_data.VDL3 & 0xFF;
  NotifyCharData[19] = sac_data.PDL3 >> 8;
  NotifyCharData[20] = sac_data.PDL3 & 0xFF;

#if (BLE_TEST_NOTIFY == 1)
  NotifyCharData[21] = CMD_SYS;
  NotifyCharData[22] = ASTA;
  NotifyCharData[23] = BSTA;
  NotifyCharData[24] = BLVL >> 8;
  NotifyCharData[25] = BLVL & 0xFF;
  NotifyCharData[26] = DTMP >> 8;
  NotifyCharData[27] = DTMP & 0xFF;
#endif  /* End of (BLE_TEST_NOTIFY == 1) */

  if(1 == Custom_App_Context.Char_read_notify_Notification_Status)
  {
    tBleStatus ret = ReadChar_SendNotify(NotifyCharData, MIN(sizeof(NotifyCharData), 27));
    if(ret != BLE_STATUS_SUCCESS)
    {
      APP_DBG_MSG("Send device packet failed, Error: %02X\n", ret);
    }
    else
    {
      APP_DBG_MSG("Send device packet success\n");
    }
  }
  else
  {
    APP_DBG_MSG("Send device packet failed, Notification disabled\n");
  }
  
}
/* USER CODE END FD_LOCAL_FUNCTIONS*/
