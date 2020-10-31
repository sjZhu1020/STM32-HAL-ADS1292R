#ifndef __HMI_UART_H
#define __HMI_UART_H

#include "usart.h"
#include "stdio.h"
#include "main.h"

#define PRINTF_UART &huart3
//UART_HandleTypeDef *printf_Uart = &huart1;

// the width in curve window
#ifndef CURVE_WIDTH
#define CURVE_WIDTH 320
#endif

// ID in curve window
#ifndef CURVE_ID
#define CURVE_ID 2
#endif
//char txt_port_name[] = "t0";

void HMI_printf(char *port_name, char *str);
void HMI_DrawCurve(int channel, int num);
int HMI_DrawCurve_FullScreen(int channel, uint8_t *num);
void HMI_ClearCurve(void);

void HMI_Fill(int x, int y, int width, int height, char *color);
void HMI_DrawLine(int x0, int y0, int x1, int y1, char *color);
void HMI_DrawRectangle(int x0, int y0, int x1, int y1, char *color);
void HMI_DrawCircle(int x, int y, int r, char *color);
void HMI_DrawFilledCircle(int x, int y, int r, char *color);

#endif