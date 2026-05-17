#include "headerwidget.h"
#include <QCoreApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QRadialGradient>

//! Цветовая палитра хэдера в стиле HoMM V
namespace HoMM {
    static const QColor bgDark    { 60,  45,  25 };
    static const QColor bgMid     { 85,  62,  32 };
    static const QColor bgLight   {115,  85,  45 };
    static const QColor goldDark  {130,  90,  20 };
    static const QColor goldMid   {185, 145,  55 };
    static const QColor goldBright{230, 190,  85 };
    static const QColor goldShine {255, 235, 145 };
    static const QColor textGold  {255, 220, 110 };

    static const QString COMBOBOX_STYLE = R"(
QComboBox {
    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
        stop:0 #3c2d14, stop:0.5 #55401e, stop:1 #3c2d14);
    color: #ffdc6e;
    border: 1px solid #b98e30;
    border-radius: 2px;
    padding-left: 6px;
    font-family: "Palatino Linotype", "Book Antiqua", serif;
    font-weight: bold;
    selection-background-color: #7a5a1a;
}
QComboBox:hover {
    border: 1px solid #ffe99a;
    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
        stop:0 #4a3618, stop:0.5 #664e24, stop:1 #4a3618);
}
QComboBox:focus {
    border: 1px solid #ffe99a;
    outline: none;
}
QComboBox::drop-down {
    subcontrol-origin: padding;
    subcontrol-position: top right;
    width: 20px;
    border-left: 1px solid #b98e30;
    background-color: transparent;
}
QComboBox::down-arrow {
    width: 0;
    height: 0;
    border-left:  5px solid transparent;
    border-right: 5px solid transparent;
    border-top:   6px solid #ffe082;
}
QComboBox QAbstractItemView {
    background-color: #3c2d14;
    color: #ffdc6e;
    border: 1px solid #b98e30;
    selection-background-color: #664e24;
    selection-color: #ffe99a;
    font-family: "Palatino Linotype", "Book Antiqua", serif;
    font-weight: bold;
    outline: none;
}
QComboBox QAbstractItemView::item {
    padding: 4px 8px;
    min-height: 22px;
}
QComboBox QAbstractItemView::item:hover {
    background-color: #664e24;
    color: #ffe99a;
}
)";
}

HeaderWidget::HeaderWidget(QWidget *parent) : QWidget(parent)
{
    installEventFilter(this);
    header_selection = new HeaderSelection(&basic_skills);
    connect(header_selection, SIGNAL(set_header_scores(int, int)), this, SLOT(set_header_scores(int, int)));
    read_json();
    add_combobox();
    add_basic_skills(0);
    add_scores(0);
    this->current_page = 0;
    for(int page_i = 0; page_i < pages.size(); page_i++)
    {
        scores_page[page_i] = 0;
        scores_header[page_i] = 0;
    }
}

void HeaderWidget::read_json()
{
    QString icon_path, title, description;
    int index;
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
            index = cur_obj["index"].toInt();
            Skill* cur_skill = new Skill(this, icon_path, title, description);
            cur_skill->index = index;
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
        if(name == pages[i])
            return 1;
    }
    return 0;
}

void HeaderWidget::combobox_changed(int page_num)
{
    remove_basic_skills();
    add_basic_skills(page_num);
    this->current_page = page_num;
    paint_scores();
    header_selection->set_cur_page(page_num);
    emit switch_page(page_num);
}

void HeaderWidget::remove_basic_skills()
{
    for(int i = 0; i < BASIC_SKILL_NUM; i++)
        cur_class_skills[i]->hide();
}

#define LEFT_PADDING_K            0.01875
#define TOP_PADDING_K             0.234375
#define TOP_PADDING_COUNTER_K     0.140625
#define COMBO_WIDTH_K             0.25
#define COMBO_HEIGHT_K            0.53125
#define ICON_DIAMETER_K           0.5
#define ICON_PADDING_LEFT_K       0.33125
#define ICON_MARGIN_BETWEEN_K     0.02
#define LABEL_PADDING_OF_SKILLS_K 0.03125
#define FONT_SIZE_K               0.37


