// Разные функции и типы для использования в plot

#include <stdbool.h>
#include <stdint.h>


#pragma once

// Значение Y, означающее "пустую" точку, у которой нет значения
#define POINT_EMPTY_Y       INT16_MAX

// Цвет
typedef uint16_t color_t;

// Точка. Координаты в пикселях. Начало координат в левом верхнем углу экрана
typedef struct
{
    int16_t x;
    int16_t y;
}
point_t;
// Область экрана. Координаты в пикселях. Начало координат в левом верхнем углу экрана
typedef struct
{
    point_t     offset; // Положение области на экране
    uint16_t    width;  // Ширина области в пикселях
    uint16_t    height; // Высота области в пикселях
}
area_t; 



// Ограничить координаты точки областью
point_t Area_Crop(area_t area, point_t point);

// Сложить координаты точек
point_t Point_Add(point_t one, point_t two);
// Вычесть координаты точек
point_t Point_Sub(point_t one, point_t two);
// Преобразовать координаты точки для построения графика:
// сложить координату X с началом координат и вычесть координату Y
// из начала координат
point_t Point_Transform(point_t origin, point_t point);
// Возвращает пустую точку
point_t Point_Empty();
// Проверить, не пустая ли точка
bool Point_IsEmpty(point_t point);

// Нарисовать отрезок
void Graph_DrawLine(point_t one, point_t two);

// Запомнить предустановленный цвет
void Graph_SaveColor(void);
// Вернуть установленный ранее цвет
void Graph_RestoreColor(void);
