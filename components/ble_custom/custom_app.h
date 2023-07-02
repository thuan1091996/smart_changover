/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_app.h
  * @author  MCD Application Team
  * @brief   Header for custom_app.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CUSTOM_APP_H
#define CUSTOM_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app_ble.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  CUSTOM_CONN_HANDLE_EVT,
  CUSTOM_DISCON_HANDLE_EVT,
} Custom_App_Opcode_Notification_evt_t;

typedef struct
{
  Custom_App_Opcode_Notification_evt_t     Custom_Evt_Opcode;
  uint16_t                                 ConnectionHandle;
} Custom_App_ConnHandle_Not_evt_t;
/* USER CODE BEGIN ET */
typedef struct {
  uint16_t CDL1; /* Current supplied on Line 1 */
  uint16_t CDL2; /* Current supplied on Line 2 */
  uint16_t CDL3; /* Current supplied on Line 3 */

  uint16_t VDL1; /* Voltage supplied on Line 1 */
  uint16_t VDL2; /* Voltage supplied on Line 2 */
  uint16_t VDL3; /* Voltage supplied on Line 3 */

  uint16_t PDL1; /* Power consumed on Line 1 */
  uint16_t PDL2; /* Power consumed on Line 2 */
  uint16_t PDL3; /* Power consumed on Line 3 */

  uint8_t CMD_SL1;  /* L1 Supply command identifier */
  uint8_t CMD_SL2;  /* L2 Supply command identifier */
  uint8_t CMD_SL3;  /* L3 Supply command identifier */

} sac_data_t;


/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define DEV_ID                                  		(0x10)

#define BLE_COMPLETE_NAME_LEN							(21)
#define BLE_INCLUDE_SERVICE_UUID_IN_ADV_PACKT			(0)
#define BLE_WRITE_CHAR_ATT_MAX_LEN				      	(50)
#define BLE_READ_NOTIFY_CHAR_ATT_MAX_LEN			  	(50)

#define BLE_TEST_NOTIFY									(1)

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions ---------------------------------------------*/
void Custom_APP_Init(void);
void Custom_APP_Notification(Custom_App_ConnHandle_Not_evt_t *pNotification);
void Device_PacketSend();
/* USER CODE BEGIN EF */

/* USER CODE END EF */

#ifdef __cplusplus
}
#endif

#endif /* CUSTOM_APP_H */
