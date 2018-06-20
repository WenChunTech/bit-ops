#include "reginfo.h"
#include <QJsonArray>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSizePolicy>
#include "flowlayout.h"
#include <QLabel>
#include <QList>
#include "bits.h"
#include <QDebug>
#include "shunting-yard.h"
#include "common.h"
#include <QRegExp>


RegInfo::RegInfo(QWidget *parent, QJsonObject obj, Bits * bits): QWidget(parent)
{
    this->reg_data_obj = obj;
    this->bits = bits;

    mainLayout = new QGridLayout;
    label_head = new QLabel();
}

void RegInfo::set_reg_name(QString input_name){
    QJsonObject tmp_obj;
    this->reg_name = input_name;
    this->full_name = reg_data_obj[reg_name].toObject()["full_name"].toString();
    this->width = reg_data_obj[reg_name].toObject()["width"].toInt();
    this->address = reg_data_obj[reg_name].toObject()["address"].toString();
    this->field_groups.clear();

    QJsonArray field_group_array = reg_data_obj[reg_name].toObject()["field_group"].toArray();

    for(int i=0; i< field_group_array.size(); i++){
        tmp_obj = field_group_array.at(i).toObject();
        FieldGroup field_group = FieldGroup(tmp_obj, this->bits);
        this->field_groups.append(field_group);
    }
}

unsigned long long RegInfo::get_reg_value_by_addr(QString reg_addr){
    if (reg_addr == this->address)
        return bits->get_data();
    else
        return 0;
}

/*
 *
 * (((d.l(spr:0x30400))&0x10)==0x10)&&(((d.q(spr:0x30017))&0xF0000)==0x10000)
 *
 *
 * */
QString RegInfo::replace_reg_by_value(QString input_str){
    unsigned long long tmp_value;
    QString result = input_str;
    QString value_string;
    QString pattern_1("spr:(0x[0-9a-fA-F]+)");
    QString pattern_2;
    QStringList reg_addr_list;
    QRegExp rx(pattern_1);
    int pos = 0;
    while ((pos = rx.indexIn(input_str, pos)) != -1){
        reg_addr_list << rx.cap(1);
        pos += rx.matchedLength();
    }
    //qDebug() <<"Addr List:"<< reg_addr_list;
    for (int i=0; i<reg_addr_list.size() ; i++){
        tmp_value = get_reg_value_by_addr(reg_addr_list[i]);
        value_string = QString::number(tmp_value,10);
        pattern_2 = "d\\.(q|l)\\(spr:" + reg_addr_list[i] + "\\)";
        //qDebug() << pattern_2;
        result.replace(QRegExp(pattern_2), value_string);
    }
    return result;
}

bool RegInfo::is_cond_fit(QStringList cond){
    QString cond_tmp;
    TokenMap vars;
    qDebug() << "Condition:" << cond;
    if (cond.size()==0){
        qDebug() << "no condition for this register!";
        return true;
    }
    for(int i=0; i< cond.size(); i++){
        cond_tmp = replace_reg_by_value(cond[i]);
        //qDebug() << "tmp cmd" << cond_tmp;
        cond_tmp = unfold_expression(cond_tmp);
        cond_tmp = convert_hex_in_str(cond_tmp);
        qDebug() << "send cmd" << cond_tmp;
        packToken my_token = calculator::calculate(\
                    cond_tmp.toStdString().c_str(),&vars);
        if (!my_token.asBool()){
            qDebug() << "Result: Condition not fit!";
            return false;
        }
    }
    qDebug() << "Result: Condition fit!";
    return true;
}

FieldGroup RegInfo::get_right_field_group(){
    for(int i=0; i<field_groups.size();i++){
        if (is_cond_fit(field_groups[i].conds)){
            return field_groups[i];
        }
    }
}

