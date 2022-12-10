
// user
#include "plot_buffer.h"


// Прототипы приватных методов

// Получить указатель на текущий график
plot_t* PlotBuffer_GetCurrentPlot(plot_buffer_t* object);
// Получить указатель на буфер текущего графика
buffer_t PlotBuffer_GetCurrentBuffer(plot_buffer_t* object);
// Текущий график - последний в массиве графиков
bool PlotBuffer_IsLastPlot(plot_buffer_t *object);


// Реализации публичных методов

// Конструктор
void PlotBuffer_Constructor(plot_buffer_t* object
                        , uint16_t  size            // Количество точек в графике
                        , uint16_t  amount          // Количество графиков
                        , plot_t**  plot_arr        // Массив указателей на графики, оттуда получаются значения  
                        , buffer_t* y_buffer_arr    // Массив буферов для значений Y графиков
                        , buffer_t  x_buffer)       // Буфер значений X, общий для всех графиков);
{
    object->m_size = size;
    object->m_amount = amount;
    object->m_plot_arr = plot_arr;
    object->m_y_buffer_arr = y_buffer_arr;
    object->m_x_buffer = x_buffer;
    
    object->m_plot_index = 0;
    object->m_value_index = 0;
}

// Создать и сохранить очередную точку для всех графиков
void PlotBuffer_CreatePoint(plot_buffer_t* object, uint16_t x)
{
    plot_t *plot = PlotBuffer_GetCurrentPlot(object);
    buffer_t buf = PlotBuffer_GetCurrentBuffer(object);

    // Проверка выхода за границу массива
    if (object->m_value_index >= object->m_size - 2)
    {
        return;
    }

    // Проверка доступности графика
    if (Plot_IsAvailable(plot))
    {
        buf[object->m_value_index] = Plot_GetValue(plot);
    }
    else
    {
        // Пустая точка
        buf[object->m_value_index] = POINT_EMPTY_Y;
    }

    object->m_new_x = x;
}

// Вернуться к началу буфера и сохранить первое значение
void PlotBuffer_CreateFirstPoint(plot_buffer_t* object)
{
    PlotBuffer_CreatePoint(object, 0);
}

// Переключиться на следующий график, если ещё есть
bool PlotBuffer_AreMorePlots(plot_buffer_t* object)
{
    if (PlotBuffer_IsLastPlot(object))
    {
        object->m_plot_index = 0;

        object->m_x_buffer[object->m_value_index] = object->m_new_x;
        object->m_value_index++;
        object->m_value_index %= object->m_size;

        return false;
    }
    else
    {
        object->m_plot_index++;
        return true;
    }
}

// Ещё остались сохранённые точки, которые надо стереть
// если нет - сброс индекса в ноль
bool PlotBuffer_AreMorePoints(plot_buffer_t* object)
{
    if (object->m_value_index <= object->m_size - 2)
    {
        return true;
    }
    else
    {
        object->m_value_index = 0;
        return false;
    }
}

// Доступны ли данные графика
bool PlotBuffer_IsAvailable(plot_buffer_t* object)
{
    plot_t* plot = PlotBuffer_GetCurrentPlot(object);

    return Plot_IsAvailable(plot);
}

// Получить точку на графике для рисования линии (вызывать дважды для концов линии)
point_t PlotBuffer_GetDrawPoint(plot_buffer_t *object)
{
    static bool one_two = false;

    // Проверка выхода за границу массива
    if (object->m_value_index < 1)
    {
        return Point_Empty();
    }

    buffer_t buf = PlotBuffer_GetCurrentBuffer(object);
    point_t res;

    uint16_t index;
    index = one_two ? (object->m_value_index) : (object->m_value_index - 1);

    res.x = one_two ? (object->m_new_x) : (object->m_x_buffer[index]);
    res.y = buf[index];

    one_two = !one_two;

    return res;
}

// Получить точку на графике для стирания линии (вызывать дважды для концов линии)
point_t PlotBuffer_GetErasePoint(plot_buffer_t *object)
{
    static bool one_two = false;

    // Проверка выхода за границу массива
    if (object->m_value_index >= object->m_size - 2)
    {
        return Point_Empty();
    }

    uint16_t index;
    index = one_two ? (object->m_value_index) : (object->m_value_index + 1);
    one_two = !one_two;

    buffer_t buf = PlotBuffer_GetCurrentBuffer(object);

    point_t res;

    res.x = object->m_x_buffer[index];
    res.y = buf[index];

    return res;
}

// Получить цвет графика
color_t PlotBuffer_GetColor(plot_buffer_t* object)
{
    plot_t* plot = PlotBuffer_GetCurrentPlot(object);

    return Plot_GetColor(plot);
}

// Стереть сохранённые точки графика
void PlotBuffer_Clear(plot_buffer_t* object)
{
    do
    {
        buffer_t buf = PlotBuffer_GetCurrentBuffer(object);

        for (uint16_t value_index = 0; value_index < object->m_size; ++value_index)
        {
            buf[value_index] = POINT_EMPTY_Y;
        }
    }
    while (PlotBuffer_AreMorePlots(object));

    object->m_value_index = 0;
}


// Реализации приватных методов

plot_t* PlotBuffer_GetCurrentPlot(plot_buffer_t* object)
{
    return object->m_plot_arr[object->m_plot_index];
}

buffer_t PlotBuffer_GetCurrentBuffer(plot_buffer_t* object)
{
    return (object->m_y_buffer_arr)[object->m_plot_index];
}

bool PlotBuffer_IsLastPlot(plot_buffer_t *object)
{
    return (object->m_plot_index >= object->m_amount - 1);
}
