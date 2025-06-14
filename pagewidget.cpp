#include "pagewidget.h"
#include <math.h>

#define RADUIS_KOEF 0.36f
#define STEP_KOEF 0.16f
#define SMALL_CIRCLE_KOEF 0.05f
#define SEGMENT_ANGLE 30
#define ICON_KOEF 0.08f 

PageWidget::PageWidget(QWidget *parent) : QWidget(parent)
{
    init_background_colors();
    read_json();
    init_skills();
}

void PageWidget::init_background_colors()
{
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

//! Здесь читаем json
void PageWidget::read_json()
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


void PageWidget::init_skills()
{
    for (int circle = 0; circle < icon_categories.length(); ++circle)
    {
        //! Сохряняем имя категории
        QString circle_name = icon_categories[circle];
        for(int s = 0; s < all_skills_data[circle_name].size(); s++)
        {
            QString icon_path = PIC_PATH + all_skills_data[circle_name][s].icon_path;
            QString title = all_skills_data[circle_name][s].title;
            QString description = all_skills_data[circle_name][s].description;

            Skill *cur_skill = new Skill(this, icon_path, title, description);
            all_skills_data[circle_name][s].skill = cur_skill;
        }
    }
}

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

//! Отрисовка скиллов
void PageWidget::paint_skills()
{
    int icon_radius_1 = static_cast<int>(radius1 + (radius2-radius1)/2);
    int icon_radius_2 = static_cast<int>(icon_radius_1 + half_min_window_size * STEP_KOEF);
    int icon_radius_3 = static_cast<int>(icon_radius_2 + half_min_window_size * STEP_KOEF);
    int icon_radius_4 = static_cast<int>(icon_radius_3 + half_min_window_size * STEP_KOEF);

    int icon_radiuses[] = {icon_radius_1, icon_radius_2, icon_radius_3, icon_radius_4};
    int cur_size = static_cast<int>(half_min_window_size * ICON_KOEF);

    int hidden_segments_count;

    for (int circle = 0; circle < icon_categories.length(); ++circle)
    {
        if (circle == icon_categories.length() - 1)
        {
            hidden_segments_count = 5;
        }
        else
        {
            hidden_segments_count = 3;
        }
        QString circle_name = icon_categories[circle];
        int skill_num = 0;
        int start_icon_offset = 9;
        int start_base_icon_offset = 5;
        for(int seg = 0; seg < 360/SEGMENT_ANGLE; seg++)
        {
            int current_angle;
            if (circle == icon_categories.length() - 1)
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
                if(!all_skills_data[circle_name][skill_num].skill->is_changed_size)
                {
                    all_skills_data[circle_name][skill_num].skill->resize(cur_size, cur_size);
                    all_skills_data[circle_name][skill_num].skill->move(x,y);
                }
                if(all_skills_data[circle_name][skill_num].skill->is_changed_size == 1)
                {
                    all_skills_data[circle_name][skill_num].skill->is_changed_size = 0;
                }
                current_angle += SEGMENT_ANGLE / hidden_segments_count;
                skill_num++;
            }
        }
    }
}
