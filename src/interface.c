
// user
#include "config.h"
#include "hx711.h"
#include "interface.h"
#include "plot.h"
#include "plot_area.h"


// Локальные переменные

// Датчики
hx711_t Sensor1;
hx711_t Sensor2;
hx711_t Sensor3;
// Область построения
plot_area_t Area;
// Буфер графиков
plot_buffer_t Buffer;
// Интерфейсы графиков
plot_t Plot1;
plot_t Plot2;
plot_t Plot3;
// Массив указателей на интерфейсы графиков
plot_t* PlotArr[SENSORS_NUMBER] = {&Plot1, &Plot2, &Plot3};
// Массивы для хранения значений графиков
uint16_t x_buffer[PLOT_X_SIZE];
uint16_t buf1[PLOT_X_SIZE];
uint16_t buf2[PLOT_X_SIZE];
uint16_t buf3[PLOT_X_SIZE];
buffer_t y_buffers[SENSORS_NUMBER] = {buf1, buf2, buf3};

// Прототипы функций для получения данных от датчиков
int16_t GetValue1();
int16_t GetValue2();
int16_t GetValue3();
bool IsAvailable1();
bool IsAvailable2();
bool IsAvailable3();


void InitPlot()
{
    // Инициализация датчиков
    HX711_Constructor(&Sensor1, SENSOR_1_PORT, SENSOR_1_CLK, SENSOR_1_DT, GAIN_64);
    HX711_Constructor(&Sensor2, SENSOR_2_PORT, SENSOR_2_CLK, SENSOR_2_DT, GAIN_64);
    HX711_Constructor(&Sensor3, SENSOR_3_PORT, SENSOR_3_CLK, SENSOR_3_DT, GAIN_64);

    // Иниализация интерфейсов графиков
    Plot_Constructor(&Plot1, GetValue1, IsAvailable1, Y_OFFSET_1, SENSOR_1_PLOT_COLOR);
    Plot_Constructor(&Plot2, GetValue2, IsAvailable2, Y_OFFSET_2, SENSOR_2_PLOT_COLOR);
    Plot_Constructor(&Plot3, GetValue3, IsAvailable3, Y_OFFSET_3, SENSOR_3_PLOT_COLOR);

    // Инициализация буфера графиков
    PlotBuffer_Constructor(&Buffer, PLOT_X_SIZE, SENSORS_NUMBER, PlotArr, y_buffers, x_buffer);

    // Инициализация области построения
    PlotArea_Constructor(&Area, PLOT_X_POS, PLOT_Y_POS, PLOT_X_SIZE, PLOT_Y_SIZE
                        , &Buffer, BACKGROUND_COLOR, GRID_COLOR
                        , PLOT_MS_PER_DIV, PLOT_PX_PER_DIV, PLOT_MS_TOTAL);
}

void UpdatePlot()
{
    HX711_Scan(&Sensor1);
    HX711_Scan(&Sensor2);
    HX711_Scan(&Sensor3);

    PlotArea_Update(&Area);
}

// Реализации функций для получения данных от датчиков
int16_t GetValue1()
{
    return HX711_GetValue(&Sensor1);
}

int16_t GetValue2()
{
    return HX711_GetValue(&Sensor2);
}

int16_t GetValue3()
{
    return HX711_GetValue(&Sensor3);
}

bool IsAvailable1()
{
    return HX711_IsAvailable(&Sensor1);
}

bool IsAvailable2()
{
    return HX711_IsAvailable(&Sensor2);
}

bool IsAvailable3()
{
    return HX711_IsAvailable(&Sensor3);
}
