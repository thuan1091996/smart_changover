/*******************************************************************************
 * Title                 :   FM25V20 F-RAM driver
 * Filename              :   fm25v20.h
 * Author                :   ItachiVN
 * Origin Date           :   2023/06/27
 * Version               :   0.0.0
 * Compiler              :   STM32CubeIDE
 * Target                :   STM32WB55
 * Notes                 :   Based on Cypress-Ramtron FM25VXX library for Amtel/Arduino
 *                           by Jerry Adair
 *******************************************************************************/
#ifndef FM25VXX_H
#define FM25VXX_H

/******************************************************************************
* Includes
*******************************************************************************/
#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "stm32wbxx_hal.h"

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
/*
 * FM25VXX opcode definitions
 */
#define FM25VXX_WREN      (0b00000110)
#define FM25VXX_WRDI      (0b00000100)
#define FM25VXX_RDSR      (0b00000101)
#define FM25VXX_WRSR      (0b00000001)
#define FM25VXX_READ      (0b00000011)
#define FM25VXX_FSTRD     (0b00001011)
#define FM25VXX_WRITE     (0b00000010)
#define FM25VXX_SLEEP     (0b10111001)
#define FM25VXX_RDID      (0b10011111)
#define FM25VXX_SNR       (0b11000011)

/*
 * FM25VXX status register bit definitions
 */
#define FM25VXX_WPEN_POS      (7)
#define FM25VXX_BP1_POS       (3)
#define FM25VXX_BP0_POS       (2)
#define FM25VXX_WEL_POS       (1)

/*
 * FM25VXX miscellaneous definitions
 */
#define FM25VXX_NULL   0x00

/*
 * Power up period of 250us (tPU) * 2.  See
 * the datasheet (page 5) for more information.
 */
#define FM25VXX_POWERUP_DELAY_uS   500

/*
 * Wakeup period of 400us (tREC).  See
 * the datasheet (page 9) for more information.
 */
#define FM25VXX_WAKEUP_DELAY_uS    400

/*
 * Base and maximum address values for
 * the various Cypress FRAM chips that
 * are supported in this library.
 */
#define FM25VXX_BASE_ADDRESS   (0x0000)
#define FM25V01_MAX_ADDRESS    (0x3FFF)
#define FM25V02_MAX_ADDRESS    (0x7FFF)
#define FM25V05_MAX_ADDRESS    (0xFFFF)
#define FM25V10_MAX_ADDRESS    (0x1FFFF)
#define FM25V20_MAX_ADDRESS    (0x3FFFF)

/*
 * Manufacturer ID stuff.  See the datasheet
 * (page 9) for more information.
 */
#define CYPRESS_RAMTRON_MANUFACTURER_ID   (0xC2)
#define FM25VXX_128K_STORAGE_CODE         (0x21)
#define FM25VXX_256K_STORAGE_CODE         (0x22)
#define FM25VXX_512K_STORAGE_CODE         (0x23)
#define FM25VXX_1M_STORAGE_CODE           (0x24)
#define FM25VXX_2M_STORAGE_CODE           (0x25)
#define FM25V10_VARIANT_CODE              (0x00)
#define FM25VN10_VARIANT_CODE             (0x01)
#define FM25VXX_CONTINUATION_CODE_NUM     (0x06)

/******************************************************************************
* Configuration Constants
*******************************************************************************/


/******************************************************************************
* Portings
*******************************************************************************/
#define FM25VXX_CS_GPIO_Port    FRAM_CS_OUTPUT_GPIO_Port
#define FM25VXX_CS_Pin          FRAM_CS_OUTPUT_Pin
#define FM25VXX_CS_LOW()        HAL_GPIO_WritePin(FM25VXX_CS_GPIO_Port, FM25VXX_CS_Pin, GPIO_PIN_RESET)
#define FM25VXX_CS_HIGH()       HAL_GPIO_WritePin(FM25VXX_CS_GPIO_Port, FM25VXX_CS_Pin, GPIO_PIN_SET)
#define delayMicroseconds(ms)   HAL_Delay(ms)

/******************************************************************************
* Macros
*******************************************************************************/
/*
 * Macro to get the third byte for FM25V10,
 * FM25VN10 and FM25V20 memory array addressing.
 */
#define thirdByte( w ) ( ( uint8_t ) ( ( w ) >> 16 ) )

/******************************************************************************
* Typedefs
*******************************************************************************/
/*
 * The names of the various Cypress FRAM
 * chips that are supported in this library.
 */
typedef enum { FM25VXX_VARIANT_UNKNOWN,
               FM25V01,
               FM25V02,
               FM25V05,
               FM25V10,
               FM25VN10,
               FM25V20
             } FM25VXXVariant;



typedef enum { FM25VXX_MAN_UNKNOWN,
               FM25VXX_MAN_CYPRESS_RAMTRON
             } FM25VXXManufacturer;

typedef enum { FM25VXX_STORAGE_UNKNOWN,
               FM25VXX_128K,
               FM25VXX_256K,
               FM25VXX_512K,
               FM25VXX_1M,
               FM25VXX_2M
             } FM25VXXFamilyDensity;

/*
 * Protected region definitions
 */
typedef enum { FM25VXX_PROTECT_NONE,
               FM25VXX_PROTECT_UPPER_QUARTER,
               FM25VXX_PROTECT_UPPER_HALF,
               FM25VXX_PROTECT_ALL
             } FM25VXXProtection;

/*
 * Used when erasing the chip
 */
typedef enum { FM25VXX_TO_THE_FENCE,
               FM25VXX_ENTIRE_ARRAY
             } FM25VXXErase;

/*
 * FM25VXX error code definitions
 */
typedef enum { FM25VXX_SUCCESS,
               FM25VXX_INVALID_WRITE_PROTECT,
               FM25VXX_ERASE_PAST_FENCE_REQUEST,
               FM25VXX_WRITE_PAST_FENCE_REQUEST,
               FM25VXX_READ_PAST_MAX_ADDRESS_REQUEST,
               FM25VXX_UNKNOWN_MANUFACTURER,
               FM25VXX_UNSUPPORTED_STORAGE_DENSITY,
               FM25VXX_CHIP_VARIANT_ERROR,
               FM25VXX_OVERWRITE_ARRAY_REQUEST,
               FM25VXX_OVERREAD_ARRAY_REQUEST
             } FM25VXXError;

/******************************************************************************
* Variables
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/
bool         FM25VXX_Initialize();
bool         FM25VXX_IsInitialized();
bool         FM25VXX_Sleep();
bool         FM25VXX_Wakeup();
void         FM25VXX_WriteStatusRegister( uint8_t sRegister );
uint8_t      FM25VXX_ReadStatusRegister();
FM25VXXError FM25VXX_WriteBytes(  uint32_t address, uint8_t  data, uint32_t len);
FM25VXXError FM25VXX_ReadBytes(uint32_t address, uint8_t *data, uint32_t len);
FM25VXXError FM25VXX_WriteProtectFM25VXX( FM25VXXProtection whatToProtect );
FM25VXXError FM25VXX_ReadManufacturer( FM25VXXManufacturer *manufacturer, FM25VXXFamilyDensity *storage, FM25VXXVariant *variant);

#endif /* FM25VXX_H */
