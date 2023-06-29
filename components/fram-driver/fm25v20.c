/*******************************************************************************
 * Title                 :   FM25V20 F-RAM driver
 * Filename              :   fm25v20.c
 * Author                :   ItachiVN
 * Origin Date           :   2023/06/27
 * Version               :   0.0.0
 * Compiler              :   STM32CubeIDE
 * Target                :   STM32WB55
 * Notes                 :   None
 *******************************************************************************/

/*************** MODULE REVISION LOG ******************************************
 *
 *    Date       Software Version	Initials	Description
 *  2023/06/27       0.0.0	         ItachiVN      Module Created.
 *
 *******************************************************************************/

/** \file fm25v20.c
 *  \brief This module contains the
 */
/******************************************************************************
 * Includes
 *******************************************************************************/
#include "fm25v20.h"
/******************************************************************************
 * Module Preprocessor Constants
 *******************************************************************************/

/******************************************************************************
 * Module Preprocessor Macros
 *******************************************************************************/

/******************************************************************************
 * Module Typedefs
 *******************************************************************************/

/******************************************************************************
 * Module Variable Definitions
 *******************************************************************************/
extern SPI_HandleTypeDef hspi1;

bool _initialized;
uint8_t _chipSelectPin;
uint8_t _writeProtectPin;
uint8_t _holdPin;
uint8_t _statusRegister;
uint32_t _currentAddress;
uint32_t _fenceAddress;
uint32_t _maxAddress;

/******************************************************************************
 * Function Prototypes
 *******************************************************************************/

/******************************************************************************
 * Function Definitions
 *******************************************************************************/
bool FM25VXX_Initialize()
{
    bool status = false;
    FM25VXXManufacturer manu;
    FM25VXXFamilyDensity family;
    FM25VXXVariant variant;

    // Make sure the SPI is initialized
    // HAL_SPI_Init(&hspi1);

    if (FM25VXX_ReadManufacturer(&manu, &family, &variant) != FM25VXX_SUCCESS)
    {
        return false;
    }

    switch (variant)
    {
        case FM25V01:
            _maxAddress = FM25V01_MAX_ADDRESS;
            break;

        case FM25V02:
            _maxAddress = FM25V02_MAX_ADDRESS;
            break;

        case FM25V05:
            _maxAddress = FM25V05_MAX_ADDRESS;
            break;

        case FM25V10:
        case FM25VN10:
            _maxAddress = FM25V10_MAX_ADDRESS;
            break;

        case FM25V20:
            _maxAddress = FM25V20_MAX_ADDRESS;
            break;

        default:
            _maxAddress = FM25V20_MAX_ADDRESS;
            break;

    } /* chip variant switch */

    /*
     * Status register value that we want to write during initialization such that 
     * there are no protected memory addresses and that the WP/ pin from the microcontroller is ignored.
     * See page 6 of the datasheet for more information regarding the layered write protection scheme of the FM25VXX.
     * If the end user wants something other than this default, then they can make a separate call to FM25VXX_WriteStatusRegister().
     */
    FM25VXX_WriteStatusRegister(0);
    _initialized = true;
    return true;

} /* Initialize() */

bool FM25VXX_IsInitialized()
{
    return (_initialized);

} /* IsInitialized() */

bool FM25VXX_Sleep()
{
    bool status = false;
    /*
      * Send the SLEEP opcode, pull the pin HIGH. At the point that cs goes HIGH,
      * that is actually the point at which the device begins to sleep, not before. 
      * See the datasheet (page 9) for more information.
     */
    FM25VXX_CS_LOW();
    if (HAL_OK == HAL_SPI_Transmit(&hspi1, &((uint8_t*){FM25VXX_SLEEP}), 1, 1000))
    {
        status = true;
    }
    FM25VXX_CS_HIGH();
    return status;
} /* Sleep() */

bool FM25VXX_Wakeup()
{
   /*
   * After being put into SLEEP mode, the device needs to be awakened. There is a period of
   * time that this will require, called the wakeup period. The device will not necessarily
   * respond to an opcode within the wakeup period. Therefore we'll do a dummy read and wait
   * the maximum wakeup period of 400us (tREC). See the datasheet (page 9) for more information.
   */
    uint8_t ret_val = false;
    uint8_t data;
    if( FM25VXX_SUCCESS ==  FM25VXX_ReadBytes(FM25VXX_NULL, &data, 1)) //Dummy read
    {
        ret_val = true;
    }
    delayMicroseconds(FM25VXX_WAKEUP_DELAY_uS);
    return ret_val;
} /* Wakeup() */

