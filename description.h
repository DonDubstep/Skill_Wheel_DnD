#ifndef DESCRIPTION_H
#define DESCRIPTION_H

#include <QLabel>
#include <QObject>
#include <QPainter>
#include <QVBoxLayout>

enum sizes
{
    MARGIN_LEFT = 15,
    MARGIN_TOP = 20,
    MARGIN_RIGHT = MARGIN_LEFT,
    MARGIN_BOTOM = MARGIN_TOP,
    MINIMUM_FRAME_WIDTH = 300,
    MINIMUM_FRAME_HEIGHT= 100,
    MAXIMUM_FRAME_WIDTH = MINIMUM_FRAME_WIDTH,
};

//! Класс рамки описания
class Description : public QWidget
{
public:
    Description(QWidget *window, QWidget* skill, QString name_text, QString desc_txt);
    ~Description();
    QLabel* description_text;
    QLabel* name;
    QWidget* parent_skill;
    QWidget* parent_window;
    QVBoxLayout* frame_vlayout;

private slots:
    void paintEvent(QPaintEvent *e);

public slots:
    void show();
};

#endif // DESCRIPTION_H
