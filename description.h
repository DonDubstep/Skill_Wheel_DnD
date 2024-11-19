#ifndef DESCRIPTION_H
#define DESCRIPTION_H

#include <QLabel>
#include <QObject>
#include <QVBoxLayout>
//! Класс рамки описания
class Description : public QLabel
{
public:
    Description(QWidget *window, QWidget* skill, QString name_text, QString desc_txt);
    ~Description();
    QLabel* description_text;
    QLabel* name;
    QWidget* parent_skill;
    QVBoxLayout* frame_vlayout;

    void show();
};

#endif // DESCRIPTION_H
