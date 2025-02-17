#ifndef SKILL_H
#define SKILL_H
#include <QLabel>
#include <QDebug>
#include "description.h"

enum icon_sizes
{
    BASE_SIZE = 32,         //! Размер иконки
    INCREACE_KOEF = 2       //! Коэффициент увеличения при наведении
};
//! Класс иконки навыка
class Skill : public QWidget
{
public:
    Skill(QWidget *parent, QString pic_path, QString name_text, QString desc_txt);
    Description* description;
    QString icon_path;
    int show_description = 0;

private slots:
    void paintEvent(QPaintEvent *event);
public slots:
};

#endif // SKILL_H
