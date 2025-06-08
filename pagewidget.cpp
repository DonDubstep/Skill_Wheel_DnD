#include "pagewidget.h"

PageWidget::PageWidget(QWidget *parent) : QWidget(parent)
{
    segment_colors = new QString*[3];
    segment_colors[0] = new QString[360/segment_angle];
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

    segment_colors[1] = new QString[360/segment_angle];
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

    segment_colors[2] = new QString[360/segment_angle];
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

#include <math.h>
#include <QDebug>
void PageWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    int width = this->width();
    int height = this->height();
    int centerX = width / 2;
    int centerY = height / 2;

    float half_min_window_size = std::min(width,height)/2;
    float radius_koef = 0.36f;
    float step_koef = 0.16f;
    float small_circle_koef = 0.05f;
    int radius1 = static_cast<int>(half_min_window_size * radius_koef);           // малый круг
    int radius2 = static_cast<int>(radius1 + half_min_window_size * step_koef);   // средний круг
    int radius3 = static_cast<int>(radius2 + half_min_window_size * step_koef);   // средний круг
    int radius4 = static_cast<int>(radius3 + half_min_window_size * step_koef);   // большой круг
    int radius5 = static_cast<int>(radius4 + half_min_window_size * step_koef / 2);   // расстояние до внешних кружочков
    int radius_small_circles = static_cast<int>(half_min_window_size * small_circle_koef);  // радиус внешних кружочков
    int radius[] = {radius2, radius3, radius4};
    qDebug() << "width: " << width;
    qDebug() << "height: " << height;
    qDebug() << "radiuses";
    qDebug() << radius2;
    qDebug() << radius3;
    qDebug() << radius4;
    qDebug() << radius5;
    qDebug() << "circle radius: " << radius_small_circles;

    painter.setPen(Qt::black);
    for(int r = 2; r >= 0; r--)
    {
        int current_angle = 0;
        for(int a = 0; a < 360/segment_angle; a++)
        {
            painter.setBrush(QColor(segment_colors[r][a]));
            painter.drawPie(centerX - radius[r], centerY - radius[r], radius[r] * 2, radius[r] * 2,
                            current_angle * 16, segment_angle * 16);
            current_angle += segment_angle;
        }
    }
    painter.setBrush(Qt::white);
    painter.drawEllipse(centerX - radius1, centerY - radius1, radius1 * 2, radius1 * 2);

//    painter.drawLine(centerX, centerY, centerX, centerY - radius5);
//    painter.drawLine(centerX, centerY, centerX + radius5, centerY);
//    painter.drawLine(centerX, centerY, centerX + static_cast<int>(radius5 * cos(9 * M_PI / 180)), centerY - static_cast<int>(radius5 * sin(9 * M_PI / 180)));
    for(int s = 0; s < 360/segment_angle; s++)
    {
        int hidden_segments_count = 5;
        int current_angle = s*30 + segment_angle / hidden_segments_count + segment_angle / hidden_segments_count / 2;
        for(int c = 0; c < 3; c++)
        {
            painter.setBrush(QColor(segment_colors[c][s]));
            int centerX_circles = centerX + static_cast<int>(radius5 * cos(current_angle * M_PI / 180));
            int centerY_circles = centerY - static_cast<int>(radius5 * sin(current_angle * M_PI / 180));
            painter.drawEllipse(centerX_circles - radius_small_circles, centerY_circles - radius_small_circles,
                                radius_small_circles * 2, radius_small_circles * 2);
            current_angle += segment_angle / hidden_segments_count;
        }
    }
}
