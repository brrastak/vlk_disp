
// user
#include "main.h"
#include "plot_area.h"


// Прототипы приватных методов

// Очистить область графика и нарисовать сетку
void PlotArea_Clear(plot_area_t* object);
// Получить количество новых пикселей, которые надо добавить к текущей позиции X
uint16_t PlotArea_GetNewPixels(plot_area_t* object);
// Нарисовать новую линию
void PlotArea_DrawNewLine(plot_area_t* object);
// Стереть старую линию
void PlotArea_EraseOldLine(plot_area_t *object);
// Стереть старые линии
void PlotArea_EraseRestLines(plot_area_t* object);
// Нарисовать линию (переведя относительные координаты в абсолюьные)
void PlotArea_DrawLine(plot_area_t* object, point_t one, point_t two);
// Перерисовать сетку
void PlotArea_DrawGrid(plot_area_t* object);


// Реализации публичных методов

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
                        , uint16_t ms_total)        // Количество миллисекунд на весь график
{
    object->m_area.offset.x = x;
    object->m_area.offset.y = y;
    object->m_area.width = width;
    object->m_area.height = height;
    object->m_plots = plots;
    object->m_background_color = background_color;
    object->m_ms_total = ms_total;
    object->m_current_x_pos = 0;
    object->m_ms_per_px = ms_total / width;
    object->m_origin.x = x;
    object->m_origin.y = y + height / 2;

    // Инициализировать объект сетки
    uint16_t px_per_div_x = ((uint32_t)width * (uint32_t)ms_per_div) / ms_total;
    Grid_Constructor(&(object->m_grid), &(object->m_area), px_per_div_x, px_per_div, grid_color);

    PlotArea_Clear(object);
    
    // Устанавливаем текущее время как время прошлой отрисовки
    object->m_prev_time_ms = HAL_GetTick();
    // Сохранение первой точки графика
    do
    {
        PlotBuffer_CreateFirstPoint(object->m_plots);
    }
    while (PlotBuffer_AreMorePlots(plots));
}

// Перерисовать
void PlotArea_Update(plot_area_t* object)
{
    // Вычисляем, сколько пикселей по горизонтали добавилось
    uint16_t pixel_add = PlotArea_GetNewPixels(object);
    // Если ни одного, не перерисовываем
    if (pixel_add == 0)
    {
        return;
    }

    // Стереть "хвост" графика
    bool erase = false;
    // Новое время последней отрисовки (соответствующее целому количеству пикселей)
    object->m_prev_time_ms += pixel_add * object->m_ms_per_px;
    // Новое значение X
    object->m_current_x_pos += pixel_add;
    // Проверка, что мы дошли до правого края
    if (object->m_current_x_pos >= object->m_area.width - 1)
    {
        object->m_current_x_pos = 0;
        erase = true;
    }

    // Рисование
    Graph_SaveColor();

    if (erase)
    {
        // Стирание "хвоста" графика (следующая точка для каждого графика)
        while (PlotBuffer_AreMorePoints(object->m_plots))
        {
            PlotArea_EraseRestLines(object);
        }
        
        do
        {
            // Стирание старой линии
            PlotArea_EraseOldLine(object);
            // Сохранение первой точки графика
            PlotBuffer_CreateFirstPoint(object->m_plots);
        }
        // Для каждого графика
        while (PlotBuffer_AreMorePlots(object->m_plots));

        // Перерисовка сетки
        PlotArea_DrawGrid(object);
    }
    else
    {
        do
        {
            // Стирание старой линии
            PlotArea_EraseOldLine(object);
            // Сохранение новой точки
            PlotBuffer_CreatePoint(object->m_plots, object->m_current_x_pos);
            // Рисование новой линии
            PlotArea_DrawNewLine(object);
        }
        // Для каждого графика
        while (PlotBuffer_AreMorePlots(object->m_plots));
    }

    Graph_RestoreColor();
}


// Реализации приватных методов

void PlotArea_Clear(plot_area_t* object)
{
    Graph_SaveColor();
    
    // Закрасить область построения
    BSP_LCD_SetTextColor(object->m_background_color);
    uint16_t x = object->m_area.offset.x;
    uint16_t y = object->m_area.offset.y;
    uint16_t x_length = object->m_area.width;
    uint16_t y_length = object->m_area.height;
    BSP_LCD_FillRect(x, y, x_length, y_length);
    // Нарисовать сетку
    PlotArea_DrawGrid(object);
    // Стереть данные в буфере
    PlotBuffer_Clear(object->m_plots);

    Graph_RestoreColor();
}

uint16_t PlotArea_GetNewPixels(plot_area_t *object)
{
    return (HAL_GetTick() - object->m_prev_time_ms) / object->m_ms_per_px;
}

void PlotArea_DrawNewLine(plot_area_t* object)
{
    BSP_LCD_SetTextColor(PlotBuffer_GetColor(object->m_plots));
    
    point_t one, two;
    one = PlotBuffer_GetDrawPoint(object->m_plots);
    two = PlotBuffer_GetDrawPoint(object->m_plots);
    PlotArea_DrawLine(object, one, two);
}

// Стереть старую линию
void PlotArea_EraseOldLine(plot_area_t *object)
{
    BSP_LCD_SetTextColor(object->m_background_color);

    point_t one, two;
    one = PlotBuffer_GetErasePoint(object->m_plots);
    two = PlotBuffer_GetErasePoint(object->m_plots);
    PlotArea_DrawLine(object, one, two);
}

void PlotArea_EraseRestLines(plot_area_t* object)
{
    BSP_LCD_SetTextColor(object->m_background_color);
    
    do
    {
        point_t one, two;
        one = PlotBuffer_GetErasePoint(object->m_plots);
        two = PlotBuffer_GetErasePoint(object->m_plots);
        PlotArea_DrawLine(object, one, two);
    }
    while (PlotBuffer_AreMorePlots(object->m_plots));
}

void PlotArea_DrawLine(plot_area_t* object, point_t one, point_t two)
{
    // Если хоть одна из точек пустая, ничего не делаем
    if (Point_IsEmpty(one) || Point_IsEmpty(two))
    {
        return;
    }

    // Вычитаем полученное значение точки из начала координат области построения
    // Чтобы получить абсолютную координату из относительной
    // И строить график "снизу вверх"
    one = Point_Transform(object->m_origin, one);
    two = Point_Transform(object->m_origin, two);
    // Обрезать координаты, если они выходят за область экрана
    one = Area_Crop(object->m_area, one);
    two = Area_Crop(object->m_area, two);

    Graph_DrawLine(one, two);
}

void PlotArea_DrawGrid(plot_area_t* object)
{
    Grid_Draw(&(object->m_grid));
}
