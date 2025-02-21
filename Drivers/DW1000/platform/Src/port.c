/*! ----------------------------------------------------------------------------
 * @file    port.c
 * @brief   HW specific definitions and functions for portability
 *
 * @attention
 *
 * Copyright 2016 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 * @author DecaWave
 */

#include "port.h"
#include "spi.h"
#include "deca_device_api.h"
#include "stm32h7xx_hal_conf.h"

/****************************************************************************//**
 *
 *                              APP global variables
 *
 *******************************************************************************/
extern UWB_Device UWB_device_array[DWT_NUM_DW_DEV];

/****************************************************************************//**
 *
 *                  Port private variables and function prototypes
 *
 *******************************************************************************/
static volatile uint32_t signalResetDone;

/****************************************************************************//**
 *
 *                              Time section
 *
 *******************************************************************************/

/* @fn    portGetTickCnt
 * @brief wrapper for to read a SysTickTimer, which is incremented with
 *        CLOCKS_PER_SEC frequency.
 *        The resolution of time32_incr is usually 1/1000 sec.
 * */
__INLINE uint32_t
portGetTickCnt(void)
{
    return HAL_GetTick();
}


/* @fn    usleep
 * @brief precise usleep() delay
 * */
#pragma GCC optimize ("O0")

int usleep(uint16_t usec)
{
    int i, j;
#pragma GCC ivdep
    for(i = 0; i < usec; i++)
    {
#pragma GCC ivdep
        for(j = 0; j < 2; j++)
        {
            __NOP();
            __NOP();
        }
    }
    return 0;
}


/* @fn    Sleep
 * @brief Sleep delay in ms using SysTick timer
 * */
__INLINE void
Sleep(uint32_t x)
{
    HAL_Delay(x);
}

/****************************************************************************//**
 *
 *                              END OF Time section
 *
 *******************************************************************************/

/****************************************************************************//**
 *
 *                              Configuration section
 *
 *******************************************************************************/

/* @fn    peripherals_init
 * */
int peripherals_init(void)
{
    /* All has been initialized in the CubeMx code, see main.c */
    return 0;
}

/**
  * @brief  Checks whether the specified EXTI line is enabled or not.
  * @param  EXTI_Line: specifies the EXTI line to check.
  *   This parameter can be:
  *     @arg EXTI_Linex: External interrupt line x where x(0..19)
  * @retval The "enable" state of EXTI_Line (SET or RESET).
  */
ITStatus EXTI_GetITEnStatus(uint32_t x)
{
    return ((NVIC->ISER[(((uint32_t) x) >> 5UL)] & \
            (uint32_t) (1UL << (((uint32_t) x) & 0x1FUL))) == (uint32_t) RESET) ? (RESET) : (SET);
}
/****************************************************************************//**
 *
 *                          End of configuration section
 *
 *******************************************************************************/

/****************************************************************************//**
 *
 *                          DW1000 port section
 *
 *******************************************************************************/

/* @fn      reset_DW1000
 * @brief   DW_RESET pin on DW1000 has 2 functions
 *          In general it is output, but it also can be used to reset the digital
 *          part of DW1000 by driving this pin low.
 *          Note, the DW_RESET pin should not be driven high externally.
 * */
void reset_DW1000(DW1000_Port_t *antenna_port)
{
    //拉低开漏的RESET引脚
    HAL_GPIO_WritePin(antenna_port->rstn_port,
                      antenna_port->rstn_pin,
                      GPIO_PIN_RESET);

    usleep(1);

    //put the pin back to output open-drain (not active)
    setup_DW1000RSTnIRQ(0, antenna_port);

    Sleep(2);
}

/**
 * @brief DW1000的RESET引脚可以用来作为外部中断 示意已经wake up完成
 * 见DATA SHEET的Use of RSTn by external circuitry
 * 主要是用来唤醒DW1000  唤醒后进入init模式  DW1000自动拉高RESET
 * @param enable
 * @param antenna_port
 */
void setup_DW1000RSTnIRQ(int enable, DW1000_Port_t *antenna_port)
{
//    GPIO_InitTypeDef GPIO_InitStruct;
//    IRQn_Type irq;
//    GPIO_InitStruct.Pin = antenna_port->rstn_pin;
//
//	irq = UWB_device_array[antenna_port->index].port.irq;
//
//    if(enable)
//    {
//        GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//        GPIO_InitStruct.Pull = GPIO_NOPULL;
//        HAL_GPIO_Init(antenna_port->rstn_port,
//                      &GPIO_InitStruct);
//
//        HAL_NVIC_EnableIRQ(irq);
//        HAL_NVIC_SetPriority(irq,
//                             0,
//                             0);
//    }
//    else
//    {
//        HAL_NVIC_DisableIRQ(irq);
//
//        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
//        GPIO_InitStruct.Pull = GPIO_NOPULL;
//        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//        HAL_GPIO_Init(antenna_port->rstn_port,
//                      &GPIO_InitStruct);
//        HAL_GPIO_WritePin(antenna_port->rstn_port,
//                          antenna_port->rstn_pin,
//                          GPIO_PIN_SET);
//    }
}

