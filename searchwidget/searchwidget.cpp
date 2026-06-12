#include "searchwidget.h"


SearchWidget::SearchWidget(QWidget* parent, HeaderWidget* header_widget, QMenuBar* menubar): QWidget(parent)
{
    this->parent = parent;
    this->header_widget = header_widget;
    this->menubar = menubar;
    setup_ui();
//    if(parent)
//        installEventFilter(this);
}

#define SEARCH_HEIGHT_K 0.0535
#define SEARCH_WIDTH_K 0.3
#define SEARCH_ELEMENT_HEIGHT_K 0.0267
#define SEARCH_H_PADDING_K 0.0056

void SearchWidget::setup_ui()
{
    auto *root = new QHBoxLayout(this);
    root->setContentsMargins(8, 8, 8, 8);
    root->setSpacing(10);

    // Строка поиска
    search_line = new QLineEdit;
    search_line->setPlaceholderText("Найти...");
    search_line->setMinimumHeight(25);

    button_find = new QPushButton("Н");
    button_find->setMinimumHeight(25);
    button_find->setFixedWidth(25);

    button_exit = new QPushButton("X");
    button_exit->setMinimumHeight(25);
    button_exit->setFixedWidth(25);

    root->addWidget(search_line);
    root->addWidget(button_find);
    root->addWidget(button_exit);

    // Соединения
    connect(button_find, &QPushButton::clicked,
            this,      &SearchWidget::on_search);
    connect(search_line,  &QLineEdit::returnPressed,
            this,      &SearchWidget::on_search);
    connect(button_exit,  &QPushButton::clicked,
            this,      &SearchWidget::on_close);

    setMinimumSize(254, 50);
    setWindowTitle("Search Widget");

    setAttribute(Qt::WA_OpaquePaintEvent);
    setAutoFillBackground(false);
}

void SearchWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    update_geometry();
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, false);
    p.fillRect(rect(), QColor("#2b1f0e"));

    // Внешняя тёмная рамка
    p.setPen(QPen(QColor("#5c3d0a"), 1));
    p.drawRect(rect().adjusted(0, 0, -1, -1));

    // Внутренняя золотая рамка
    p.setPen(QPen(QColor("#8a5c1a"), 1));
    p.drawRect(rect().adjusted(4, 4, -5, -5));

    // Угловые треугольные акценты
    p.setPen(Qt::NoPen);
    p.setBrush(QColor("#c9941a"));

    const int sz = 10;
    auto corner = [&](int x, int y, int sx, int sy) {
        QPolygon poly;
        poly << QPoint(x, y)
             << QPoint(x + sx * sz, y)
             << QPoint(x, y + sy * sz);
        p.drawPolygon(poly);
    };

    corner(0,          0,           1,  1);
    corner(width() - 1, 0,          -1,  1);
    corner(0,          height() - 1, 1, -1);
    corner(width() - 1, height() - 1, -1, -1);
    // Стиль
    setStyleSheet(R"(
        SearchWidget {
            background-color: #2b1f0e;
        }

        QLineEdit {
            background-color: #1c1408;
            border: 1px solid #6b4510;
            color: #f0d890;
            font-family: "Palatino Linotype", "Book Antiqua", serif;
            font-size: 12pt;
            padding: 4px 12px;
            selection-background-color: #8a5c1a;
            selection-color: #fff8dc;
        }
        QLineEdit:focus {
            border: 1px solid #c9941a;
        }

        QPushButton {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #8a5c18, stop:1 #4a2e08);
            border: 1px solid #c9941a;
            color: #f5d97a;
            font-family: "Palatino Linotype", "Book Antiqua", serif;
            font-size: 10pt;
            font-weight: bold;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #b07820, stop:1 #6a3e10);
            color: #fff8dc;
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #4a2e08, stop:1 #8a5c18);
        }
    )");
}

void SearchWidget::update_geometry()
{
    int margin_central_widget = 9;
    qDebug() << parent->width() << "X" << parent->height();
    int w = qMax(254, static_cast<int>(parent->width() * SEARCH_WIDTH_K));
    int h = static_cast<int>(parent->height() * SEARCH_HEIGHT_K);
    int x = qMax(0,static_cast<int>(parent->width() - w) - margin_central_widget);
    int y = static_cast<int>(header_widget->height() + menubar->height() + SEARCH_H_PADDING_K * parent->height());
//    int y = 0;
    setGeometry(x,y,w,h);
}

void SearchWidget::on_search()
{
    qDebug() << "SEARCHING...";
}

void SearchWidget::on_close()
{
    this->search_line->clear();
    this->hide();
}

