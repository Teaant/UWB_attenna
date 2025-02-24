# ifndef __AOA_H__
# define __AOA_H__

#include "main.h"
#include "compiler.h"

#define UWB_CARRIER_FREQ_CH2 		(4.0e9f)
#define LAMDA_M 					(SPEED_OF_LIGHT / UWB_CARRIER_FREQ_CH2)	//wave length of carrier wave
#define D_M							(0.02f)   //distance between two antennas

#define BUFFER_LEN      (128)


typedef struct
{
	uint16_t src_addr;  //接收到的相位的源地址
	uint32_t sequence;
	float phi;
	float beta;
} __align4 AoAParamTypeDef;

typedef struct{
	uint8_t my_dw_id;
	uint8_t rcphase;
	uint32_t rx_ts;
	float fp_angle;
	uint16_t fp_index;
	uint16_t fp_amp1;
	uint16_t fp_amp2;
	uint16_t fp_amp3;
	uint32_t fp_amp_sum;
	uint16_t std_noise;
	uint8_t avalible;
	uint16_t rxpacc;
	uint16_t rxpacc_nosat;
	uint16_t cir_pwr;
}AoADiagnosticTypeDef;


typedef struct
{
    uint8_t txBuffer[BUFFER_LEN];
    uint8_t rxBuffer[BUFFER_LEN];
}__align4 Buffer_t;

typedef struct
{
	SPI_HandleTypeDef * hspi;
	GPIO_TypeDef * spi_csn_port;
	uint16_t spi_csn_pin;
	GPIO_TypeDef * wakeup_port;
	uint16_t wakeup_pin;
	GPIO_TypeDef * rstn_port;
	uint16_t rstn_pin;
	GPIO_TypeDef * irq_port;
	uint16_t irq_pin;
	uint32_t irq;

	/**
	 * @brief 不同的DW1000连接不同的hspi、irq等
	 * 但是port->hspi == &hspi1这种判断不方便 因为多个DW1000可以复用SPI和IRQ
	 * 用index分辨不同的DW1000(例如本DW1000在设备数组的索引)
	 */
	uint8_t index;
	uint8_t available;

    Buffer_t antenna_buffer;  //收发缓冲

    AoAParamTypeDef aoa_param;

    AoADiagnosticTypeDef aoa_diagnose;


} __align4 DW1000_Port_t;

typedef struct
{
    uint16_t PAN_id;        //个域网ID,同一簇大概率都是同一个PANID
    uint16_t my_addr;       //被某种分配方法(自行实现)分配的唯一地址
    uint8_t avalible;       //是否可用(已初始化、未被锁等)

    DW1000_Port_t port;   	//硬件

} __align4 UWB_Device;

# endif
