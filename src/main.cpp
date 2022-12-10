#include "display.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    test_setup::Display display;
    display.show();

    return app.exec();
}
