#include "skill.h"
#include <QPainter>
#include "settings.h"

Skill::Skill(QWidget *parent, QString icon_path, QString name_text, QString desc_txt) : QWidget(parent)
{
    this->setFixedSize(BASE_SIZE, BASE_SIZE);
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
