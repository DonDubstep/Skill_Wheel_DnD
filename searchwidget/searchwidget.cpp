#include "searchwidget.h"


SearchWidget::SearchWidget(QWidget* parent, HeaderWidget* header_widget, QMenuBar* menubar): QWidget(parent)
{
    this->parent = parent;
    this->header_widget = header_widget;
    this->menubar = menubar;
    setup_ui();
    parent->installEventFilter(this);

    // Стиль
    setStyleSheet(R"(
        SearchWidget {
            background-color: #2b1f0e;
        }

        QLineEdit {
            background-color: #1c1408;
            border: 1px solid #6b4510;
            color: #f0d890;
            font-family: "Palatino Linotype", "Book Antiqua", serif;
            font-size: 12pt;
            padding: 4px 12px;
            selection-background-color: #8a5c1a;
            selection-color: #fff8dc;
        }
        QLineEdit:focus {
            border: 1px solid #c9941a;
        }

        QPushButton {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #8a5c18, stop:1 #4a2e08);
            border: 1px solid #c9941a;
            color: #f5d97a;
            font-family: "Palatino Linotype", "Book Antiqua", serif;
            font-size: 10pt;
            font-weight: bold;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #b07820, stop:1 #6a3e10);
            color: #fff8dc;
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #4a2e08, stop:1 #8a5c18);
        }
                  )");
}

void SearchWidget::show_up()
{
    this->show();
    update_geometry();
    this->search_line->setFocus();
}

//! Получаем ссылки на абсолютно все скиллы для дальнейшего поиска
void SearchWidget::get_all_skills(PageWidget *pages[], QMap<QString, QVector<Skill *> > *basic_skills)
{
    for(int page_i = 0; page_i < NUM_OF_PAGES; page_i++)
    {
        page_skills_data[page_i] = pages[page_i]->get_page_skill_data();
    }
    this->basic_skills = basic_skills;
}

#define SEARCH_HEIGHT_K 0.0535
#define SEARCH_WIDTH_K 0.3
#define SEARCH_ELEMENT_HEIGHT_K 0.0267
#define SEARCH_H_PADDING_K 0.0056

void SearchWidget::setup_ui()
{
    auto *root = new QHBoxLayout(this);
    root->setContentsMargins(8, 8, 8, 8);
    root->setSpacing(10);

    // Строка поиска
    search_line = new QLineEdit;
    search_line->setPlaceholderText("Найти...");
    search_line->setMinimumHeight(25);

    button_find = new QPushButton("Н");
    button_find->setMinimumHeight(25);
    button_find->setFixedWidth(25);

    button_exit = new QPushButton("X");
    button_exit->setMinimumHeight(25);
    button_exit->setFixedWidth(25);

    root->addWidget(search_line);
    root->addWidget(button_find);
    root->addWidget(button_exit);

    // Соединения
    connect(button_find, &QPushButton::clicked,
            this,      &SearchWidget::on_search);
    connect(search_line,  &QLineEdit::returnPressed,
            this,      &SearchWidget::on_search);
    connect(button_exit,  &QPushButton::clicked,
            this,      &SearchWidget::on_close);

    setMinimumSize(254, 50);
    setWindowTitle("Search Widget");

    setAttribute(Qt::WA_OpaquePaintEvent);
    setAutoFillBackground(false);
}

void SearchWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, false);
    p.fillRect(rect(), QColor("#2b1f0e"));

    // Внешняя тёмная рамка
    p.setPen(QPen(QColor("#5c3d0a"), 1));
    p.drawRect(rect().adjusted(0, 0, -1, -1));

    // Внутренняя золотая рамка
    p.setPen(QPen(QColor("#8a5c1a"), 1));
    p.drawRect(rect().adjusted(4, 4, -5, -5));

    // Угловые треугольные акценты
    p.setPen(Qt::NoPen);
    p.setBrush(QColor("#c9941a"));

    const int sz = 10;
    auto corner = [&](int x, int y, int sx, int sy) {
        QPolygon poly;
        poly << QPoint(x, y)
             << QPoint(x + sx * sz, y)
             << QPoint(x, y + sy * sz);
        p.drawPolygon(poly);
    };

    corner(0,          0,           1,  1);
    corner(width() - 1, 0,          -1,  1);
    corner(0,          height() - 1, 1, -1);
    corner(width() - 1, height() - 1, -1, -1);

}


bool SearchWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == parent && event->type() == QEvent::Resize)
    {
        update_geometry();
    }
    return QWidget::eventFilter(watched, event);
}

void SearchWidget::update_geometry()
{
    int margin_central_widget = 9;
    int w = qMax(254, static_cast<int>(parent->width() * SEARCH_WIDTH_K));
    int h = static_cast<int>(parent->height() * SEARCH_HEIGHT_K);
    int x = qMax(0,static_cast<int>(parent->width() - w) - margin_central_widget);
    int y = static_cast<int>(header_widget->height() + menubar->height() + SEARCH_H_PADDING_K * parent->height());
    setGeometry(x,y,w,h);
}

//! Проверка, содержит ли название или описание скилла ключевые слова
bool SearchWidget::is_skill_has_keyword(Skill *skill, QString key_word)
{
    return skill->description->name_str.contains(key_word,  Qt::CaseInsensitive) ||
           skill->description->desciption_str.contains(key_word,  Qt::CaseInsensitive);
}

