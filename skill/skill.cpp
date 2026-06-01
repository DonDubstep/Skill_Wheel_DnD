#include "skill.h"
#include <QDebug>

Skill::Skill(QWidget *parent, QString icon_path, QString name_text, QString desc_txt) : QWidget(parent)
{
    this->installEventFilter(this);
    this->icon_path = icon_path;
    this->description = new Description(parent, this, name_text, desc_txt);
    this->state = NONE;
    this->description->hide();
    this->show();
    this->setObjectName("Skill");
}

bool Skill::operator == (const Skill* skill) const
{
    return this->index == skill->index;
}

//! Отрисовываем иконку скилла
void Skill::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QImage img(this->icon_path);

    if(this->state == UNSELECTED)
    {
        QImage alpha = img.alphaChannel();
        QImage gray = img.convertToFormat(QImage::Format_Grayscale8);
        gray.setAlphaChannel(alpha);
        img = gray;
    }
    QRect rect(0,0,width(), height());
    painter.drawImage(rect,img );
    painter.end();

    QWidget::paintEvent(event);
}

//! Обработчик событий
bool Skill::eventFilter(QObject *object, QEvent *event)
{
    // При наведении на скилл мышкой
    if(event->type() == QEvent::Enter)
    {
        this->setFocus();
        // Увеличиваем если только уже не увеличили
        if(dont_hide_description_flag == 0)
        {
            this->is_changed_size = 1;
            update();
        }
        return true;
    }
    // Убираем мышку со скилла
    else if(event->type() == QEvent::Leave)
    {
        this->clearFocus();
        // Уменьшаем, если не закрепили скилл
        if(dont_hide_description_flag == 0)
        {
            this->is_changed_size = 0;
            update();
        }
        return true;
    }
    // Нажимаем кнопку на клавиатуре
    else if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *key_event = static_cast<QKeyEvent*>(event);
        // Нажимаем букву T
        if(key_event->key() == Qt::Key_T && is_changed_size)
        {
            dont_hide_description_flag = !dont_hide_description_flag;
            this->clearFocus();
        }
        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress)
    {
        emit icon_selected(this);
        return true;
    }
    return QWidget::eventFilter(object, event);
}

