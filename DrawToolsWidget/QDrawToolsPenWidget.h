#ifndef QDRAWTOOLSPENWIDGET_H
#define QDRAWTOOLSPENWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QPen>
#include <QBrush>
#include <QRadialGradient>
#include <QButtonGroup>
#include <QGridLayout>
#include <QColor>

#include "OperateItem/QTransparentSlider.h"
#include "OperateItem/QTransparentButton.h"
#include "QDrawToolsIconLabel.h"

class QDrawToolsPenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QDrawToolsPenWidget(QWidget *parent = 0);
    ~QDrawToolsPenWidget();
signals:
    void changeDrawEvent();
public slots:

private slots:
    // 样式
    void onSelectSolidType();   // 实心
    void onSelectRadialType();  // 渐变
    // 颜色
    void onSelectBlackColor();
    void onSelectWhiteColor();
    // 宽度
    void onWidthLossFocus();
    void onChangeWidthSliderValue(int);
    void onChangeWidthEditValue(QString);
    void onPressWidthButton();
    // 透明度
    void onTransparentLossFocus();
    void onChangeTransparentSliderValue(int);
    void onChangeTransparentEditValue(QString);
    void onPressTransparentButton();
    // 硬度
    void onHardnessLossFocus();
    void onChangeHardnessSliderValue(int);
    void onChangeHardnessEditValue(QString);
    void onPressHardnessButton();

public:
    QPen* GetPen();
private:
    void InitPenType();
    void InitPenColor();
    void InitPenWidth();
    void InitPenTransparent();
    void InitPenHardness();
    void InitLayout();

private:
    // 样式
    QLabel *m_pTypeLabel;
    QDrawToolsIconLabel *m_pSolidTypeButton;            // 实心画笔
    QDrawToolsIconLabel *m_pRadialTypeButton;          // 渐变画笔
    QPixmap *m_pSolidTypeIcon;
    QPixmap *m_pRadialTypeIcon;
    // 颜色
    QRadioButton *m_pColorWhiteButton;
    QRadioButton *m_pColorBlackButton;
    // 笔宽
    QLabel *m_pWidthLabel;
    QLineEdit *m_pWidthEdit;
    QSliderButtonEx *m_pWidthButton;
    QAutoHideSlider *m_pWidthSlider;
    // 透明度
    QLabel *m_pTransparentLabel;
    QLineEdit *m_pTransparentEdit;
    QSliderButtonEx *m_pTransparentButton;
    QAutoHideSlider *m_pTransparentSlider;
    // 硬度
    QLabel *m_pHardnessLabel;
    QLineEdit *m_pHardnessEdit;
    QSliderButtonEx *m_pHardnessButton;
    QAutoHideSlider *m_pHardnessSlider;
    //
    QPen *m_pPen;
    QBrush *m_pBrush;
    QRadialGradient *m_pRadialGradient;

    //
    int m_iType; // 0为实心，1为渐变
    int m_iWidth;
    int m_iTransparent;
    int m_iHardness;
    QColor m_penColor;
    QColor m_colorWhite;
    QColor m_colorWhiteAlpha;
    QColor m_colorBlack;
    QColor m_colorBlackAlpha;
};

#endif // QDRAWTOOLSPENWIDGET_H
