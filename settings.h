#ifndef SETTINGS_H
#define SETTINGS_H
#include <QStringList>

class Skill;

enum icon_sizes
{
    BASE_SIZE = 32,         //! Размер иконки
    INCREACE_KOEF = 2       //! Коэффициент увеличения при наведении
};

enum wheel_size
{
    WHEEL_WIDTH = 799,      //! Ширина колеса навыков
    WHEEL_HEIGHT = 799      //! Высота колеса навыков
};

enum sizes
{
    MINIMUM_FRAME_WIDTH = 300,  //! Минимальная ширина окна описания
};

enum pages
{
    NUM_OF_PAGES = 8
};

enum skill_state_e
{
    NONE,
    SELECTED,
    UNSELECTED
};

const QStringList json_pages
{
    "Mage",
    "Demon_Lord",
    "Knight",
    "Necromancer",
    "Ranger",
    "Rune_Priest",
    "Warlock",
    "Barbarian"
};

const QStringList sector_names
{
    "Magic_of_chaos",
    "Magic_of_darkness",
    "Magic_of_light",
    "Machine_management",
    "Education",
    "Logistics",
    "Leadership",
    "Luck",
    "Protection",
    "Attack",
    "Sorcery",
    "Summoning_magic",
};

const QStringList circle_names
{
    "Base_circle",
    "Circle_1",
    "Circle_2",
    "Circle_3"
};


const QStringList pages
{
    "Маг",
    "П. Демонов",
    "Рыцарь",
    "Некромант",
    "Рейнджер",
    "Рунный жрец",
    "Чернокнижник",
    "Варвар"
};

struct sector_data_t
{
    Skill* base_circle[3];
    Skill* circle_1[3];
    Skill* circle_2[3];
    Skill* circle_3[3];
};

struct circle_skill_data_t
{
    sector_data_t magic_of_chaos;
    sector_data_t magic_of_darkness;
    sector_data_t magic_of_light;
    sector_data_t machine_management;
    sector_data_t education;
    sector_data_t logistics;
    sector_data_t leadership;
    sector_data_t luck;
    sector_data_t protection;
    sector_data_t attack;
    sector_data_t sorcery;
    sector_data_t summoning_magic;
};


#define RADUIS_KOEF 0.36f
#define STEP_KOEF 0.16f
#define SMALL_CIRCLE_KOEF 0.05f
#define SEGMENT_ANGLE 30
#define ICON_KOEF 0.08f

#define NUM_OF_CATEGORIES 4
#define NUM_OF_DEPENDS 30
#define NUM_OF_CIRCLES 4
#endif // SETTINGS_H
