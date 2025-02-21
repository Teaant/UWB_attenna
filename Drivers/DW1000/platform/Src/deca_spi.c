/*! ----------------------------------------------------------------------------
 * @file    deca_spi.c
 * @brief   SPI access functions
 *
 * @attention
 *
 * Copyright 2015 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 * @author DecaWave
 */

#include "deca_spi.h"
#include "deca_device_api.h"
#include "port.h"
#include "stm32h7xx_hal_def.h"

/****************************************************************************//**
 *
 *                              DW1000 SPI section
 *
 *******************************************************************************/
/*! ------------------------------------------------------------------------------------------------------------------
 * Function: openspi()
 *
 * Low level abstract function to open and initialise access to the SPI device.
 * returns 0 for success, or -1 for error
 */
int openspi(/*SPI_TypeDef* SPIx*/)
{
    return 0;
} // end openspi()

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: closespi()
 *
 * Low level abstract function to close the the SPI device.
 * returns 0 for success, or -1 for error
 */
int closespi(void)
{
    return 0;
} // end closespi()

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: writetospi()
 *
 * Low level abstract function to write to the SPI
 * Takes two separate byte buffers for write header and write data
 * returns 0 for success
 */
#pragma GCC optimize ("O3")
int writetospi(uint16_t headerLength,
               const    uint8_t *headerBuffer,
               uint32_t bodyLength,
               const    uint8_t *bodyBuffer,
               DW1000_Port_t *antenna_port)
{
    decaIrqStatus_t  stat ;
    SPI_HandleTypeDef *hspi = antenna_port->hspi;
    stat = decamutexon(antenna_port);

    while (HAL_SPI_GetState(hspi) != HAL_SPI_STATE_READY);

    HAL_GPIO_WritePin(antenna_port->spi_csn_port, antenna_port->spi_csn_pin, GPIO_PIN_RESET); /**< Put chip select line low */

    HAL_SPI_Transmit(hspi, (uint8_t *)&headerBuffer[0], headerLength, HAL_MAX_DELAY);    /* Send header in polling mode */
    HAL_SPI_Transmit(hspi, (uint8_t *)&bodyBuffer[0], bodyLength, HAL_MAX_DELAY);        /* Send data in polling mode */

    HAL_GPIO_WritePin(antenna_port->spi_csn_port, antenna_port->spi_csn_pin, GPIO_PIN_SET); /**< Put chip select line high */

    decamutexoff(stat, antenna_port);

    return 0;
} // end writetospi()


/*! ------------------------------------------------------------------------------------------------------------------
 * Function: readfromspi()
 *
 * Low level abstract function to read from the SPI
 * Takes two separate byte buffers for write header and read data
 * returns the offset into read buffer where first byte of read data may be found,
 * or returns 0
 */
#pragma GCC optimize ("O3")
int readfromspi(uint16_t headerLength,
                const uint8_t *headerBuffer,
                uint32_t readlength,
                uint8_t *readBuffer,
                DW1000_Port_t *antenna_port)
{
    decaIrqStatus_t  stat ;
    SPI_HandleTypeDef *hspi = antenna_port->hspi;
    stat = decamutexon(antenna_port) ;

    /* Blocking: Check whether previous transfer has been finished */
    while (HAL_SPI_GetState(hspi) != HAL_SPI_STATE_READY);

    HAL_GPIO_WritePin(antenna_port->spi_csn_port, antenna_port->spi_csn_pin, GPIO_PIN_RESET); /**< Put chip select line low */

    HAL_SPI_Transmit(hspi, headerBuffer, headerLength, HAL_MAX_DELAY); //No timeout

    HAL_SPI_Receive(hspi, readBuffer, readlength, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(antenna_port->spi_csn_port, antenna_port->spi_csn_pin, GPIO_PIN_SET); /**< Put chip select line high */

    decamutexoff(stat, antenna_port);

    return 0;
} // end readfromspi()

/****************************************************************************//**
 *
 *                              END OF DW1000 SPI section
 *
 *******************************************************************************/

