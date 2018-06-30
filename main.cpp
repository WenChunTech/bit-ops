#include <QApplication>
#include <bitelement.h>
#include <QStyleFactory>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("fusion"));
    //QApplication::setStyle(QStyleFactory::create("WindowsVista"));
    BitElement bit_element;
    bit_element.show();
    bit_element.setFixedSize(bit_element.width(),bit_element.height());
    return a.exec();
}
