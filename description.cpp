#include "description.h"
#include <QCoreApplication>
#include <QDebug>

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

Description::Description(QWidget *window, QWidget* skill, QString name_text, QString desc_txt) : QLabel(window)
{
    //! Устанавливаем фон описания
    this->setPixmap(QPixmap(QCoreApplication::applicationDirPath() + "/FonOpisania.png"));
    this->setScaledContents(true);
    //! Название
    name = new QLabel (name_text, this);
    //! Описание
    description_text = new QLabel(desc_txt);
    description_text->setWordWrap(true);
    //! Устанавливаем формат текста
    description_text->setStyleSheet("font-family: helvetica;"
                              "font-size: 14px;"
                              "color: white;"
                             );
    name->setStyleSheet("font-family: helvetica;"
                              "font-size: 16px;"
                              "color: rgb(236, 206, 0);"
                              "font-weight: bold"
                             );
    //! Центрируем текст
    description_text->setAlignment(Qt::AlignHCenter);
    name->setAlignment(Qt::AlignHCenter);
    //! Запоминаем указатель на иконку навыка
    parent_skill = skill;
    parent_window = window;
    //! Компоновка по вертикали
    frame_vlayout = new QVBoxLayout(this);
    frame_vlayout->addWidget(name);
    frame_vlayout->addWidget(description_text);
    frame_vlayout->setContentsMargins(MARGIN_LEFT, MARGIN_TOP, MARGIN_LEFT, MARGIN_BOTOM);
    //! Устанавливаем размеры для динамического изменения размера рамки
    this->setMinimumSize(MINIMUM_FRAME_WIDTH, MINIMUM_FRAME_HEIGHT);
    //! Правило по которому будет растягиваться рамка
    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);
    //! Вычисляем высоту текста в рамке
    int frame_h = description_text->sizeHint().height() + name->sizeHint().height() + MARGIN_TOP * 2 + 10; //! 10 - эмпирически вычисленное значение, чтобы не обрезался текст
    this->setMaximumSize(MAXIMUM_FRAME_WIDTH,frame_h);
    frame_vlayout->invalidate();
    //! Растягиваем рамку под текст
    this->adjustSize();
}

Description::~Description()
{
    delete this;
}

//! Переопределённый метод для появления рамки в нужном месте
void Description::show()
{
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

    QLabel::show();
}
