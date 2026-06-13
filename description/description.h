#ifndef DESCRIPTION_H
#define DESCRIPTION_H

#include <QLabel>
#include <QObject>
#include <QVBoxLayout>
#include "settings.h"
#include <QPainter>


//! Класс рамки описания
class Description : public QWidget
{
public:
    Description(QWidget *window, QWidget* skill, QString name_text, QString desc_txt);
    QString name_str;
    QString desciption_str;
    //! Окно на котором рисуется рамка

private:
    //! Название
    QLabel* name;
    //! Текст описания
    QLabel* description_text;
    //! Скилл, к которому привязано описание
    QWidget* parent_skill;
    QWidget* parent_window;
    QVBoxLayout* frame_vlayout;


private slots:
    void paintEvent(QPaintEvent *e);

public slots:
    void show();
};

#endif // DESCRIPTION_H
