#include "skill.h"
#include <QEvent>
#include <QKeyEvent>
#include <QPainter>



Skill::Skill(QWidget *parent, QString icon_path, QString name_text, QString desc_txt) : QWidget(parent)
{
    this->setFixedSize(BASE_SIZE, BASE_SIZE);
    this->icon_path = icon_path;
    this->description = new Description(parent, this, name_text, desc_txt);
    this->description->hide();
    this->show();
    this->setObjectName("Skill");
}

void Skill::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,QPixmap(this->icon_path).scaled(size()));
    QWidget::paintEvent(event);
}

//! Функция для отлова любых взаимодействий с элементом (наведение мыши, прожатие кнопок)
//bool Skill::eventFilter(QObject *object, QEvent *event)
//{
//    Q_UNUSED(object)
//    //! Если наводим курсором
//    if(event->type() == QEvent::Enter)
//    {
//        //! Увеличиваем объект
//        this->resize(BASE_SIZE * INCREACE_KOEF, BASE_SIZE * INCREACE_KOEF);
//        //! Смещаем объект так, чтобы увеличение происходило без сдвига
//        int offset = BASE_SIZE / 2;
//        this->move(this->x() - offset, this->y() - offset);
//        this->raise();
//        description->raise();
//        description->show();
//        return true;
////        this->setEnabled(true);
//    }
//    //! Если убираем курсор
//    else if(event->type() == QEvent::Leave)
//    {
//        //! Смещаем объект так, чтобы уменьшение происходило без сдвига
//        int offset = BASE_SIZE / 2;
//        this->move(this->x() + offset, this->y() + offset);
//        //! Уменьшаем объект
//        this->resize(BASE_SIZE, BASE_SIZE);
//        if (!show_description)
//            description->hide();
//        return true;
//    }
//    else if(event->type() == QEvent::KeyPress)
//    {
//        qDebug() << "bbb";
//        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
//        if (keyEvent->key() == Qt::Key_T)
//        {
//            qDebug() << "aaa";
////            show_description = !show_description;
//            return true;
//        }
//    }

//    return QWidget::eventFilter(object, event);
//}
