#ifndef REGDISPLAY_H
#define REGDISPLAY_H

#include <QWidget>
#include <bits.h>
#include <QJsonObject>

class QLineEdit;
class QLabel;
class RegInfo;
class QGridLayout;

class RegDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit RegDisplay(QWidget *parent, Bits *bits);


private:
    Bits * bits;
    QLineEdit * txt_reg_name;
    QLabel	* label_reg_name;
    RegInfo * reg_info;
    bool is_reg_name(QString input_name);
    QJsonObject reg_data_obj;
    QGridLayout * main_layout;
    QString cur_reg_name;

signals:

public slots:

private slots:
    void update_display();
    void txt_reg_name_changed(QString reg_name);

};

#endif // REGDISPLAY_H