//! Выбор определённого скилла на всех страницах
void SearchWidget::select_skill_on_all_pages(int sector_i, int circle_i, int s)
{
    Skill* cur_skill;
    for(int page_i = 0; page_i < pages.size(); page_i++)
    {
        sector_data_t* cur_sector_ptr = page_skills_data[page_i]->sector_ptrs[sector_i];
        Skill** cur_circle_ptr;
        switch (circle_i)
        {
        case CIRCLE_1:  cur_circle_ptr = cur_sector_ptr->circle_1;      break;
        case CIRCLE_2:  cur_circle_ptr = cur_sector_ptr->circle_2;      break;
        case CIRCLE_3:  cur_circle_ptr = cur_sector_ptr->circle_3;      break;
        default:        cur_circle_ptr = cur_sector_ptr->base_circle;   break;
        }
        cur_skill = cur_circle_ptr[s];
        cur_skill->state = SELECTED;
        cur_skill->update();
    }
}

//! Анселекция (закрашиваение серым) определённого скилла на всех страницах
void SearchWidget::unselect_skill_on_all_pages(int sector_i, int circle_i, int s)
{
    Skill* cur_skill;
    for(int page_i = 0; page_i < pages.size(); page_i++)
    {
        sector_data_t* cur_sector_ptr = page_skills_data[page_i]->sector_ptrs[sector_i];
        Skill** cur_circle_ptr;
        switch (circle_i)
        {
        case CIRCLE_1:  cur_circle_ptr = cur_sector_ptr->circle_1;      break;
        case CIRCLE_2:  cur_circle_ptr = cur_sector_ptr->circle_2;      break;
        case CIRCLE_3:  cur_circle_ptr = cur_sector_ptr->circle_3;      break;
        default:        cur_circle_ptr = cur_sector_ptr->base_circle;   break;
        }
        cur_skill = cur_circle_ptr[s];
        cur_skill->state = UNSELECTED;
        cur_skill->update();
    }
}

//! Делаем поиск среди всех скиллов по ключевым словам из строки поиска
void SearchWidget::on_search()
{
    parent->setUpdatesEnabled(false);
    QString key_word = search_line->text();
    Skill* cur_skill;

    bool found_sector_skills = false;
    int first_found_page = -1;
    // Проверяем все скиллы секторов
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        sector_data_t* cur_sector_ptr = page_skills_data[0]->sector_ptrs[sector_i];
        for(int circle_i = 0; circle_i < circle_names.size(); circle_i++)
        {
            Skill** cur_circle_ptr;
            switch (circle_i)
            {
            case CIRCLE_1:  cur_circle_ptr = cur_sector_ptr->circle_1;      break;
            case CIRCLE_2:  cur_circle_ptr = cur_sector_ptr->circle_2;      break;
            case CIRCLE_3:  cur_circle_ptr = cur_sector_ptr->circle_3;      break;
            default:        cur_circle_ptr = cur_sector_ptr->base_circle;   break;
            }
            for(int s = 0; s < 3; s++)
            {
                cur_skill = cur_circle_ptr[s];
                if(is_skill_has_keyword(cur_skill, key_word))
                {
                    select_skill_on_all_pages(sector_i, circle_i, s);
                    found_sector_skills = true;
                }
                else
                {
                    unselect_skill_on_all_pages(sector_i, circle_i, s);
                }
            }
        }
    }

    // Проверяем все скиллы хэдера
    for(int page_i = 0; page_i < pages.size(); page_i++)
    {
        QString page_name = pages[page_i];
        for(int s = 0; s < (*basic_skills)[page_name].size(); s++)
        {
            cur_skill = (*basic_skills)[page_name][s];
            if(is_skill_has_keyword(cur_skill, key_word))
            {
                cur_skill->state = SELECTED;
                // Переключаем на страницу, где впервые нашли скилл, если не нашли секторные скиллы
                if(first_found_page == -1 && !found_sector_skills)
                {
                    first_found_page = page_i;
                    emit switch_page(page_i);
                }
            }
            else
            {
                cur_skill->state = UNSELECTED;
            }
            cur_skill->update();
        }
    }

    // Проверяем все центральные скиллы
    for(int page_i = 0; page_i < pages.size(); page_i++)
    {
        for(int s = 0; s < 4; s++)
        {
            cur_skill = page_skills_data[page_i]->center_skills[s];
            if(is_skill_has_keyword(cur_skill, key_word))
            {
                cur_skill->state = SELECTED;
                // Переключаем на страницу, где впервые нашли скилл, если не нашли секторные скиллы
                if(first_found_page == -1 && !found_sector_skills)
                {
                    first_found_page = page_i;
                    emit switch_page(page_i);
                }
            }
            else
            {
                cur_skill->state = UNSELECTED;
            }
            cur_skill->update();
        }
    }
    parent->setUpdatesEnabled(true);
    parent->update();
}

//! Закытие виджета поиска
void SearchWidget::on_close()
{
    this->search_line->clear();
    this->hide();

    parent->setUpdatesEnabled(false);
    Skill* cur_skill;
    // Сбрасываем состояния всех скиллов
    for(int page_i = 0; page_i < pages.size(); page_i++)
    {
        QString page_name = pages[page_i];
        // все скиллы хэдера
        for(int s = 0; s < (*basic_skills)[page_name].size(); s++)
        {
            cur_skill = (*basic_skills)[page_name][s];
            cur_skill->state = NONE;
            cur_skill->update();
        }

        // Все скиллы секторов
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
                default:        cur_circle_ptr = cur_sector_ptr->base_circle;   break;
                }
                for(int s = 0; s < 3; s++)
                {
                    cur_skill = cur_circle_ptr[s];
                    cur_skill->state = NONE;
                    cur_skill->update();
                }
            }
        }

        // Все центральные скиллы
        for(int s = 0; s < 4; s++)
        {
            cur_skill = page_skills_data[page_i]->center_skills[s];
            cur_skill->state = NONE;
            cur_skill->update();
        }
    }
    parent->setUpdatesEnabled(true);
    parent->update();
}

