#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <math.h>
#include <QPainter>

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
    background[0]->setPixmap(QPixmap(QCoreApplication::applicationDirPath() + "/src/skillwheelBACKGROUND.png"));
    background[0]->move(0,0);
    background[0]->resize(int(WHEEL_WIDTH),int(WHEEL_HEIGHT));
    background[0]->setScaledContents(true);
    background[1] = new QLabel(ui->tab2);
    background[1]->setPixmap(QPixmap(QCoreApplication::applicationDirPath() + "/src/skillwheelBACKGROUND.png"));
    background[1]->move(0,0);
    background[1]->resize(int(WHEEL_WIDTH),int(WHEEL_HEIGHT));
    background[1]->setScaledContents(true);
    background[2] = new QLabel(ui->tab3);
    background[2]->setPixmap(QPixmap(QCoreApplication::applicationDirPath() + "/src/skillwheelBACKGROUND.png"));
    background[2]->move(0,0);
    background[2]->resize(int(WHEEL_WIDTH),int(WHEEL_HEIGHT));
    background[2]->setScaledContents(true);
    background[3] = new QLabel(ui->tab4);
    background[3]->setPixmap(QPixmap(QCoreApplication::applicationDirPath() + "/src/skillwheelBACKGROUND.png"));
    background[3]->move(0,0);
    background[3]->resize(int(WHEEL_WIDTH),int(WHEEL_HEIGHT));
    background[3]->setScaledContents(true);
    background[4] = new QLabel(ui->tab5);
    background[4]->setPixmap(QPixmap(QCoreApplication::applicationDirPath() + "/src/skillwheelBACKGROUND.png"));
    background[4]->move(0,0);
    background[4]->resize(int(WHEEL_WIDTH),int(WHEEL_HEIGHT));
    background[4]->setScaledContents(true);
    background[5] = new QLabel(ui->tab6);
    background[5]->setPixmap(QPixmap(QCoreApplication::applicationDirPath() + "/src/skillwheelBACKGROUND.png"));
    background[5]->move(0,0);
    background[5]->resize(int(WHEEL_WIDTH),int(WHEEL_HEIGHT));
    background[5]->setScaledContents(true);
    background[6] = new QLabel(ui->tab7);
    background[6]->setPixmap(QPixmap(QCoreApplication::applicationDirPath() + "/src/skillwheelBACKGROUND.png"));
    background[6]->move(0,0);
    background[6]->resize(int(WHEEL_WIDTH),int(WHEEL_HEIGHT));
    background[6]->setScaledContents(true);
    background[7] = new QLabel(ui->tab8);
    background[7]->setPixmap(QPixmap(QCoreApplication::applicationDirPath() + "/src/skillwheelBACKGROUND.png"));
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
    for (int dir_num = 0; dir_num < icon_categories.length(); ++dir_num)
    {
        //! Сохряняем имя категории
        QString cur_dir_name = icon_categories[dir_num];
        //! Берём массив по этому имени
        const QJsonArray arr = obj[cur_dir_name].toArray();
        //! Проходимся по массиву этой категории
        for (const QJsonValue& cur_val : arr) {
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
    paint_icons_page(ui->tab1);
    paint_icons_page(ui->tab2);
    paint_icons_page(ui->tab3);
    paint_icons_page(ui->tab4);
    paint_icons_page(ui->tab5);
    paint_icons_page(ui->tab6);
    paint_icons_page(ui->tab7);
    paint_icons_page(ui->tab8);
    this->update();
}

//! Здесь рисуем иконки только для конкретной страницы
void MainWindow::paint_icons_page(QWidget *page)
{
    int sectors = 12;
    int n_pics_in_sector = 3;
    double x,y;
    double angle;
    double radius[] = {376, 310, 253, 194};
    for(int n_circle = 0; n_circle < all_skills_data.size(); n_circle++)
    {
        QString circle_name = icon_categories[n_circle];
        int cur_circle_el = 0;
        for(int sector = 0; sector < sectors; sector++)
        {
            for(int i = 0; i < 3; i++)
            {
                if(i == 0)
                    angle = 2 * M_PI /sectors * (sector + (i+1.)/ (n_pics_in_sector+1))-0.03;
                else if(i == 1)
                    angle = 2 * M_PI /sectors * (sector + (i+1.)/ (n_pics_in_sector+1));
                else
                    angle = 2 * M_PI /sectors * (sector + (i+1.)/ (n_pics_in_sector+1))+0.03;

                x = (WHEEL_WIDTH / 2 - 16 + 2.4) + radius[n_circle] * cos(angle);     //! 2.4 - выверенное смещение, не править!
                y = (WHEEL_HEIGHT / 2 - 16 + 1) + radius[n_circle] * -sin(angle);     //! 1 - выверенное смещение, не править!
                Skill* skill = all_skills_data[circle_name][cur_circle_el].skill;
                skill = new Skill(page, all_skills_data[circle_name][cur_circle_el].title, all_skills_data[circle_name][cur_circle_el].descriprion);
                skill->setPixmap(QPixmap(PIC_PATH + all_skills_data[circle_name][cur_circle_el].icon_path));
                skill->move(int(x),int(y));
                cur_circle_el++;
            }
        }
    }
}

void MainWindow::change_page(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}

