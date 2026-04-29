#ifndef PRESETSAVER_H
#define PRESETSAVER_H

#include <QObject>
#include <pagewidget.h>

class PresetHandler: public QObject
{
    Q_OBJECT
public:
    PresetHandler(PageWidget** pages, QMap<QString, QVector<Skill*>> *basic_skills, QWidget* parent);

public slots:
    // Функции сохранения пресетов
    void save_preset();
    void save_as_preset();
    void get_active_skills();
    void save_to_file(QString filePath);
    void print_active_skills();

    // Функции открытия пресетов
    void open_preset();

signals:
    void activate_read_skills(QVector<int>*, QVector<int>*);
private:
    QWidget* parent;
    // Функции сохранения пресетов
    page_skills_data_t* page_skills_data[8];
    QMap<QString, QVector<Skill*>> *basic_skills;
    QVector<int> active_page_skills[NUM_OF_PAGES];
    QVector<int> active_basic_skills[NUM_OF_PAGES];

    QString make_active_skills_to_str(QVector<int>* active_basic_skills);
    QString createNextSaveFile();
    QString chose_save_file_name();

    // Функции открытия пресетов
    void read_save();
    void activate_saved_skills();
    QVector<int> parse_skills_from_str(QString active_skills_str);
    QVector<int> read_page_skills[NUM_OF_PAGES];
    QVector<int> read_basic_skills[NUM_OF_PAGES];
};

#endif // PRESETSAVER_H
