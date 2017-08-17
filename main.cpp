#include "worktime.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Worktime w;
    w.show();

    return a.exec();
}
