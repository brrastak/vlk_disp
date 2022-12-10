
// Класс области построения графиков

// user
#include "graph_common.h"
#include "grid.h"
#include "plot.h"
#include "plot_buffer.h"
// cstd
#include <stdint.h>


#pragma once


// Вспомогательные типы

// Поля класса
typedef struct
{
    area_t          m_area;             // Область построения графика
    plot_buffer_t*  m_plots;            // Графики
    color_t         m_background_color; // Цвет фона
    grid_t          m_grid;             // Сетка     
    uint16_t        m_ms_total;         // Количество миллисекунд на весь график
    uint16_t        m_current_x_pos;    // Текущая позиция X для рисования на экране
    uint32_t        m_prev_time_ms;     // Предыдущий момент перерисовки экрана
    uint16_t        m_ms_per_px;        // Количество миллисекунд, приходящееся на один пиксель
    point_t         m_origin;           // Начало координат
}
plot_area_t;

// Публичные методы класса

// Конструктор
void PlotArea_Constructor(plot_area_t* object
                        , uint16_t x                // Координаты области построения
                        , uint16_t y
                        , uint16_t width            // Размеры области построения
                        , uint16_t height
                        , plot_buffer_t* plots      // Буфер хранения данных графиков
                        , color_t background_color  // Цвет фона
                        , color_t grid_color        // Цвет сетки
                        , uint16_t ms_per_div       // Развёртка: количество миллисекунд на деление сетки
                        , uint16_t px_per_div       // Вертикальный шаг сетки в пикселях
                        , uint16_t ms_total);       // Количество миллисекунд на весь график
                        
// Перерисовать
void PlotArea_Update(plot_area_t* object);
