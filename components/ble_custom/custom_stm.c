/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_stm.c
  * @author  MCD Application Team
  * @brief   Custom Example Service.
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
#include "common_blesvc.h"
#include "custom_stm.h"

/* USER CODE BEGIN Includes */
#include "custom_app.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct{
  uint16_t  CustomCustom_SvcHdle;                    /**< CUSTOM_SVC handle */
  uint16_t  CustomChar_Read_NotifyHdle;                  /**< CHAR_READ_NOTIFY handle */
  uint16_t  CustomChar_WriteHdle;                  /**< CHAR_WRITE handle */
}CustomContext_t;

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
uint8_t SizeChar_Read_Notify = 1;
uint8_t SizeChar_Write = 1;

/**
 * START of Section BLE_DRIVER_CONTEXT
 */
static CustomContext_t CustomContext;

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
}while(0)

/* Hardware Characteristics Service */
/*
 The following 128bits UUIDs have been generated from the random UUID
 generator:
 D973F2E0-B19E-11E2-9E96-0800200C9A66: Service 128bits UUID
 D973F2E1-B19E-11E2-9E96-0800200C9A66: Characteristic_1 128bits UUID
 D973F2E2-B19E-11E2-9E96-0800200C9A66: Characteristic_2 128bits UUID
 */