void FM25VXX_WriteStatusRegister(uint8_t sRegister)
{
    _statusRegister = sRegister;

    FM25VXX_CS_LOW();
    HAL_SPI_Transmit(&hspi1, &((uint8_t*){FM25VXX_WREN}), 1, 1000);
    FM25VXX_CS_HIGH();

    FM25VXX_CS_LOW();
    HAL_SPI_Transmit(&hspi1, &((uint8_t*){FM25VXX_WRSR}), 1, 1000);
    HAL_SPI_Transmit(&hspi1, &_statusRegister, 1, 1000);
    FM25VXX_CS_HIGH();

} /* WriteStatusRegister() */

uint8_t FM25VXX_ReadStatusRegister()
{
    uint8_t currentStatusRegister = 0;
    FM25VXX_CS_LOW();
    HAL_SPI_Transmit(&hspi1, &((uint8_t*){FM25VXX_RDSR}), 1, 1000);
    HAL_SPI_Receive(&hspi1, &currentStatusRegister, 1, 1000);
    FM25VXX_CS_HIGH();

    return (currentStatusRegister);

} /* ReadStatusRegister() */

FM25VXXError FM25VXX_WriteBytes(uint32_t address, uint8_t data, uint32_t len)
{
    uint8_t write_addr[2] = {0};
    /*
     * Basic sanity check for writing past the fence
     */
    if (address >= _fenceAddress)
    {
        return (FM25VXX_WRITE_PAST_FENCE_REQUEST);
    }

    FM25VXX_CS_LOW();
    HAL_SPI_Transmit(&hspi1, &((uint8_t*){FM25VXX_WREN}), 1, 1000);
    FM25VXX_CS_HIGH();

    FM25VXX_CS_LOW();
    /* Prepare write address */
    write_addr[0] = highByte(address);
    write_addr[1] = lowByte(address);
    if (_maxAddress >= FM25V10_MAX_ADDRESS)
    {
        HAL_SPI_Transmit(&hspi1, &((uint8_t*){FM25VXX_WRITE}), 3, 1000);
    }
    HAL_SPI_Transmit(&hspi1, write_addr, 2, 1000);

    /* Transmit data */
    HAL_SPI_Transmit(&hspi1, &data, len, 1000);
    FM25VXX_CS_HIGH();
    return (FM25VXX_SUCCESS);

} /* WriteBytes() */

FM25VXXError FM25VXX_ReadBytes(uint32_t address, uint8_t *data, uint32_t len)
{
    uint8_t read_addr[2] ={0};
    /*
     * Basic sanity check for reading past the _maxAddress
     */
    if (address > _maxAddress)
    {
        return (FM25VXX_READ_PAST_MAX_ADDRESS_REQUEST);
    }
    FM25VXX_CS_LOW();
    /* Prepare read address */
    read_addr[0] = highByte(address);
    read_addr[1] = lowByte(address);
    HAL_SPI_Transmit(&hspi1, &((uint8_t*){FM25VXX_READ}), 1, 1000);
    
    if (_maxAddress >= FM25V10_MAX_ADDRESS)
    {
        /* In case 3B address */
        HAL_SPI_Transmit(&hspi1, &((uint8_t*){thirdByte(address)}), 1, 1000);
    }
    HAL_SPI_Transmit(&hspi1, read_addr, 2, 1000);

    /* Receive len bytes */
    HAL_SPI_Receive(&hspi1, data, len, 1000);
    FM25VXX_CS_HIGH();
    return (FM25VXX_SUCCESS);

} /* ReadByte() */

