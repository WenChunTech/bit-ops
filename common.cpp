#include "common.h"
#include <QString>
#include <QStringList>
#include <vector>
#include <ctype.h>
#include <QDebug>
#include <bits.h>

 /*	e.g. "0x10+100" --> "16+100"
  */
QString convert_hex_in_str(QString input_str){
    QString output_str;
    QStringList output_list;
    int index,index_0x;
    long long tmp_int;
    bool ok;
    index = 0;
    input_str = input_str.toLower();
    index_0x = input_str.indexOf("0x");
    while ( index_0x != -1){
        if (index_0x != 0){
            output_list.append(input_str.mid(index,index_0x-index));
        }
        index = index_0x + 2;
        while (isxdigit(input_str[index].cell())){
            index ++;
        }
        output_list.append(input_str.mid(index_0x,index-index_0x));
        index_0x = input_str.indexOf("0x",index);
    }
    output_list.append(input_str.mid(index));
    qDebug() << output_list ;

    for (int i = 0; i < output_list.size(); ++i){
        if (output_list.at(i).left(2)=="0x"){
            Bits bits(64);
            bits.set_hex_string(output_list.at(i).toStdString());
            output_str += QString::fromStdString(bits.get_dec_signed_string());
        }
        else{
            output_str += output_list.at(i);
        }
    }
    qDebug() << output_str ;
    return output_str;
}
