
// user
#include "drawthread.h"

// Qt
#include <QWidget>
#include <QPixmap>
#include <QLabel>

// std
#include <cstdint>

#pragma once

namespace test_setup
{
    class Display : public QWidget
    {
        Q_OBJECT

    public:
        Display(QWidget *parent = nullptr);
        ~Display();

        void paintEvent(QPaintEvent *event);

        void drawLine(uint16_t x1, uint16_t y1, uint32_t x2, uint16_t y2, QColor color);
        void drawRectangle(uint16_t x0, uint16_t y0, uint32_t width, uint16_t height, QColor color);
        void drawFilledRectangle(uint16_t x0, uint16_t y0, uint32_t width, uint16_t height, QColor color);

    private slots:
        void draw();

    private:
        static constexpr int scale = 2;

        static constexpr int x_pos = 300;
        static constexpr int y_pos = 200;
        static constexpr int width = 480 * scale;
        static constexpr int height = 320 * scale;

        DrawThread thread;
        QPixmap img;
        QLabel* label;
    };
}
