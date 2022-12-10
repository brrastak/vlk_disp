
// user
#include "plot.h"


// Реализации публичных методов

// Конструктор
void Plot_Constructor(plot_t* object
    , value_func_t  value_func  // Указатель на функцию, возвращающую значение для графика
    , avail_func_t  avail_func  // Указатель на функцию, возвращающую доступность графика
    , int16_t       origin_y    // Положение нуля по вертикали
    , color_t       color)      // Цвет графика
{
    object->m_value_func = value_func;
    object->m_avail_func = avail_func;
    object->m_origin_y = origin_y;
    object->m_color = color;
}

// Получить значение для графика (с учётом заданного смещения по вертикали)
int16_t Plot_GetValue(plot_t* object)
{
    return object->m_value_func() + object->m_origin_y;
}

// Доступен ли график
bool Plot_IsAvailable(plot_t* object)
{
    return object->m_avail_func();
}

// Получить цвет графика
color_t Plot_GetColor(plot_t* object)
{
    return object->m_color;
}
