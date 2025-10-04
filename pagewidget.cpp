#include "pagewidget.h"
#include <math.h>
#include <QDebug>

PageWidget::PageWidget(QWidget *parent) : QWidget(parent)
{
    installEventFilter(this);
    temp_depends_struct = new QVector<temp_depends_struct_t>();

    init_background_colors();
    init_sector_pointers();
    read_json();
    make_dependencies();
//    skill_dependencies.make_depends(&all_skills_data);
}

//! Заносим данные цветов сегментов кругов
void PageWidget::init_background_colors()
{
    // Внутренний круг
    segment_colors = new QString*[3];
    segment_colors[0] = new QString[360/SEGMENT_ANGLE];
    segment_colors[0][0] = "#FFCCB2";
    segment_colors[0][1] = "#FF99FF";
    segment_colors[0][2] = "#FFE478";
    segment_colors[0][3] = "#FF9470";
    segment_colors[0][4] = "#B2B2B2";
    segment_colors[0][5] = "#DEB054";
    segment_colors[0][6] = "#ABD94E";
    segment_colors[0][7] = "#FFFF85";
    segment_colors[0][8] = "#CCCCFF";
    segment_colors[0][9] = "#FF7575";
    segment_colors[0][10] = "#8FB4FF";
    segment_colors[0][11] = "#B6FF6F";

    // Средний круг
    segment_colors[1] = new QString[360/SEGMENT_ANGLE];
    segment_colors[1][0] = "#FFDAC7";
    segment_colors[1][1] = "#FFC2FF";
    segment_colors[1][2] = "#FFEEAB";
    segment_colors[1][3] = "#FFBEA8";
    segment_colors[1][4] = "#CCCCCC";
    segment_colors[1][5] = "#EDC77C";
    segment_colors[1][6] = "#C6E882";
    segment_colors[1][7] = "#FFFFB0";
    segment_colors[1][8] = "#DEDEFF";
    segment_colors[1][9] = "#FFB2B2";
    segment_colors[1][10] = "#B0CAFF";
    segment_colors[1][11] = "#D2FFA8";

    // Внешний круг
    segment_colors[2] = new QString[360/SEGMENT_ANGLE];
    segment_colors[2][0] = "#FFE9DE";
    segment_colors[2][1] = "#FFE0FF";
    segment_colors[2][2] = "#FFF7D9";
    segment_colors[2][3] = "#FFDBCF";
    segment_colors[2][4] = "#E5E5E5";
    segment_colors[2][5] = "#FFE5B2";
    segment_colors[2][6] = "#E5FABB";
    segment_colors[2][7] = "#FFFFD6";
    segment_colors[2][8] = "#EDEDFF";
    segment_colors[2][9] = "#FFD4D4";
    segment_colors[2][10] = "#DBE7FF";
    segment_colors[2][11] = "#E5FFCE";
}

void PageWidget::init_sector_pointers()
{
    for(int i = 0; i < 12; i++)
    {
        sector_ptrs[i] = &circle_skills.magic_of_chaos + i;
    }
}


void PageWidget::read_json()
{
    QString title, description, icon_path;
    short index;
    temp_depends_struct_t temp_skill_with_depends;

    QFile file(QCoreApplication::applicationDirPath() + "/src/data.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qWarning("Не открыть файл");
    //! Читаем данные из файла
    QByteArray raw_data = file.readAll();
    //! Преобразуем в формат Json
    QJsonDocument doc = QJsonDocument::fromJson(raw_data);
    //! Преобразуем в объект Json
    QJsonObject root = doc.object();
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        QString cur_sector = sector_names[sector_i];
        QJsonObject sector_obj = root[cur_sector].toObject();

        sector_data_t* cur_sector_ptr = sector_ptrs[sector_i];

        for(int circle_i = 0; circle_i < circle_names.size(); circle_i++)
        {
            QString cur_cirlce = circle_names[circle_i];
            const QJsonArray circle_arr = sector_obj[cur_cirlce].toArray();
            for(int i = 0; i < 3; i++)
            {
                QJsonObject cur_skill_data = circle_arr[i].toObject();
                title = cur_skill_data["title"].toString();
                description = cur_skill_data["description"].toString();
                icon_path = cur_skill_data["icon_path"].toString();
                index = static_cast<short>(cur_skill_data["index"].toInt());

                QJsonArray json_depends_arr = cur_skill_data["depends"].toArray();
                QVector<short> depends;
                if(json_depends_arr.size() != 0)
                {
                    depends.reserve(json_depends_arr.size());
                    std::transform(json_depends_arr.begin(), json_depends_arr.end(),
                                   std::back_inserter(depends),
                                   [](const QJsonValue& value)
                                    {return static_cast<short>(value.toInt());
                                    });
                }
                Skill* cur_skill = new Skill(this, PIC_PATH + icon_path, title, description);
                cur_skill->index = index;

                temp_skill_with_depends.skill = cur_skill;
                temp_skill_with_depends.depends = depends;
                temp_depends_struct->append(temp_skill_with_depends);

                connect(cur_skill, SIGNAL(icon_selected(Skill*)),this, SLOT(selection_mode_on(Skill*)));
                switch (circle_i)
                {
                case 0: cur_sector_ptr->base_circle[i] = cur_skill; break;
                case 1: cur_sector_ptr->circle_1[i] = cur_skill;    break;
                case 2: cur_sector_ptr->circle_2[i] = cur_skill;    break;
                case 3: cur_sector_ptr->circle_3[i] = cur_skill;    break;
                }

            }
        }
    }
    file.close();
}

