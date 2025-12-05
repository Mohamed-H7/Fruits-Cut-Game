#include "dialog.h"
#include <QApplication>

/*
# ---------------------------
# - Ad-Soyad: Mohamed Hamdo
# - Öğrenci num: 20010011524
# - 1.Ödev
# ---------------------------
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();
    return a.exec();
}
