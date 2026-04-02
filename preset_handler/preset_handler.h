#ifndef PRESETSAVER_H
#define PRESETSAVER_H

#include <QObject>
#include <pagewidget.h>

class PresetHandler: public QObject
{
    Q_OBJECT
public:
    PresetHandler(PageWidget** pages, QMap<QString, QVector<Skill*>> *basic_skills);

public slots:
    void save_preset();
    void get_active_skills();
//    void create_record();
//    void save_to_file();

    void print_active_skills();

private:
    page_skills_data_t* page_skills_data[8];
    QMap<QString, QVector<Skill*>> *basic_skills;
    QVector<int> active_page_skills[NUM_OF_PAGES];
    QVector<int> active_basic_skills[NUM_OF_PAGES];
};

#endif // PRESETSAVER_H
