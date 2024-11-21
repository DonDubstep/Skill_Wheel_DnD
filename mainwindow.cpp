#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    window_ptr = this;
    read_json();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//! Здесь читаем json
//! warning: отделить отрисовку
void MainWindow::read_json()
{
    QFile file(QCoreApplication::applicationDirPath() + "/data.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qWarning("Не открыть файл");
    QByteArray raw_data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(raw_data);
    QJsonObject obj = doc.object();
    QJsonArray arr = obj["Attack"].toArray();
    skill_struct cur_skill;
    int x = 100;
    int y = 50;
    for (const QJsonValue& cur_val : arr) {
        QJsonObject cur_obj = cur_val.toObject();
        cur_skill.icon_path = cur_obj["icon_path"].toString();
        cur_skill.title = cur_obj["title"].toString();
        cur_skill.descriprion = cur_obj["description"].toString();
        cur_skill.title_color = cur_obj["title_color"].toString();
        QPixmap* pic = new QPixmap(PIC_PATH + cur_obj["icon_path"].toString());
        cur_skill.skill = new Skill(this, cur_obj["title"].toString(), cur_obj["description"].toString());
        cur_skill.skill->setPixmap(*pic);
        cur_skill.skill->move(x, y);
        y += 100;
        all_skills_data.attack.append(cur_skill);
    }
    x = 300;
    y = 50;
    arr = obj["BattleCries"].toArray();
    for (const QJsonValue &cur_val : arr)
    {
        QJsonObject cur_obj = cur_val.toObject();
        cur_skill.icon_path = cur_obj["icon_path"].toString();
        cur_skill.title = cur_obj["title"].toString();
        cur_skill.descriprion = cur_obj["description"].toString();
        cur_skill.title_color = cur_obj["title_color"].toString();
        cur_skill.skill = new Skill(this, cur_obj["title"].toString(), cur_obj["description"].toString());
        cur_skill.skill->setPixmap(QPixmap(PIC_PATH + cur_obj["icon_path"].toString()));
        cur_skill.skill->move(x, y);
        y += 100;
        all_skills_data.battle_cries.append(cur_skill);
    }
}

void MainWindow::show_icons()
{

}

