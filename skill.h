#ifndef SKILL_H
#define SKILL_H
#include <QLabel>
#include <QDebug>
#include "description.h"

//! Класс иконки навыка
class Skill : public QLabel
{
public:
    Skill(QWidget *parent=nullptr);
    bool eventFilter(QObject* object, QEvent* event);
    Description* description;
protected:

};

#endif // SKILL_H
