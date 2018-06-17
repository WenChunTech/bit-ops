#ifndef COMMON_H
#define COMMON_H

class QString;

/* convert hex num in str to dec format
 *	e.g. "0x10+100" --> "16+100"
*/
QString convert_hex_in_str(QString input_str);

#endif // COMMON_H
