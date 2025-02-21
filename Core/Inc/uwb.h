#include <stdio.h>
#include <string.h>

#include "deca_device_api.h"
#include "deca_regs.h"
#include "deca_spi.h"
#include "port.h"
#include "main.h"

#ifndef __UWB_H_
#define __UWB_H_
/* Inter-ranging delay period, in milliseconds. */
#define RNG_DELAY_MS 1000

/* Default antenna delay values for 64 MHz PRF. See NOTE 1 below. */
//#define TX_ANT_DLY 16505
#define TX_ANT_DLY 16451

//#define RX_ANT_DLY 16505
#define RX_ANT_DLY 16451

#define TEST_TS 0.5f
#define PI 3.1415926

/* Length of the common part of the message (up to and including the function code, see NOTE 2 below). */
#define ALL_MSG_COMMON_LEN 10
/* Index to access some of the fields in the frames involved in the process. */
#define ALL_MSG_SN_IDX 2
#define FINAL_MSG_POLL_TX_TS_IDX 11
#define FINAL_MSG_RESP_RX_TS_IDX 16
#define FINAL_MSG_FINAL_TX_TS_IDX 21

//BY ZhuRan start
#define FINAL_MSG_DISPLACEMENT_IDX 26
//# define FINAL_MSG_END_FILTING_IDX 34
# define FINAL_MSG_DISPLACEMENT_LEN 8


# define ACK_MSG_COORDINATE_IDX 43
//# define ACK_MSG_COORDINATE_UPDATE_IDX 51
# define ACK_MSG_COORDINATE_LEN 8
//BY ZhuRan end



#define FINAL_MSG_TS_LEN 5
#define ALL_MSG_ADD_LEN 2
#define ALL_MSG_SRC_IDX 80
#define ALL_MSG_DIST_IDX 82
#define ACK_MSG_TOF_IDX 11
#define ACK_MSG_D1_IDX 19
#define ACK_MSG_D2_IDX 27
#define ACK_MSG_D3_IDX 35

#define POLL_MSG_TEST_IDX 90
#define RESP_MSG_TEST_IDX 90
/* Buffer to store received messages.
 * Its size is adjusted to longest frame that this example code is supposed to handle. */
#define RX_BUF_LEN 24

/* UWB microsecond (uus) to device time unit (dtu, around 15.65 ps) conversion factor.
 * 1 uus = 512 / 499.2 �s and 1 �s = 499.2 * 128 dtu. */
#define UUS_TO_DWT_TIME 65536

/* Delay between frames, in UWB microseconds. See NOTE 4 below. */
/* This is the delay from Frame RX timestamp to TX reply timestamp used for calculating/setting the DW1000's delayed TX function. This includes the
 * frame length of approximately 2.46 ms with above configuration. */
#define POLL_RX_TO_RESP_TX_DLY_UUS 3000
/* This is the delay from the end of the frame transmission to the enable of the receiver, as programmed for the DW1000's wait for response feature. */
#define RESP_TX_TO_FINAL_RX_DLY_UUS 0
/* Receive final timeout. See NOTE 5 below. */
#define FINAL_RX_TIMEOUT_UUS 0
/* This is the delay from the end of the frame transmission to the enable of the receiver, as programmed for the DW1000's wait for response feature. */
#define POLL_TX_TO_RESP_RX_DLY_UUS 0
/* This is the delay from Frame RX timestamp to TX reply timestamp used for calculating/setting the DW1000's delayed TX function. This includes the
 * frame length of approximately 2.66 ms with above configuration. */
#define RESP_RX_TO_FINAL_TX_DLY_UUS 10000
/* Receive response timeout. See NOTE 5 below. */
#define RESP_RX_TIMEOUT_UUS 0

#define FINAL_RX_TO_ACK_TX_DLY_UUS 20000
#define ACK_RX_TIMEOUT_UUS 0
#define FINAL_TX_TO_ACK_RX_DLY_UUS 0

/* Preamble timeout, in multiple of PAC size. See NOTE 6 below. */
#define PRE_TIMEOUT 0

/* Timestamps of frames transmission/reception.
 * As they are 40-bit wide, we need to define a 64-bit int type to handle them. */
typedef signed long long int64;
typedef unsigned long long uint64;

/* Speed of light in air, in metres per second. */
#define SPEED_OF_LIGHT 299702547


#define RXPACC_ADJUSTMENT (-18) //adjustment for decawave PRF length 16
#define A_PRF64M (121.74f)


typedef enum
{
    UWB_OK       = 0x00,
    UWB_ERROR    = 0x01,
    UWB_BUSY     = 0x02,
    UWB_TIMEOUT  = 0x03
} UWB_StatusTypeDef;

UWB_StatusTypeDef Antenna_Array_Init();
void rxOkCallback(const dwt_cb_data_t *, DW1000_Port_t *);
void rxToCallback(const dwt_cb_data_t *, DW1000_Port_t *);
void rxErrCallback(const dwt_cb_data_t *, DW1000_Port_t *);

UWB_StatusTypeDef Check_DW1000RSTn(uint32_t Timeout);

void uwb_tx_sync_msg();
void uwb_tx_measure_msg();

float uwb_calculate_rx_power(uint16_t cir_pwr, uint16_t rxpacc, uint16_t rxpacc_nosat);
float uwb_calculate_fp_power(uint16_t fp_amp1, uint16_t fp_amp2, uint16_t fp_amp3, uint16_t rxpacc, uint16_t rxpacc_nosat);
#endif
