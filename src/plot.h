
// Класс графика

// user
#include "graph_common.h"
// cstd
#include "stdbool.h"
#include "stdint.h" 

#pragma once


// Вспомогательные типы

// Указатель на функцию, возвращающую одно значение для графика
typedef int16_t(*value_func_t)(void);
// Указатель на функцию, которая сообщает, доступны ли данные из этого источника
typedef bool(*avail_func_t)(void);


// Поля класса
typedef struct
{
    value_func_t    m_value_func;   // Указатель на функцию, возвращающую значение для графика
    avail_func_t    m_avail_func;   // Указатель на функцию, возвращающую доступность графика
    int16_t         m_origin_y;     // Положение нуля по вертикали
    color_t         m_color;        // Цвет графика
}
plot_t;


// Публичные методы

// Конструктор
void Plot_Constructor(plot_t*       object
                    , value_func_t  value_func  // Указатель на функцию, возвращающую значение для графика
                    , avail_func_t  avail_func  // Указатель на функцию, возвращающую доступность графика
                    , int16_t       origin_y    // Положение нуля по вертикали
                    , color_t       color);     // Цвет графика
// Получить значение для графика (с учётом заданного смещения по вертикали)
int16_t Plot_GetValue(plot_t* object);
// Доступен ли график
bool Plot_IsAvailable(plot_t* object);
// Получить цвет графика
color_t Plot_GetColor(plot_t* object);