void PageWidget::make_dependencies()
{
    for(temp_depends_struct_t skill_el : *temp_depends_struct)
    {
        for(int d = 0; d < skill_el.depends.size(); d++)
        {
            for(temp_depends_struct_t dependend_skill : *temp_depends_struct)
            {
                if(dependend_skill.skill->index == skill_el.depends[d])
                {
                    skill_el.skill->depends.append(dependend_skill.skill);
                    break;
                }
            }
        }
    }
}

//! Обработчик события перерисовки
void PageWidget::paintEvent(QPaintEvent *e)
{
    painter = new QPainter(this);

    int width = this->width();
    int height = this->height();
    centerX = width / 2;
    centerY = height / 2;

    paint_concentric_circles();
    paint_small_circles();
    paint_skills();

    delete painter;
    QWidget::paintEvent(e);
}

void PageWidget::resizeEvent(QResizeEvent *e)
{

    QWidget::resizeEvent(e);
}

//! Обработчик событий
bool PageWidget::eventFilter(QObject *watched, QEvent *event)
{
    // Если нажимаем мышкой в свободном месте
    if(event->type() == QEvent::MouseButtonPress)
    {
        selection_mode_off();
        return true;
    }
    return QWidget::eventFilter(watched, event);
}

//! Отрисовка основных кругов
void PageWidget::paint_concentric_circles()
{
    half_min_window_size = std::min(this->width(), this->height()) / 2;
    radius1 = static_cast<int>(half_min_window_size * RADUIS_KOEF);           // малый круг
    radius2 = static_cast<int>(radius1 + half_min_window_size * STEP_KOEF);   // средний круг
    radius3 = static_cast<int>(radius2 + half_min_window_size * STEP_KOEF);   // средний круг
    radius4 = static_cast<int>(radius3 + half_min_window_size * STEP_KOEF);   // большой круг
    radius5 = static_cast<int>(radius4 + half_min_window_size * STEP_KOEF / 2);   // расстояние до внешних кружочков
    radius_small_circles = static_cast<int>(half_min_window_size * SMALL_CIRCLE_KOEF);  // радиус внешних кружочков
    int radius[] = {radius2, radius3, radius4};

    painter->setPen(Qt::black);
    for(int r = 2; r >= 0; r--)
    {
        int current_angle = 0;
        for(int a = 0; a < 360/SEGMENT_ANGLE; a++)
        {
            painter->setBrush(QColor(segment_colors[r][a]));
            painter->drawPie(centerX - radius[r], centerY - radius[r], radius[r] * 2, radius[r] * 2,
                            current_angle * 16, SEGMENT_ANGLE * 16);
            current_angle += SEGMENT_ANGLE;
        }
    }
    painter->setBrush(Qt::white);
    painter->setPen(Qt::black);
    painter->drawEllipse(centerX - radius1, centerY - radius1, radius1 * 2, radius1 * 2);
}

//! Отрисовка внешних кружочков
void PageWidget::paint_small_circles()
{
    for(int s = 0; s < 360/SEGMENT_ANGLE; s++)
    {
        int hidden_segments_count = 5;
        int current_angle = s*SEGMENT_ANGLE + SEGMENT_ANGLE / hidden_segments_count + SEGMENT_ANGLE / hidden_segments_count / 2;
        for(int c = 0; c < 3; c++)
        {
            painter->setBrush(QColor(segment_colors[c][s]));
            int centerX_circles = centerX + static_cast<int>(radius5 * cos(current_angle * M_PI / 180));
            int centerY_circles = centerY - static_cast<int>(radius5 * sin(current_angle * M_PI / 180));
            painter->drawEllipse(centerX_circles - radius_small_circles, centerY_circles - radius_small_circles,
                                radius_small_circles * 2, radius_small_circles * 2);
            current_angle += SEGMENT_ANGLE / hidden_segments_count;
        }
    }
}

