
// Класс, который хранит и отдаёт данные графиков

// user
#include "graph_common.h"
#include "plot.h"

// cstd
#include "stdbool.h"
#include "stdint.h"


#pragma once


// Вспомогательные типы

// Массив значений - буфер
typedef uint16_t* buffer_t;


// Поля класса
typedef struct
{
    uint16_t    m_size;                 // Количество точек в графике
    uint16_t    m_amount;               // Количество графиков
    plot_t**    m_plot_arr;             // Массив указателей на графики, оттуда получаются значения        
    buffer_t*   m_y_buffer_arr;         // Массив буферов для значений Y графиков
    buffer_t    m_x_buffer;             // Буфер значений X, общий для всех графиков
    uint16_t    m_plot_index;           // Индекс текущего графика
    uint16_t    m_value_index;          // Индекс текущего значения
    uint16_t    m_new_x;                // Временное хранение значения X для новых точек
}
plot_buffer_t;


// Публичные методы

// Конструктор
void PlotBuffer_Constructor(plot_buffer_t* object
                        , uint16_t  size            // Количество точек в графике
                        , uint16_t  amount          // Количество графиков
                        , plot_t**  plot_arr        // Массив графиков, оттуда получаются значения        
                        , buffer_t* y_buffer_arr    // Массив буферов для значений Y графиков
                        , buffer_t  x_buffer);      // Буфер значений X, общий для всех графиков);

// Переключиться на следующий график, если ещё есть
bool PlotBuffer_AreMorePlots(plot_buffer_t *object);
// Ещё остались сохранённые точки, которые надо стереть
// если нет - сброс индекса в ноль
bool PlotBuffer_AreMorePoints(plot_buffer_t *object);
// Доступны ли данные графика
bool PlotBuffer_IsAvailable(plot_buffer_t *object);

// Создать и сохранить очередную точку для всех графиков
void PlotBuffer_CreatePoint(plot_buffer_t* object, uint16_t x);
// Вернуться к началу буфера и сохранить первое значение
// для текущего графика
void PlotBuffer_CreateFirstPoint(plot_buffer_t* object);

// Получить точку на графике для рисования линии (вызывать дважды для концов линии)
// для текущего графика
point_t PlotBuffer_GetDrawPoint(plot_buffer_t* object);
// Получить точку на графике для стирания линии (вызывать дважды для концов линии)
// для текущего графика
point_t PlotBuffer_GetErasePoint(plot_buffer_t *object);

// Получить цвет графика
// для текущего графика
color_t PlotBuffer_GetColor(plot_buffer_t* object);
// Стереть сохранённые точки графика
void PlotBuffer_Clear(plot_buffer_t* object);
