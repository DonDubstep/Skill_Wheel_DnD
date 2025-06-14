#ifndef SKILL_H
#define SKILL_H
#include "description.h"
#include <QEvent>

//! Класс иконки навыка
class Skill : public QWidget
{
public:
    Skill(QWidget *parent, QString pic_path, QString name_text, QString desc_txt);
    Description* description;
    QString icon_path;
    //! Флаг запрета скрытия рамки описания и уменьшения иконки (при 1 - запрещаем, 0 - разрешаем)
    int dont_hide_description_flag = 0;
    int is_changed_size = 0;

private slots:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject* object, QEvent* event) override;
    void zoom_widget();
    void zoom_out_widget();
public slots:
};

#endif // SKILL_H
