#include "headerwidget.h"
#include <QCoreApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

HeaderWidget::HeaderWidget(QWidget *parent) : QWidget(parent)
{
    installEventFilter(this);
    layout = new QHBoxLayout(this);
    read_json();
    add_combobox();
    add_basic_skills(0);
//    layout->addSpacing(500);
//    this->setStyleSheet("background-image: url(./src/FonRamki.png);");
}

//! Здесь читаем json
void HeaderWidget::read_json()
{
    QFile file(QCoreApplication::applicationDirPath() + "/src/data.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qWarning("Не открыть файл");
    QByteArray raw_data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(raw_data);
    QJsonObject obj = doc.object();
    skill_struct cur_skill;
    QString cur_class_name = "Basic_skills";
    const QJsonArray class_arr = obj[cur_class_name].toArray();
    for (const QJsonValue& cur_val : class_arr) {
        QJsonObject cur_obj = cur_val.toObject();
        QString class_name = cur_val["class_name"].toString();
        if(is_class_name_exists(class_name))
        {
            QString icon_path = PIC_PATH + cur_obj["icon_path"].toString();
            QString title = cur_obj["title"].toString();
            QString description = cur_obj["description"].toString();
            Skill* cur_skill = new Skill(this, icon_path, title, description);
            cur_skill->hide();
            basic_skills[class_name].append(cur_skill);
        }
    }

}
int HeaderWidget::is_class_name_exists(QString name)
{
    for(int i = 0; i < pages.size(); i++)
    {
        QString page_el = pages[i];
        if(name == page_el)
            return 1;
    }
    return 0;
}

void HeaderWidget::add_combobox()
{
    combo_pages = new QComboBox();
    combo_pages->addItems(pages);
    layout->addWidget(combo_pages);
    connect(combo_pages, SIGNAL(currentIndexChanged(int)), this, SLOT(combobox_changed(int)));
}

void HeaderWidget::combobox_changed(int page_num)
{
    remove_basic_skills();
    add_basic_skills(page_num);
}

void HeaderWidget::remove_basic_skills()
{
    for(int i = 0; i < 4; i++)
    {
        layout->removeWidget(cur_class_skills[i]);
        cur_class_skills[i]->hide();
    }
}

void HeaderWidget::add_basic_skills(int page_num)
{
    QString class_name = pages[page_num];
    int icon_size = this->height()/2;
    for(int i = 0; i < basic_skills[class_name].size(); i++)
    {
        basic_skills[class_name][i]->resize(icon_size,icon_size);
        cur_class_skills[i] = basic_skills[class_name][i];
        layout->addWidget(cur_class_skills[i]);
        cur_class_skills[i]->show();
    }
}

//! Обработчик события перерисовки
void HeaderWidget::paintEvent(QPaintEvent *e)
{
    QString class_name = pages[combo_pages->currentIndex()];
    int icon_size = this->height()/2;
    for(int i = 0; i < basic_skills[class_name].size(); i++)
    {
        if(basic_skills[class_name][i]->is_changed_size == 0)
        {
            basic_skills[class_name][i]->resize(icon_size,icon_size);
        }
        else
        {
            basic_skills[class_name][i]->resize(icon_size*2,icon_size*2);

        }
    }

    QWidget::paintEvent(e);
}
//! Обработчик событий
bool HeaderWidget::eventFilter(QObject *watched, QEvent *event)
{
    // Если нажимаем мышкой в свободном месте
//    if(event->type() == QEvent::MouseButtonPress)
//    {
//        selection_mode_off();
//        return true;
//    }
    return QWidget::eventFilter(watched, event);
}
