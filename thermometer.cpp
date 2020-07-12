#include "thermometer.h"
#include <QDebug>

class ThermometerPrivate
{
    Q_DECLARE_PUBLIC(Thermometer)
    ThermometerPrivate(Thermometer *parent) :
        q_ptr(parent),
        animation(new QSequentialAnimationGroup(parent)),
        minValue(-30),
        maxValue(70),
        value(minValue),
        s_type(Thermometer::ScaleType::OneS),
        easing(QEasingCurve::OutQuad)
    {

    }

    Thermometer *q_ptr;
    QSequentialAnimationGroup *animation;
    qreal minValue;
    qreal maxValue;
    qreal value;
    qreal cvalue;
    Thermometer::ScaleType s_type;
    QEasingCurve easing;
};

Thermometer::Thermometer(QWidget *parent) :
    QWidget(parent),
    d_ptr(new ThermometerPrivate(this))
{
    Q_D(Thermometer);

    this->setMinimumWidth(100);

    // 往动画
    QPropertyAnimation *tAnimation = new QPropertyAnimation(this, "value", this);
    // 反动画
    QPropertyAnimation *sAnimation = new QPropertyAnimation(this, "value", this);

    d->animation->addAnimation(tAnimation);
    d->animation->addAnimation(sAnimation);

    connect(d->animation, &QSequentialAnimationGroup::finished, this, &Thermometer::finished);
}

qreal Thermometer::value()
{
    Q_D(Thermometer);

    return  d->value;
}

void Thermometer::setValue(const qreal svalue)
{
    Q_D(Thermometer);

    d->value = svalue;
    this->update();
}

void Thermometer::setMinValue(const int sminValue)
{
    Q_D(Thermometer);

    d->minValue = sminValue;
}

void Thermometer::setMaxValue(const int maxValue)
{
    Q_D(Thermometer);

    d->maxValue = maxValue;
}

void Thermometer::setScaleType(const Thermometer::ScaleType stype)
{
    Q_D(Thermometer);

    d->s_type = stype;
}

void Thermometer::setThermometer(const qreal c_ther)
{
    Q_D(Thermometer);

    d->cvalue = c_ther;
}

void Thermometer::setEasing(QEasingCurve aEasing)
{
    Q_D(Thermometer);

    d->easing = aEasing;
}

void Thermometer::start()
{
    Q_D(Thermometer);

    if (d->animation->state() == QSequentialAnimationGroup::Running)
        d->animation->stop();

    QPropertyAnimation *tAnimation = qobject_cast<QPropertyAnimation *>(d->animation->animationAt(0));
    QPropertyAnimation *sAnimation = qobject_cast<QPropertyAnimation *>(d->animation->animationAt(1));
    qreal randNum = generateRandomNum(d->cvalue, d->maxValue);

    tAnimation->setStartValue(d->minValue);
    tAnimation->setEndValue(randNum);
    tAnimation->setDuration(2000);
    tAnimation->setEasingCurve(d->easing);
    sAnimation->setStartValue(randNum);
    sAnimation->setEndValue(d->cvalue);
    sAnimation->setDuration(2000);
    sAnimation->setEasingCurve(d->easing);
    d->animation->start();
}

void Thermometer::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    Q_D(Thermometer);

    QColor tColor;
    QPainter mPainter(this);

    if (d->value >= 37.3 || d->value < 36.0) {
        // 温度异常
        tColor = Qt::red;
    } else
        tColor = Qt::green;

    mPainter.setRenderHints(QPainter::Antialiasing);
    mPainter.setPen(Qt::NoPen);
    mPainter.setBrush(tColor);
    // 默认设置温度计低端圆的半径为10
    mPainter.drawEllipse((this->width() - 20) / 2, this->height() - 20, 20, 20);

    // 默认圆上端预留矩形长度为25  6*25  默认在圆下两个单位开始画
    mPainter.drawRect((this->width() - 6) / 2, this->height()-43, 6, 25);

    // 开始画刻度和刻度标签
    int duration = static_cast<int>((d->maxValue-d->minValue) + 1);
    // 每10度作为一个大刻度
    int pCount = duration / 10;
    // 计算每个大刻度需要多少长度的位置
    qreal pHeight = (this->height() - 43 - 43) / pCount;  // 由于底部预留区域为20+23 顶部预留区域20+23
    // 计算每个小刻度需要多长距离
    qreal sHeight = pHeight / (10 / d->s_type);
    // 记录需要迭代的大刻度y值
    qreal pY = this->height() - 43;
    // 记录需要迭代的小刻度y值
    qreal sY = this->height() - 43;

    for (int i = 0; i < duration; ++i, sY -= sHeight) {
        if (!(i % 10)) {
            // 大刻度 画大一点的线 和添加文字
            QPen lPen(Qt::darkCyan);
            lPen.setWidth(2);

            mPainter.setBrush(Qt::NoBrush);
            mPainter.setPen(lPen);
            // 画左边刻度
            mPainter.drawLine(QPointF((this->width() - 6) / 2 - 3, pY), QPointF((this->width() - 6) / 2 - 6, pY)); // 画3单位的线
            // 画右边刻度
            mPainter.drawLine(QPointF((this->width() + 6) / 2 + 3, pY), QPointF((this->width() + 6) / 2 + 6, pY));
            // 画左边刻度文字
            QFontMetricsF metrif(this->font());
            qreal fontW = metrif.width(QString::number(d->minValue+i));
            mPainter.drawText(QRectF(QPointF((this->width() - 6) / 2 - 10 - fontW, pY-metrif.height()/2), QSize(fontW, metrif.height())), Qt::AlignCenter, QString::number(d->minValue+i));
            pY -= pHeight;
            continue;
        }

        // 画左边小刻度
        QPen sPen(Qt::darkCyan);
        sPen.setWidth(1);
        mPainter.setPen(sPen);
        mPainter.setBrush(Qt::NoBrush);
        mPainter.drawLine(QPointF((this->width() - 6) / 2 - 3, sY), QPointF((this->width() - 6) / 2 - 5, sY));
        // 画右边小刻度
        mPainter.drawLine(QPointF((this->width() + 6) / 2 + 3, sY), QPointF((this->width() + 6) / 2 + 5, sY));
    }

    // 画原始长度颜色
    mPainter.fillRect(QRectF(QPointF((this->width() - 6) / 2, 20), QSizeF(6, this->height() - 43 -20)), Qt::darkCyan);

    // 画温度
    // 计算需要画的长度, 由于pY最后一次迭代过因此需要加一次
    qreal tHeight = ((this->height() - 43) - (pY + pHeight)) * ((d->value - d->minValue) / (d->maxValue-d->minValue));
    mPainter.fillRect(QRectF(QPointF((this->width() - 6) / 2, (this->height() - 43) - tHeight), QSizeF(6, tHeight)), tColor);;

    // 画℃的标志
    QFont mFont = this->font();

    mFont.setBold(true);
    mFont.setWeight(16);
    mPainter.setPen(Qt::darkCyan);
    mPainter.setFont(mFont);
    mPainter.drawText((this->width() - 6) / 2 - 20, 20, "℃");
}

qreal Thermometer::generateRandomNum(qreal min, qreal max)
{
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    if (max - min < 2)
        return max;

    qreal diff = max - min - 2;
    qreal m1 = static_cast<qreal>(qrand() % 100) / 100;
    qreal retval = min + 2 + m1 * diff;

    return retval;
}
