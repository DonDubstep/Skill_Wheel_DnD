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
        PageWidget *page = new PageWidget();
        ui->tabWidget->addTab(page, QString::number(i+1));
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


