#include "QTransparentSlider.h"
#include <QVBoxLayout>
QAutoHideSlider::QAutoHideSlider(Qt::Orientation orientation, QWidget *parent)
    : QSlider(orientation, parent)
{
}

void QAutoHideSlider::focusOutEvent(QFocusEvent *event)
{
    this->hide();
}
