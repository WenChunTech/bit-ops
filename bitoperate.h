#ifndef BITOPERATE_H
#define BITOPERATE_H

#include <QWidget>
class QPushButton;
class QCheckBox;
class QLineEdit;
class QComboBox;
class QLabel;
#include "bits.h"

class BitOperate : public QWidget
{
    Q_OBJECT
public:
    explicit BitOperate(QWidget *parent, Bits * bits);

public slots:
    void clear_num();
    void reverse_num();
    void set_num();
    void shift_left();
    void shift_right();

private:
    Bits * bits;
    QPushButton * btn_clear;
    QPushButton * btn_reverse;
    QPushButton * btn_set;
    QPushButton * btn_shift_left;
    QLineEdit 	* txt_shift_bit_num;
    QLabel		* label_from;
    QLabel		* label_to;
    QLineEdit 	* bit_range_from;
    QLineEdit 	* bit_range_to;
    QPushButton * btn_shift_right;
    QComboBox 	* shift_mode;
    QLineEdit   * txt_cmd;
int get_range_start();
int get_range_end();

private slots:
    void update_display();
    void send_cmd();
    void set_shift_mode(QString mode);
};

#endif // BITOPERATE_H
