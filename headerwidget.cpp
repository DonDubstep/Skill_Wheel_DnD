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
    header_selection = new HeaderSelection(&basic_skills);
    connect(header_selection, SIGNAL(set_header_scores(int)), this, SLOT(set_header_scores(int)));
    read_json();
    add_combobox();
    add_basic_skills(0);
    add_scores(0);
}

//! Здесь читаем json
void HeaderWidget::read_json()
{
    QString icon_path, title, description;
    QFile file(QCoreApplication::applicationDirPath() + "/src/data.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qWarning("Не открыть файл");
    QByteArray raw_data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(raw_data);
    QJsonObject root = doc.object();
    QJsonObject class_obj = root["Basic_skills"].toObject();
    for(int class_i = 0; class_i < json_pages.size(); class_i++)
    {
        QString json_class_name = json_pages[class_i];
        const QJsonArray class_arr = class_obj[json_class_name].toArray();
        for(const QJsonValue& cur_val : class_arr)
        {
            QJsonObject cur_obj = cur_val.toObject();
            icon_path = PIC_PATH + cur_obj["icon_path"].toString();
            title = cur_obj["title"].toString();
            description = cur_obj["description"].toString();
            Skill* cur_skill = new Skill(this, icon_path, title, description);
            cur_skill->hide();
            QString class_name = pages[class_i];
            connect(cur_skill, SIGNAL(icon_selected(Skill*)), header_selection, SLOT(selection_header_on(Skill*)));
            basic_skills[class_name].append(cur_skill);
        }
    }
    file.close();
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
    header_selection->set_cur_page(page_num);
    emit switch_page(page_num);
}

void HeaderWidget::remove_basic_skills()
{
    for(int i = 0; i < BASIC_SKILL_NUM; i++)
    {
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
    paint_combobox();
    paint_basic_skills();
    paint_scores();
    QWidget::resizeEvent(e);
}

void HeaderWidget::set_scores_page(int score)
{
    scores_page = score;
    paint_scores();
}

void HeaderWidget::set_header_scores(int score)
{
    scores_header = score;
    paint_scores();
}

void HeaderWidget::null_scores()
{
    scores_header = 0;
    scores_page = 0;
    paint_scores();
}

void HeaderWidget::add_combobox()
{
    combo_pages = new QComboBox(this);
    combo_pages->addItems(pages);
    connect(combo_pages, SIGNAL(currentIndexChanged(int)), this, SLOT(combobox_changed(int)));
    header_selection->set_cur_page(0);
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
#define LABEL_PADDING_OF_SKILLS_K 0.03125
#define FONT_SIZE_K 0.4

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
    int font_size = static_cast<int>(this->height() * FONT_SIZE_K);
    QFont font = QFont("Cambria", font_size);
    font.setBold(true);
    font.setItalic(true);
    scores->setFont(font);
    double x_end_of_prev_elements = this->width() * ICON_PADDING_LEFT_K + BASIC_SKILL_NUM * (this->height() * ICON_DIAMETER_K + this->width() * ICON_MARGIN_BETWEEN_K);
    int x = static_cast<int>(x_end_of_prev_elements + this->width() * LABEL_PADDING_OF_SKILLS_K);
    int y = static_cast<int>(this->height() * TOP_PADDING_K);
    scores->move(x, y);
    scores->resize(font_size * 2, font_size);
    int total_scores = scores_page + scores_header;
    scores->setText(QString::number(total_scores));
}

