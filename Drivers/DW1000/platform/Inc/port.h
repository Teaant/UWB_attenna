/*! ----------------------------------------------------------------------------
 * @file    port.h
 * @brief   HW specific definitions and functions for portability
 *
 * @attention
 *
 * Copyright 2015 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 * @author DecaWave
 */


#ifndef PORT_H_
#define PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include "compiler.h"
#include "antenna.h"

#include "stm32h7xx.h"

/* DW1000 IRQ handler type. */
typedef void (*port_deca_isr_t)(DW1000_Port_t *antenna_port);

/* DW1000 IRQ handler declaration. */
extern port_deca_isr_t port_deca_isr;

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn port_set_deca_isr()
 *
 * @brief This function is used to install the handling function for DW1000 IRQ.
 *
 * NOTE:
 *   - As EXTI9_5_IRQHandler does not check that port_deca_isr is not null, the user application must ensure that a
 *     proper handler is set by calling this function before any DW1000 IRQ occurs!
 *   - This function makes sure the DW1000 IRQ line is deactivated while the handler is installed.
 *
 * @param deca_isr function pointer to DW1000 interrupt handler to install
 *
 * @return none
 */
void port_set_deca_isr(port_deca_isr_t deca_isr, DW1000_Port_t *antenna_port);

#define BUFFLEN     (64) //(4096+128)

#define BUF_SIZE    (64)

#define USB_SUPPORT

typedef struct
{
    uint16_t        usblen;                 /**< for RX from USB */
    uint8_t         usbbuf[BUF_SIZE*3];     /**< for RX from USB */
}__packed app_t;


extern app_t    app;


/*****************************************************************************************************************//*
**/

 /****************************************************************************//**
  *
  *                                 Types definitions
  *
  *******************************************************************************/
typedef uint64_t        uint64 ;

typedef int64_t         int64 ;


#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

/****************************************************************************//**
 *
 *                              MACRO function
 *
 *******************************************************************************/

#define GPIO_ResetBits(x,y)             HAL_GPIO_WritePin(x,y, RESET)
#define GPIO_SetBits(x,y)               HAL_GPIO_WritePin(x,y, SET)
#define GPIO_ReadInputDataBit(x,y)      HAL_GPIO_ReadPin(x,y)

//x是GPIOx(A...K)
#define GPIO_PORT_INDEX(x)              (((uint32_t)x - (uint32_t)GPIOA) / 0x400)

/* NSS pin is SW controllable */
#define port_SPIx_set_chip_select()     HAL_GPIO_WritePin(DW_NSS_GPIO_Port, DW_NSS_Pin, GPIO_PIN_SET)
#define port_SPIx_clear_chip_select()   HAL_GPIO_WritePin(DW_NSS_GPIO_Port, DW_NSS_Pin, GPIO_PIN_RESET)

/****************************************************************************//**
 *
 *                              port function prototypes
 *
 *******************************************************************************/

void Sleep(uint32_t Delay);
unsigned long portGetTickCnt(void);

void port_wakeup_dw1000(DW1000_Port_t* antenna_port);
void port_wakeup_dw1000_fast(DW1000_Port_t* antenna_port);

void port_set_dw1000_slowrate(DW1000_Port_t* antenna_port);
void port_set_dw1000_fastrate(DW1000_Port_t* antenna_port);

void process_dwRSTn_irq(void);
void process_deca_irq(DW1000_Port_t *antenna_port);

int  peripherals_init(void);
void spi_peripheral_init(void);

void setup_DW1000RSTnIRQ(int enable, DW1000_Port_t* antenna_port);

void reset_DW1000(DW1000_Port_t* antenna_port);

ITStatus EXTI_GetITEnStatus(uint32_t x);

uint32_t port_GetEXT_IRQStatus(DW1000_Port_t *antenna_port);
uint32_t port_CheckEXT_IRQ(DW1000_Port_t *antenna_port);
void port_DisableEXT_IRQ(DW1000_Port_t *antenna_port);
void port_EnableEXT_IRQ(DW1000_Port_t *antenna_port);

#ifdef __cplusplus
}
#endif

#endif /* PORT_H_ */
/*
 * Taken from the Linux Kernel
 *
 */

#ifndef _LINUX_CIRC_BUF_H
#define _LINUX_CIRC_BUF_H 1

struct circ_buf {
    char *buf;
    int head;
    int tail;
};

/* Return count in buffer.  */
#define CIRC_CNT(head,tail,size) (((head) - (tail)) & ((size)-1))

/* Return space available, 0..size-1.  We always leave one free char
   as a completely full buffer has head == tail, which is the same as
   empty.  */
#define CIRC_SPACE(head,tail,size) CIRC_CNT((tail),((head)+1),(size))

/* Return count up to the end of the buffer.  Carefully avoid
   accessing head and tail more than once, so they can change
   underneath us without returning inconsistent results.  */
#define CIRC_CNT_TO_END(head,tail,size) \
    ({int end = (size) - (tail); \
      int n = ((head) + end) & ((size)-1); \
      n < end ? n : end;})

/* Return space available up to the end of the buffer.  */
#define CIRC_SPACE_TO_END(head,tail,size) \
    ({int end = (size) - 1 - (head); \
      int n = (end + (tail)) & ((size)-1); \
      n <= end ? n : end+1;})

#endif /* _LINUX_CIRC_BUF_H  */

