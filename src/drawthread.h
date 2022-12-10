
#include <QThread>

#pragma once


namespace test_setup
{

class DrawThread : public QThread
{
    Q_OBJECT

public:
    DrawThread();

signals:
    void draw();

protected:
    void run() override;

private:
    static constexpr int delay_ms = 100;
};

}
