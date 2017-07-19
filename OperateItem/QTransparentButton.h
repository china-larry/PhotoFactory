/**
 * 自定义透明度Eidt中按钮控件，弹出透明度滚动条
 *
 */
#ifndef QTRANSPARENTBUTTON_H
#define QTRANSPARENTBUTTON_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>


class QSliderButtonEx : public QPushButton
{
    Q_OBJECT
public:
    explicit QSliderButtonEx(QLineEdit *pLineEdit);
    
signals:
    
public slots:
    
};

#endif // QTRANSPARENTBUTTON_H
