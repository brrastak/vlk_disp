
// hardware substitution

// cstd
#include "stdbool.h"
#include "stdint.h"

#pragma once

#define GPIO_PIN_SET    true
#define GPIO_PIN_RESET  false

typedef uint32_t GPIO_TypeDef;

#define GPIOB           (GPIO_TypeDef*)12345
#define GPIO_PIN_12     12
#define GPIO_PIN_13     13
#define GPIO_PIN_14     14
#define GPIO_PIN_15     15
#define GPIO_PIN_16     16
#define GPIO_PIN_17     17

// STM32
bool HAL_GPIO_ReadPin(GPIO_TypeDef*, uint32_t);
void HAL_GPIO_WritePin(GPIO_TypeDef*, uint32_t, bool);
uint32_t HAL_GetTick();

// Graphical library
#define LCD_COLOR_BLACK         1
#define LCD_COLOR_DARKGRAY      2
#define LCD_COLOR_GREEN         3
#define LCD_COLOR_RED           4
#define LCD_COLOR_YELLOW        5

uint16_t BSP_LCD_GetTextColor();
void BSP_LCD_SetTextColor(uint16_t);

void BSP_LCD_DrawRect(uint16_t x, uint16_t y, uint16_t x_length, uint16_t y_length);
void BSP_LCD_FillRect(uint16_t x, uint16_t y, uint16_t x_length, uint16_t y_length);
void BSP_LCD_DrawVLine(uint16_t x, uint16_t y, uint16_t length);
void BSP_LCD_DrawHLine(uint16_t x, uint16_t y, uint16_t length);
void BSP_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