void PageWidget::paint_skills()
{
    int icon_radius_1 = static_cast<int>(radius1 + (radius2-radius1)/2);
    int icon_radius_2 = static_cast<int>(icon_radius_1 + half_min_window_size * STEP_KOEF);
    int icon_radius_3 = static_cast<int>(icon_radius_2 + half_min_window_size * STEP_KOEF);
    int icon_radius_4 = static_cast<int>(icon_radius_3 + half_min_window_size * STEP_KOEF);

    int icon_radiuses[] = {icon_radius_1, icon_radius_2, icon_radius_3, icon_radius_4};
    int cur_size = static_cast<int>(half_min_window_size * ICON_KOEF);

    int hidden_segments_count;

    for (int circle = 0; circle < NUM_OF_CIRCLES; ++circle)
    {
        if (circle == NUM_OF_CIRCLES - 1)
        {
            hidden_segments_count = 5;
        }
        else
        {
            hidden_segments_count = 3;
        }
        int skill_num = 0;
        int start_icon_offset = 9;
        int start_base_icon_offset = 5;
        for(int seg = 0; seg < 360/SEGMENT_ANGLE; seg++)
        {
            sector_data_t* cur_sector_ptr = sector_ptrs[seg];
            int current_angle;
            if (circle == NUM_OF_CIRCLES - 1)
            {
                current_angle = seg*SEGMENT_ANGLE + start_icon_offset;
            }
            else
            {
                current_angle = seg*SEGMENT_ANGLE + start_base_icon_offset;
            }
            for(int c = 0; c < 3; c++)
            {
                int x = centerX + static_cast<int>(icon_radiuses[circle] * cos(current_angle * M_PI / 180)) - cur_size / 2;
                int y = centerY - static_cast<int>(icon_radiuses[circle] * sin(current_angle * M_PI / 180)) - cur_size / 2;
                Skill* cur_skill;
                switch (circle)
                {
                case 0: cur_skill = cur_sector_ptr->circle_3[c];    break;
                case 1: cur_skill = cur_sector_ptr->circle_2[c];    break;
                case 2: cur_skill = cur_sector_ptr->circle_1[c];    break;
                default: cur_skill = cur_sector_ptr->base_circle[c];break;
                }
                if(cur_skill->is_changed_size == 0)
                {
                    cur_skill->move(x,y);
                    cur_skill->resize(cur_size, cur_size);
                    cur_skill->description->hide();
                }
                else
                {
                    cur_skill->resize(cur_size*2, cur_size*2);
                    cur_skill->move(x-cur_size / 2 , y - cur_size / 2);
                    cur_skill->description->show();
                    cur_skill->description->raise();
                }

                current_angle += SEGMENT_ANGLE / hidden_segments_count;
                skill_num++;
            }
        }
    }
}


//! Закрашивает все скиллы серыми, если они не относятся к выбранному скиллу
void PageWidget::selection_mode_on(Skill* selected_skill)
{
    select_dependencies(selected_skill);
    check_skills_availability();
    gray_unselected_skills();
}

void PageWidget::select_dependencies(Skill* selected_skill)
{
    selected_skill->state = SELECTED;
    select_base_skill(selected_skill);
    for(Skill* related_skill : selected_skill->depends)
    {
        related_skill->state = SELECTED;
        select_dependencies(related_skill);
    }
}

void PageWidget::select_base_skill(Skill* selected_skill)
{
    sector_data_t *sector = &circle_skills.magic_of_chaos;
    find_skill_in_struct(selected_skill, &sector);
//    qDebug() << sector->base_circle[0]->icon_path;
//    qDebug() << circle_i;
    if(!(sector->base_circle[2] == selected_skill))
    {
        int i = 2;
        while(sector->base_circle[i]->state == SELECTED)
        {
            i--;
            // Пока не отладим логику скрытия скиллов
            if(i < 0)
            {
                qDebug() << "Выход за границу базовых скиллов!!!";
                i++;
                break;
            }
        }
        sector->base_circle[i]->state = SELECTED;
        sector->base_circle[i]->repaint();
    }
}

