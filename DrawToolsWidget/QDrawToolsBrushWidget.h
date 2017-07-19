#ifndef QDRAWTOOLSBRUSHWIDGET_H
#define QDRAWTOOLSBRUSHWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QBrush>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QConicalGradient>

#include "OperateItem/QTransparentSlider.h"
#include "OperateItem/QTransparentButton.h"
#include "QDrawToolsIconLabel.h"

class QDrawToolsBrushWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QDrawToolsBrushWidget(QWidget *parent = 0);
    ~QDrawToolsBrushWidget();
signals:
    void changeDrawEvent();
public slots:

private slots:
    // 样式
    void onSelectLinearPattern();
    void onSelectRadialPattern();
    void onSelectConicalPattern();
    // 颜色
    void onSelectBlackColor();
    void onSelectWhiteColor();
    // 透明度
    void onTransparentLossFocus();
    void onChangeTransparentSliderValue(int);
    void onChangeTransparentEditValue(QString);
    void onPressTransparentButton();

private:
    void InitBrushPattern();
    void InitBrushColor();
    void InitBrushTransparent();
    void InitLayout();
    //
    void ReSetBrush(QBrush *pBrush);// 重置brush，资源重置
    void ChangeTransValue();        // 透明度修改，重置画刷数据

public:
    QBrush* GetBrush();

private:
    // 样式
    QLabel *m_pPatternLabel;
    QDrawToolsIconLabel *m_pLinearPatternButton;
    QDrawToolsIconLabel *m_pRadialPatternButton;
    QDrawToolsIconLabel *m_pConicalPatternButton;
    QPixmap *m_pLinearPatternIcon;
    QPixmap *m_pRadialPatternIcon;
    QPixmap *m_pConicalPatternIcon;
    // 颜色
    QRadioButton *m_pColorWhiteButton;
    QRadioButton *m_pColorBlackButton;
    // 透明度
    QLabel *m_pTransparentLabel;
    QLineEdit *m_pTransparentEdit;
    QSliderButtonEx *m_pTransparentButton;
    QAutoHideSlider *m_pTransparentSlider;
    //
    QBrush *m_pBrush;
    QLinearGradient *m_pLinearGradient;
    QRadialGradient *m_pRadialGradient;
    QConicalGradient *m_pConicalGradient;

    int m_iPattern;        // 0为linear，1为radial， 2为conical
    int m_iTransparent;

    QColor m_brushColor;

};

#endif // QDRAWTOOLSBRUSHWIDGET_H
