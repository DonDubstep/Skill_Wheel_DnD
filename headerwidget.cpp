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
    read_json();
    add_combobox();
    add_basic_skills(0);
    add_scores(0);
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


void HeaderWidget::combobox_changed(int page_num)
{
    remove_basic_skills();
    add_basic_skills(page_num);
}

void HeaderWidget::remove_basic_skills()
{
    for(int i = 0; i < BASIC_SKILL_NUM; i++)
    {
        layout->removeWidget(cur_class_skills[i]);
        cur_class_skills[i]->hide();
    }
}

//! Обработчик события перерисовки
void HeaderWidget::paintEvent(QPaintEvent *e)
{

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

void HeaderWidget::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e)
    paint_combobox();
    paint_basic_skills();
    paint_scores();
}

void HeaderWidget::add_combobox()
{
    combo_pages = new QComboBox(this);
    combo_pages->addItems(pages);
    connect(combo_pages, SIGNAL(currentIndexChanged(int)), this, SLOT(combobox_changed(int)));
    paint_combobox();
}
void HeaderWidget::add_basic_skills(int page_num)
{
    QString class_name = pages[page_num];
    for(int i = 0; i < basic_skills[class_name].size(); i++)
    {
        cur_class_skills[i] = basic_skills[class_name][i];
    }
    paint_basic_skills();
}

void HeaderWidget::add_scores(int score)
{
    scores = new QLabel();
    scores->setParent(this);
    scores->setText(QString::number(score));
    paint_scores();
}

#define LEFT_PADDING_K 0.01875
#define TOP_PADDING_K 0.234375
#define COMBO_WIDTH_K 0.25
#define COMBO_HEIGHT_K 0.53125
#define ICON_DIAMETER_K 0.5
#define ICON_PADDING_LEFT_K 0.33125
#define ICON_MARGIN_BETWEEN_K 0.02
#define LABEL_PADDING_K 0.5825


void HeaderWidget::paint_combobox()
{
    int x = static_cast<int>(this->width() * LEFT_PADDING_K);
    int y = static_cast<int>(this->height() * TOP_PADDING_K);
    int w = static_cast<int>(this->width() * COMBO_WIDTH_K);
    int h = static_cast<int>(this->height() * COMBO_HEIGHT_K);
    combo_pages->setGeometry(x, y, w ,h);
}
void HeaderWidget::paint_basic_skills()
{
    int icon_size = static_cast<int>(this->height() * ICON_DIAMETER_K);
    int x, y;
    for(int i = 0; i < BASIC_SKILL_NUM; i++)
    {
        x = static_cast<int>(this->width() * ICON_PADDING_LEFT_K + i * (this->height() * ICON_DIAMETER_K + this->width() * ICON_MARGIN_BETWEEN_K));
        y = static_cast<int>(this->height() * TOP_PADDING_K);
        cur_class_skills[i]->move(x, y);
        cur_class_skills[i]->resize(icon_size, icon_size);
        cur_class_skills[i]->show();
    }

}
void HeaderWidget::paint_scores()
{
    QFont font = QFont("Helvetica", 20, true);
    font.setBold(true);
    scores->setFont(font);
    int x = static_cast<int>(this->width() * LABEL_PADDING_K);
    int y = static_cast<int>(this->height() * TOP_PADDING_K);
    scores->move(x, y);
}

