#include "sbqimo.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    sbQiMo w;
    w.show();
    return a.exec();
}
