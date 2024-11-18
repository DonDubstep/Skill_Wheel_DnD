#ifndef DESCRIPTION_H
#define DESCRIPTION_H

#include <QLabel>
#include <QObject>
//! Класс рамки описания
class Description : public QLabel
{
public:
    Description(QString text, QWidget *window, QWidget* skill);
    ~Description();
    QLabel* text_label;
    QWidget* parent_skill;

    void show();
};

#endif // DESCRIPTION_H
