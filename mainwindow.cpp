#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <math.h>
#include <QPainter>
#include <QEvent>
#include <QKeyEvent>
#include <pagewidget.h>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->installEventFilter(this);
    ui->combo_pages->addItems(pages);
    connect(ui->combo_pages, SIGNAL(currentIndexChanged(int)), this, SLOT(change_page(int)));
    ui->tabWidget->tabBar()->setVisible(false);

    addTabs();

    read_json();
    show_icons();

}

MainWindow::~MainWindow()
{
    delete ui;
}

//! Добавляет страницы в виджет QTabWidget
void MainWindow::addTabs()
{
    for(int i = 0; i < NUM_OF_PAGES; i++)
    {
        PageWidget *page = new PageWidget;
        ui->tabWidget->addTab(page, QString::number(i+1));
    }
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
            cur_skill.description = cur_obj["description"].toString();
            //! Сохраняем в единую структуру данных
            all_skills_data[cur_dir_name].append(cur_skill);
        }
    }
}

//! Здесь рисуем иконки
void MainWindow::show_icons()
{
    for(int i = 0; i < NUM_OF_PAGES; i++)
    {
        paint_icons_page(ui->tabWidget->widget(i));
    }
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
                QString icon_path = PIC_PATH + all_skills_data[circle_name][cur_circle_el].icon_path;
                QString title = all_skills_data[circle_name][cur_circle_el].title;
                QString description = all_skills_data[circle_name][cur_circle_el].description;
                all_skills_data[circle_name][cur_circle_el].skill = new Skill(page,icon_path, title, description);
                all_skills_data[circle_name][cur_circle_el].skill->move(int(x),int(y));
                all_skills_data[circle_name][cur_circle_el].skill->installEventFilter(this);
                cur_circle_el++;
            }
        }
    }
}

//! Фильтр обработки событий
bool MainWindow::eventFilter(QObject* object, QEvent* event)
{
    // Принимаем события скилла
    if(object->objectName() == "Skill")
    {
        int dont_hide_description_flag = get_dont_hide_description_flag(static_cast<Skill*>(object));
        // При наведении на скилл мышкой
        if(event->type() == QEvent::Enter)
        {
            static_cast<Skill*>(object)->setFocus();
            // Увеличиваем если только уже не увеличили
            if(dont_hide_description_flag == 0)
            {
                zoom_widget(static_cast<Skill*>(object));
            }
            return true;
        }
        // Убираем мышку со скилла
        else if(event->type() == QEvent::Leave)
        {
            static_cast<Skill*>(object)->clearFocus();
            // Уменьшаем, если не закрепили скилл
            if(dont_hide_description_flag == 0)
            {
                zoom_out_widget(static_cast<Skill*>(object));
                delete_from_pinned_skills(object);
            }
            return true;
        }
        // Нажимаем кнопку на клавиатуре
        else if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *key_event = static_cast<QKeyEvent*>(event);
            // Нажимаем букву T
            if(key_event->key() == Qt::Key_T)
            {
                // Добавляем скилл в вектор зафиксированных
                if(get_dont_hide_description_flag(static_cast<Skill*>(object)) == 0)
                    pinned_skills.push_back(object);
                // Фиксируем флагом
                switch_dont_hide_description_flag(static_cast<Skill*>(object));
            }
            return true;
        }
    }
    // Принимаем события всего окна
    else if(object->objectName() == "MainWindow")
    {
        // Если нажали мышкой
        if(event->type() == QEvent::MouseButtonPress)
        {
            while(!pinned_skills.isEmpty())
            {
                Skill* skill = static_cast<Skill*>(pinned_skills.last());
                switch_dont_hide_description_flag(skill);
                skill->clearFocus();
                zoom_out_widget(skill);
                pinned_skills.pop_back();
            }
        }
    }
    return QMainWindow::eventFilter(object, event);
}
//! Функция смены вкладки для перехода между классами
void MainWindow::change_page(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}

//! Функция увеличения иконки скилла и отображения рамки описания
void MainWindow::zoom_widget(Skill* skill)
{
    skill->setFixedSize(BASE_SIZE * INCREACE_KOEF, BASE_SIZE * INCREACE_KOEF);
    int offset = BASE_SIZE / 2;
    skill->move(skill->x() - offset, skill->y() - offset);
    skill->description->show();
    skill->description->raise();
}

//! Функция возвращения иконки скилла в исходное положение и скрытие рамки
void MainWindow::zoom_out_widget(Skill *skill)
{
    skill->setFixedSize(BASE_SIZE, BASE_SIZE);
    int offset = BASE_SIZE / 2;
    skill->move(skill->x() + offset, skill->y() + offset);
    skill->description->hide();
}

//! Функция переключения флага запрета или разрешения скилла выходить из режима наведения
void MainWindow::switch_dont_hide_description_flag(Skill *skill)
{
    skill->dont_hide_description_flag = !skill->dont_hide_description_flag;
}

//! Получаем флаг запрета скрытия рамки описания и уменьшения иконки
int MainWindow::get_dont_hide_description_flag(Skill *skill)
{
    return skill->dont_hide_description_flag;
}

//! Очищаем массив зафиксированных скллов
void MainWindow::delete_from_pinned_skills(QObject *object)
{
    if(!pinned_skills.isEmpty())
    {
        for(int i = 0; i < pinned_skills.size(); i++)
        {
            if(pinned_skills[i] == object)
            {
                pinned_skills.remove(i);
                return;
            }
        }
    }
}


