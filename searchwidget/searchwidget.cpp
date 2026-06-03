#include "searchwidget.h"


SearchWidget::SearchWidget(QWidget* parent): QWidget(parent)
{
    setup_ui();
}

void SearchWidget::setup_ui()
{
    auto *root = new QHBoxLayout(this);
    root->setContentsMargins(16, 16, 16, 16);
    root->setSpacing(10);

    // Строка поиска
    lineEdit = new QLineEdit;
    lineEdit->setPlaceholderText("Введите ключевое слово...");
    lineEdit->setMinimumHeight(32);

    button_ok = new QPushButton("Найти");
    button_ok->setMinimumHeight(32);
    button_ok->setFixedWidth(90);

    button_X = new QPushButton("X");
    button_X->setMinimumHeight(32);
    button_X->setFixedWidth(32);

    root->addWidget(lineEdit);
    root->addWidget(button_ok);
    root->addWidget(button_X);

    // Соединения
    connect(button_ok, &QPushButton::clicked,
            this,      &SearchWidget::on_search);
    connect(lineEdit,  &QLineEdit::returnPressed,
            this,      &SearchWidget::on_search);

    setMinimumSize(400, 64);
    setWindowTitle("Search Widget");

    setAttribute(Qt::WA_OpaquePaintEvent);
    setAutoFillBackground(false);

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

void SearchWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);


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
}

void SearchWidget::on_search()
{
    qDebug() << "SEARCHING...";
}
