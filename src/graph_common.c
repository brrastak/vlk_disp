
#include "graph_common.h"
#include "main.h"

// Локальные переменные
color_t oldcolor;       // Временное хранение цвета для восстановления

// Ограничить число x значениями n1+1, n2-1, (n1 < n2)
uint16_t Graph_Crop(uint16_t x, uint16_t n1, uint16_t n2);

// Реализации функций

// Ограничить координаты точки областью
point_t Area_Crop(area_t area, point_t point)
{
    point.x = Graph_Crop(point.x, area.offset.x, area.offset.x + area.width);
    point.y = Graph_Crop(point.y, area.offset.y, area.offset.y + area.height);

    return point;
}

// Сложить координаты точек
point_t Point_Add(point_t one, point_t two)
{
    one.x += two.x;
    one.y += two.y;

    return one;
}

// Вычесть координаты точек
point_t Point_Sub(point_t one, point_t two)
{
    one.x -= two.x;
    one.y -= two.y;

    return one;
}

// Преобразовать координаты точки для построения графика:
// сложить координату X с началом координат и вычесть координату Y
// из начала координат
point_t Point_Transform(point_t origin, point_t point)
{
    point.x += origin.x;
    point.y = origin.y - point.y;

    return point;
}

// Возвращает пустую точку
point_t Point_Empty()
{
    point_t res;

    res.x = 0;
    res.y = POINT_EMPTY_Y;

    return res;
}

// Проверить, не пустая ли точка
bool Point_IsEmpty(point_t point)
{
    return point.y == POINT_EMPTY_Y;
}

// Нарисовать отрезок
void Graph_DrawLine(point_t one, point_t two)
{
    BSP_LCD_DrawLine(one.x, one.y, two.x, two.y);
}

// Запомнить предустановленный цвет
void Graph_SaveColor(void)
{
    oldcolor = BSP_LCD_GetTextColor();
}

// Вернуть установленный ранее цвет
void Graph_RestoreColor(void)
{
    BSP_LCD_SetTextColor(oldcolor);
}

uint16_t Graph_Crop(uint16_t x, uint16_t n1, uint16_t n2)
{
    if (x < n1 + 1)
    {
        x = n1 + 1;
    }
    if (x > n2 - 1)
    {
        x = n2 - 1;
    }

    return x;
}
