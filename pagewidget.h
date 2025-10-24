#ifndef PAGEWIDGET_H
#define PAGEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMap>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "selection.h"
#include <QCoreApplication>


class PageWidget : public QWidget
{
    Q_OBJECT
public:
    PageWidget(QWidget *parent = nullptr);
    Selection* selection;

signals:
    void selection_off();

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

    circle_skill_data_t circle_skills;
    sector_data_t* sector_ptrs[12];

    const QString PIC_PATH = QCoreApplication::applicationDirPath() + "/src/HoMM5_Skills/";

    int find_in_dependent_skills(QVector<Skill*> dependent_skills, Skill* skill);
    bool eventFilter(QObject *watched, QEvent *event) override;
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void init_background_colors();
    void init_sector_pointers();
    void read_json();
    void paint_concentric_circles();
    void paint_small_circles();
    void paint_skills();
};

#endif // PAGEWIDGET_H
