#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QPolygon>

class SearchWidget : public QWidget
{
public:
    SearchWidget(QWidget* parent);

private:
    void setup_ui();
    void on_search();
    void paintEvent(QPaintEvent *) override;

    QLineEdit    *lineEdit;
    QPushButton  *button_ok;
    QPushButton  *button_X;
};

#endif // SEARCHWIDGET_H
