#include "description.h"
#include <QCoreApplication>

Description::Description(QString text, QWidget *window, QWidget* skill) : QLabel(window)
{
    this->setPixmap(QPixmap(QCoreApplication::applicationDirPath() + "/FonOpisania.png"));
    this->resize(200,200);
    this->setScaledContents(true);
    this->move(200,00);
    text_label = new QLabel(text, this);
    parent_skill = skill;
}

Description::~Description()
{
    delete this;
}

//! Переопределённый метод для появления рамки в нужном месте
void Description::show()
{
    this->move(parent_skill->x(), parent_skill->y()+80);
    QLabel::show();
}
