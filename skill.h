#ifndef SKILL_H
#define SKILL_H
#include "description.h"

//! Класс иконки навыка
class Skill : public QWidget
{
public:
    Skill(QWidget *parent, QString pic_path, QString name_text, QString desc_txt);
    Description* description;
    QString icon_path;
    //! Флаг запрета скрытия рамки описания и уменьшения иконки (при 1 - запрещаем, 0 - разрешаем)
    int dont_hide_description_flag = 0;

private slots:
    void paintEvent(QPaintEvent *event);
public slots:
};

#endif // SKILL_H