void RegInfo::clean(){
    this->label_head->setHidden(true);
    if (!widget_field_list.isEmpty()){
        for (int i=0; i< widget_field_list.size(); i++){
            QList<QWidget *> widgets = widget_field_list[i]->findChildren<QWidget *>();
            foreach (QWidget * widget, widgets) {
                delete widget;
            }
            widget_field_list[i]->deleteLater();
        }
        widget_field_list.clear();
    }
}

FieldGroup::FieldGroup(QJsonObject obj, Bits * bits)
{
    this->bits = bits;
    QJsonArray conds_obj = obj["conditions"].toArray();
    for (int i=0; i < conds_obj.size(); i++){
        this->conds.append(conds_obj.at(i).toString());
    }
    QJsonArray fields_obj = obj["field_info"].toArray();
    for (int i=0; i < fields_obj.size(); i++){
        this->field_infos.append(FieldInfo(fields_obj.at(i).toObject(),this->bits));
    }
}


FieldInfo::FieldInfo(QJsonObject obj, Bits *bits)
{
    this->bits = bits;
    QString display_values = obj["display_value"].toString();
    this->display_value = display_values.split(',');
    this->display_format = obj["display_format"].toString();
    this->end_bit = obj["end_bit"].toString().toInt();
    this->start_bit = obj["start_bit"].toString().toInt();
    this->field_full_name = obj["field_full_name"].toString();
    this->field_name = obj["field_name"].toString();
}

unsigned long long FieldInfo::get_field_data(){
    return bits->get_sub_data(this->start_bit,this->end_bit);
}

QString FieldInfo::get_field_string(){
    return display_value[this->get_field_data()];
}
#define BASE_HEIGHT 0
#define ITEM_HEIGHT 20
int calc_height(int item_num){
    return BASE_HEIGHT+(item_num/2+3)*ITEM_HEIGHT;
}
void RegInfo::update_display(QString reg_name){
    clean();
    set_reg_name(reg_name);
    build();
//  qDebug() << calc_height(widget_field_list.size());
    this->setFixedHeight(calc_height(widget_field_list.size()));
}

void RegInfo::clean_display(){
    this->setFixedHeight(BASE_HEIGHT);
    clean();
}

void RegInfo::build(){
    label_head->setText("<b>Full Name: </b>" + this->full_name + "&nbsp;&nbsp;&nbsp;&nbsp;" + "<b>Address: </b>" + this->address.mid(2));
    mainLayout->addWidget(label_head, 0, 0, 1, 2);
    this->label_head->setHidden(false);
//  qDebug() << "add reg full name Label!";
    FieldGroup right_field_group = get_right_field_group();
    widget_field_list.append(right_field_group.build());
    mainLayout->setSpacing(0);
    for (int i=0; i< widget_field_list.size(); i++){
        mainLayout->addWidget(widget_field_list[i],i/2+1,i%2,1,1);
    }
    this->setLayout(mainLayout);
}

QList<QWidget*> FieldGroup::build(){
    QList<QWidget *> widget_field_list;
    for(int i=0; i<field_infos.size(); i++){
        widget_field_list.append(field_infos[i].build());
    }
    return widget_field_list;
}

QWidget * FieldInfo::build(){
    QWidget * result_widget = new QWidget();
    QHBoxLayout * main_layout = new QHBoxLayout();
    QLabel * label_field_name = new QLabel("<b>" + this->field_name + "</b>");
    QLabel * label_bit_info = new QLabel("["+QString::number(this->start_bit)+":"+\
                                         QString::number(this->end_bit)+"]");
    QLabel * label_value = new QLabel();
    label_field_name->setToolTip("<b>" + this->field_name + "</b>" + ": " +this->field_full_name);
    label_field_name->setFixedWidth(50);
    label_bit_info->setFixedWidth(50);

    label_value->setText(get_field_string());
    main_layout->addWidget(label_field_name);
    main_layout->addWidget(label_bit_info);
    main_layout->addSpacing(100);
    main_layout->addWidget(label_value);
    main_layout->setSpacing(0);
    main_layout->setMargin(2);
    result_widget->setLayout(main_layout);

    return result_widget;
}
