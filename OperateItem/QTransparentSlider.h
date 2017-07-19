/**
 * 自定义透明度滚动条控件，处理焦点丢失等
 *
 */
#ifndef QTRANSPARENTSLIDER_H
#define QTRANSPARENTSLIDER_H

#include <QSlider>
#include <QLineEdit>

class QAutoHideSlider : public QSlider
{
    Q_OBJECT
public:
    QAutoHideSlider(Qt::Orientation orientation, QWidget *parent);

protected:
    virtual void focusOutEvent(QFocusEvent *event);

signals:
    void lossFocus();

};

#endif // QTRANSPARENTSLIDER_H
