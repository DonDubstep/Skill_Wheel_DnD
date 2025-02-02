#include "skill.h"
#include <QEvent>
#include <QKeyEvent>

enum icon_sizes
{
    BASE_SIZE = 32,         //! Размер иконки
    INCREACE_KOEF = 2       //! Коэффициент увеличения при наведении
};

int Skill::show_description = 0;

Skill::Skill(QWidget *parent, QString name_text, QString desc_txt) : QLabel(parent)
{
//    this->move(500,500);
    this->setScaledContents(true);
    this->resize(BASE_SIZE,BASE_SIZE);
    //! Подключаем обработчик событий
    this->installEventFilter(this);
    this->setFocusPolicy(Qt::StrongFocus);
    setFocus();

    description = new Description(parent, this, name_text, desc_txt);
//    description->show();
    description->hide();
}

//! Функция для отлова любых взаимодействий с элементом (наведение мыши, прожатие кнопок)
bool Skill::eventFilter(QObject *object, QEvent *event)
{
    Q_UNUSED(object)
    //! Если наводим курсором
    if(event->type() == QEvent::Enter)
    {
        //! Увеличиваем объект
        this->resize(BASE_SIZE * INCREACE_KOEF, BASE_SIZE * INCREACE_KOEF);
        //! Смещаем объект так, чтобы увеличение происходило без сдвига
        int offset = BASE_SIZE / 2;
        this->move(this->x() - offset, this->y() - offset);
        this->raise();
        description->raise();
        description->show();
        return true;
//        this->setEnabled(true);
    }
    //! Если убираем курсор
    else if(event->type() == QEvent::Leave)
    {
        //! Смещаем объект так, чтобы уменьшение происходило без сдвига
        int offset = BASE_SIZE / 2;
        this->move(this->x() + offset, this->y() + offset);
        //! Уменьшаем объект
        this->resize(BASE_SIZE, BASE_SIZE);
        if (!show_description)
            description->hide();
        return true;
    }
    else if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_T)
        {
            show_description = !show_description;
            return true;
        }
    }

    return QWidget::eventFilter(object, event);
}
