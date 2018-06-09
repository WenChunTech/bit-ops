#include "bitoperate.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QWidget>
#include <QRegExpValidator>
#include <QComboBox>
#include <QLabel>
#include <algorithm>
#include <QMessageBox>

BitOperate::BitOperate(QWidget *parent, Bits *bits) : QWidget(parent)
{
    this->bits = bits;
    QHBoxLayout *mainLayout = new QHBoxLayout;
    QGridLayout *shiftLayout = new QGridLayout;
    QWidget * shiftWidget = new QWidget;
    label_from = new QLabel("From");
    label_to = new QLabel("To");
    bit_range_from = new QLineEdit("0",this);
    bit_range_to = new QLineEdit("63",this);
    btn_clear = new QPushButton("clear", this);
    btn_set = new QPushButton("set", this);
    btn_reverse = new QPushButton("reverse", this);
    btn_shift_left = new QPushButton("<<shift",this);
    txt_shift_bit_num = new QLineEdit("0",this);
    btn_shift_right = new QPushButton("shift>>",this);
    shift_mode = new QComboBox(this);

    shift_mode->setToolTip("Shift Mode");
    shift_mode->addItem("Logic");
    shift_mode->addItem("Arith");
    shift_mode->addItem("Rotate");
    shift_mode->setEditable(false);

    QRegExp rx_dec("[0-9]+");
    QValidator *dec_validator = new QRegExpValidator(rx_dec, this);
    txt_shift_bit_num->setValidator(dec_validator);
    txt_shift_bit_num->setMaxLength(2);
    txt_shift_bit_num->setAlignment(Qt::AlignCenter);
    bit_range_from->setValidator(dec_validator);
    bit_range_from->setMaxLength(2);
    bit_range_from->setAlignment(Qt::AlignCenter);

    label_from->setFixedWidth(40);
    label_to->setFixedWidth(20);
    bit_range_from->setFixedWidth(40);
    bit_range_to->setFixedWidth(40);

    bit_range_to->setValidator(dec_validator);
    bit_range_to->setMaxLength(2);
    bit_range_to->setAlignment(Qt::AlignCenter);

    shiftLayout->addWidget(btn_shift_left,		0,0,1,1);
    shiftLayout->addWidget(txt_shift_bit_num,	0,1,2,1);
    shiftLayout->addWidget(btn_shift_right,		0,2,1,1);
    shiftLayout->setSpacing(0);
    shiftLayout->setMargin(1);
    shiftWidget->setLayout(shiftLayout);
    shiftWidget->setFixedWidth(200);

    mainLayout->addWidget(label_from);
    mainLayout->addWidget(bit_range_from);
    mainLayout->addWidget(label_to);
    mainLayout->addWidget(bit_range_to);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(btn_clear);
    mainLayout->addWidget(btn_set);
    mainLayout->addWidget(btn_reverse);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(shiftWidget);
    mainLayout->addWidget(shift_mode);
    this->setLayout(mainLayout);
    txt_shift_bit_num->setText("1");
//    btn_clear->setFocusPolicy(Qt::NoFocus);
//    btn_reverse->setFocusPolicy(Qt::NoFocus);
//    btn_set->setFocusPolicy(Qt::NoFocus);
//    btn_shift_left->setFocusPolicy(Qt::NoFocus);
//    btn_shift_right->setFocusPolicy(Qt::NoFocus);
//    shift_mode->setFocusPolicy(Qt::NoFocus);
    connect(shift_mode,SIGNAL(currentTextChanged(QString)),this,SLOT(set_shift_mode(QString)));
    connect(btn_clear,SIGNAL(clicked(bool)),this,SLOT(clear_num()));
    connect(btn_reverse,SIGNAL(clicked(bool)),this,SLOT(reverse_num()));
    connect(btn_set,SIGNAL(clicked(bool)),this,SLOT(set_num()));
    connect(btn_shift_left,SIGNAL(clicked(bool)),this,SLOT(shift_left()));
    connect(btn_shift_right,SIGNAL(clicked(bool)),this,SLOT(shift_right()));
    connect(bits,SIGNAL(value_changed()),this,SLOT(update_display()));
}

int BitOperate::get_range_start(){
    int temp1, temp2;
    int start,max_end;
    temp1 = this->bit_range_from->text().toInt();
    temp2 = this->bit_range_to->text().toInt();
    max_end = (int)bits->get_width()-1;
    if (temp1>max_end && temp2 > max_end)
        start = -1;
    else
        start = std::min(temp1,temp2);
    return start;
}

int BitOperate::get_range_end(){
    int temp1, temp2;
    int end,max_end;
    temp1 = this->bit_range_from->text().toInt();
    temp2 = this->bit_range_to->text().toInt();
    max_end = (int)bits->get_width()-1;
    if (temp1>max_end && temp2 > max_end)
        end = -1;
    else
        end = std::max(temp1,temp2);
    return std::min(end,(int)bits->get_width()-1);

}

void BitOperate::clear_num(){
    if ((get_range_end()==-1)||(get_range_start()==-1)){
        QMessageBox msgBox;
        msgBox.setText("Please check the range!");
        msgBox.exec();
        return;
    }else{
        bits->clear_bits(get_range_start(),get_range_end());
    }
}
void BitOperate::set_num(){
    if ((get_range_end()==-1)||(get_range_start()==-1)){
        QMessageBox msgBox;
        msgBox.setText("Please check the range!");
        msgBox.exec();
        return;
    }else{
        bits->set_bits(get_range_start(),get_range_end());
    }
}
void BitOperate::reverse_num(){
    if ((get_range_end()==-1)||(get_range_start()==-1)){
        QMessageBox msgBox;
        msgBox.setText("Please check the range!");
        msgBox.exec();
        return;
    }else{
        bits->reverse_bits(get_range_start(),get_range_end());
    }
}

void BitOperate::shift_left(){
    int shift_bits = this->txt_shift_bit_num->text().toInt();
    bits->shift_left(shift_bits);
}

void BitOperate::shift_right(){
    int shift_bits = this->txt_shift_bit_num->text().toInt();
    bits->shift_right(shift_bits);
}

void BitOperate::update_display(){
    switch (bits->get_shift_mode()){
        case Bits::SHF_LOGIC:
            shift_mode->setCurrentText("Logic");
            break;
        case Bits::SHF_ARITH:
            shift_mode->setCurrentText("Arith");
            break;
        case Bits::SHF_ROTATE:
            shift_mode->setCurrentText("Rotate");
            break;
    }
}

void BitOperate::set_shift_mode(QString mode){
    if (mode=="Logic")
        bits->set_shift_mode(Bits::SHF_LOGIC);
    else if (mode=="Arith")
        bits->set_shift_mode(Bits::SHF_ARITH);
    else if (mode=="Rotate")
        bits->set_shift_mode(Bits::SHF_ROTATE);
}