FM25VXXError FM25VXX_ReadManufacturer(FM25VXXManufacturer *manufacturer,
                                      FM25VXXFamilyDensity *storage,
                                      FM25VXXVariant *variant)
{
    uint8_t data_recv[FM25VXX_CONTINUATION_CODE_NUM]={0};
    *manufacturer = FM25VXX_MAN_UNKNOWN;
    *storage = FM25VXX_STORAGE_UNKNOWN;
    *variant = FM25VXX_VARIANT_UNKNOWN;

    FM25VXX_CS_LOW();
    HAL_SPI_Transmit(&hspi1, &((uint8_t*){FM25VXX_RDID}), 1, 100);

    /*
     * Read/ignore the 6 continuation codes
     */
      HAL_SPI_Receive(&hspi1, data_recv, FM25VXX_CONTINUATION_CODE_NUM, 100);

    /*
     * Read 3 bytes - manufacturer, storage, variant
     * If not CYPRESS_RAMTRON_MANUFACTURER_ID then there's a serious problem.  Hah!
     */
    
    HAL_SPI_Receive(&hspi1, data_recv, 3, 100);
    if(data_recv[0] != CYPRESS_RAMTRON_MANUFACTURER_ID)
    {
        FM25VXX_CS_HIGH();
        return (FM25VXX_UNKNOWN_MANUFACTURER);
    }
    *manufacturer = FM25VXX_MAN_CYPRESS_RAMTRON;

    switch (data_recv[1])
    {
      case FM25VXX_128K_STORAGE_CODE:
         *storage = FM25VXX_128K;
         *variant = FM25V01;
         break;

      case FM25VXX_256K_STORAGE_CODE:
         *storage = FM25VXX_256K;
         *variant = FM25V02;
         break;

      case FM25VXX_512K_STORAGE_CODE:
         *storage = FM25VXX_512K;
         *variant = FM25V05;
         break;

      case FM25VXX_1M_STORAGE_CODE:
         *storage = FM25VXX_1M;
         break;

      case FM25VXX_2M_STORAGE_CODE:
         *storage = FM25VXX_2M;
         *variant = FM25V20;
         break;

      default:
         FM25VXX_CS_HIGH();
         return (FM25VXX_UNSUPPORTED_STORAGE_DENSITY);
         break;

    } /* storage switch */

    /*
    * Read the chip variant. As of 11/13, only the 1M variant offered the unique
    * serial number feature, so that is the only condition for which we will need
    * to verify the chip variant. Update this to match the Cypress product line if
    * necessary. Ordinarily I would just move this up into the previous switch,
    * but leaving it here for a "cleaner" functionality expansion should that
    * become necessary in the future.
    */
 
    if (*variant == FM25VXX_VARIANT_UNKNOWN)
    {
        switch (data_recv[2])
        {
        case FM25V10_VARIANT_CODE:
            *variant = FM25V10;
            break;

        case FM25VN10_VARIANT_CODE:
            *variant = FM25VN10;
            break;

        default:
            /*
             * Things went really South and weirdly
             * so if you get here.  Shouldn't happen.
             */
            FM25VXX_CS_HIGH();
            return (FM25VXX_CHIP_VARIANT_ERROR);
            break;

        } /* variant switch */

    } /* if this chip has an FM25VNXX variant */

    FM25VXX_CS_HIGH();

    return (FM25VXX_SUCCESS);

} /* ReadManufacturer() */

FM25VXXError FM25VXX_WriteProtectFM25VXX(FM25VXXProtection whatToProtect)
{
    switch (whatToProtect)
    {
      case FM25VXX_PROTECT_UPPER_QUARTER:
         FM25VXX_WriteStatusRegister((1 << FM25VXX_WPEN_POS) | (1 << FM25VXX_BP1_POS));
         break;
      case FM25VXX_PROTECT_UPPER_HALF:
         FM25VXX_WriteStatusRegister((1 << FM25VXX_WPEN_POS) | (1 << FM25VXX_BP0_POS));
         break;
      case FM25VXX_PROTECT_ALL:
         FM25VXX_WriteStatusRegister((1 << FM25VXX_WPEN_POS) | (1 << FM25VXX_BP0_POS) | (1 << FM25VXX_BP1_POS));
         break;
      case FM25VXX_PROTECT_NONE:
         FM25VXX_WriteStatusRegister((1 << FM25VXX_WPEN_POS));
         break;
      default:
         return (FM25VXX_INVALID_WRITE_PROTECT);
         break;
    }

    return (FM25VXX_SUCCESS);
} /* WriteProtectFM25VXX() */