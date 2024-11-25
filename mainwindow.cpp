#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <math.h>

enum wheel_size
{
    WHEEL_WIDTH = 799,
    WHEEL_HEIGHT = 799
};

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->combo_pages->addItems(pages);
    connect(ui->combo_pages, SIGNAL(currentIndexChanged(int)), this, SLOT(change_page(int)));
    ui->tabWidget->tabBar()->setVisible(false);

    // Делаем фон на всех страницах
    background[0] = new QLabel(ui->tab1);
    background[0]->setPixmap(QPixmap("skillwheelBACKGROUND_white.png"));
    background[0]->move(0,0);
    background[0]->resize(int(WHEEL_WIDTH),int(WHEEL_HEIGHT));
    background[0]->setScaledContents(true);
    background[1] = new QLabel(ui->tab2);
    background[1]->setPixmap(QPixmap("skillwheelBACKGROUND_white.png"));
    background[1]->move(0,0);
    background[1]->resize(int(WHEEL_WIDTH),int(WHEEL_HEIGHT));
    background[1]->setScaledContents(true);
    background[2] = new QLabel(ui->tab3);
    background[2]->setPixmap(QPixmap("skillwheelBACKGROUND_white.png"));
    background[2]->move(0,0);
    background[2]->resize(int(WHEEL_WIDTH),int(WHEEL_HEIGHT));
    background[2]->setScaledContents(true);
    background[3] = new QLabel(ui->tab4);
    background[3]->setPixmap(QPixmap("skillwheelBACKGROUND_white.png"));
    background[3]->move(0,0);
    background[3]->resize(int(WHEEL_WIDTH),int(WHEEL_HEIGHT));
    background[3]->setScaledContents(true);
    background[4] = new QLabel(ui->tab5);
    background[4]->setPixmap(QPixmap("skillwheelBACKGROUND_white.png"));
    background[4]->move(0,0);
    background[4]->resize(int(WHEEL_WIDTH),int(WHEEL_HEIGHT));
    background[4]->setScaledContents(true);
    background[5] = new QLabel(ui->tab6);
    background[5]->setPixmap(QPixmap("skillwheelBACKGROUND_white.png"));
    background[5]->move(0,0);
    background[5]->resize(int(WHEEL_WIDTH),int(WHEEL_HEIGHT));
    background[5]->setScaledContents(true);
    background[6] = new QLabel(ui->tab7);
    background[6]->setPixmap(QPixmap("skillwheelBACKGROUND_white.png"));
    background[6]->move(0,0);
    background[6]->resize(int(WHEEL_WIDTH),int(WHEEL_HEIGHT));
    background[6]->setScaledContents(true);
    background[7] = new QLabel(ui->tab8);
    background[7]->setPixmap(QPixmap("skillwheelBACKGROUND_white.png"));
    background[7]->move(0,0);
    background[7]->resize(int(WHEEL_WIDTH),int(WHEEL_HEIGHT));
    background[7]->setScaledContents(true);

    read_json();
    show_icons();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//! Здесь читаем json
void MainWindow::read_json()
{
    QFile file(QCoreApplication::applicationDirPath() + "/data.json");
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
    for (int dir_num = 0; dir_num < icon_categories.length(); ++dir_num)
    {
        //! Сохряняем имя категории
        QString cur_dir_name = icon_categories[dir_num];
        //! Берём массив по этому имени
        QJsonArray arr = obj[cur_dir_name].toArray();
        //! Проходимся по массиву этой категории
        for (const QJsonValue& cur_val : qAsConst(arr)) {
            QJsonObject cur_obj = cur_val.toObject();
            //! Считываем данные из полей
            cur_skill.icon_path = cur_obj["icon_path"].toString();
            cur_skill.title = cur_obj["title"].toString();
            cur_skill.descriprion = cur_obj["description"].toString();
            //! Сохраняем в единую структуру данных
            all_skills_data[cur_dir_name].append(cur_skill);
        }
    }
}

//! Здесь рисуем иконки
void MainWindow::show_icons()
{
    double radius = 253; // 196 // 253 // 310 //376
    int sectors = 12;
    int n_pics_in_sector = 3;
    double x,y;
    double angle;
    int j = 0;
    for(int sector = 0; sector < sectors; sector++)
    {
        for(int i = 0; i < 3; i++)
        {

            angle = 2 * M_PI /sectors * (sector + (i+1.)/ (n_pics_in_sector+1));
            x = (WHEEL_WIDTH / 2 - 15 + 3.5) + radius * cos(angle);     //! 3.5 - выверенное смещение, не править!
            y = (WHEEL_HEIGHT / 2 - 15 + 3.5) + radius * -sin(angle);   //! 3.5 - выверенное смещение, не править!
            Skill* skill = all_skills_data["Attack"][j].skill;
            skill = new Skill(ui->tab1, all_skills_data["Attack"][j].title, all_skills_data["Attack"][j].descriprion);
            skill->setPixmap(QPixmap(PIC_PATH + all_skills_data["Attack"][j++].icon_path));
            skill->move(int(x),int(y));
        }
    }
    this->update();
}

void MainWindow::change_page(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}

