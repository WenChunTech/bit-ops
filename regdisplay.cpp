#include "regdisplay.h"
#include <QLineEdit>
#include <QLabel>
#include <QWidget>
#include <QGridLayout>
#include <reginfo.h>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QMessageBox>
#include <QScrollArea>
#include <QPalette>

RegDisplay::RegDisplay(QWidget *parent, Bits *bits) : QWidget(parent)
{
    QFile file("reg_data_armv8.json");
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox msgBox;
        msgBox.setText("Can't open register data file!");
        msgBox.exec();
    }
    QByteArray ba = file.readAll();
    QJsonParseError e;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(ba, &e);
    reg_data_obj = jsonDoc.object();
    this->bits = bits;

    main_layout = new QGridLayout;
    txt_reg_name = new QLineEdit("");
    label_reg_name = new QLabel("Reg Name:");
    reg_info = new RegInfo(this,reg_data_obj, bits);
    main_layout->addWidget(label_reg_name,	0,0,1,1);
    main_layout->addWidget(txt_reg_name,	0,1,1,1);
    main_layout->addWidget(reg_info,	1,0,1,2);
    this->setLayout(main_layout);
    connect(txt_reg_name,SIGNAL(textChanged(QString)),this,SLOT(txt_reg_name_changed(QString)));
    connect(bits, SIGNAL(value_changed()),this,SLOT(update_display()));
}

void RegDisplay::update_display(){
    if (is_reg_name(this->cur_reg_name)){
        reg_info->update_display(cur_reg_name);
        this->txt_reg_name->setStyleSheet("");
    }
    else{
        reg_info->clean_display();
        this->txt_reg_name->setStyleSheet("QLineEdit { background-color: #FF8888 }");
    }
}

bool RegDisplay::is_reg_name(QString input_name){
    return	reg_data_obj.contains(input_name);
}

void RegDisplay::txt_reg_name_changed(QString reg_name){
    this->cur_reg_name = reg_name.toUpper();
    this->update_display();
}

