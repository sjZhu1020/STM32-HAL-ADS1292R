#include "ads1292r.h"

uint8_t ADS1292R_data_buf[9] = {0}; //ADS1292R receive data buffer
volatile uint8_t ADS1292R_tmp[9] = {0};
uint8_t ADS1292R_receive_flag = 0;
volatile uint8_t ADS1292R_reg[12]; //ADS1292R register array

ADS1292_CONFIG1 ADS1292R_config1 = {DATA_RATE_500SPS}; //set sample rate to 500

ADS1292_CONFIG2 ADS1292R_config2 = {PDB_LOFF_COMP_ON, //Lead-off comparators enabled
                                    PDB_REFBUF_ON,    //Reference buffer enabled
                                    VREF_242V,        //2.42V reference voltage
                                    CLK_EN_OFF,       //interal clock output disabled
                                    INT_TEST_OFF,     //close test signal
                                    TEST_FREQ_DC};    //set test signal ferquency is DC

ADS1292_CHSET ADS1292R_ch1set = {PD_ON,             //ch1 power on
                                 GAIN_2,            //set PGA gain is 2
                                 MUX_Normal_input}; //normal electrode input

ADS1292_CHSET ADS1292R_ch2set = {PD_ON,             //ch2 power on
                                 GAIN_6,            //set PGA gain is 6
                                 MUX_Normal_input}; //normal electrode input

ADS1292_RLD_SENS ADS1292R_rld_sens = {PDB_RLD_ON,         //RLD buffer power on
                                      RLD_LOFF_SENSE_OFF, /*RLD lead-off sense off
                                                           (RLD lead-off testing and RLD input may not work together)*/
                                      RLD_CANNLE_ON,      //RLD2 input on
                                      RLD_CANNLE_ON,
                                      RLD_CANNLE_OFF, //breathing channel don't need RLD input
                                      RLD_CANNLE_OFF};

ADS1292_LOFF_SENS ADS1292R_loff_sens = {FLIP2_OFF,     //disable the controller of current directions for lead-off derivation for channel 2
                                        FLIP1_OFF,     //disable the controller of current directions for lead-off derivation for channel 1
                                        RLD_CANNLE_ON, //enable lead-off detection
                                        RLD_CANNLE_ON,
                                        RLD_CANNLE_OFF, //breathing channel don't need lead-off detection
                                        RLD_CANNLE_OFF};

ADS1292_RESP1 ADS1292R_resp1 = {RESP_DEMOD_ON,             //enable channel 1 demodulation circuitry
                                RESP_MOD_ON,               //enable channel 1 modulation circuitry
                                0X0A,                      //the phase of respiration demodulation control signal 112.5 degrees
                                RESP_CTRL_CLOCK_INTERNAL}; //set internal respiration with interal clock

ADS1292_RESP2 ADS1292R_resp2 = {CALIB_OFF,              //disable offset calibration
                                                        //turn on channel offset correction and execute OFFSETCAL command if change the gain during conversion
                                FREQ_32K,               //respiration control frequency
                                RLDREF_INT_INTERNALLY}; //RLDREF generated internality (AVDD + AVSS) / 2

/**
  * @name   ADS1292R_SetBuffer(void)
  *
  * @brief  Sets register arrays.
  *
  * @note                  
  *
  * @retval None
  */
