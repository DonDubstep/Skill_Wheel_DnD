#include "skill.h"
#include <QPainter>
#include "settings.h"
#include <QDebug>

Skill::Skill(QWidget *parent, QString icon_path, QString name_text, QString desc_txt) : QWidget(parent)
{
    this->installEventFilter(this);
    this->resize(BASE_SIZE, BASE_SIZE);
    this->icon_path = icon_path;
    this->description = new Description(parent, this, name_text, desc_txt);
    this->description->hide();
    this->show();
    this->setObjectName("Skill");
}

//! Отрисовываем иконку скилла
void Skill::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,QPixmap(this->icon_path).scaled(size()));
    QWidget::paintEvent(event);
}
bool Skill::eventFilter(QObject *object, QEvent *event)
{
//    int dont_hide_description_flag = get_dont_hide_description_flag(static_cast<Skill*>(object));
    // При наведении на скилл мышкой
    if(event->type() == QEvent::Enter)
    {
        static_cast<Skill*>(object)->setFocus();
        // Увеличиваем если только уже не увеличили
        if(dont_hide_description_flag == 0)
        {
            zoom_widget();
        }
        return true;
    }
    // Убираем мышку со скилла
    else if(event->type() == QEvent::Leave)
    {
        static_cast<Skill*>(object)->clearFocus();
        // Уменьшаем, если не закрепили скилл
        if(dont_hide_description_flag == 0)
        {
            zoom_out_widget();
//            delete_from_pinned_skills(object);
        }
        return true;
    }
    // Нажимаем кнопку на клавиатуре
//    else if(event->type() == QEvent::KeyPress)
//    {
//        QKeyEvent *key_event = static_cast<QKeyEvent*>(event);
//        // Нажимаем букву T
//        if(key_event->key() == Qt::Key_T)
//        {
//            // Добавляем скилл в вектор зафиксированных
//            if(get_dont_hide_description_flag(static_cast<Skill*>(object)) == 0)
//                pinned_skills.push_back(object);
//            // Фиксируем флагом
//            switch_dont_hide_description_flag(static_cast<Skill*>(object));
//        }
//        return true;
//    }
    return QWidget::eventFilter(object, event);
}
#include "pagewidget.h"
//! Функция увеличения иконки скилла и отображения рамки описания
void Skill::zoom_widget()
{
    this->is_changed_size = 2;
    int offset = this->width() / 2;
    this->resize(this->width() * INCREACE_KOEF, this->height() * INCREACE_KOEF);
    this->move(this->x() - offset, this->y() - offset);
    this->description->show();
    this->description->raise();
}

//! Функция возвращения иконки скилла в исходное положение и скрытие рамки
void Skill::zoom_out_widget()
{
    this->is_changed_size = 1;
    this->resize(this->width() / 2, this->height() / 2);
    int offset = this->width()  / 2;
    this->move(this->x() + offset, this->y() + offset);
    this->description->hide();
}