/* @fn      port_wakeup_dw1000
 * @brief   "slow" waking up of DW1000 using DW_CS only
 * */
void port_wakeup_dw1000(DW1000_Port_t *antenna_port)
{
    HAL_GPIO_WritePin(antenna_port->wakeup_port,
                      antenna_port->wakeup_pin,
                      GPIO_PIN_RESET);
    Sleep(1);
    HAL_GPIO_WritePin(antenna_port->wakeup_port,
                      antenna_port->wakeup_pin,
                      GPIO_PIN_SET);
    Sleep(7);                       //wait 7ms for DW1000 XTAL to stabilise
}

/* @fn      port_wakeup_dw1000_fast
 * @brief   waking up of DW1000 using DW_CS and DW_RESET pins.
 *          The DW_RESET signalling that the DW1000 is in the INIT state.
 *          the total fast wakeup takes ~2.2ms and depends on crystal startup time
 * */
void port_wakeup_dw1000_fast(DW1000_Port_t *antenna_port)
{
#define WAKEUP_TMR_MS   (10)

    uint32_t x = 0;
    uint32_t timestamp = HAL_GetTick(); //protection

    setup_DW1000RSTnIRQ(0,
                        antenna_port);         //disable RSTn IRQ
    signalResetDone = 0;            //signalResetDone connected to RST_PIN_IRQ
    setup_DW1000RSTnIRQ(1,
                        antenna_port);         //enable RSTn IRQ
    HAL_GPIO_WritePin(antenna_port->wakeup_port,
                      antenna_port->wakeup_pin,
                      GPIO_PIN_RESET);

    //need to poll to check when the DW1000 is in the IDLE, the CPLL interrupt is not reliable
    //when RSTn goes high the DW1000 is in INIT, it will enter IDLE after PLL lock (in 5 us)
    while((signalResetDone == 0) && \
          ((HAL_GetTick() - timestamp) < WAKEUP_TMR_MS))
    {
        x++;     //when DW1000 will switch to an IDLE state RSTn pin will high
    }
    setup_DW1000RSTnIRQ(0,
                        antenna_port);         //disable RSTn IRQ
    HAL_GPIO_WritePin(antenna_port->wakeup_port,
                      antenna_port->wakeup_pin,
                      GPIO_PIN_SET);

    //it takes ~35us in total for the DW1000 to lock the PLL, download AON and go to IDLE state
    usleep(35);
}


/**
 * @brief 好像是强制初始化(Init中会关闭SPI) 可能需要加锁(ing)
 * @param antenna_port
 */
void port_set_dw1000_slowrate(DW1000_Port_t *antenna_port)
{
    antenna_port->hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
    HAL_SPI_Init(antenna_port->hspi);
}

/* @fn      port_set_dw1000_fastrate
 * @brief   set 18MHz
 *          note: hspi1 is clocked from 72MHz
 * */
void port_set_dw1000_fastrate(DW1000_Port_t *antenna_port)
{
    antenna_port->hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    HAL_SPI_Init(antenna_port->hspi);
}



/****************************************************************************//**
 *
 *                          End APP port section
 *
 *******************************************************************************/



/****************************************************************************//**
 *
 *                              IRQ section
 *
 *******************************************************************************/

