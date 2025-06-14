#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMap>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <skill.h>
#include <QCoreApplication>

struct skill_struct
{
    QString icon_path;
    QString title;
    QString description;
    QString title_color;
    Skill* skill;
};

class PageWidget : public QWidget
{
public:
    PageWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *) override;
    void init_background_colors();
    void forceUpdate();
    void read_json();
    void init_skills();

    int enable_repaint = 1;

private:
    int segment_angle = 30;
    QString** segment_colors;
    QVector<QString> icon_categories
    {
        "Circle3",
        "Circle2",
        "Circle1",
        "CircleBase",
    };
    QMap<QString, QVector<skill_struct>> all_skills_data;
    const QString PIC_PATH = QCoreApplication::applicationDirPath() + "/src/HoMM5_Skills/";
};

#endif // TABWIDGET_H
