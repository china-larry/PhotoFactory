/**
 * 操作面板控件，用于处理操作混合模式或透明度等
 *
 */
#ifndef QOPERATELABEL_H
#define QOPERATELABEL_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QCommandLinkButton>
#include <QGridLayout>
#include <QDockWidget>

#include "QTransparentButton.h"
#include "QTransparentSlider.h"

class QOperateLabel : public QWidget
{
    Q_OBJECT
public:
    explicit QOperateLabel(QWidget *parent = 0);
    
signals:
    void changeMixModeValue(int);
    void changeTransValue(int);
public slots:
    void onTransLossFocus();
    void onChangeTransEditValue(QString);
    void onChangeTransSliderValue(int);
private slots:
    void onPressTransButton();
    void onChangeMixModeValue(int);
public:
    void SetMixModel(int iMixModel);
    void SetTransparent(int iTransparent);
    int GetMixModel();
    int GetTransparent();
private:
    void InitMinModelData();
private:
    // 控件
    QLabel *m_pMinLabel;
    QLabel *m_pTransparentLabel;
    QComboBox *m_pMixModelWidget;
    // 透明度
    QLineEdit *m_pTransEdit;
    QSliderButtonEx *m_pTransButton;
    QAutoHideSlider *m_pTransSlider;
    // 布局管理器
    QGridLayout *m_pGridLayout;
    //
    int m_iMixModelValue;
    int m_iTransparentValue;
};

#endif // QOPERATELABEL_H
