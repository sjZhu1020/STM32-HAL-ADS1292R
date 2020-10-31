#ifndef __ADS1292R_H
#define __ADS1292R_H

#include "main.h"
#include "spi.h"
#include "hmi_uart.h"

#define ADS1292R_SCLK_Port ADS1292R_SCLK_GPIO_Port
#define ADS1292R_MISO_Port ADS1292R_MISO_GPIO_Port
#define ADS1292R_MOSI_Port ADS1292R_MOSI_GPIO_Port
#define ADS1292R_CS_Port ADS1292R_CS_GPIO_Port
#define ADS1292R_DRDY_Port ADS1292R_DRDY_GPIO_Port
#define ADS1292R_START_Port ADS1292R_START_GPIO_Port
#define ADS1292R_RES_Port ADS1292R_RES_GPIO_Port

#define ADS1292R_CS_HIGH HAL_GPIO_WritePin(ADS1292R_CS_Port, ADS1292R_CS_Pin, GPIO_PIN_SET)
#define ADS1292R_CS_LOW HAL_GPIO_WritePin(ADS1292R_CS_Port, ADS1292R_CS_Pin, GPIO_PIN_RESET)
#define ADS1292R_RES_HIGH HAL_GPIO_WritePin(ADS1292R_RES_Port, ADS1292R_RES_Pin, GPIO_PIN_SET)
#define ADS1292R_RES_LOW HAL_GPIO_WritePin(ADS1292R_RES_Port, ADS1292R_RES_Pin, GPIO_PIN_RESET)
#define ADS1292R_START_HIGH HAL_GPIO_WritePin(ADS1292R_START_Port, ADS1292R_START_Pin, GPIO_PIN_SET)
#define ADS1292R_START_LOW HAL_GPIO_WritePin(ADS1292R_START_Port, ADS1292R_START_Pin, GPIO_PIN_RESET)

/* ADS1292R command definition */
#define ADS1292R_WAKEUP 0X02
#define ADS1292R_STANDBY 0X04
#define ADS1292R_ADSRESET 0X06
#define ADS1292R_START 0X08
#define ADS1292R_STOP 0X0A
#define ADS1292R_OFFSETCAL 0X1A
/* data read command */
#define ADS1292R_RDATAC 0X10
#define ADS1292R_SDATAC 0X11
#define ADS1292R_RDATA 0X12
/* reg read command */
#define ADS1292R_RREG 0X20 //read 001r rrrr 000n nnnn, only defined high 8 bits, low 8 bits are set when sending command
#define ADS1292R_WREG 0X40 //write 010r rrrr 000n nnnn
                           /*	r rrrr=register address
		                                        n nnnn=data to send/write*/

/* ADS1292R internal reg address definition */
#define ADS1292R_ID 0X00        //ID control reg
#define ADS1292R_CONFIG1 0X01   //configuration reg 1
#define ADS1292R_CONFIG2 0X02   //configuration reg 2
#define ADS1292R_LOFF 0X03      //lead-off control reg
#define ADS1292R_CH1SET 0X04    //channel 1 setting
#define ADS1292R_CH2SET 0X05    //channel 2 setting
#define ADS1292R_RLD_SENS 0X06  //RLD sense selection
#define ADS1292R_LOFF_SENS 0X07 //lead-off sense selection
#define ADS1292R_LOFF_STAT 0X08 //lead-off status
#define ADS1292R_RESP1 0X09     //respiration control reg 1
#define ADS1292R_RESP2 0X0A     //respiration control reg 2
#define ADS1292R_GPIO 0X0B      //GPIO control reg

/* reg configuration */
/* ID  address = 0x00 */
#define DEVICE_ID_ADS1292 0X53
#define DEVICE_ID_ADS1292R 0X73
/* CONFIG1  address = 0x01 */
#define DATA_RATE_125SPS 0X00 //samping rate
#define DATA_RATE_250SPS 0X01
#define DATA_RATE_500SPS 0X02
#define DATA_RATE_1kSPS 0X03
#define DATA_RATE_2kSPS 0X04
#define DATA_RATE_4kSPS 0X05
#define DATA_RATE_8kSPS 0X06
/* CONFIG2  address = 0x02 */
#define PDB_LOFF_COMP_OFF 0 //Lead-off comparators disabled(default)
#define PDB_LOFF_COMP_ON 1

#define PDB_REFBUF_OFF 0 //Reference buffer is powered down (default)
#define PDB_REFBUF_ON 1

#define VREF_242V 0 //2.42 V reference(default)
#define VREF_4V 1   //4.033 V referenceV

#define CLK_EN_OFF 0 //Oscillator clock output disabled(default)
#define CLK_EN_ON 1

#define INT_TEST_OFF 0 //close internal test signal(default)
#define INT_TEST_ON 1

#define TEST_FREQ_DC 0 //test with DC(default)
#define TEST_FREQ_AC 1 //1Hz square wave
/* CHSET  address = 0x04 */
#define PD_ON 0 //normal operation(default)
#define PD_OFF 1

