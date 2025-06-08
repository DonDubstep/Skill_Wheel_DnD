#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QWidget>
#include <QPainter>

class PageWidget : public QWidget
{
public:
    PageWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *) override;

private:
    int segment_angle = 30;
    QString** segment_colors;
};

#endif // TABWIDGET_H
