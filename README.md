# STM32-HAL-ADS1292R

This project is using STM32F407ZGT to drive TI ADS1292R through HAL library.

The project is created and initialized by STM32CubeMX and using MDK-ARM to compile. In this project, there are 2private library files to  drive ADS1292R, you can find them in folder `ads1292r`:

>ads1292r.h
>
>ads1292r.c



There are 2 working modes :

>ADS1292R_ADCStartNormal(void): working in continuous conversion mode
>
>ADS1292R_ADCStartTest(void): working in internal testing mode



Please refer to <https://www.ti.com.cn/product/cn/ADS1292R> to configure registers while you want ADS1292R to work at other modes.