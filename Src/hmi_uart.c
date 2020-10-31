#include "hmi_uart.h"

char endchar[] = "\xff\xff\xff";

/* 将printf()重定向到串口发送函数 */
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#else
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
    HAL_UART_Transmit(PRINTF_UART, (uint8_t *)&ch, 1, 0xFFFF);

    return ch;
}

void HMI_printf(char *port_name, char *str)
{
    printf("%s.txt=\"%s\"", port_name, str);
    printf(endchar);
}

void HMI_DrawCurve(int channel, int num)
{
    printf("add %d,%d,%d", CURVE_ID, channel, num);
    printf(endchar);
}

int HMI_DrawCurve_FullScreen(int channel, uint8_t *num)
{
    uint8_t RX_buffer[4] = {0};

    printf("addt %d,%d,%d", CURVE_ID, channel, CURVE_WIDTH);
    printf(endchar);
    HAL_UART_Receive(PRINTF_UART, RX_buffer, 4, 0xFFFF);
    //HAL_Delay(5);
    while (RX_buffer[0] != 0xFE)
    {
        return HAL_ERROR;
    }
    for (int i = 0; i < CURVE_WIDTH; i++)
    {
        HAL_UART_Transmit(PRINTF_UART, (uint8_t *)&num[i], 1, 0xFFFF);
    }
    HAL_UART_Receive(PRINTF_UART, RX_buffer, 4, 0xFF);
    if (RX_buffer[0] == 0xFD)
    {
        return HAL_OK;
    }
    else
    {
        return HAL_ERROR;
    }
}

void HMI_ClearCurve(void)
{
    printf("cle %d,255", CURVE_ID);
    printf(endchar);
}

/* @param color:
*               RED
*               BLUE
*               GRAY
*               BLACK
*               WHITE
*               GREEN
*               BROWN
*               YELLOW            
*/
void HMI_Fill(int x, int y, int width, int height, char *color)
{
    printf("fill %d,%d,%d,%d,%s", x, y, width, height, color);
    printf(endchar);
}

void HMI_DrawLine(int x0, int y0, int x1, int y1, char *color)
{
    printf("line %d,%d,%d,%d,%s", x0, y0, x1, y1, color);
    printf(endchar);
}

void HMI_DrawRectangle(int x0, int y0, int x1, int y1, char *color)
{
    printf("draw %d,%d,%d,%d,%s", x0, y0, x1, y1, color);
    printf(endchar);
}

void HMI_DrawCircle(int x, int y, int r, char *color)
{
    printf("cir %d,%d,%d,%s", x, y, r, color);
    printf(endchar);
}

void HMI_DrawFilledCircle(int x, int y, int r, char *color)
{
    printf("cirs %d,%d,%d,%s", x, y, r, color);
    printf(endchar);
}