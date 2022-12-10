
#pragma once

// Конфигурация портов
#define SENSOR_1_PORT           GPIOB
#define SENSOR_1_CLK            GPIO_PIN_12
#define SENSOR_1_DT             GPIO_PIN_13

#define SENSOR_2_PORT           GPIOB
#define SENSOR_2_CLK            GPIO_PIN_14
#define SENSOR_2_DT             GPIO_PIN_15

#define SENSOR_3_PORT           GPIOB
#define SENSOR_3_CLK            GPIO_PIN_16
#define SENSOR_3_DT             GPIO_PIN_17

// Цвета
#define BACKGROUND_COLOR        LCD_COLOR_BLACK
#define GRID_COLOR              LCD_COLOR_DARKGRAY
#define SENSOR_1_PLOT_COLOR     LCD_COLOR_GREEN
#define SENSOR_2_PLOT_COLOR     LCD_COLOR_RED
#define SENSOR_3_PLOT_COLOR     LCD_COLOR_YELLOW

// Количество датчиков
#define SENSORS_NUMBER          3

// Параметры области построения графика
#define PLOT_X_POS              110
#define PLOT_Y_POS              60
#define PLOT_X_SIZE             320
#define PLOT_Y_SIZE             240
#define PLOT_MS_PER_DIV         250ul
#define PLOT_PX_PER_DIV         20
#define PLOT_MS_TOTAL           (PLOT_MS_PER_DIV * 10)


// Смещение графиков данных сенсоров по вертикали
#define Y_OFFSET_1              70
#define Y_OFFSET_2              0
#define Y_OFFSET_3              -70
