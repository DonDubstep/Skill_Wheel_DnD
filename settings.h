#ifndef SETTINGS_H
#define SETTINGS_H

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


#define RADUIS_KOEF 0.36f
#define STEP_KOEF 0.16f
#define SMALL_CIRCLE_KOEF 0.05f
#define SEGMENT_ANGLE 30
#define ICON_KOEF 0.08f

#define NUM_OF_CATEGORIES 4
#define NUM_OF_DEPENDS 30
#endif // SETTINGS_H
