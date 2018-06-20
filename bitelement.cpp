#include "bitelement.h"
#include <QGridLayout>
#define MORE_TXT "More..."
#define LESS_TXT "Less..."

BitElement::BitElement(QWidget *parent) : QWidget(parent)
{
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSpacing(0);
    bitbtns = new Bitbuttons(this, bits);
    line_edits = new BitLineEdits(this, bits);
    bit_operate = new BitOperate(this,bits);
    bit_options = new bitOptions(this, bits);
    btn_more =  new QPushButton(MORE_TXT);
    reg_display = new RegDisplay(this,bits);
    // row col rowSpan coSpan
    mainLayout->addWidget(bitbtns,0,0,1,2);
    mainLayout->addWidget(line_edits,0,2,2,1);
    mainLayout->addWidget(bit_operate,1,0,1,1);
    mainLayout->addWidget(btn_more,1,1,1,1);
    mainLayout->addWidget(bit_options,0,3,2,1);
    mainLayout->addWidget(reg_display,2,0,1,3);
    this->reg_display->setHidden(true);

    connect(btn_more, SIGNAL(clicked(bool)),this,SLOT(press_more_btn(bool)));
    setLayout(mainLayout);
}

void BitElement::press_more_btn(bool){
    if (btn_more->text()==MORE_TXT){
        this->reg_display->setHidden(false);
        btn_more->setText(LESS_TXT);
    }
    else if (btn_more->text()==LESS_TXT){
        this->reg_display->setHidden(true);
        btn_more->setText(MORE_TXT);
    }
}
