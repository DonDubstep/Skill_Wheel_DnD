#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QPolygon>
#include <QEvent>
#include <QMenuBar>
#include "headerwidget.h"
#include "pagewidget.h"

class SearchWidget : public QWidget
{
    Q_OBJECT
public:
    SearchWidget(QWidget* parent, HeaderWidget* header_widget, QMenuBar* menubar);

public slots:
    void show_up();
    void get_all_skills(PageWidget* pages[], QMap<QString, QVector<Skill*>>* basic_skills);
private:
    void setup_ui();
    void on_search();
    void on_close();
    void update_geometry();
    bool is_skill_has_keyword(Skill* skill, QString key_word);
    void select_skill_on_all_pages(int sector_i, int circle_i, int s);
    void unselect_skill_on_all_pages(int sector_i, int circle_i, int s);
    HeaderWidget* header_widget;
    QMenuBar* menubar;

    QWidget* parent;
    QLineEdit    *search_line;
    QPushButton  *button_find;
    QPushButton  *button_exit;

    page_skills_data_t* page_skills_data[NUM_OF_PAGES];
    QMap<QString, QVector<Skill*>>* basic_skills;

protected:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void switch_page(int);
    void set_all_selection_off();

};

#endif // SEARCHWIDGET_H
