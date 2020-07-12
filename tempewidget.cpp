#include "tempewidget.h"
#include <QDebug>

class TempeWidgetPrivate
{
    Q_DECLARE_PUBLIC(TempeWidget)

    TempeWidgetPrivate(TempeWidget *parent) :
        q_ptr(parent),
        sGroup(new QSequentialAnimationGroup(parent)),
        style(TempeWidget::TempeStyle::Red),
        aColor(QColor(255, 0, 0, 0)),
        ther(new Thermometer(parent))
    {

    }

    ~TempeWidgetPrivate(){}

    TempeWidget *q_ptr;
    QSequentialAnimationGroup *sGroup;
    QLinearGradient leftLinear;
    QLinearGradient rightLinear;
    TempeWidget::TempeStyle style;
    QColor aColor;
    qreal thermometer;
    Thermometer *ther;
};

TempeWidget::TempeWidget(QWidget *parent) :
    QLabel(parent),
    d_ptr(new TempeWidgetPrivate(this))
{
    Q_D(TempeWidget);

    QPropertyAnimation *lightColorAnimation = new QPropertyAnimation(this, "aColor");
    QPropertyAnimation *darkColorAnimation = new QPropertyAnimation(this, "aColor");

    lightColorAnimation->setDuration(2000);
    darkColorAnimation->setDuration(2000);

    if (d->style == TempeStyle::Green) {
        lightColorAnimation->setStartValue(QVariant::fromValue<QColor>(QColor(0, 255, 0, 0)));
        lightColorAnimation->setEndValue(QVariant::fromValue<QColor>(QColor(0, 255, 0, 120)));
        darkColorAnimation->setStartValue(QVariant::fromValue<QColor>(QColor(0, 255, 0, 120)));
        darkColorAnimation->setEndValue(QVariant::fromValue<QColor>(QColor(0, 255, 0, 0)));
    } else if (d->style == TempeStyle::Red) {
        lightColorAnimation->setStartValue(QVariant::fromValue<QColor>(QColor(255, 0, 0, 0)));
        lightColorAnimation->setEndValue(QVariant::fromValue<QColor>(QColor(255, 0, 0, 120)));
        darkColorAnimation->setStartValue(QVariant::fromValue<QColor>(QColor(255, 0, 0, 120)));
        darkColorAnimation->setEndValue(QVariant::fromValue<QColor>(QColor(255, 0, 0, 0)));
    }

    d->sGroup->addAnimation(lightColorAnimation);
    d->sGroup->addAnimation(darkColorAnimation);

    connect(d->sGroup, &QSequentialAnimationGroup::finished, [ = ](){
        d->sGroup->start();
    });

    QHBoxLayout *layout = new QHBoxLayout(this);

    layout->addWidget(d->ther);
    layout->addStretch();
    connect(d->ther, &Thermometer::finished, [ = ](){
        if (d->thermometer > 36 && d->thermometer < 37.3)
            setTempeStyle(TempeStyle::Green);
        else
            setTempeStyle(TempeStyle::Red);

        d->sGroup->start();
    });
}

QColor TempeWidget::getAColor()
{
    Q_D(TempeWidget);

    return d->aColor;
}

void TempeWidget::setAColor(const QColor &color)
{
    Q_D(TempeWidget);

    d->aColor = color;
    emit aColorChange(color);
    this->update();
}

void TempeWidget::setThermometer(const qreal ther)
{
    Q_D(TempeWidget);

    if (d->sGroup->state() == QSequentialAnimationGroup::Running) {
        d->sGroup->stop();
        d->aColor = Qt::transparent;
        this->update();
    }

    d->thermometer = ther;
    d->ther->setThermometer(ther);
    d->ther->start();
}

void TempeWidget::setTempeStyle(const TempeWidget::TempeStyle &tempS)
{
    Q_D(TempeWidget);

    d->style = tempS;

    QPropertyAnimation *lightColorAnimation = qobject_cast<QPropertyAnimation *>(d->sGroup->animationAt(0));
    QPropertyAnimation *darkColorAnimation = qobject_cast<QPropertyAnimation *>(d->sGroup->animationAt(1));

    if (d->style == TempeStyle::Green) {
        lightColorAnimation->setStartValue(QVariant::fromValue<QColor>(QColor(0, 255, 0, 0)));
        lightColorAnimation->setEndValue(QVariant::fromValue<QColor>(QColor(0, 255, 0, 120)));
        darkColorAnimation->setStartValue(QVariant::fromValue<QColor>(QColor(0, 255, 0, 120)));
        darkColorAnimation->setEndValue(QVariant::fromValue<QColor>(QColor(0, 255, 0, 0)));
    } else if (d->style == TempeStyle::Red) {
        lightColorAnimation->setStartValue(QVariant::fromValue<QColor>(QColor(255, 0, 0, 0)));
        lightColorAnimation->setEndValue(QVariant::fromValue<QColor>(QColor(255, 0, 0, 120)));
        darkColorAnimation->setStartValue(QVariant::fromValue<QColor>(QColor(255, 0, 0, 120)));
        darkColorAnimation->setEndValue(QVariant::fromValue<QColor>(QColor(255, 0, 0, 0)));
    }
}

void TempeWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    Q_D(TempeWidget);

    QPainter mPainter(this);
    mPainter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    mPainter.setPen(Qt::NoPen);

    if (d->style == TempeStyle::Red) {
        d->leftLinear.setColorAt(1, QColor(255, 0, 0, 0));
        d->rightLinear.setColorAt(0, QColor(255, 0, 0, 0));
    } else if (d->style == TempeStyle::Green) {
        d->leftLinear.setColorAt(1, QColor(0, 255, 0, 0));
        d->rightLinear.setColorAt(0, QColor(0, 255, 0, 0));
    }

    d->leftLinear.setStart(0, 0);
    d->leftLinear.setFinalStop(40, 0);
    d->leftLinear.setColorAt(0, d->aColor);
    mPainter.setBrush(d->leftLinear);
    mPainter.drawRect(0, 0, 40, this->height());

    d->rightLinear.setStart(this->width()-40, 0);
    d->rightLinear.setFinalStop(this->width(), 0);
    d->rightLinear.setColorAt(1, d->aColor);
    mPainter.setBrush(d->rightLinear);
    mPainter.drawRect(this->width() - 40, 0, 40, this->height());

}
