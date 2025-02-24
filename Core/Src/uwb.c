#include "main.h"
#include "spi.h"
#include "usart.h"
#include "uwb.h"
#include <math.h>

//Tanya_add
#include "aoa_queue.h"
#include "tim.h"
#include "uwb_msg.h"

//#define ACC_MEM_TEST 1
/* Default communication configuration. We use here EVK1000's default mode (mode 3). */
static dwt_config_t config = {
		2,               /* Channel number. */
		DWT_PRF_64M,     /* Pulse repetition frequency. */
		DWT_PLEN_1024,   /* Preamble length. Used in TX only. */
		DWT_PAC32,       /* Preamble acquisition chunk size. Used in RX only. */
		9,               /* TX preamble code. Used in TX only. */
		9,               /* RX preamble code. Used in RX only. */
		0,               /* 0 to use standard SFD, 1 to use non-standard SFD. */
		DWT_BR_850K,     /* Data rate. */
		DWT_PHRMODE_STD, /* PHY header mode. */
		(4000 + 64 - 32) /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
};

/* Declaration of static functions. */
static uint64 get_tx_timestamp_u64(DW1000_Port_t *pports);

static uint64 get_rx_timestamp_u64(DW1000_Port_t *pports);

extern volatile UWB_Device UWB_device_array[DWT_NUM_DW_DEV];

//Tanya_add
volatile uint32_t data_sequence = 0;


UWB_StatusTypeDef Antenna_Array_Init()
{
	for(int i = 0; i < DWT_NUM_DW_DEV; i++)
	{
		UWB_device_array[0].port.index = 0;
		UWB_device_array[0].port.hspi = &hspi1;
		UWB_device_array[0].port.irq_pin = UWB1_IRQ_Pin;
		UWB_device_array[0].port.irq_port = UWB1_IRQ_GPIO_Port;
		UWB_device_array[0].port.rstn_pin = UWB1_RESET_Pin;
		UWB_device_array[0].port.rstn_port = UWB1_RESET_GPIO_Port;
		UWB_device_array[0].port.spi_csn_pin = UWB1_CSn_Pin;
		UWB_device_array[0].port.spi_csn_port = UWB1_CSn_GPIO_Port;
		UWB_device_array[0].port.wakeup_pin = UWB1_WAKEUP_Pin;
		UWB_device_array[0].port.wakeup_port = UWB1_WAKEUP_GPIO_Port;
		UWB_device_array[0].port.irq = EXTI0_IRQn;

		UWB_device_array[1].port.index = 1;
		UWB_device_array[1].port.hspi = &hspi1;
		UWB_device_array[1].port.irq_pin = UWB2_IRQ_Pin;
		UWB_device_array[1].port.irq_port = UWB2_IRQ_GPIO_Port;
		UWB_device_array[1].port.rstn_pin = UWB2_RESET_Pin;
		UWB_device_array[1].port.rstn_port = UWB2_RESET_GPIO_Port;
		UWB_device_array[1].port.spi_csn_pin = UWB2_CSn_Pin;
		UWB_device_array[1].port.spi_csn_port = UWB2_CSn_GPIO_Port;
		UWB_device_array[1].port.wakeup_pin = UWB2_WAKEUP_Pin;
		UWB_device_array[1].port.wakeup_port = UWB2_WAKEUP_GPIO_Port;
		UWB_device_array[1].port.irq = EXTI9_5_IRQn;

		UWB_device_array[2].port.index = 2;
		UWB_device_array[2].port.hspi = &hspi2;
		UWB_device_array[2].port.irq_pin = UWB3_IRQ_Pin;
		UWB_device_array[2].port.irq_port = UWB3_IRQ_GPIO_Port;
		UWB_device_array[2].port.rstn_pin = UWB3_RESET_Pin;
		UWB_device_array[2].port.rstn_port = UWB3_RESET_GPIO_Port;
		UWB_device_array[2].port.spi_csn_pin = UWB3_CSn_Pin;
		UWB_device_array[2].port.spi_csn_port = UWB3_CSn_GPIO_Port;
		UWB_device_array[2].port.wakeup_pin = UWB3_WAKEUP_Pin;
		UWB_device_array[2].port.wakeup_port = UWB3_WAKEUP_GPIO_Port;
		UWB_device_array[2].port.irq = EXTI15_10_IRQn;

		UWB_device_array[3].port.index = 3;
		UWB_device_array[3].port.hspi = &hspi2;
		UWB_device_array[3].port.irq_pin = UWB4_IRQ_Pin;
		UWB_device_array[3].port.irq_port = UWB4_IRQ_GPIO_Port;
		UWB_device_array[3].port.rstn_pin = UWB4_RESET_Pin;
		UWB_device_array[3].port.rstn_port = UWB4_RESET_GPIO_Port;
		UWB_device_array[3].port.spi_csn_pin = UWB4_CSn_Pin;
		UWB_device_array[3].port.spi_csn_port = UWB4_CSn_GPIO_Port;
		UWB_device_array[3].port.wakeup_pin = UWB4_WAKEUP_Pin;
		UWB_device_array[3].port.wakeup_port = UWB4_WAKEUP_GPIO_Port;
		UWB_device_array[3].port.irq = EXTI9_5_IRQn;
	}

	/* Reset and initialise DW1000.
	 * For initialisation, DW1000 clocks must be temporarily set to crystal speed. After initialisation SPI rate can be increased for optimum
	 * performance. */

	for(int i = 0; i < DWT_NUM_DW_DEV; i++)
	{
		reset_DW1000((DW1000_Port_t *)&UWB_device_array[i].port);
	}

	for(int i = 0; i < DWT_NUM_DW_DEV; i++)
	{
		DW1000_Port_t *antenna_port = (DW1000_Port_t *) &UWB_device_array[i].port;
		int trys = 10;
		port_set_dw1000_slowrate(antenna_port);

		dwt_setlocaldataptr(i);

		do
		{
			if(dwt_initialise(DWT_LOADUCODE,
							  antenna_port) == DWT_SUCCESS)
			{
				UWB_device_array[i].avalible = 1;
			}
			else
			{
				reset_DW1000((DW1000_Port_t *)&UWB_device_array[i].port);
				deca_sleep(5);
				trys--;
			}
		} while((trys > 0) && UWB_device_array[i].avalible != 1);
		if(trys <= 0)
		{
			return UWB_ERROR;
		}

		port_set_dw1000_fastrate(antenna_port);

		/*Setup Interrupt*/
		dwt_setinterrupt((SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_ERR | SYS_STATUS_ALL_RX_TO),
						 2,
						 antenna_port);

		//核心 四个回调函数
		dwt_setcallbacks(NULL,
						 rxOkCallback,
						 rxToCallback,
						 rxErrCallback);
		port_set_deca_isr(dwt_isr,
						  antenna_port);

		dwt_write32bitreg(SYS_STATUS_ID,
						  SYS_STATUS_ALL_RX_GOOD | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR,
						  antenna_port);

		dwt_configure(&config,
					  antenna_port);

		dwt_setrxantennadelay(RX_ANT_DLY,
							  antenna_port);
		dwt_settxantennadelay(TX_ANT_DLY,
							  antenna_port);

		dwt_setpreambledetecttimeout(PRE_TIMEOUT,
									 antenna_port);

		/* Configure LED */
		dwt_setleds(1,
					antenna_port);

		/* Set Sync to OSTR*/
		dwt_setecctrl(EC_CTRL_OSTRM,
					  antenna_port);
	}
	return UWB_OK;
}