void HeaderWidget::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    const QRect r = rect();

    // 1. Фон
    QLinearGradient bgGrad(0, 0, 0, r.height());
    bgGrad.setColorAt(0.00, HoMM::bgDark);
    bgGrad.setColorAt(0.20, HoMM::bgMid);
    bgGrad.setColorAt(0.50, HoMM::bgLight);
    bgGrad.setColorAt(0.80, HoMM::bgMid);
    bgGrad.setColorAt(1.00, HoMM::bgDark);
    p.fillRect(r, bgGrad);

    // Золотистое свечение по центру
    QRadialGradient glow(r.center(), r.width() * 0.45);
    glow.setColorAt(0.0, QColor(80, 55, 10, 45));
    glow.setColorAt(1.0, QColor(0,   0,  0,  0));
    p.fillRect(r, glow);

    // 2. Внешняя рамка
    {
        QRectF outer = QRectF(r).adjusted(2, 2, -2, -2);
        p.setPen(QPen(QColor(0, 0, 0, 100), 1.5));
        p.drawRect(outer.adjusted(1, 1, 1, 1));

        QLinearGradient og(outer.left(), 0, outer.right(), 0);
        og.setColorAt(0.00, HoMM::goldDark);
        og.setColorAt(0.20, HoMM::goldBright);
        og.setColorAt(0.50, HoMM::goldShine);
        og.setColorAt(0.80, HoMM::goldBright);
        og.setColorAt(1.00, HoMM::goldDark);
        p.setPen(QPen(QBrush(og), 1.5));
        p.drawRect(outer);
    }

    // 3. Внутренняя рамка
    {
        QRectF inner = QRectF(r).adjusted(6, 6, -6, -6);
        QLinearGradient ig(inner.left(), 0, inner.right(), 0);
        ig.setColorAt(0.00, QColor(HoMM::goldDark.red(), HoMM::goldDark.green(), HoMM::goldDark.blue(), 0));
        ig.setColorAt(0.20, HoMM::goldMid);
        ig.setColorAt(0.50, HoMM::goldBright);
        ig.setColorAt(0.80, HoMM::goldMid);
        ig.setColorAt(1.00, QColor(HoMM::goldDark.red(), HoMM::goldDark.green(), HoMM::goldDark.blue(), 0));
        p.setPen(QPen(QBrush(ig), 1.0));
        p.drawRect(inner);
    }

    // 4. Горизонтальные разделители
    auto drawDivider = [&](int y) {
        const int x1 = r.left()  + 18;
        const int x2 = r.right() - 18;
        QLinearGradient dg(x1, y, x2, y);
        dg.setColorAt(0.00, QColor(HoMM::goldDark.red(), HoMM::goldDark.green(), HoMM::goldDark.blue(), 0));
        dg.setColorAt(0.15, HoMM::goldMid);
        dg.setColorAt(0.50, HoMM::goldShine);
        dg.setColorAt(0.85, HoMM::goldMid);
        dg.setColorAt(1.00, QColor(HoMM::goldDark.red(), HoMM::goldDark.green(), HoMM::goldDark.blue(), 0));
        p.setPen(QPen(QBrush(dg), 1.0));
        p.drawLine(x1, y, x2, y);
    };
    drawDivider(r.top()    + 11);
    drawDivider(r.bottom() - 11);

    // 5. Угловые ромбы с засечками
    auto drawDiamond = [&](QPointF c, qreal sz) {
        QPolygonF d;
        d << QPointF(c.x(),     c.y()-sz)
          << QPointF(c.x()+sz,  c.y()   )
          << QPointF(c.x(),     c.y()+sz)
          << QPointF(c.x()-sz,  c.y()   );

        QRadialGradient rg(c, sz);
        rg.setColorAt(0.0, HoMM::goldShine);
        rg.setColorAt(0.5, HoMM::goldBright);
        rg.setColorAt(1.0, HoMM::goldDark);

        p.save();
        p.setBrush(QBrush(rg));
        p.setPen(QPen(HoMM::goldDark, 0.7));
        p.drawPolygon(d);

        qreal in = sz * 0.38;
        QPolygonF di;
        di << QPointF(c.x(),    c.y()-in)
           << QPointF(c.x()+in, c.y()   )
           << QPointF(c.x(),    c.y()+in)
           << QPointF(c.x()-in, c.y()   );
        p.setBrush(HoMM::bgDark);
        p.setPen(Qt::NoPen);
        p.drawPolygon(di);
        p.restore();
    };

    auto drawCorner = [&](QPointF c) {
        const qreal sz = 4.5;
        drawDiamond(c, sz);
        qreal hd = (c.x() < r.center().x()) ? 1.0 : -1.0;
        qreal vd = (c.y() < r.center().y()) ? 1.0 : -1.0;
        p.setPen(QPen(HoMM::goldMid, 1.0));
        p.drawLine(QPointF(c.x() + hd*(sz+2), c.y()), QPointF(c.x() + hd*(sz+12), c.y()));
        p.drawLine(QPointF(c.x(), c.y() + vd*(sz+2)), QPointF(c.x(), c.y() + vd*(sz+12)));
    };

    const int off = 9;
    drawCorner({ qreal(r.left()  + off), qreal(r.top()    + off) });
    drawCorner({ qreal(r.right() - off), qreal(r.top()    + off) });
    drawCorner({ qreal(r.left()  + off), qreal(r.bottom() - off) });
    drawCorner({ qreal(r.right() - off), qreal(r.bottom() - off) });

    // 6. Золотые круглые рамки вокруг иконок навыков
    {
        int icon_size = static_cast<int>(this->height() * ICON_DIAMETER_K);
        int icon_y    = static_cast<int>(this->height() * TOP_PADDING_K);
        qreal pen_w   = qMax(1.5, icon_size * 0.045); // толщина ~4.5% от размера иконки

        for(int i = 0; i < BASIC_SKILL_NUM; i++)
        {
            int icon_x = static_cast<int>(this->width() * ICON_PADDING_LEFT_K
                         + i * (this->height() * ICON_DIAMETER_K
                         + this->width() * ICON_MARGIN_BETWEEN_K));

            QRectF iconRect(icon_x, icon_y, icon_size, icon_size);

            // Тонкая тёмная тень чуть смещённая — даёт объём
            p.setPen(QPen(QColor(0, 0, 0, 120), pen_w + 1.5));
            p.setBrush(Qt::NoBrush);
            p.drawEllipse(iconRect.adjusted(1, 1, 1, 1));

            // Основная золотая обводка с градиентом
            QLinearGradient ringGrad(iconRect.topLeft(), iconRect.bottomRight());
            ringGrad.setColorAt(0.00, HoMM::goldShine);
            ringGrad.setColorAt(0.35, HoMM::goldBright);
            ringGrad.setColorAt(0.65, HoMM::goldMid);
            ringGrad.setColorAt(1.00, HoMM::goldDark);
            p.setPen(QPen(QBrush(ringGrad), pen_w));
            p.drawEllipse(iconRect);

            // Тонкий внутренний контур — добавляет двойную рамку
            QLinearGradient innerRing(iconRect.topLeft(), iconRect.bottomRight());
            innerRing.setColorAt(0.0, HoMM::goldMid);
            innerRing.setColorAt(1.0, HoMM::goldDark);
            qreal inset = pen_w + 1.0;
            p.setPen(QPen(QBrush(innerRing), 0.8));
            p.drawEllipse(iconRect.adjusted(inset, inset, -inset, -inset));
        }
    }

    QWidget::paintEvent(e);
}

