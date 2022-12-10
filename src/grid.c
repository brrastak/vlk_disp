
#include "grid.h"


// Реализации публичных методов

// Конструктор
void Grid_Constructor(grid_t* object
    , area_t*   area            // Указатель на область рисования сетки
    , uint16_t  px_per_div_x    // Количество пикселей на деление сетки по горизонтали
    , uint16_t  px_per_div_y    // Количество пикселей на деление сетки по вертикали
    , color_t   color)          // Цвет линий сетки
{
    object->m_area = area;
    object->m_px_per_div_x = px_per_div_x;
    object->m_px_per_div_y = px_per_div_y;
    object->m_color = color;
}

// Нарисовать сетку
void Grid_Draw(grid_t* object)
{
    // Запоминаем предустановленный цвет и устанавливаем цвет сетки
    Graph_SaveColor();
    BSP_LCD_SetTextColor(object->m_color);

    // Координаты, длины граней сетки и шаг сетки
    uint16_t x0 = object->m_area->offset.x;
    uint16_t y0 = object->m_area->offset.y;
    uint16_t x_length = object->m_area->width;
    uint16_t y_length = object->m_area->height;
    uint16_t x_step = object->m_px_per_div_x;
    uint16_t y_step = object->m_px_per_div_y;

    // Рисуем рамку
    BSP_LCD_DrawRect(x0, y0, x_length, y_length);

    // Рисуем вертикальные линии
    for (uint16_t x_pos = x0 + x_step; x_pos < x0 + x_length; x_pos += x_step)
    {
        BSP_LCD_DrawVLine(x_pos, y0, y_length);
    }

    // Рисуем горизонтальные линии
    for (uint16_t y_pos = y0 + y_step; y_pos < y0 + y_length; y_pos += y_step)
    {
        BSP_LCD_DrawHLine(x0, y_pos, x_length);
    }

    //Возвращаем установленный ранее цвет
    Graph_RestoreColor();
}

// Установить количество пикселей на деление сетки по горизонтали
void Grid_SetPxPerDivX(grid_t* object, uint16_t px_per_div)
{
    object->m_px_per_div_x = px_per_div;
}

// Получить значение пикселей на деление сетки по горизонтали
uint16_t Grid_GetPxPerDivX(grid_t* object)
{
    return object->m_px_per_div_x;
}

// Установить количество пикселей на деление сетки по вертикали
void Grid_SetPxPerDivY(grid_t* object, uint16_t px_per_div)
{
    object->m_px_per_div_y = px_per_div;
}

// Получить значение пикселей на деление сетки по вертикали
uint16_t Grid_GetPxPerDivY(grid_t* object)
{
    return object->m_px_per_div_y;
}
