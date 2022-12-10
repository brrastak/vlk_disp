
// user
extern "C"
{
    #include "mock.h"
}
extern "C"
{
    #include "config.h"
}
#include "mock_cpp.h"

#include <QPainter>
#include <QPixmap>


// local
namespace
{

test_setup::Display* display;
uint16_t curColor = LCD_COLOR_BLACK;
uint32_t curTick = 0;
bool returnNumber = false;

QColor GetColor(int color);
uint32_t GetTick();

int32_t dotted(uint32_t time)
{
    return (time % 500 > 250) ? 1 : 0;
}

}


void mock::SetDisplay(test_setup::Display* Display)
{
    display = Display;
}


// STM32
extern "C" bool HAL_GPIO_ReadPin(GPIO_TypeDef* port, uint32_t pin)
{
    if (returnNumber == false)
    {
        return false;
    }

    static mock::Wave signal1(30.0, 0.3, 0.0);
    static mock::Wave signal2(30.0, 1.0, 0.0);
    static mock::Wave signal3(30.0, 7.0, 0.0);

    static mock::BitStream bits1;
    static mock::BitStream bits2;
    static mock::BitStream bits3;

    // if (pin == SENSOR_1_DT)
    // {
    //     return bits1.getBit(dotted(GetTick()));
    //     return bits1.getBit(1);
    // }
    // if (pin == SENSOR_2_DT)
    // {
    //     return bits2.getBit(-1);
    // }
    // if (pin == SENSOR_3_DT)
    // {
    //     return bits3.getBit(-3);
    // }
    if (pin == SENSOR_1_DT)
    {
        return bits1.getBit(signal1.getValue(GetTick()));
    }
    if (pin == SENSOR_2_DT)
    {
        return bits2.getBit(signal2.getValue(GetTick()));
    }
    if (pin == SENSOR_3_DT)
    {
        return bits3.getBit(signal3.getValue(GetTick()));
    }

    return false;
}

extern "C" void HAL_GPIO_WritePin(GPIO_TypeDef* port, uint32_t pin, bool set)
{
    static int pulseCounter = 0;

    if (set == false)
    {
        return;
    }

    returnNumber = (pulseCounter >= 0)
                    && (pulseCounter < 24);

    pulseCounter++;
    pulseCounter %= 27;
}

extern "C" uint32_t HAL_GetTick()
{
    curTick += 20 + mock::rand_value(20);

    return curTick;
}

extern "C" uint16_t BSP_LCD_GetTextColor()
{
    return curColor;
}

extern "C" void BSP_LCD_SetTextColor(uint16_t color)
{
    curColor = color;
}

extern "C" void BSP_LCD_DrawRect(uint16_t x, uint16_t y, uint16_t x_length, uint16_t y_length)
{
    display->drawRectangle(x, y, x_length, y_length, GetColor(curColor));
}

extern "C" void BSP_LCD_FillRect(uint16_t x, uint16_t y, uint16_t x_length, uint16_t y_length)
{
    display->drawFilledRectangle(x, y, x_length, y_length, GetColor(curColor));
}

extern "C" void BSP_LCD_DrawVLine(uint16_t x, uint16_t y, uint16_t length)
{
    display->drawLine(x, y, x, y + length, GetColor(curColor));
}

extern "C" void BSP_LCD_DrawHLine(uint16_t x, uint16_t y, uint16_t length)
{
    display->drawLine(x, y, x + length, y, GetColor(curColor));
}

extern "C" void BSP_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    display->drawLine(x1, y1, x2, y2, GetColor(curColor));
}

namespace
{

QColor GetColor(int color)
{
    switch (color)
    {
    case LCD_COLOR_BLACK:
        return Qt::black;
    case LCD_COLOR_DARKGRAY:
        return Qt::darkGray;
    case LCD_COLOR_GREEN:
        return Qt::green;
    case LCD_COLOR_RED:
        return Qt::red;
    case LCD_COLOR_YELLOW:
        return Qt::yellow;
    default:
        return Qt::black;
    }
}

uint32_t GetTick()
{
    return curTick;
}

}

int mock::rand_value(int max)
{
    static bool isInited = false;

    if (!isInited)
    {
        std::srand(std::time(nullptr));
        isInited = true;
    }

    if (max == 0)
    {
        return 0;
    }

    return std::rand() / (RAND_MAX / max);
}
