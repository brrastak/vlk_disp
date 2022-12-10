
// Класс сетки на области построения графика

// user
#include "graph_common.h"
#include "main.h"

#pragma once


// Поля класса
typedef struct
{
    area_t*     m_area;             // Указатель на область рисования сетки
    uint16_t    m_px_per_div_x;     // Количество пикселей на деление сетки по горизонтали
    uint16_t    m_px_per_div_y;     // Количество пикселей на деление сетки по вертикали
    color_t     m_color;            // Цвет линий сетки
}
grid_t;

// Публичные методы класса

// Конструктор
void Grid_Constructor(grid_t*   object
                    , area_t*   area            // Указатель на область рисования сетки
                    , uint16_t  px_per_div_x    // Количество пикселей на деление сетки по горизонтали
                    , uint16_t  px_per_div_y    // Количество пикселей на деление сетки по вертикали
                    , color_t   color);         // Цвет линий сетки
// Нарисовать сетку
void Grid_Draw(grid_t* object);
// Установить количество пикселей на деление сетки по горизонтали
void Grid_SetPxPerDivX(grid_t* object, uint16_t px_per_div);
// Получить значение пикселей на деление сетки по горизонтали
uint16_t Grid_GetPxPerDivX(grid_t* object);
// Установить количество пикселей на деление сетки по вертикали
void Grid_SetPxPerDivY(grid_t* object, uint16_t px_per_div);
// Получить значение пикселей на деление сетки по вертикали
uint16_t Grid_GetPxPerDivY(grid_t* object);
