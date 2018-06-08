#include <QApplication>
#include <bitelement.h>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("fusion"));
    BitElement bit_element;
    bit_element.show();
    bit_element.setFixedSize(bit_element.width(),bit_element.height());
    return a.exec();
}
