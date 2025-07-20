#include "headerwidget.h"
#include <QCoreApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

HeaderWidget::HeaderWidget(QWidget *parent) : QWidget(parent)
{
    this->setStyleSheet("background-image: url(./src/FonRamki.png);");
    layout = new QHBoxLayout(this);
    init_basic_skills();
    read_json();
    add_combobox();
}

void HeaderWidget::init_basic_skills()
{
    for (int i = 0; i < 8; i++)
    {
        basic_skills[pages[i]] = new Skill*[4];
    }
}

//! Здесь читаем json
void HeaderWidget::read_json()
{
    QFile file(QCoreApplication::applicationDirPath() + "/src/data.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qWarning("Не открыть файл");
    //! Читаем данные из файла
    QByteArray raw_data = file.readAll();
    //! Преобразуем в формат Json
    QJsonDocument doc = QJsonDocument::fromJson(raw_data);
    //! Преобразуем в объект Json
    QJsonObject obj = doc.object();
    //! Сохранять текущий элемент будем сюда
    skill_struct cur_skill;
    //! Для каждой категории иконок
    for (int dir_num = 0; dir_num < 1; ++dir_num)
    {
        //! Сохряняем имя категории
        QString cur_class_name = pages[dir_num];
        //! Берём массив по этому имени
        const QJsonArray class_arr = obj[cur_class_name].toArray();
        int i = 0;
        //! Проходимся по массиву этой категории
        for (const QJsonValue& cur_val : class_arr) {
            QString class_name = cur_val["class_name"].toString();
            if(is_class_name_exists(class_name))
            {
                QJsonObject cur_obj = cur_val.toObject();
                QString icon_path = cur_obj["icon_path"].toString();
                QString title = cur_obj["icon_path"].toString();
                QString description = cur_obj["icon_path"].toString();
                basic_skills[cur_class_name][i++] = new Skill(this, PIC_PATH + icon_path,title, description);
            }
        }
    }
}
int HeaderWidget::is_class_name_exists(QString name)
{
    for(int i = 0; i < pages.size(); i++)
    {
        if(name == pages[i])
            return 1;
    }
    return 0;
}

void HeaderWidget::add_combobox()
{
    combo_pages = new QComboBox();
    for(int i = 0; i < pages.size(); i++)
    {
        combo_pages->addItem(pages[i]);
    }
    layout->addWidget(combo_pages);
}
