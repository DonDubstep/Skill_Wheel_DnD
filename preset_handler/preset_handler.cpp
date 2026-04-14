 #include "preset_handler.h"
#include <QDebug>
#include <QDir>

PresetHandler::PresetHandler(PageWidget** pages, QMap<QString, QVector<Skill*>> *basic_skills)
{
    for(int p = 0; p < NUM_OF_PAGES; p++)
    {
        this->page_skills_data[p] = pages[p]->get_page_skill_data();
    }
    this->basic_skills = basic_skills;
}

//! Вызывается по событию "Ctrl+S"
void PresetHandler::save_preset()
{
    get_active_skills();
    save_to_file();
}

//! Формирование вектора активных скиллов
void PresetHandler::get_active_skills()
{
    Skill* cur_skill;
    QString class_name;
    int index;
    // Ищем на всех страницах классов
    for(int page_i = 0; page_i < NUM_OF_PAGES; page_i++)
    {
        // Проверяем какие скиллы на странице активны
        for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
        {
            sector_data_t* cur_sector_ptr = page_skills_data[page_i]->sector_ptrs[sector_i];
            for(int circle_i = 0; circle_i < circle_names.size(); circle_i++)
            {
                Skill** cur_circle_ptr;
                switch (circle_i)
                {
                case CIRCLE_1:  cur_circle_ptr = cur_sector_ptr->circle_1;      break;
                case CIRCLE_2:  cur_circle_ptr = cur_sector_ptr->circle_2;      break;
                case CIRCLE_3:  cur_circle_ptr = cur_sector_ptr->circle_3;      break;
                default:        cur_circle_ptr = cur_sector_ptr->base_circle;  break;
                }
                for(int s = 0; s < 3; s++)
                {
                    cur_skill = cur_circle_ptr[s];
                    if(cur_skill->state == SELECTED)
                    {
                        active_page_skills[page_i].append(cur_skill->index);
                    }
                }
            }
        }

        // Проверяем, какие скиллы в заголовке активны
        class_name = pages[page_i];
        for(int i = 0; i < (*basic_skills)[class_name].size(); i++)
        {
            if((*basic_skills)[class_name][i]->state == SELECTED)
            {
                index = (*basic_skills)[class_name][i]->index;
                active_basic_skills[page_i].append(index);
            }
        }
    }
}

//! Главная функция сохранения в файл
void PresetHandler::save_to_file()
{
    QJsonObject root_obj;
    for(int page_i = 0; page_i < NUM_OF_PAGES; page_i++)
    {
        QString class_name = json_pages[page_i];
        QJsonObject class_obj;
        QString basic_skills_str = make_active_skills_to_str(&active_basic_skills[page_i]);
        QString page_skills_str = make_active_skills_to_str(&active_page_skills[page_i]);
        class_obj["basic_skills"] = basic_skills_str;
        class_obj["page_skills"] = page_skills_str;
        root_obj[class_name] = class_obj;
    }
    QJsonDocument json_doc(root_obj);
    QString file_path = createNextSaveFile();
    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Ошибка создания файла:" << file_path;
    }
    file.write(json_doc.toJson((QJsonDocument::Indented)));
    file.close();
}

//! Дебаггинг формирования активных скиллов
void PresetHandler::print_active_skills()
{
    qDebug() << "page_skills:";
    for(int page_i = 0; page_i < NUM_OF_PAGES; page_i++)
    {
        qDebug() << "page" << page_i;
        QDebug dbg = qDebug();
        for(int index : active_page_skills[page_i])
        {
            dbg << index << ",";
        }
    }
    qDebug() << "basic_skills:";
    for(int page_i = 0; page_i < NUM_OF_PAGES; page_i++)
    {
        qDebug() << "page" << page_i;
        QDebug dbg = qDebug();
        for(int index : active_basic_skills[page_i])
        {
            dbg << index << ",";
        }
    }
}

//! Формируем строку из массива для записи в save-файл
QString PresetHandler::make_active_skills_to_str(QVector<int> *active_skills)
{
    QString result = "";
    for(int s = 0; s < active_skills->size(); s++)
    {
        result += QString::number((*active_skills)[s]);
        // Ставим пробел как разделитель между индексами скиллов
        if(s != active_skills->size() - 1)
        {
            result += " ";
        }
    }
    //
    active_skills->clear();
    return result;
}

//! Функция создания нового файла сохранения
QString PresetHandler::createNextSaveFile()
{
    int saveNumber = 1;
    QString filePath;

    // Ищем первый свободный номер
    while (true)
    {
        filePath = QString("src/saves/save%1.json").arg(saveNumber);

        if (!QFile::exists(filePath))
        {
            break;
        }
        saveNumber++;
    }

    // Создаём директорию если её нет
    QDir dir;
    if (!dir.exists("src/saves"))
    {
        dir.mkpath("src/saves");
    }

    // Создаём и открываем файл
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Создан файл:" << filePath;
        file.close();
        return filePath;
    }
    else
    {
        qDebug() << "Ошибка создания файла:" << filePath;
        return QString();
    }
}
