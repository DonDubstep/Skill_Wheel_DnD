#include "description.h"

Description::Description(QWidget *window, QWidget* skill, QString name_text, QString desc_txt) : QWidget(window)
{
    //! Слой для выравнивания по вертикали
    QVBoxLayout* layout = new QVBoxLayout(this);

    //! Название
    name = new QLabel (name_text, this);
    name->setAlignment(Qt::AlignHCenter);
    name->setStyleSheet("font-family: helvetica;"
                              "font-size: 16px;"
                              "color: rgb(236, 206, 0);"
                              "font-weight: bold"
                             );
    layout->addWidget(name);
    //! Описание
    description_text = new QLabel(desc_txt,this);
    description_text->setWordWrap(true);
    description_text->setMaximumWidth(MINIMUM_FRAME_WIDTH);
    description_text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    description_text->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    description_text->setStyleSheet("font-family: helvetica;"
                                    "font-size: 14px;"
                                    "color: white;"
                                    );
    layout->addWidget(description_text);

    //! Запоминаем указатель на иконку навыка
    parent_skill = skill;
    parent_window = window;
}

Description::~Description()
{
    delete this;
}

//! Переопределенный метод для отрисовки фонав
void Description::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, QPixmap("./src/FonOpisania.png").scaled(size()));
    QWidget::paintEvent(e);
}

//! Переопределённый метод для появления рамки в нужном месте
void Description::show()
{
    this->adjustSize();
    int x,y;
    //! Распологаем посередине относительно иконки по Х
    x = parent_skill->x() - this->width() / 2 + 30;
    //! Располагаем у под иконкой
    y = parent_skill->y()+60;
    //! Если выходим за левый край
    if(x < 0)
        x = 0;
    //! Если выходим за правый край
    else if ((x + this->width()) > parent_window->width())
        x = parent_window->width() - this->width();
    //! Если выходим за нижный край
    if((y + this->height()) > parent_window->height())
        y = parent_skill->y() - this->height();
    //! Присваиваем вычисленные координаты
    this->move(x,y);
    QWidget::show();
}
