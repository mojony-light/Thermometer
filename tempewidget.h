#ifndef TEMPEWIDGET_H
#define TEMPEWIDGET_H

#include "thermometer.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QWidget>
#include <QPainter>
#include <QLinearGradient>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QPaintEvent>

class TempeWidgetPrivate;
class TempeWidget : public QLabel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(TempeWidget)
    Q_ENUMS(TempeStyle)
    Q_PROPERTY(QColor aColor READ getAColor WRITE setAColor NOTIFY aColorChange);

public:
    enum TempeStyle{Green, Red};
    explicit TempeWidget(QWidget *parent = nullptr);
    QColor getAColor();
    void setAColor(const QColor &color);
    void setThermometer(const qreal ther);
    void setTempeStyle(const TempeStyle &tempS);

protected:
    void paintEvent(QPaintEvent *e);

private:
    TempeWidgetPrivate *d_ptr;

signals:
    void aColorChange(const QColor &color);

};

#endif // TEMPEWIDGET_H