void ADS1292R_SetBuffer(void)
{
    ADS1292R_reg[ADS1292R_ID] = DEVICE_ID_ADS1292R; //ID read only

    ADS1292R_reg[ADS1292R_CONFIG1] = 0x00;
    ADS1292R_reg[ADS1292R_CONFIG1] |= ADS1292R_config1.Data_Rate;

    ADS1292R_reg[ADS1292R_CONFIG2] = 0x00;
    ADS1292R_reg[ADS1292R_CONFIG2] |= ADS1292R_config2.Test_Freq;
    ADS1292R_reg[ADS1292R_CONFIG2] |= ADS1292R_config2.Int_Test << 1;
    ADS1292R_reg[ADS1292R_CONFIG2] |= ADS1292R_config2.Clk_EN << 3;
    ADS1292R_reg[ADS1292R_CONFIG2] |= ADS1292R_config2.Vref << 4;
    ADS1292R_reg[ADS1292R_CONFIG2] |= ADS1292R_config2.Pdb_Refbuf << 5;
    ADS1292R_reg[ADS1292R_CONFIG2] |= ADS1292R_config2.Pdb_Loff_Comp << 6;
    ADS1292R_reg[ADS1292R_CONFIG2] |= 0x80; //set default

    ADS1292R_reg[ADS1292R_LOFF] = 0xF0;

    ADS1292R_reg[ADS1292R_CH1SET] = 0x00;
    ADS1292R_reg[ADS1292R_CH1SET] |= ADS1292R_ch1set.MUX;
    ADS1292R_reg[ADS1292R_CH1SET] |= ADS1292R_ch1set.GAIN << 4;
    ADS1292R_reg[ADS1292R_CH1SET] |= ADS1292R_ch1set.PD << 7;

    ADS1292R_reg[ADS1292R_CH2SET] = 0x00;
    ADS1292R_reg[ADS1292R_CH2SET] |= ADS1292R_ch1set.MUX;
    ADS1292R_reg[ADS1292R_CH2SET] |= ADS1292R_ch1set.GAIN << 4;
    ADS1292R_reg[ADS1292R_CH2SET] |= ADS1292R_ch1set.PD << 7;

    ADS1292R_reg[ADS1292R_RLD_SENS] = 0X00;
    ADS1292R_reg[ADS1292R_RLD_SENS] |= ADS1292R_rld_sens.Rld1P;
    ADS1292R_reg[ADS1292R_RLD_SENS] |= ADS1292R_rld_sens.Rld1N << 1;
    ADS1292R_reg[ADS1292R_RLD_SENS] |= ADS1292R_rld_sens.Rld2P << 2;
    ADS1292R_reg[ADS1292R_RLD_SENS] |= ADS1292R_rld_sens.Rld2N << 3;
    ADS1292R_reg[ADS1292R_RLD_SENS] |= ADS1292R_rld_sens.Rld_Loff_Sense << 4;
    ADS1292R_reg[ADS1292R_RLD_SENS] |= ADS1292R_rld_sens.Pdb_Rld << 5;
    ADS1292R_reg[ADS1292R_RLD_SENS] = 0XC0; //set default

    ADS1292R_reg[ADS1292R_LOFF_SENS] = 0X00;
    ADS1292R_reg[ADS1292R_LOFF_SENS] |= ADS1292R_loff_sens.Loff1P;
    ADS1292R_reg[ADS1292R_LOFF_SENS] |= ADS1292R_loff_sens.Loff1N << 1;
    ADS1292R_reg[ADS1292R_LOFF_SENS] |= ADS1292R_loff_sens.Loff2P << 2;
    ADS1292R_reg[ADS1292R_LOFF_SENS] |= ADS1292R_loff_sens.Loff2N << 3;
    ADS1292R_reg[ADS1292R_LOFF_SENS] |= ADS1292R_loff_sens.Flip1 << 4;
    ADS1292R_reg[ADS1292R_LOFF_SENS] |= ADS1292R_loff_sens.Flip2 << 5;

    ADS1292R_reg[ADS1292R_LOFF_STAT] = 0X00; /*[6]=0 set the ratio between fCLK and fMOD, fCLK=fMOD/4  
                                            [4:0]read only, set status of lead-off and elevtrode status */

    ADS1292R_reg[ADS1292R_RESP1] = 0X00;
    ADS1292R_reg[ADS1292R_RESP1] |= ADS1292R_resp1.RESP_Ctrl;
    ADS1292R_reg[ADS1292R_RESP1] |= ADS1292R_resp1.RESP_ph << 2;
    ADS1292R_reg[ADS1292R_RESP1] |= ADS1292R_resp1.RESP_modEN << 6;
    ADS1292R_reg[ADS1292R_RESP1] |= ADS1292R_resp1.RESP_DemodEN << 7;
    ADS1292R_reg[ADS1292R_RESP1] = 0X02; //set default

    ADS1292R_reg[ADS1292R_RESP2] = 0X00;
    ADS1292R_reg[ADS1292R_RESP2] |= ADS1292R_resp2.Rldref_Int << 1;
    ADS1292R_reg[ADS1292R_RESP2] |= ADS1292R_resp2.freq << 2;
    ADS1292R_reg[ADS1292R_RESP2] |= ADS1292R_resp2.Calib << 7;
    ADS1292R_reg[ADS1292R_RESP2] = 0X01;

    ADS1292R_reg[ADS1292R_GPIO] = 0X0C; /* GPIO set input [7:4] must be 0	
                                                    [3:2]==11 GPIO is input  
                                                    [1:0] when set input，return GPIO status;
                                                    when set output , set the output value*/
}

/**
  * @name   ADS1292R_SPI_TransmitReceive(uint8_t tx_data)
  *
  * @brief  Using SPI to transmit data and receive data.
  *
  * @note   
  *
  * @param  tx_data: the data to transmit with SPI                
  *
  * @retval rx_data
  */