void HeaderWidget::resizeEvent(QResizeEvent *e)
{
    paint_combobox();
    paint_basic_skills();
    paint_scores();
    QWidget::resizeEvent(e);
}

void HeaderWidget::set_scores_page(int score, int page_index)
{
    scores_page[page_index] = score;
    if(scores_page[page_index] == 0)
        emit set_page_skills_selected_0_in_header_selection();
    paint_scores();
}

void HeaderWidget::set_header_scores(int score, int page_index)
{
    scores_header[page_index] = score;
    if(page_index == current_page)
        paint_scores();
}

void HeaderWidget::null_scores(int page_index)
{
    scores_header[page_index] = 0;
    scores_page[page_index] = 0;
    if(page_index == current_page)
        paint_scores();
}

void HeaderWidget::add_combobox()
{
    combo_pages = new QComboBox(this);
    combo_pages->addItems(pages);
    combo_pages->setStyleSheet(HoMM::COMBOBOX_STYLE);
    connect(combo_pages, SIGNAL(currentIndexChanged(int)), this, SLOT(combobox_changed(int)));
    header_selection->set_cur_page(0);
    paint_combobox();
}

void HeaderWidget::add_basic_skills(int page_num)
{
    QString class_name = pages[page_num];
    for(int i = 0; i < basic_skills[class_name].size(); i++)
        cur_class_skills[i] = basic_skills[class_name][i];
    paint_basic_skills();
}

