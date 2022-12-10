#include "drawthread.h"


namespace test_setup
{

DrawThread::DrawThread()
{
}

void DrawThread::run()
{
    while (true)
    {
        msleep(delay_ms);
        emit draw();
    }
}

}
