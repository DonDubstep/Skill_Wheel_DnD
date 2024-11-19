#include "skill.h"
#include <QEvent>

enum icon_sizes
{
    BASE_SIZE = 30,         //! Размер иконки
    INCREACE_KOEF = 2       //! Коэффициент увеличения при наведении
};

Skill::Skill(QWidget *parent, QString name_text, QString desc_txt) : QLabel(parent)
{
    this->move(500,100);
    this->setScaledContents(true);
    this->resize(BASE_SIZE,BASE_SIZE);
    //! Подключаем обработчик событий
    this->installEventFilter(this);
    description = new Description(parent, this, name_text, desc_txt);
    description->hide();
}

//! Функция для отлова любых взаимодействий с элементом (наведение мыши, прожатие кнопок)
bool Skill::eventFilter(QObject *object, QEvent *event)
{
    Q_UNUSED(object)
    // Если наводим курсором
    if(event->type() == QEvent::Enter)
    {
        //! Увеличиваем объект
        this->resize(BASE_SIZE * INCREACE_KOEF, BASE_SIZE * INCREACE_KOEF);
        //! Смещаем объект так, чтобы увеличение происходило без сдвига
        int offset = BASE_SIZE / 2;
        this->move(this->x() - offset, this->y() - offset);
        description->show();

    }
    //! Если убираем курсор
    else if(event->type() == QEvent::Leave)
    {
        //! Смещаем объект так, чтобы уменьшение происходило без сдвига
        int offset = BASE_SIZE / 2;
        this->move(this->x() + offset, this->y() + offset);
        //! Уменьшаем объект
        this->resize(BASE_SIZE, BASE_SIZE);
        description->hide();
    }
    return false;
}