#define COPY_CUSTOM_SVC_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0x4c,0x51,0xf3,0xc6,0xaa,0xaa,0x11,0xee,0xbe,0x56,0x02,0x42,0xac,0x12,0x00,0x02)
#define COPY_CHAR_READ_NOTIFY_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x4c,0x51,0xf3,0xc6,0xbb,0xbb,0x11,0xee,0xbe,0x56,0x02,0x42,0xac,0x12,0x00,0x02)
#define COPY_CHAR_WRITE_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x4c,0x51,0xf3,0xc6,0xcc,0xcc,0x11,0xee,0xbe,0x56,0x02,0x42,0xac,0x12,0x00,0x02)

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
  aci_gatt_write_permit_req_event_rp0   *write_perm_req;
  aci_gatt_read_permit_req_event_rp0    *read_req;
  Custom_STM_App_Notification_evt_t     Notification;
  /* USER CODE BEGIN Custom_STM_Event_Handler_1 */

  /* USER CODE END Custom_STM_Event_Handler_1 */

  return_value = SVCCTL_EvtNotAck;
  event_pckt = (hci_event_pckt *)(((hci_uart_pckt*)Event)->data);
  aci_att_exchange_mtu_resp_event_rp0 *exchange_mtu_resp;
  tBleStatus result;

  switch (event_pckt->evt)
  {
    case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
      blecore_evt = (evt_blecore_aci*)event_pckt->data;
      switch (blecore_evt->ecode)
      {

        case ACI_ATT_EXCHANGE_MTU_RESP_VSEVT_CODE:
        {
			APP_DBG_MSG("ACI_ATT_EXCHANGE_MTU_RESP_VSEVT_CODE \n");
			exchange_mtu_resp = (aci_att_exchange_mtu_resp_event_rp0 *)blecore_evt->data;
			APP_DBG_MSG("MTU_size = %d \n",exchange_mtu_resp->Server_RX_MTU );
			result = hci_le_set_data_length(exchange_mtu_resp->Connection_Handle,exchange_mtu_resp->Server_RX_MTU,2120);
			if (result != BLE_STATUS_SUCCESS)
			{
			  APP_DBG_MSG("set data length command error %x \n", result);
			}
			else
			{
			  APP_DBG_MSG("set data length command success \n");
			}
			break;

        }

        case ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE:
          /* USER CODE BEGIN EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_BEGIN */

          /* USER CODE END EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_BEGIN */
          attribute_modified = (aci_gatt_attribute_modified_event_rp0*)blecore_evt->data;
          if (attribute_modified->Attr_Handle == (CustomContext.CustomChar_Read_NotifyHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1 */

            /* USER CODE END CUSTOM_STM_Service_1_Char_1 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_1_Char_1_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_1_Char_1_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_CHAR_READ_NOTIFY_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_1_Char_1_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_1_Char_1_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_CHAR_READ_NOTIFY_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_1_Char_1_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_default */

                /* USER CODE END CUSTOM_STM_Service_1_Char_1_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomChar_Read_NotifyHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomChar_WriteHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_2_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */
            APP_DBG_MSG(">>== WRITE EVENT FROM GATT CLIENT: Received %d Bytes:", attribute_modified->Attr_Data_Length);
            //Forward data to app notify
            Notification.Custom_Evt_Opcode = CUSTOM_STM_CHAR_WRITE_WRITE_EVT;
            Notification.DataTransfered.Length = attribute_modified->Attr_Data_Length;
            Notification.DataTransfered.pPayload = attribute_modified->Attr_Data;
            Custom_STM_App_Notification(&Notification);

            /* USER CODE END CUSTOM_STM_Service_1_Char_2_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */
          } /* if (attribute_modified->Attr_Handle == (CustomContext.CustomChar_WriteHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          /* USER CODE BEGIN EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_END */

          /* USER CODE END EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_END */
          break;

        case ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE :
          /* USER CODE BEGIN EVT_BLUE_GATT_READ_PERMIT_REQ_BEGIN */

          /* USER CODE END EVT_BLUE_GATT_READ_PERMIT_REQ_BEGIN */
          read_req = (aci_gatt_read_permit_req_event_rp0*)blecore_evt->data;
          if (read_req->Attribute_Handle == (CustomContext.CustomChar_Read_NotifyHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /*USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1 */
            //Forward data to app
            Notification.Custom_Evt_Opcode = CUSTOM_STM_CHAR_READ_NOTIFY_READ_EVT;
            Custom_STM_App_Notification(&Notification);
            /*USER CODE END CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1*/
            aci_gatt_allow_read(read_req->Connection_Handle);
            /*USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2 */

            /*USER CODE END CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2*/
          } /* if (read_req->Attribute_Handle == (CustomContext.CustomChar_Read_NotifyHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          /* USER CODE BEGIN EVT_BLUE_GATT_READ_PERMIT_REQ_END */

          /* USER CODE END EVT_BLUE_GATT_READ_PERMIT_REQ_END */
          break;

        case ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE:
          /* USER CODE BEGIN EVT_BLUE_GATT_WRITE_PERMIT_REQ_BEGIN */

          /* USER CODE END EVT_BLUE_GATT_WRITE_PERMIT_REQ_BEGIN */
          write_perm_req = (aci_gatt_write_permit_req_event_rp0*)blecore_evt->data;
          if (write_perm_req->Attribute_Handle == (CustomContext.CustomChar_WriteHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* Allow or reject a write request from a client using aci_gatt_write_resp(...) function */
            /*USER CODE BEGIN CUSTOM_STM_Service_1_Char_2_ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE */

            // Allow write request to client
            APP_DBG_MSG(">>== ACCEPT WRITE REQ FROM CLIENT \n");
            aci_gatt_write_resp(write_perm_req->Connection_Handle,
            					write_perm_req->Attribute_Handle,
								0x00, /* write_status = 0 (no error))*/
								0,    /* err_code */
								write_perm_req->Data_Length,
								(uint8_t *)&write_perm_req->Data[0]);
            /*USER CODE END CUSTOM_STM_Service_1_Char_2_ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE*/
          } /*if (write_perm_req->Attribute_Handle == (CustomContext.CustomChar_WriteHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/

          /* USER CODE BEGIN EVT_BLUE_GATT_WRITE_PERMIT_REQ_END */

          /* USER CODE END EVT_BLUE_GATT_WRITE_PERMIT_REQ_END */
          break;
        /* USER CODE BEGIN BLECORE_EVT */

        /* USER CODE END BLECORE_EVT */
        default:
          /* USER CODE BEGIN EVT_DEFAULT */

          /* USER CODE END EVT_DEFAULT */
          break;
      }
      /* USER CODE BEGIN EVT_VENDOR*/

      /* USER CODE END EVT_VENDOR*/
      break; /* HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE */

      /* USER CODE BEGIN EVENT_PCKT_CASES*/

      /* USER CODE END EVENT_PCKT_CASES*/

    default:
      /* USER CODE BEGIN EVENT_PCKT*/

      /* USER CODE END EVENT_PCKT*/
      break;
  }

  /* USER CODE BEGIN Custom_STM_Event_Handler_2 */

  /* USER CODE END Custom_STM_Event_Handler_2 */

  return(return_value);
}/* end Custom_STM_Event_Handler */

/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void SVCCTL_InitCustomSvc(void)
{

  Char_UUID_t  uuid;
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  /* USER CODE BEGIN SVCCTL_InitCustomSvc_1 */

  /* USER CODE END SVCCTL_InitCustomSvc_1 */

  /**
   *  Register the event handler to the BLE controller
   */
  SVCCTL_RegisterSvcHandler(Custom_STM_Event_Handler);

  /**
   *          CUSTOM_SVC
   *
   * Max_Attribute_Records = 1 + 2*2 + 1*no_of_char_with_notify_or_indicate_property + 1*no_of_char_with_broadcast_property
   * service_max_attribute_record = 1 for CUSTOM_SVC +
   *                                2 for CHAR_READ_NOTIFY +
   *                                2 for CHAR_WRITE +
   *                                1 for CHAR_READ_NOTIFY configuration descriptor +
   *                              = 6
   */

  COPY_CUSTOM_SVC_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_service(UUID_TYPE_128,
                             (Service_UUID_t *) &uuid,
                             PRIMARY_SERVICE,
                             6,
                             &(CustomContext.CustomCustom_SvcHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_service command: CUSTOM_SVC, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_service command: CUSTOM_SVC \n\r");
  }

  /**
   *  CHAR_READ_NOTIFY
   */
  COPY_CHAR_READ_NOTIFY_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomCustom_SvcHdle,
                          UUID_TYPE_128, &uuid,
						  BLE_READ_NOTIFY_CHAR_ATT_MAX_LEN,
                          CHAR_PROP_READ | CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                          0x10,
                          CHAR_VALUE_LEN_VARIABLE,
                          &(CustomContext.CustomChar_Read_NotifyHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : CHAR_READ_NOTIFY, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : CHAR_READ_NOTIFY \n\r");
  }
  /**
   *  CHAR_WRITE
   */
  COPY_CHAR_WRITE_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomCustom_SvcHdle,
                          UUID_TYPE_128, &uuid,
						  BLE_WRITE_CHAR_ATT_MAX_LEN,
                          CHAR_PROP_WRITE,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                          0x10,
                          CHAR_VALUE_LEN_VARIABLE,
                          &(CustomContext.CustomChar_WriteHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : CHAR_WRITE, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : CHAR_WRITE \n\r");
  }

  /* USER CODE BEGIN SVCCTL_InitCustomSvc_2 */

  /* USER CODE END SVCCTL_InitCustomSvc_2 */

  return;
}

/**
 * @brief  Send update to peer device over notification
 * @param  p_data: pointer to data to be sent
 * @param  len: length of data to be sent
 */
tBleStatus ReadChar_SendNotify(uint8_t* p_data, uint16_t len)
{
      tBleStatus ret;
      if( ( p_data == NULL ) || (len == 0) || ( len > BLE_READ_NOTIFY_CHAR_ATT_MAX_LEN) )
      {
        APP_DBG_MSG(" Invalid Params \n\r");
        return BLE_STATUS_INVALID_PARAMS;
      }
      ret = aci_gatt_update_char_value(CustomContext.CustomCustom_SvcHdle,
                                       CustomContext.CustomChar_Read_NotifyHdle,
                                       0, /* charValOffset */
                                       (uint8_t)len, /* charValueLen */
                                       (uint8_t *)  p_data);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value CHAR_READ_NOTIFY command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value CHAR_READ_NOTIFY command\n\r");
      }
      return ret;
}



tBleStatus Custom_STM_App_Update_Char(Custom_STM_Char_Opcode_t CharOpcode, uint8_t *pPayload)
{
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  /* USER CODE BEGIN Custom_STM_App_Update_Char_1 */

  /* USER CODE END Custom_STM_App_Update_Char_1 */

  switch (CharOpcode)
  {

    case CUSTOM_STM_CHAR_READ_NOTIFY:
      ret = aci_gatt_update_char_value(CustomContext.CustomCustom_SvcHdle,
                                       CustomContext.CustomChar_Read_NotifyHdle,
                                       0, /* charValOffset */
                                       SizeChar_Read_Notify, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value CHAR_READ_NOTIFY command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value CHAR_READ_NOTIFY command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_1_Char_1*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_1_Char_1*/
      break;

    case CUSTOM_STM_CHAR_WRITE:
      ret = aci_gatt_update_char_value(CustomContext.CustomCustom_SvcHdle,
                                       CustomContext.CustomChar_WriteHdle,
                                       0, /* charValOffset */
                                       SizeChar_Write, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value CHAR_WRITE command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value CHAR_WRITE command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_1_Char_2*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_1_Char_2*/
      break;

    default:
      break;
  }

  /* USER CODE BEGIN Custom_STM_App_Update_Char_2 */

  /* USER CODE END Custom_STM_App_Update_Char_2 */

  return ret;
}
