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
#include <QEvent>
#include <QMenuBar>
#include "headerwidget.h"

class SearchWidget : public QWidget
{
public:
    SearchWidget(QWidget* parent, HeaderWidget* header_widget, QMenuBar* menubar);

private:
    void setup_ui();
    void on_search();
    void on_close();
    void update_geometry();
    HeaderWidget* header_widget;
    QMenuBar* menubar;

    QWidget* parent;
    QLineEdit    *search_line;
    QPushButton  *button_find;
    QPushButton  *button_exit;

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // SEARCHWIDGET_H
