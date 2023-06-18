/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : custom_stm.c
 * Description        : Custom Example Service.
 *
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
#include "custom_stm.h"
#include "common_blesvc.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct{
  uint16_t CustomAppServiceHdle;   /**< AppService handle */
  uint16_t CustomDataFrameHdle;    /**< DataFrame handle */
  uint16_t CustomDataStatusHdle;   /**< DataStatus handle */
} CustomContext_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define UUID_128_SUPPORTED  1

#if (UUID_128_SUPPORTED == 1)
#define BM_UUID_LENGTH  UUID_TYPE_128
#else
#define BM_UUID_LENGTH  UUID_TYPE_16
#endif

#define BM_REQ_CHAR_SIZE    (3)

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
#define CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET         2
#define CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET              1
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_DRIVER_CONTEXT
 */
PLACE_IN_SECTION("BLE_DRIVER_CONTEXT") static CustomContext_t CustomContext;

/**
 * END of Section BLE_DRIVER_CONTEXT
 */

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static SVCCTL_EvtAckStatus_t Custom_STM_Event_Handler(void *pckt);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
/* USER CODE BEGIN PFD */

/* USER CODE END PFD */

/* Private functions ----------------------------------------------------------*/

#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
        uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
            uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
                uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
} while (0)

/* Hardware Characteristics Service */
/*
 The following 128bits UUIDs have been generated from the random UUID
 generator:
 D973F2E0-B19E-11E2-9E96-0800200C9A66: Service 128bits UUID
 D973F2E1-B19E-11E2-9E96-0800200C9A66: Characteristic_1 128bits UUID
 D973F2E2-B19E-11E2-9E96-0800200C9A66: Characteristic_2 128bits UUID
 */
#define COPY_SMART_CHANGEOVER_SERVICE_UUID(uuid_struct)	COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x00,0x00,0x01,0x11,0xe1,0x9a,0xb4,0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_DATAFRAME_UUID(uuid_struct)				COPY_UUID_128(uuid_struct,0x00,0x00,0xaa,0xcc,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_DATASTATUS_UUID(uuid_struct)				COPY_UUID_128(uuid_struct,0x00,0x04,0x00,0x00,0x00,0x01,0x11,0xe1,0xac,0x36,0x00,0x02,0xa5,0xd5,0xc5,0x1b)

/* USER CODE BEGIN PF */

/* USER CODE END PF */

/**
 * @brief  Event handler
 * @param  Event: Address of the buffer holding the Event
 * @retval Ack: Return whether the Event has been managed or not
 */
