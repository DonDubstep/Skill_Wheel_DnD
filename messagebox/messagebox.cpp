#include "messagebox.h"

void show_text_message(QWidget* parent, int number_of_strings, ...)
{
    QMessageBox msgBox(parent);
    va_list iter_factor;
    va_start(iter_factor, number_of_strings);
    QString message_text;
    for(int s = 0; s < number_of_strings; s++)
    {
        message_text += QString(va_arg(iter_factor, char*));
    }
    va_end(iter_factor);
    msgBox.setText(message_text);
    msgBox.exec();
}