/**
 * @brief 由DW1000的IRQ引脚触发的STM32外部中断 所有测距通信数据的来源
 * 这里需要根据GPIO_Port和GPIO_Pin分辨出是哪个DW1000引起的中断
 * 本函数和不同的引脚连接有关 因此不同的项目移植本函数都要进行修改(还没有想到通用移植办法)
 * @param GPIO_Pin
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    uint8_t pin_index = __builtin_ctz(GPIO_Pin);
    uint32_t syscfg_exticr = SYSCFG->EXTICR[pin_index / 4];
    uint8_t port_index = (syscfg_exticr >> (4 * (pin_index % 4))) & 0xFF;

	/* 这里的dwt_setlocaldataptr(index)非常重要 index一定要和UWB_device_array[]的索引对上 */
    if(GPIO_Pin == UWB1_IRQ_Pin && (GPIO_PORT_INDEX(UWB1_IRQ_GPIO_Port) == port_index))
    {
        dwt_setlocaldataptr(0);
        process_deca_irq(&UWB_device_array[0].port);
    }
    else if(GPIO_Pin == UWB2_IRQ_Pin && (GPIO_PORT_INDEX(UWB2_IRQ_GPIO_Port) == port_index))
	{
		dwt_setlocaldataptr(1);
		process_deca_irq(&UWB_device_array[1].port);
	}
	else if(GPIO_Pin == UWB3_IRQ_Pin && (GPIO_PORT_INDEX(UWB3_IRQ_GPIO_Port) == port_index))
	{
		dwt_setlocaldataptr(2);
		process_deca_irq(&UWB_device_array[2].port);
	}
	else if(GPIO_Pin == UWB4_IRQ_Pin && (GPIO_PORT_INDEX(UWB4_IRQ_GPIO_Port) == port_index))
	{
		dwt_setlocaldataptr(3);
		process_deca_irq(&UWB_device_array[3].port);
	}
    else if((GPIO_Pin == UWB1_RESET_Pin && (GPIO_PORT_INDEX(UWB1_RESET_GPIO_Port) == port_index))
			|| (GPIO_Pin == UWB2_RESET_Pin && (GPIO_PORT_INDEX(UWB2_RESET_GPIO_Port) == port_index))
				|| (GPIO_Pin == UWB3_RESET_Pin && (GPIO_PORT_INDEX(UWB3_RESET_GPIO_Port) == port_index))
					|| (GPIO_Pin == UWB4_RESET_Pin && (GPIO_PORT_INDEX(UWB4_RESET_GPIO_Port) == port_index)))
    {
        signalResetDone = 1;
    }
}

/* @fn      process_deca_irq
 * @brief   main call-back for processing of DW1000 IRQ
 *          it re-enters the IRQ routing and processes all events.
 *          After processing of all events, DW1000 will clear the IRQ line.
 * */
__INLINE void process_deca_irq(DW1000_Port_t *antenna_port)
{
    while(port_CheckEXT_IRQ(antenna_port) != 0)
    {

        port_deca_isr(antenna_port);

    } //while DW1000 IRQ line active
}


/* @fn      port_DisableEXT_IRQ
 * @brief   wrapper to disable DW_IRQ pin IRQ
 *          in current implementation it disables all IRQ from lines 5:9
 * */
__INLINE void port_DisableEXT_IRQ(DW1000_Port_t *antenna_port)
{
    NVIC_DisableIRQ(antenna_port->irq);
}

/* @fn      port_EnableEXT_IRQ
 * @brief   wrapper to enable DW_IRQ pin IRQ
 *          in current implementation it enables all IRQ from lines 5:9
 * */
__INLINE void port_EnableEXT_IRQ(DW1000_Port_t *antenna_port)
{
    NVIC_EnableIRQ(antenna_port->irq);
}


/* @fn      port_GetEXT_IRQStatus
 * @brief   wrapper to read a DW_IRQ pin IRQ status
 * */
__INLINE uint32_t port_GetEXT_IRQStatus(DW1000_Port_t *antenna_port)
{
    return EXTI_GetITEnStatus(antenna_port->irq);
}


/* @fn      port_CheckEXT_IRQ
 * @brief   wrapper to read DW_IRQ input pin state
 * */
__INLINE uint32_t port_CheckEXT_IRQ(DW1000_Port_t *antenna_port)
{
    return HAL_GPIO_ReadPin(antenna_port->irq_port,
                            antenna_port->irq_pin);
}


/****************************************************************************//**
 *
 *                              END OF IRQ section
 *
 *******************************************************************************/


#define REPORT_BUFSIZE  0x2000

static struct
{
    HAL_LockTypeDef Lock;     /*!< locking object*/
}
        txhandle = {.Lock = HAL_UNLOCKED};

static char rbuf[REPORT_BUFSIZE];               /**< circular report buffer, data to be transmitted in flush_report_buff() Thread */
static struct circ_buf report_buf = {.buf = rbuf,
        .head= 0,
        .tail= 0};

/* DW1000 IRQ handler definition. */
port_deca_isr_t port_deca_isr = NULL;

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
void port_set_deca_isr(port_deca_isr_t deca_isr, DW1000_Port_t *antenna_port)
{
    /* Check DW1000 IRQ activation status. */
    ITStatus en = port_GetEXT_IRQStatus(antenna_port);

    /* If needed, deactivate DW1000 IRQ during the installation of the new handler. */
    if(en)
    {
        port_DisableEXT_IRQ(antenna_port);
    }
    port_deca_isr = deca_isr;
    if(en)
    {
        port_EnableEXT_IRQ(antenna_port);
    }
}


/****************************************************************************//**
 *
 *******************************************************************************/

