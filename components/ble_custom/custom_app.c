/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : custom_app.c
 * Description        : Custom Example Application (Server)
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define CUSTOM_APP_MAX_LEN_FRAME	255

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  /* USER CODE BEGIN CUSTOM_APP_Context_t */
  uint8_t  FrameData[CUSTOM_APP_MAX_LEN_FRAME];
  uint16_t FrameLength;
  Custom_App_ConnHandle_Not_evt_t *CustomAppConnection;
  /* USER CODE END CUSTOM_APP_Context_t */
} Custom_App_Context_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private function prototypes -----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

PLACE_IN_SECTION("BLE_APP_CONTEXT") static Custom_App_Context_t Custom_App_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */

/* USER CODE BEGIN PV */
static tCustomAppNotification Custom_App_EventHandle_T = {0};

/* USER CODE END PV */

/* Functions Definition ------------------------------------------------------*/
void Custom_APP_Init(tCustomAppNotification EventHandle)
{
	Custom_App_EventHandle_T = EventHandle;
	return;
}

void Custom_APP_Notification(const Custom_App_ConnHandle_Not_evt_t *pNotification)
{
	APP_DBG_MSG("-- Custom_APP_Notification: TODO \n");

	switch(pNotification->Custom_Evt_Opcode)
	{
		case CUSTOM_DISCON_HANDLE_EVT:
		case CUSTOM_CONN_HANDLE_EVT:
		{
			Custom_App_Context.CustomAppConnection = (Custom_App_ConnHandle_Not_evt_t *)pNotification;

			if (Custom_App_EventHandle_T.Events)
				Custom_App_EventHandle_T.Events(pNotification->Custom_Evt_Opcode);
		}
		break;

		default:
		break;
	}

	return;
}

/* Calls the service function to update the characteristic (notify) */
void Custom_APP_Send_Notification (uint8_t *data, uint8_t Len)
{
	APP_DBG_MSG("Custom_APP_Send_Notification \n");

	Custom_App_Opcode_Notification_evt_t OpCode = CUSTOM_STM_NOTIFY_DATAFRAME;
	switch (OpCode)
	{
		case CUSTOM_STM_NOTIFY_DATAFRAME:
			memcpy(Custom_App_Context.FrameData, data, Len);
			Custom_App_Context.FrameLength = Len;
			Custom_STM_App_Update_Char(CUSTOM_STM_NOTIFY_DATAFRAME, Custom_App_Context.FrameData, Custom_App_Context.FrameLength);
			break;

		default:
			break;
	}
}

void Custom_APP_Context_UpdateDataFrame(uint8_t *data, uint8_t Len)
{
	if (0 != Len)
	{
		memcpy(Custom_App_Context.FrameData, data, Len);
		Custom_App_Context.FrameLength = Len;

		Custom_STM_App_Update_Char(CUSTOM_STM_WRITE_DATAFRAME, Custom_App_Context.FrameData, Custom_App_Context.FrameLength);
	}
	else
	{
		Custom_App_Context.FrameLength = 0;
	}
}

/* Overwrite & called by Low Layer, Custom_STM.h */
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification)
{
	APP_DBG_MSG("-- Custom_STM_App_Notification:\n");

	switch(pNotification->Custom_Evt_Opcode)
	{
		case CUSTOM_STM_DATAFRAME_READ_EVT:
			APP_DBG_MSG("CUSTOM_STM_DATAFRAME_READ_EVT");
			break;

		case CUSTOM_STM_DATAFRAME_WRITE_EVT:
			APP_DBG_MSG("CUSTOM_STM_DATAFRAME_WRITE_EVT");

			if (Custom_App_EventHandle_T.Resquest)
			{
				Custom_App_EventHandle_T.Resquest(pNotification->DataTransfered.pPayload, pNotification->DataTransfered.Length);
			}
			break;

		case CUSTOM_STM_PERMIT_READ_EVT:
			APP_DBG_MSG("CUSTOM_STM_PERMIT_READ_EVT");

			if (Custom_App_EventHandle_T.Response)
			{
				Custom_App_EventHandle_T.Response(pNotification->DataTransfered.pPayload, &pNotification->DataTransfered.Length);

				if (!pNotification->DataTransfered.Length)
					aci_gatt_deny_read(Custom_App_Context.CustomAppConnection->ConnectionHandle,CUSTOM_APP_ERROR_NOT_FOUND_DATA);
				else
					aci_gatt_allow_read(Custom_App_Context.CustomAppConnection->ConnectionHandle);
			}
			else
			{
				aci_gatt_deny_read(Custom_App_Context.CustomAppConnection->ConnectionHandle,CUSTOM_APP_ERROR_NOT_FOUND_HANDLE);
			}
			break;

		case CUSTOM_STM_DATAFRAME_NOTIFY_ENABLED_EVT:
			APP_DBG_MSG("CUSTOM_STM_DATAFRAME_NOTIFY_ENABLED_EVT");
			break;

		case CUSTOM_STM_DATAFRAME_NOTIFY_DISABLED_EVT:
			APP_DBG_MSG("CUSTOM_STM_DATAFRAME_NOTIFY_DISABLED_EVT");
			break;

		case CUSTOM_STM_DATASTATUS_READ_EVT:
			break;

		case CUSTOM_STM_DATASTATUS_NOTIFY_ENABLED_EVT:
			APP_DBG_MSG("CUSTOM_STM_DATASTATUS_NOTIFY_ENABLED_EVT");
			break;

		case CUSTOM_STM_DATASTATUS_NOTIFY_DISABLED_EVT:
			APP_DBG_MSG("CUSTOM_STM_DATASTATUS_NOTIFY_DISABLED_EVT");
			break;

		default:
			break;
	}
	return;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/

/* USER CODE END FD_LOCAL_FUNCTIONS*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