static uint64 get_tx_timestamp_u64(DW1000_Port_t *pports)
{
	uint8 ts_tab[5];
	uint64 ts = 0;
	int i;
	dwt_readtxtimestamp(ts_tab,
						pports);
	for(i = 4; i >= 0; i--)
	{
		ts <<= 8;
		ts |= ts_tab[i];
	}
	return ts;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn get_rx_timestamp_u64()
 *
 * @brief Get the RX time-stamp in a 64-bit variable.
 *        /!\ This function assumes that length of time-stamps is 40 bits, for both TX and RX!
 *
 * @param  none
 *
 * @return  64-bit value of the read time-stamp.
 */
static uint64 get_rx_timestamp_u64(DW1000_Port_t *pports)
{
	uint8 ts_tab[5];
	uint64 ts = 0;
	int i;
	dwt_readrxtimestamp(ts_tab,
						pports);
	for(i = 4; i >= 0; i--)
	{
		ts <<= 8;
		ts |= ts_tab[i];
	}
	return ts;
}

static float uwb_get_fp_angle(uint16_t fp_index, DW1000_Port_t *pports)
{
    uint8_t acc_buffer[5];
    uint8_t len = 4;
    int16_t cir_real, cir_imag;
    dwt_readaccdata(acc_buffer,
                    len + 1,
                    4 * fp_index,
                    pports);
    memcpy(&cir_real,
           &acc_buffer[1],
           sizeof(int16_t));
    memcpy(&cir_imag,
           &acc_buffer[3],
           sizeof(int16_t));
    return atan2f((float) cir_imag,
                  (float) cir_real);
}


static void read_pdoa(DW1000_Port_t *antenna_port, uint64 rx_ts)
{
//	uint64_t rx_ts_64 = (uint64_t) get_rx_timestamp_u64(pports);
//	uint32_t rx_ts = (uint32_t) rx_ts_64;
//	uint16_t src_car_id = uwb_node.pdoa_buffer.header.src;
//	uint8_t my_dw_id = pports - &UWB.ports[0];
	uint8_t my_dw_id = antenna_port->index;

	uint8_t rcphase;
	dwt_rxdiag_t tempdiag;
	uint8_t tempacc[129];
	uint16_t fp_index;
	float fp_angle;
	AoADiagnosticTypeDef *pdiag = &(antenna_port->aoa_diagnose);

	dwt_readrcphase(&rcphase, antenna_port);
	dwt_readdiagnostics(&tempdiag, antenna_port);
	fp_index = (uint16_t) round(((float) (tempdiag.firstPath & 0x3F) / 0x3F))
			+ (tempdiag.firstPath >> 6);
	fp_angle = uwb_get_fp_angle(fp_index, antenna_port);

	//私以为这些都没什么必要的？或可也打印出去
	pdiag->avalible = 1;
	pdiag->fp_amp1 = tempdiag.firstPathAmp1;
	pdiag->fp_amp2 = tempdiag.firstPathAmp2;
	pdiag->fp_amp3 = tempdiag.firstPathAmp3;
	pdiag->fp_amp_sum = pdiag->fp_amp1 + pdiag->fp_amp2 + pdiag->fp_amp3;
	pdiag->fp_angle = fp_angle;
	pdiag->fp_index = fp_index;
	pdiag->my_dw_id = my_dw_id;
	pdiag->rcphase = rcphase;
	pdiag->rx_ts = rx_ts;
	pdiag->std_noise = tempdiag.stdNoise;
	pdiag->cir_pwr = tempdiag.maxGrowthCIR;
	pdiag->rxpacc = tempdiag.rxPreamCount;
	pdiag->rxpacc_nosat = tempdiag.rxPreamCountNOSAT;

	//test
	if (pdiag->fp_amp1 < (uint16_t) 256) {
		pdiag->avalible = 0;
		return;
	}

	antenna_port->aoa_param.phi = fp_angle;
	antenna_port->aoa_param.beta = (float) rcphase / 64.0 * PI;
	antenna_port->aoa_param.sequence = data_sequence;

	/**
	 * @TODO enqueueData  (with index)
	 */
	enqueueData(antenna_port->index);

}



void rxOkCallback(const dwt_cb_data_t *cbData, DW1000_Port_t *antenna_port)
{
	uint64 rx_timestamp = 0;
	/* Clear reception timeout to start next ranging process. */
	dwt_setrxtimeout(0,
					 antenna_port);

	/* Activate reception immediately. */
	dwt_rxenable(DWT_START_RX_IMMEDIATE,
				 antenna_port);

	dwt_readrxdata(antenna_port->antenna_buffer.rxBuffer,
				   cbData->datalength,
				   0,
				   antenna_port);

	rx_timestamp = get_rx_timestamp_u64(antenna_port);

	UWB_Msg_Header_t* pmsg = (UWB_Msg_Header_t*)(antenna_port->antenna_buffer.rxBuffer);
	data_sequence = pmsg->sequence;

	read_pdoa(antenna_port, rx_timestamp);

	/* Clear reception timeout to start next ranging process. */
	dwt_setrxtimeout(0, antenna_port);

	/* Activate reception immediately. */
	dwt_rxenable(DWT_START_RX_IMMEDIATE, antenna_port);

}


void rxToCallback(const dwt_cb_data_t *cbData, DW1000_Port_t *antenna_port)
{
	/* Clear reception timeout to start next ranging process. */
	dwt_setrxtimeout(0,
					 antenna_port);

	/* Activate reception immediately. */
	dwt_rxenable(DWT_START_RX_IMMEDIATE,
				 antenna_port);
}

void rxErrCallback(const dwt_cb_data_t *cbData, DW1000_Port_t *antenna_port)
{

	/* Clear reception timeout to start next ranging process. */
	dwt_setrxtimeout(0,
					 antenna_port);

	/* Activate reception immediately. */
	dwt_rxenable(DWT_START_RX_IMMEDIATE,
				 antenna_port);
}


//void uwb_tx_sync_msg()
//{
//    UWB_Device *pports = &UWB.ports[0];
//
//    //	dwt_forcetrxoff(pports); // Turn the RX off
//    //	dwt_rxreset(pports);
//    dwt_write32bitreg(SYS_STATUS_ID,
//                      SYS_STATUS_TXFRS,
//                      pports);
//    UWB.txBuffer[ALL_MSG_SN_IDX] = AOA_SYNC_MSG;
//    memcpy(&UWB.txBuffer[ALL_MSG_SRC_IDX],
//           &UWB.leaderID,
//           sizeof(uint16_t));
//    dwt_writetxdata(sizeof(UWB.txBuffer),
//                    UWB.txBuffer,
//                    0,
//                    pports);
//    dwt_writetxfctrl(sizeof(UWB.txBuffer),
//                     0,
//                     0,
//                     pports);
//    memset(UWB.txBuffer,
//           0,
//           sizeof(UWB.txBuffer));
//    dwt_starttx(DWT_START_TX_IMMEDIATE,
//                pports);
//    //	dwt_rxenable(DWT_START_RX_IMMEDIATE, pports);
//}
//
//void uwb_tx_measure_msg()
//{
//
//    UWB_Device *pports = &UWB.ports[0];
//
//    //	dwt_forcetrxoff(pports); // Turn the RX off
//    //	dwt_rxreset(pports);
//    dwt_write32bitreg(SYS_STATUS_ID,
//                      SYS_STATUS_TXFRS,
//                      pports);
//    UWB.txBuffer[ALL_MSG_SN_IDX] = AOA_MEASURE_MSG1;
//    memcpy(&UWB.txBuffer[ALL_MSG_SRC_IDX],
//           &UWB.leaderID,
//           sizeof(uint16_t));
//    dwt_writetxdata(sizeof(UWB.txBuffer),
//                    UWB.txBuffer,
//                    0,
//                    pports);
//    dwt_writetxfctrl(sizeof(UWB.txBuffer),
//                     0,
//                     0,
//                     pports);
//    memset(UWB.txBuffer,
//           0,
//           sizeof(UWB.txBuffer));
//    dwt_starttx(DWT_START_TX_IMMEDIATE,
//                pports);
//    //	dwt_rxenable(DWT_START_RX_IMMEDIATE, pports);
//}

float uwb_calculate_rx_power(uint16_t cir_pwr, uint16_t rxpacc, uint16_t rxpacc_nosat)
{
	float adjusted_rxpacc;
	float temp1, temp2;
	const float two_17 = (float) (1 << 17);
	if(rxpacc == rxpacc_nosat)
	{
		adjusted_rxpacc = rxpacc + RXPACC_ADJUSTMENT;
	}
	else
	{
		adjusted_rxpacc = rxpacc;
	}
	temp1 = (float) cir_pwr * two_17;
	temp2 = powf(adjusted_rxpacc,
				 2);
	temp1 = temp1 / temp2;
	temp1 = log10f(temp1);
	temp1 = 10 * temp1 - A_PRF64M;
	return temp1;
	//	return 10*log10f((float)cir_pwr * two_17 / powf(adjusted_rxpacc,2)) - A_PRF64M;
}

float uwb_calculate_fp_power(uint16_t fp_amp1, uint16_t fp_amp2, uint16_t fp_amp3, uint16_t rxpacc, uint16_t rxpacc_nosat)
{
	float adjusted_rxpacc;
	if(rxpacc == rxpacc_nosat)
	{
		adjusted_rxpacc = rxpacc + RXPACC_ADJUSTMENT;
	}
	else
	{
		adjusted_rxpacc = rxpacc;
	}
	return 10 * log10f((powf((float) fp_amp1,
							 2) + powf((float) fp_amp2,
									   2) + powf((float) fp_amp3,
												 2)) / powf(adjusted_rxpacc,
															2)) - A_PRF64M;
}

UWB_StatusTypeDef Check_DW1000RSTn(uint32_t Timeout)
{
	uint32_t tickstart = HAL_GetTick();

	/**
	 * 在output模式下也可以读取引脚电平 参考手册原话：
	 * When the I/O port is programmed as output:
	 * The data present on the I/O pin are sampled into the input data register every AHB clock cycle
	 * A read access to the input data register gets the I/O state
	 */

	while(HAL_GPIO_ReadPin(UWB1_RESET_GPIO_Port,
						   UWB1_RESET_Pin) == GPIO_PIN_RESET
		  && HAL_GPIO_ReadPin(UWB2_RESET_GPIO_Port,
							  UWB2_RESET_Pin) == GPIO_PIN_RESET
		  && HAL_GPIO_ReadPin(UWB3_RESET_GPIO_Port,
							  UWB3_RESET_Pin) == GPIO_PIN_RESET
		  && HAL_GPIO_ReadPin(UWB4_RESET_GPIO_Port,
							  UWB4_RESET_Pin) == GPIO_PIN_RESET)
	{
		if((HAL_GetTick() - tickstart) > Timeout)
		{
			return UWB_TIMEOUT;
		}
	}

	/*从INIT到IDLE还需要几us 等1ms足够*/
	deca_sleep(1);

	return UWB_OK;
}