uint8_t ADS1292R_SPI_TransmitReceive(uint8_t tx_data)
{
    uint8_t rx_data = 0;
    HAL_SPI_TransmitReceive(&hspi1, &tx_data, &rx_data, 1, 0XFFFF);
    return rx_data;
}

/**
  * @name   ADS1292R_SendCmd(uint8_t cmd)
  *
  * @brief  Send command to ADS1292R.
  *
  * @note   
  *
  * @param  cmd: the command to send            
  *
  * @retval rx_data
  *
  * @version  
  */
uint8_t ADS1292R_SendCmd(uint8_t cmd)
{
    ADS1292R_CS_LOW;
    HAL_Delay(1);

    uint8_t rx_data = ADS1292R_SPI_TransmitReceive(cmd);
    HAL_Delay(1);
    ADS1292R_CS_HIGH;
    return rx_data;
}

/**
  * @name   ADS1292R_RwReg(uint8_t cmd,uint8_t data)
  *
  * @brief  Write and read internal reg of ADS1292R.
  *
  * @note   
  *
  * @param  cmd: read or write command
  *            @arg  ADS1292R_RREG
  *            @arg  ADS1292R_WREG 
  * @param  data: content to send to register         
  *
  * @retval rx_data
  */
uint8_t ADS1292R_RwReg(uint8_t cmd, uint8_t data)
{
    uint8_t rx_data = 0;

    ADS1292R_CS_LOW;
    HAL_Delay(1);
    ADS1292R_SPI_TransmitReceive(cmd);
    ADS1292R_SPI_TransmitReceive(0x00);

    if ((cmd & ADS1292R_RREG) == ADS1292R_RREG)
    {
        rx_data = ADS1292R_SPI_TransmitReceive(0x00);
    }
    else
    {
        rx_data = ADS1292R_SPI_TransmitReceive(data);
    }
    HAL_Delay(1);
    ADS1292R_CS_HIGH;
    return rx_data;
}

/**
  * @name   ADS1292R_WriteMultiRegs(uint8_t reg, uint8_t *data,uint8_t len)
  *
  * @brief  Write some numbers of registers from specified register.
  *
  * @note   
  *
  * @param  reg: starting reg address
  * @param  data: the pointer to data
  * @param  len: data's length                
  *
  * @retval None
  */
void ADS1292R_WriteMultiRegs(uint8_t reg, uint8_t *data, uint8_t len)
{
    uint8_t i;

    ADS1292R_CS_LOW;
    HAL_Delay(1);

    ADS1292R_SPI_TransmitReceive(ADS1292R_WREG | reg);
    HAL_Delay(1);
    ADS1292R_SPI_TransmitReceive(len - 1);

    for (i = 0; i < len; i++)
    {
        HAL_Delay(1);
        ADS1292R_SPI_TransmitReceive(*data);
        data++;
    }

    HAL_Delay(1);
    ADS1292R_CS_HIGH;
}

/**
  * @name   ADS1292R_ReadMultiRegs(uint8_t reg, uint8_t *data,uint8_t len)
  *
  * @brief  Read some numbers of registers from specified register.
  *
  * @note   
  *
  * @param  reg: starting reg address
  * @param  data: the pointer to data
  * @param  len: data's length                
  *
  * @retval None
  */
void ADS1292R_ReadMultiRegs(uint8_t reg, uint8_t *data, uint8_t len)
{
    uint8_t i;

    ADS1292R_CS_LOW;
    HAL_Delay(1);

    ADS1292R_SPI_TransmitReceive(ADS1292R_RREG | reg);
    HAL_Delay(1);
    ADS1292R_SPI_TransmitReceive(len - 1);

    for (i = 0; i < len; i++)
    {
        HAL_Delay(1);
        data = ADS1292R_SPI_TransmitReceive(0X00);
        data++;
    }

    HAL_Delay(1);
    ADS1292R_CS_HIGH;
}

/**
  * @name   ADS1292R_GetValue(void)
  *
  * @brief  Read 72bits data 1100+LOFF_STAT[4:0]+GPIO[1:0]+13*0+2CHx24bits, 9 bytes in total.
  *
  * @note   
  *
  * @param  None.            
  *
  * @retval None. The received data has saved in array ADS1292R_data_buf.
  */
void ADS1292R_GetValue(void)
{
    ADS1292R_CS_LOW;
    HAL_SPI_TransmitReceive(&hspi1, ADS1292R_tmp, ADS1292R_data_buf, 9, 0xff);
    ADS1292R_CS_HIGH;
}

