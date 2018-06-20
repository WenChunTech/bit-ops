#ifndef BITELEMENT_H
#define BITELEMENT_H

#include <QWidget>
#include "bitbuttons.h"
#include "bitlineedits.h"
#include "bitoperate.h"
#include "bits.h"
#include "bitoptions.h"
#include "regdisplay.h"
class QPushButton;

class BitElement : public QWidget
{
    Q_OBJECT
public:
    explicit BitElement(QWidget *parent = nullptr);
private:
    Bitbuttons * bitbtns;
    BitLineEdits * line_edits;
    BitOperate * bit_operate;
    bitOptions * bit_options;
    Bits * bits = new Bits(64);
    QPushButton * btn_more;
    RegDisplay * reg_display;
signals:

public slots:
    void press_more_btn(bool);
};

#endif // BITELEMENT_H
