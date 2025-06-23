#ifndef PAGEWIDGET_H
#define PAGEWIDGET_H

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
    Q_OBJECT
public:
    PageWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void init_background_colors();
    void read_json();
    void init_skills();
    void paint_concentric_circles();
    void paint_small_circles();
    void paint_skills();
//    void paint_circ

    int enable_repaint = 1;

private:
    int centerX;
    int centerY;
    float half_min_window_size;
    int radius1;
    int radius2;
    int radius3;
    int radius4;
    int radius5;
    int radius_small_circles;

    QPainter* painter;
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

public slots:
    void selection_mode_on(Skill*);
    void selection_mode_off();
};

#endif // PAGEWIDGET_H