/**
  * @name   ADS1292R_Init(void)
  *
  * @brief  Init ADS1292R.
  *
  * @note   
  *
  * @param  None                
  *
  * @retval None
  */
void ADS1292R_Init(void)
{
    uint8_t device_id = 0;
    ADS1292R_SetBuffer();

    ADS1292R_CS_HIGH;
    ADS1292R_START_LOW;
    ADS1292R_RES_HIGH;
    HAL_Delay(10);

    ADS1292R_CS_HIGH;
    ADS1292R_START_LOW;
    ADS1292R_RES_LOW; //enter power-down mode
    HAL_Delay(10);
    ADS1292R_RES_HIGH; //exit power-down mode
    HAL_Delay(10);
    ADS1292R_RES_LOW; //sent reset pulse
    HAL_Delay(1);
    ADS1292R_RES_HIGH;
    HAL_Delay(100);                    //wait to stabilizing，start to use ADS1292R
    ADS1292R_SendCmd(ADS1292R_SDATAC); //stop Read Data Continuously mode
    HAL_Delay(100);
    ADS1292R_SendCmd(ADS1292R_ADSRESET);
    HAL_Delay(1000);
    ADS1292R_SendCmd(ADS1292R_SDATAC);
    HAL_Delay(100);

    HAL_Delay(1);
    while (device_id != DEVICE_ID_ADS1292R)
    {
        device_id = ADS1292R_RwReg(ADS1292R_RREG | ADS1292R_ID, 0x00);
        HAL_Delay(200);
    }
    HMI_printf("t0", "ADS1292R detected");

    ADS1292R_WriteMultiRegs(ADS1292R_CONFIG1, ADS1292R_reg + 1, 11);
}

/**
  * @name   ADS1292R_ADCStartNormal(void)
  *
  * @brief  Setup ADS1292R with default mode.
  *
  * @note   
  *
  * @param  none              
  *
  * @retval None
  *
  * @version  
  */
void ADS1292R_ADCStartNormal(void)
{
    ADS1292R_SendCmd(ADS1292R_SDATAC); //stop Read Data Continuously mode
    HAL_Delay(100);

    ADS1292R_SetBuffer();
    ADS1292R_WriteMultiRegs(ADS1292R_CONFIG1, ADS1292R_reg + 1, 11);
    HAL_Delay(10); //write into dafault reg configuration

    ADS1292R_SendCmd(ADS1292R_RDATAC); //enable Read Data Continuous mode
    HAL_Delay(10);
    ADS1292R_SendCmd(ADS1292R_START); //start conversions
    HAL_Delay(10);
}

/**
  * @name   ADS1292R_ADCStartTest(void)
  *
  * @brief  Setup ADS1292R with testing mode.
  *
  * @note   
  *
  * @param  none                 
  *
  * @retval None 
  */
void ADS1292R_ADCStartTest(void)
{
    ADS1292R_config2.Int_Test = INT_TEST_ON;   //open internal testing signal
    ADS1292R_config2.Test_Freq = TEST_FREQ_AC; //test with 1 Hz square wave

    ADS1292R_ch1set.PD = PD_ON;            //open channel 1,2
    ADS1292R_ch1set.GAIN = GAIN_6;         //set PGA gain is 6
    ADS1292R_ch1set.MUX = MUX_Test_signal; //set testing mode

    ADS1292R_ch2set.PD = PD_ON;
    ADS1292R_ch2set.GAIN = GAIN_6;
    ADS1292R_ch2set.MUX = MUX_Test_signal;

    ADS1292R_resp1.RESP_DemodEN = RESP_DEMOD_OFF; //close demodulation and modulation
    ADS1292R_resp1.RESP_modEN = RESP_MOD_OFF;
    ADS1292R_resp1.RESP_ph = 0X00;                       //the phase is 0 degree
    ADS1292R_resp1.RESP_Ctrl = RESP_CTRL_CLOCK_EXTERNAL; //set internal respiration with external clock

    ADS1292R_SendCmd(ADS1292R_SDATAC); //stop Read Data Continuous mode
    HAL_Delay(100);
    ADS1292R_WriteMultiRegs(ADS1292R_WREG | ADS1292R_CONFIG1, ADS1292R_reg + 1, 11);
    HAL_Delay(10);

    ADS1292R_SendCmd(ADS1292R_RDATAC); //enable Read Data Continuous mode
    HAL_Delay(10);
    ADS1292R_SendCmd(ADS1292R_START); //start conversions
    HAL_Delay(10);
}
