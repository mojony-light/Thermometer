#ifndef THERMOMETER_H
#define THERMOMETER_H

#include <QWidget>
#include <QSequentialAnimationGroup>
#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QFontMetricsF>
#include <QPropertyAnimation>
#include <QTime>

class ThermometerPrivate;
class Thermometer : public QWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Thermometer)
    Q_PROPERTY(qreal value READ value WRITE setValue)
    Q_ENUMS(ScaleType)

public:
    enum ScaleType{
        OneS = 1,
        TwoS = 2,
        FiveS = 5
    };

    explicit Thermometer(QWidget *parent = nullptr);
    qreal value();
    void setValue(const qreal svalue);
    void setMinValue(const int sminValue);
    void setMaxValue(const int maxValue);
    void setScaleType(const ScaleType stype);
    void setThermometer(const qreal c_ther);
    void setEasing(QEasingCurve aEasing);
    void start();

protected:
    void paintEvent(QPaintEvent *e);
    qreal generateRandomNum(qreal min, qreal max);

private:
    ThermometerPrivate *d_ptr;

signals:
    void finished();

};

#endif // THERMOMETER_H
