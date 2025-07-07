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
#include "skills_data.h"


class PageWidget : public QWidget
{
    Q_OBJECT
public:
    PageWidget(QWidget *parent = nullptr);

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

    QMap<QString, QVector<skill_struct>> all_skills_data;
    SkillDependencies skill_dependencies;
    const QString PIC_PATH = QCoreApplication::applicationDirPath() + "/src/HoMM5_Skills/";

    int find_in_dependent_skills(QVector<Skill*> dependent_skills, Skill* skill);
    bool eventFilter(QObject *watched, QEvent *event) override;
    void paintEvent(QPaintEvent *) override;
    void init_background_colors();
    void read_json();
    void init_skills();
    void paint_concentric_circles();
    void paint_small_circles();
    void paint_skills();

public slots:
    void selection_mode_on(Skill*);
    void selection_mode_off();
};

#endif // PAGEWIDGET_H
