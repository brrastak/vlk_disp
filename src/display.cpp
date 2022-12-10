#include "display.h"
#include "mock_cpp.h"

#include <QPainter>
#include <QIcon>

extern "C"
{
    #include "interface.h"
}


namespace test_setup
{

Display::Display(QWidget *parent)
    : QWidget(parent), img { QPixmap(width, height)}
{
    setGeometry(x_pos, y_pos, width, height);
    setWindowTitle("PLOT TEST");

    QIcon icon;
    icon.addFile("brastak.ico", QSize(256, 256));
    setWindowIcon(icon);

    img.fill(Qt::white);

    label = new QLabel(this);

    label->show();
    label->setGeometry(0, 0, width, height);

    // Start test
    mock::SetDisplay(this);
    InitPlot();

    connect(&thread, &DrawThread::draw,
                this, &Display::draw);
    thread.start();
}

Display::~Display()
{
    thread.terminate();
    thread.wait();
}

void Display::draw()
{
    UpdatePlot();
    update();
}

void Display::paintEvent(QPaintEvent *event)
{
    label->setPixmap(img);
}

void Display::drawLine(uint16_t x1, uint16_t y1, uint32_t x2, uint16_t y2, QColor color)
{
    QPainter painter;
    painter.begin(&img);

    QPen penLine(QBrush(Qt::NoBrush), scale, Qt::SolidLine, Qt::RoundCap);
    penLine.setColor(color);
    painter.setPen(penLine);
    painter.drawLine(x1 * scale, y1 * scale, x2 * scale, y2 * scale);

    painter.end();
}

void Display::drawRectangle(uint16_t x0, uint16_t y0, uint32_t width, uint16_t height, QColor color)
{
    drawLine(x0, y0, x0 + width, y0, color);
    drawLine(x0 + width, y0, x0 + width, y0 + height, color);
    drawLine(x0, y0 + height, x0 + width, y0 + height, color);
    drawLine(x0, y0, x0, y0 + height, color);
}

void Display::drawFilledRectangle(uint16_t x0, uint16_t y0, uint32_t width, uint16_t height, QColor color)
{
    QPainter painter;
    painter.begin(&img);

    painter.setPen(QPen(Qt::NoPen));
    painter.setBrush(color);
    painter.drawRect(x0 * scale, y0 * scale, width * scale, height * scale);

    painter.end();
}

}