#define GAIN_6 0 //gain 6(default)
#define GAIN_1 1
#define GAIN_2 2
#define GAIN_3 3
#define GAIN_4 4
#define GAIN_8 5
#define GAIN_12 6

#define MUX_Normal_input 0  //Normal electrode input(default)
#define MUX_input_shorted 1 //Input shorted
#define MUX_Test_signal 5   //Test signal
#define MUX_RLD_DRP 6
#define MUX_RLD_DRM 7
#define MUX_RLD_DRPM 8
#define MUX_RSP_IN3P 9 //Route IN3P and IN3N to channel 1 inputs
/* RLD_SENS  address = 0x06 */
#define PDB_RLD_OFF 0 //(default)
#define PDB_RLD_ON 1

#define RLD_LOFF_SENSE_OFF 0 //(default)
#define RLD_LOFF_SENSE_ON 1

#define RLD_CANNLE_OFF 0 //(default)
#define RLD_CANNLE_ON 1
/* LOFF_SENS  address = 0x07 */
#define FLIP2_OFF 0 // Current direction selection(default)
#define FLIP2_ON 1

#define FLIP1_OFF 0 //(default)
#define FLIP1_ON 1

#define LOFF_CANNLE_OFF 0 //(default)
#define LOFF_CANNLE_ON 1
/* RESP1  address = 0x09 */
#define RESP_DEMOD_OFF 0 //(default)
#define RESP_DEMOD_ON 1

#define RESP_MOD_OFF 0 //(default)
#define RESP_MOD_ON 1

#define RESP_CTRL_CLOCK_INTERNAL 0
#define RESP_CTRL_CLOCK_EXTERNAL 1
/* RESP2  address = 0x0A */
#define CALIB_OFF 0 //(default)
#define CALIB_ON 1

#define FREQ_32K 0 //(default)
#define FREQ_64K 1

#define RLDREF_INT_EXTERN 0     //external fed signal RLDREF
#define RLDREF_INT_INTERNALLY 1 //internal

typedef struct
{
    uint8_t Data_Rate; //ADC channel samping rate
} ADS1292_CONFIG1;

typedef struct
{
    uint8_t Pdb_Loff_Comp; //Lead-off comparator power-down
    uint8_t Pdb_Refbuf;    //Reference buffer power-down
    uint8_t Vref;          //internal voltage reference
    uint8_t Clk_EN;        //CLK connection
    uint8_t Int_Test;      //Test signal selection
    uint8_t Test_Freq;     //Test signal frequency
} ADS1292_CONFIG2;

typedef struct
{
    uint8_t PD;   //Channel power-down
    uint8_t GAIN; //PGA gain setting
    uint8_t MUX;  //input selection
} ADS1292_CHSET;

typedef struct
{
    uint8_t Pdb_Rld;        //RLD buffer power
    uint8_t Rld_Loff_Sense; //RLD lead-off sense function
    uint8_t Rld2N;          //Channel 2 RLD negative inputs
    uint8_t Rld2P;          //Channel 2 RLD positive inputs
    uint8_t Rld1N;          //Channel 1 RLD negative inputs
    uint8_t Rld1P;          //RLD1P: Channel 1 RLD positive inputs
} ADS1292_RLD_SENS;

typedef struct
{
    uint8_t Flip2; //Current direction selection
    uint8_t Flip1;
    uint8_t Loff2N; //Channel 2 lead-off detection negative inputs
    uint8_t Loff2P; //Channel 2 lead-off detection positive inputs
    uint8_t Loff1N; //Channel 1 lead-off detection negative inputs
    uint8_t Loff1P; //Channel 1 lead-off detection positive inputs
} ADS1292_LOFF_SENS;

typedef struct
{
    uint8_t RESP_DemodEN; //Enables respiration demodulation circuitry
    uint8_t RESP_modEN;   //Enables respiration modulation circuitry
    uint8_t RESP_ph;
    uint8_t RESP_Ctrl;
} ADS1292_RESP1;

typedef struct
{
    uint8_t Calib;      //Calibration on
    uint8_t freq;       //Respiration control frequency
    uint8_t Rldref_Int; //RLDREF signal
} ADS1292_RESP2;

void ADS1292R_SetBuffer(void);

uint8_t ADS1292R_SPI_TransmitReceive(uint8_t tx_data);
uint8_t ADS1292R_SendCmd(uint8_t cmd);
uint8_t ADS1292R_RwReg(uint8_t cmd, uint8_t data);
void ADS1292R_WriteMultiRegs(uint8_t reg, uint8_t *data, uint8_t len);
void ADS1292R_ReadMultiRegs(uint8_t reg, uint8_t *data, uint8_t len);

void ADS1292R_GetValue(void);

void ADS1292R_Init(void);
void ADS1292R_ADCStartNormal(void);
void ADS1292R_ADCStartTest(void);

#endif