void HeaderWidget::add_scores(int score)
{
    scores = new QLabel(this);
    scores->setText(QString::number(score));
    scores->setAttribute(Qt::WA_TranslucentBackground);
    scores->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    paint_scores();
}

void HeaderWidget::paint_combobox()
{
    int x = static_cast<int>(this->width()  * LEFT_PADDING_K);
    int y = static_cast<int>(this->height() * TOP_PADDING_K);
    int w = static_cast<int>(this->width()  * COMBO_WIDTH_K);
    int h = static_cast<int>(this->height() * COMBO_HEIGHT_K);
    combo_pages->setGeometry(x, y, w, h);

    // Масштабируем шрифт вместе с виджетом
    QFont f("Palatino Linotype", qMax(8, h / 2));
    f.setBold(true);
    combo_pages->setFont(f);
}

void HeaderWidget::paint_basic_skills()
{
    int icon_size = static_cast<int>(this->height() * ICON_DIAMETER_K);
    for(int i = 0; i < BASIC_SKILL_NUM; i++)
    {
        int x = static_cast<int>(this->width()  * ICON_PADDING_LEFT_K
                + i * (this->height() * ICON_DIAMETER_K + this->width() * ICON_MARGIN_BETWEEN_K));
        int y = static_cast<int>(this->height() * TOP_PADDING_K);
        cur_class_skills[i]->move(x, y);
        cur_class_skills[i]->resize(icon_size, icon_size);
        cur_class_skills[i]->show();
    }
}

void HeaderWidget::paint_scores()
{
    int font_size = static_cast<int>(this->height() * FONT_SIZE_K);

    QFont font("Palatino Linotype", font_size);
    font.setBold(true);
    font.setItalic(true);
    scores->setFont(font);

    // Золотой цвет через палитру
    QPalette pal = scores->palette();
    pal.setColor(QPalette::WindowText, HoMM::textGold);
    scores->setPalette(pal);

    double x_end = this->width() * ICON_PADDING_LEFT_K
                   + BASIC_SKILL_NUM * (this->height() * ICON_DIAMETER_K
                   + this->width() * ICON_MARGIN_BETWEEN_K);
    int x = static_cast<int>(x_end + this->width() * LABEL_PADDING_OF_SKILLS_K);
    int y = static_cast<int>(this->height() * TOP_PADDING_COUNTER_K);
    int total = scores_page[current_page] + scores_header[current_page];
    scores->setText(QString::number(total));

    // Ширина лейбла считается точно по шрифту — число не обрежется
    // Запас: ширина самого числа + ещё три цифры на рост при ресайзе
    QFontMetrics fm(font);
    int text_w = fm.horizontalAdvance(QString::number(total)) + fm.horizontalAdvance("000");
    int text_h = fm.height();
    scores->move(x, y);
    scores->resize(text_w, text_h);
}
