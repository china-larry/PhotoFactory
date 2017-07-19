#include "QOperateLabel.h"

QOperateLabel::QOperateLabel(QWidget *parent)
    : QWidget(parent)
    , m_iMixModelValue(0)
    , m_iTransparentValue(100)
{
    // 文字说明
    m_pMinLabel = new QLabel(this);
    m_pMinLabel->setText("混合模式：");
    m_pTransparentLabel = new QLabel(this);
    m_pTransparentLabel->setText(" 不透明度：");
    // 混合控件
    m_pMixModelWidget = new QComboBox(this);
    m_pMixModelWidget->setMinimumHeight(20);
    m_pMixModelWidget->setMinimumWidth(80);
    InitMinModelData();
    connect(m_pMixModelWidget, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangeMixModeValue(int)));
    // 透明滚动条
    m_pTransSlider = new QAutoHideSlider(Qt::Horizontal, this);
    m_pTransSlider->setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    m_pTransSlider->hide();
    m_pTransSlider->setRange(0, 100);
 //   m_pTransSlider->setTickPosition(QSlider::TicksLeft); // 刻度显示
    m_pTransSlider->setValue(100);
    connect(m_pTransSlider, SIGNAL(lossFocus()), this, SLOT(onTransLossFocus()));
    connect(m_pTransSlider, SIGNAL(valueChanged(int)), this, SLOT(onChangeTransSliderValue(int)));

    // 透明度
    m_pTransEdit = new QLineEdit("100", this);
    m_pTransEdit->setMinimumHeight(20);
  //  m_pTransEdit->setInputMask("");
    m_pTransButton = new QSliderButtonEx(m_pTransEdit);
    //
    connect(m_pTransEdit, SIGNAL(textEdited(QString)), this, SLOT(onChangeTransEditValue(QString)));
    connect(m_pTransButton, SIGNAL(clicked()), this, SLOT(onPressTransButton()));


    // 布局
    m_pGridLayout = new QGridLayout(this);
   // m_pGridLayout->setSpacing(10);
    m_pGridLayout->addWidget(m_pMinLabel, 0, 0, 1, 1);
    m_pGridLayout->addWidget(m_pMixModelWidget, 0, 1, 1, 1);
    m_pGridLayout->addWidget(m_pTransparentLabel, 0, 2, 1, 1);
    m_pGridLayout->addWidget(m_pTransEdit, 0, 3, 1, 1);
    m_pGridLayout->setVerticalSpacing(500);
}

void QOperateLabel::onTransLossFocus()
{
    m_pTransSlider->hide();
}

void QOperateLabel::onPressTransButton()
{
    // 设置滚动条位置及大小
    m_pTransSlider->setGeometry(m_pTransparentLabel->geometry().x()+10,
                                m_pTransparentLabel->geometry().y() + 20, 120, 15);
    m_pTransSlider->raise();
    m_pTransSlider->show();
    m_pTransSlider->setFocus();
    m_pTransSlider->setValue(m_iTransparentValue);
}

void QOperateLabel::onChangeMixModeValue(int iIndex)
{
    m_iMixModelValue = iIndex;
    emit changeMixModeValue(m_iMixModelValue);
}

void QOperateLabel::onChangeTransEditValue(QString strTrans)
{
    m_iTransparentValue = strTrans.toInt();
    if(m_iTransparentValue > 100)
    {
        m_iTransparentValue = 100;
        m_pTransEdit->setText(QString::number(100));
    }
    if(m_iTransparentValue < 0)
    {
        m_iTransparentValue = 0;
        m_pTransEdit->setText(QString::number(0));
    }
    m_pTransSlider->setValue(m_iTransparentValue);

    // 转换为0-255
    qreal qTransValue = (qreal)m_iTransparentValue * 255.0d / 100.0d;
    emit changeTransValue((int)qTransValue);
}

void QOperateLabel::onChangeTransSliderValue(int iTransValue)
{
    m_iTransparentValue = iTransValue;
    QString strTrans = QString::number(m_iTransparentValue);
    m_pTransEdit->setText(strTrans);

    // 转换为0-255
    qreal qTransValue = (qreal)m_iTransparentValue * 255.0d / 100.0d;
    emit changeTransValue((int)qTransValue);
}

void QOperateLabel::SetMixModel(int iMixModel)
{
    m_iMixModelValue = iMixModel;
    m_pMixModelWidget->setCurrentIndex(m_iMixModelValue);
    this->update();
    m_pMixModelWidget->update();
}
// set值为0-255
void QOperateLabel::SetTransparent(int iTransparent)
{
    qreal qTransValue = (qreal)iTransparent * 100.0d / 255.0d;
    m_iTransparentValue = (int)qTransValue;
    QString strTrans = QString::number(m_iTransparentValue);
    m_pTransEdit->setText(strTrans);
    this->update();
    m_pTransEdit->update();
}

int QOperateLabel::GetMixModel()
{
    return m_iMixModelValue;
}

int QOperateLabel::GetTransparent()
{
    return m_iTransparentValue;
}

void QOperateLabel::InitMinModelData()
{
    m_pMixModelWidget->setMaxVisibleItems(21);
    m_pMixModelWidget->addItem("正常", 0);
    m_pMixModelWidget->addItem("溶解", 1);
    m_pMixModelWidget->addItem("变暗",2);
    m_pMixModelWidget->addItem("正片叠底",3);
    m_pMixModelWidget->addItem("颜色加深", 4);
    m_pMixModelWidget->addItem("线性加深", 5);
    m_pMixModelWidget->addItem("变亮",6);
    m_pMixModelWidget->addItem("滤色",7);
    m_pMixModelWidget->addItem("颜色减淡", 8);
    m_pMixModelWidget->addItem("线性减淡", 9);
    m_pMixModelWidget->addItem("叠加",10);
    m_pMixModelWidget->addItem("柔光",11);
    m_pMixModelWidget->addItem("强光", 12);
    m_pMixModelWidget->addItem("亮光", 13);
    m_pMixModelWidget->addItem("线性光",14);
    m_pMixModelWidget->addItem("点光",15);
    m_pMixModelWidget->addItem("实色混合", 16);
    m_pMixModelWidget->addItem("差值", 17);
    m_pMixModelWidget->addItem("排除",18);
    m_pMixModelWidget->addItem("色相",19);
    m_pMixModelWidget->addItem("饱和度",20);
}