static SVCCTL_EvtAckStatus_t Custom_STM_Event_Handler(void *Event)
{
  SVCCTL_EvtAckStatus_t return_value;
  hci_event_pckt *event_pckt;
  evt_blecore_aci *blecore_evt;
  aci_gatt_attribute_modified_event_rp0 *attribute_modified;
  Custom_STM_App_Notification_evt_t     Client_Request;

  return_value = SVCCTL_EvtNotAck;
  event_pckt = (hci_event_pckt *)(((hci_uart_pckt*)Event)->data);

  switch(event_pckt->evt)
  {
  	  case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
      blecore_evt = (evt_blecore_aci*)event_pckt->data;
      switch(blecore_evt->ecode)
      {
		  case ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE:
			  attribute_modified = (aci_gatt_attribute_modified_event_rp0*)blecore_evt->data;

			  /* --------------------Data status characteristic handler --------------------*/
			  if(attribute_modified->Attr_Handle == (CustomContext.CustomDataStatusHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
			  {
				  return_value = SVCCTL_EvtAckFlowEnable;
				  switch(attribute_modified->Attr_Data[0])
				  {
					  /* Disabled Client_Request management */
					  case (!(COMSVC_Notification)):
						  Client_Request.Custom_Evt_Opcode = CUSTOM_STM_DATASTATUS_NOTIFY_DISABLED_EVT;
						  Custom_STM_App_Notification(&Client_Request);
					  break;

					  /* Enabled Client_Request management */
					  case COMSVC_Notification:
						  Client_Request.Custom_Evt_Opcode = CUSTOM_STM_DATASTATUS_NOTIFY_ENABLED_EVT;
						  Custom_STM_App_Notification(&Client_Request);
					  break;

					  default:
					  break;
				  }
			  }
			  /* --------------------Data frame characteristic handler --------------------*/
			  else if(attribute_modified->Attr_Handle == (CustomContext.CustomDataFrameHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
			  {
				  return_value = SVCCTL_EvtAckFlowEnable;
				  Client_Request.Custom_Evt_Opcode = CUSTOM_STM_DATAFRAME_WRITE_EVT;
				  Client_Request.DataTransfered.Length = attribute_modified->Attr_Data_Length;
				  Client_Request.DataTransfered.pPayload = attribute_modified->Attr_Data;
				  Custom_STM_App_Notification(&Client_Request);
			  }

			  else if(attribute_modified->Attr_Handle == (CustomContext.CustomDataFrameHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
			  {
				  return_value = SVCCTL_EvtAckFlowEnable;
				  switch(attribute_modified->Attr_Data[0])
				  {
				  /* Disabled Client_Request management */
				  case (!(COMSVC_Notification)):
					  Client_Request.Custom_Evt_Opcode = CUSTOM_STM_DATASTATUS_NOTIFY_DISABLED_EVT;
					  Custom_STM_App_Notification(&Client_Request);
					  break;

				  /* Enabled Client_Request management */
				  case COMSVC_Notification:
					  Client_Request.Custom_Evt_Opcode = CUSTOM_STM_DATAFRAME_NOTIFY_ENABLED_EVT;
					  Custom_STM_App_Notification(&Client_Request);
					  break;

				  default:
					  break;
				  }

			  }
			  break;

		  case ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE :
			  return_value = SVCCTL_EvtAckFlowEnable;
			  Client_Request.Custom_Evt_Opcode = CUSTOM_STM_PERMIT_READ_EVT;
			  Custom_STM_App_Notification(&Client_Request);

		  break;

		  case ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE:
		  break;

		  default:
		  break;
      }
      break; /* HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE */

      default:
	  break;
  }

  return(return_value);
}/* end Custom_STM_Event_Handler */

/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void Custom_STM_Service_Init(void)
{
  Char_UUID_t  uuid;

  /**
   *  Register the event handler to the BLE controller
   */
  SVCCTL_RegisterSvcHandler(Custom_STM_Event_Handler);

  /*
   * Max_Attribute_Records = 1 + 2*2 + 1*no_of_char_with_notify_or_indicate_property + 1*no_of_char_with_broadcast_property
   * service_max_attribute_record = 1 for AppService
   * 							  + 2 for DataFrame
   *                              + 1 for DataFrame configuration descriptor
   *                              + 2 for DataStatus
   *                              + 1 for DataStatus configuration descriptor
   *                              = 7
   */

  COPY_SMART_CHANGEOVER_SERVICE_UUID(uuid.Char_UUID_128);
  aci_gatt_add_service(UUID_TYPE_128,
                       (Service_UUID_t *) &uuid,
                       PRIMARY_SERVICE,
                       7,
                       &(CustomContext.CustomAppServiceHdle));

  /**
   *  DataFrame - AE82 ED19
   */
  COPY_DATAFRAME_UUID(uuid.Char_UUID_128);
  aci_gatt_add_char(CustomContext.CustomAppServiceHdle,
                    UUID_TYPE_128, &uuid,
					MAX_CHAR_LEN,
                    CHAR_PROP_READ | CHAR_PROP_WRITE | CHAR_PROP_NOTIFY,
                    ATTR_PERMISSION_NONE,
                    GATT_NOTIFY_ATTRIBUTE_WRITE  | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                    0x10,
                    CHAR_VALUE_LEN_VARIABLE,
                    &(CustomContext.CustomDataFrameHdle));

  /**
   *  DataStatus - A5D5 C51B
   */
  COPY_DATASTATUS_UUID(uuid.Char_UUID_128);
  aci_gatt_add_char(CustomContext.CustomAppServiceHdle,
                    UUID_TYPE_128, &uuid,
                    MAX_CHAR_LEN,
                    CHAR_PROP_READ | CHAR_PROP_NOTIFY,
                    ATTR_PERMISSION_NONE,
                    GATT_NOTIFY_ATTRIBUTE_WRITE,
                    0x10,
					CHAR_VALUE_LEN_VARIABLE,
                    &(CustomContext.CustomDataStatusHdle));

  uint8_t init_framevalue[] = { 1, 0 };
  aci_gatt_update_char_value(CustomContext.CustomAppServiceHdle,
                             CustomContext.CustomDataFrameHdle,
                             0, /* charValOffset */
							 2, /* charValueLen */
                             (uint8_t *) init_framevalue);

  aci_gatt_update_char_value(CustomContext.CustomAppServiceHdle,
                             CustomContext.CustomDataStatusHdle,
                             0, /* charValOffset */
							 2, /* charValueLen */
                             (uint8_t *) init_framevalue);
  return;
}

/**
 * @brief  Characteristic update
 * @param  CharOpcode: Characteristic identifier
 * @param  Service_Instance: Instance of the service to which the characteristic belongs
 *
 */
tBleStatus Custom_STM_App_Update_Char(Custom_STM_Char_Opcode_t CharOpcode, uint8_t *pPayload, uint8_t PayloadLen)
{
  tBleStatus result = BLE_STATUS_INVALID_PARAMS;
  /* USER CODE BEGIN Custom_STM_App_Update_Char_1 */

  /* USER CODE END Custom_STM_App_Update_Char_1 */

  switch(CharOpcode)
  {
    case CUSTOM_STM_NOTIFY_DATAFRAME:
    case CUSTOM_STM_WRITE_DATAFRAME:
      result = aci_gatt_update_char_value(CustomContext.CustomAppServiceHdle,
                                          CustomContext.CustomDataFrameHdle,
                                          0, /* charValOffset */
										  PayloadLen, /* charValueLen */
                                          (uint8_t *) pPayload);
      /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1*/

      /* USER CODE END CUSTOM_STM_Service_1_Char_1*/
      break;

    case CUSTOM_STM_DATASTATUS:
      result = aci_gatt_update_char_value(CustomContext.CustomAppServiceHdle,
                                          CustomContext.CustomDataStatusHdle,
                                          0, /* charValOffset */
										  PayloadLen, /* charValueLen */
                                          (uint8_t *) pPayload);
      /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_2*/

      /* USER CODE END CUSTOM_STM_Service_1_Char_2*/
      break;

    default:
      break;
  }

    /* USER CODE BEGIN Custom_STM_App_Update_Char_2 */

    /* USER CODE END Custom_STM_App_Update_Char_2 */

    return result;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