void PageWidget::find_skill_in_struct(Skill *selected_skill, sector_data_t **ret_sector)
{
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        sector_data_t* cur_sector = sector_ptrs[sector_i];
        for(int circle_i = 0; circle_i < circle_names.size(); circle_i++)
        {
            Skill** cur_circle_ptr;
            switch (circle_i)
            {
            case 0: cur_circle_ptr = cur_sector->base_circle;   break;
            case 1: cur_circle_ptr = cur_sector->circle_1;      break;
            case 2: cur_circle_ptr = cur_sector->circle_2;      break;
            default: cur_circle_ptr = cur_sector->circle_3;     break;
            }
            for(int s = 0; s < 3; s++)
            {
                if(cur_circle_ptr[s] == selected_skill)
                {
                    *ret_sector = cur_sector;
                    return;
                }
            }
        }
    }
    qDebug() << "not found";
}

void PageWidget::check_skills_availability()
{
    qDebug() << "****imagine that I hid unaviable skills****";
//    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
//    {
//        sector_data_t* cur_sector_ptr = sector_ptrs[sector_i];
//        for(int circle_i = 0; circle_i < circle_names.size(); circle_i++)
//        {
//            Skill** cur_circle_ptr;
//            switch (circle_i)
//            {
//            case 0: cur_circle_ptr = cur_sector_ptr->base_circle;   break;
//            case 1: cur_circle_ptr = cur_sector_ptr->circle_1;      break;
//            case 2: cur_circle_ptr = cur_sector_ptr->circle_2;      break;
//            default: cur_circle_ptr = cur_sector_ptr->circle_3;     break;
//            }
//            for(int s = 0; s < 3; s++)
//            {

//            }
//        }
//    }
}

void PageWidget::gray_unselected_skills()
{
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        sector_data_t* cur_sector_ptr = sector_ptrs[sector_i];
        for(int circle_i = 0; circle_i < circle_names.size(); circle_i++)
        {
            Skill** cur_circle_ptr;
            switch (circle_i)
            {
            case 0: cur_circle_ptr = cur_sector_ptr->base_circle;   break;
            case 1: cur_circle_ptr = cur_sector_ptr->circle_1;      break;
            case 2: cur_circle_ptr = cur_sector_ptr->circle_2;      break;
            default: cur_circle_ptr = cur_sector_ptr->circle_3;     break;
            }
            for(int s = 0; s < 3; s++)
            {
                if(cur_circle_ptr[s]->state != SELECTED && !cur_circle_ptr[s]->isHidden())
                {
                    cur_circle_ptr[s]->state = UNSELECTED;
                }
                cur_circle_ptr[s]->repaint();
            }
        }
    }
}

////! Делает все скиллы цветными
void PageWidget::selection_mode_off()
{
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        sector_data_t* cur_sector_ptr = sector_ptrs[sector_i];
        for(int circle_i = 0; circle_i < circle_names.size(); circle_i++)
        {
            cur_sector_ptr->base_circle[0]->state = NONE;
            cur_sector_ptr->base_circle[1]->state = NONE;
            cur_sector_ptr->base_circle[2]->state = NONE;
            cur_sector_ptr->circle_1[0]->state = NONE;
            cur_sector_ptr->circle_1[1]->state = NONE;
            cur_sector_ptr->circle_1[2]->state = NONE;
            cur_sector_ptr->circle_2[0]->state = NONE;
            cur_sector_ptr->circle_2[1]->state = NONE;
            cur_sector_ptr->circle_2[2]->state = NONE;
            cur_sector_ptr->circle_3[0]->state = NONE;
            cur_sector_ptr->circle_3[1]->state = NONE;
            cur_sector_ptr->circle_3[2]->state = NONE;
            cur_sector_ptr->base_circle[0]->repaint();
            cur_sector_ptr->base_circle[1]->repaint();
            cur_sector_ptr->base_circle[2]->repaint();
            cur_sector_ptr->circle_1[0]->repaint();
            cur_sector_ptr->circle_1[1]->repaint();
            cur_sector_ptr->circle_1[2]->repaint();
            cur_sector_ptr->circle_2[0]->repaint();
            cur_sector_ptr->circle_2[1]->repaint();
            cur_sector_ptr->circle_2[2]->repaint();
            cur_sector_ptr->circle_3[0]->repaint();
            cur_sector_ptr->circle_3[1]->repaint();
            cur_sector_ptr->circle_3[2]->repaint();
        }
    }
}
