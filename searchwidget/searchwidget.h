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
    Q_OBJECT
public:
    SearchWidget(QWidget* parent, HeaderWidget* header_widget, QMenuBar* menubar);

public slots:
    void show_up();
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
    bool eventFilter(QObject *watched, QEvent *event) override;

};

#endif // SEARCHWIDGET_H
