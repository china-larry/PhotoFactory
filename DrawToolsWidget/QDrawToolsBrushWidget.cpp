#include "QDrawToolsBrushWidget.h"

#include <QButtonGroup>
#include <QHBoxLayout>

QDrawToolsBrushWidget::QDrawToolsBrushWidget(QWidget *parent)
    : QWidget(parent)
    , m_iPattern(0)
    , m_iTransparent(100)
{    
    m_brushColor = QColor(0, 0, 0, 255);
    //
    m_pLinearGradient = new QLinearGradient();
    m_pLinearGradient->setColorAt(0, QColor(0, 0, 0, 0));
    m_pLinearGradient->setColorAt(0.0001, QColor(0, 0, 0, 255));
    m_pLinearGradient->setColorAt(1, QColor(0, 0, 0, 0));
    //
    m_pRadialGradient = new QRadialGradient();
    m_pRadialGradient->setColorAt(0, QColor(0, 0, 0, 255));
    m_pRadialGradient->setColorAt(1, QColor(0, 0, 0, 0));
    //
    m_pConicalGradient = new QConicalGradient();
    m_pConicalGradient->setColorAt(0, QColor(0, 0, 0, 255));
    m_pConicalGradient->setColorAt(1, QColor(0, 0, 0, 0));

    //
    m_pBrush = new QBrush(*m_pLinearGradient);
    // 图标
    m_pLinearPatternIcon = new QPixmap(":/picture/image/linear_brush.png");
    m_pRadialPatternIcon = new QPixmap(":/picture/image/radia_brush.png");
    m_pConicalPatternIcon = new QPixmap(":/picture/image/conical_brush.png");
    //
    InitBrushPattern();
    InitBrushColor();
    InitBrushTransparent();
    InitLayout();
}

QDrawToolsBrushWidget::~QDrawToolsBrushWidget()
{
    if(m_pLinearGradient != NULL)
    {
        delete m_pLinearGradient;
        m_pLinearGradient = NULL;
    }
    if(m_pRadialGradient != NULL)
    {
        delete m_pRadialGradient;
        m_pRadialGradient = NULL;
    }
    if(m_pConicalGradient != NULL)
    {
        delete m_pConicalGradient;
        m_pConicalGradient = NULL;
    }
    if(m_pBrush != NULL)
    {
        delete m_pBrush;
        m_pBrush = NULL;
    }
    if(m_pLinearPatternIcon != NULL)
    {
        delete m_pLinearPatternIcon;
        m_pLinearPatternIcon = NULL;
    }
    if(m_pRadialPatternIcon != NULL)
    {
        delete m_pRadialPatternIcon;
        m_pRadialPatternIcon = NULL;
    }
    if(m_pConicalPatternIcon != NULL)
    {
        delete m_pConicalPatternIcon;
        m_pConicalPatternIcon = NULL;
    }
}

QBrush *QDrawToolsBrushWidget::GetBrush()
{
    return m_pBrush;
}


void QDrawToolsBrushWidget::InitBrushPattern()
{
    // 样式
    m_pPatternLabel = new QLabel("样式", this);
    m_pLinearPatternButton = new QDrawToolsIconLabel(this);
    m_pLinearPatternButton->SetPixmap(m_pLinearPatternIcon);
    m_pLinearPatternButton->SetSelect(true);
    m_pRadialPatternButton = new QDrawToolsIconLabel(this);
    m_pRadialPatternButton->SetPixmap(m_pRadialPatternIcon);
    m_pConicalPatternButton = new QDrawToolsIconLabel(this);
    m_pConicalPatternButton->SetPixmap(m_pConicalPatternIcon);
    //
    connect(m_pLinearPatternButton, SIGNAL(sigMousePress()), this, SLOT(onSelectLinearPattern()));
    connect(m_pRadialPatternButton, SIGNAL(sigMousePress()), this, SLOT(onSelectRadialPattern()));
    connect(m_pConicalPatternButton, SIGNAL(sigMousePress()), this, SLOT(onSelectConicalPattern()));
}

void QDrawToolsBrushWidget::InitBrushColor()
{
    QButtonGroup *pColorGroup = new QButtonGroup(this);
    m_pColorBlackButton = new QRadioButton("黑色", this);
    m_pColorBlackButton->setChecked(true);
    m_pColorWhiteButton = new QRadioButton("白色", this);
    pColorGroup->addButton(m_pColorBlackButton);
    pColorGroup->addButton(m_pColorWhiteButton);

    connect(m_pColorBlackButton, SIGNAL(clicked()), this, SLOT(onSelectBlackColor()));
    connect(m_pColorWhiteButton, SIGNAL(clicked()), this, SLOT(onSelectWhiteColor()));
}

void QDrawToolsBrushWidget::InitBrushTransparent()
{
    m_pTransparentLabel = new QLabel("透明度", this);
    //
    m_pTransparentSlider = new QAutoHideSlider(Qt::Horizontal, this);

    m_pTransparentSlider->setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    m_pTransparentSlider->hide();
    m_pTransparentSlider->setRange(0, 100);
    m_pTransparentSlider->setValue(100);
    connect(m_pTransparentSlider, SIGNAL(lossFocus()), this, SLOT(onTransparentLossFocus()));
    connect(m_pTransparentSlider, SIGNAL(valueChanged(int)), this, SLOT(onChangeTransparentSliderValue(int)));
    //
    m_pTransparentEdit = new QLineEdit("100", this);
    m_pTransparentEdit->setMinimumHeight(20);
    m_pTransparentEdit->setMaximumWidth(50);
    m_pTransparentButton = new QSliderButtonEx(m_pTransparentEdit);
    //
    connect(m_pTransparentEdit, SIGNAL(textEdited(QString)), this, SLOT(onChangeTransparentEditValue(QString)));
    connect(m_pTransparentButton, SIGNAL(clicked()), this, SLOT(onPressTransparentButton()));
}

void QDrawToolsBrushWidget::InitLayout()
{
    QHBoxLayout *pGridLayout = new QHBoxLayout;
    // 样式
    pGridLayout->addWidget(m_pPatternLabel);
    pGridLayout->addWidget(m_pLinearPatternButton);
    pGridLayout->addWidget(m_pRadialPatternButton);
    pGridLayout->addWidget(m_pConicalPatternButton);
    // 颜色
    pGridLayout->addWidget(m_pColorBlackButton);
    pGridLayout->addWidget(m_pColorWhiteButton);
    // 透明度
    pGridLayout->addWidget(m_pTransparentLabel);
    pGridLayout->addWidget(m_pTransparentEdit);

    QSpacerItem *pSpacerItem = new QSpacerItem(100, 20);
    pGridLayout->addSpacerItem(pSpacerItem);

    setLayout(pGridLayout);
}

void QDrawToolsBrushWidget::onSelectLinearPattern()
{
    m_iPattern  = 0;
    m_pLinearPatternButton->SetSelect(true);
    m_pRadialPatternButton->SetSelect(false);
    m_pConicalPatternButton->SetSelect(false);

    ReSetBrush(new QBrush(*m_pLinearGradient));
}

void QDrawToolsBrushWidget::onSelectRadialPattern()
{
    m_iPattern  = 1;
    m_pLinearPatternButton->SetSelect(false);
    m_pRadialPatternButton->SetSelect(true);
    m_pConicalPatternButton->SetSelect(false);
    ReSetBrush(new QBrush(*m_pRadialGradient));
}

void QDrawToolsBrushWidget::onSelectConicalPattern()
{
    m_iPattern  = 2;
    m_pLinearPatternButton->SetSelect(false);
    m_pRadialPatternButton->SetSelect(false);
    m_pConicalPatternButton->SetSelect(true);
    ReSetBrush(new QBrush(*m_pConicalGradient));
}

void QDrawToolsBrushWidget::onSelectBlackColor()
{
    m_brushColor = QColor(0, 0, 0, 255 * m_iTransparent / 100);
    //
    m_pLinearGradient->setColorAt(0, QColor(0, 0, 0, 0));
    m_pLinearGradient->setColorAt(0.001, m_brushColor);
    m_pLinearGradient->setColorAt(1, QColor(0, 0, 0, 0));
    //
    m_pRadialGradient->setColorAt(0, m_brushColor);
    m_pRadialGradient->setColorAt(1, QColor(0, 0, 0, 0));
    //
    m_pConicalGradient->setColorAt(0, m_brushColor);
    m_pConicalGradient->setColorAt(1, QColor(0, 0, 0, 0));
    if(m_iPattern == 0)
    {
        ReSetBrush(new QBrush(*m_pLinearGradient));
    }
    else if(m_iPattern ==1)
    {
        ReSetBrush(new QBrush(*m_pRadialGradient));
    }
    else
    {
        ReSetBrush(new QBrush(*m_pConicalGradient));
    }
}

void QDrawToolsBrushWidget::onSelectWhiteColor()
{
    m_brushColor = QColor(255, 255, 255, 255 * m_iTransparent / 100);
    //
    m_pLinearGradient->setColorAt(0, QColor(255, 255, 255, 0));
    m_pLinearGradient->setColorAt(0.001, m_brushColor);
    m_pLinearGradient->setColorAt(1, QColor(255, 255, 255, 0));
    //
    m_pRadialGradient->setColorAt(0, m_brushColor);
    m_pRadialGradient->setColorAt(1, QColor(255, 255, 255, 0));
    //
    m_pConicalGradient->setColorAt(0, m_brushColor);
    m_pConicalGradient->setColorAt(1, QColor(255, 255, 255, 0));
    if(m_iPattern == 0)
    {
        ReSetBrush(new QBrush(*m_pLinearGradient));
    }
    else if(m_iPattern ==1)
    {
        ReSetBrush(new QBrush(*m_pRadialGradient));
    }
    else
    {
        ReSetBrush(new QBrush(*m_pConicalGradient));
    }
}

void QDrawToolsBrushWidget::onTransparentLossFocus()
{
    m_pTransparentSlider->hide();
}

void QDrawToolsBrushWidget::onChangeTransparentSliderValue(int iTransparent)
{
    m_iTransparent = iTransparent;
    QString strTransparent = QString::number(m_iTransparent);
    m_pTransparentEdit->setText(strTransparent);
    // 重置画刷数据
    ChangeTransValue();

    emit changeDrawEvent();
}

void QDrawToolsBrushWidget::onChangeTransparentEditValue(QString qstrTransparent)
{
    m_iTransparent = qstrTransparent.toInt();
    if(m_iTransparent > 100)
    {
        m_iTransparent = 100;
        m_pTransparentEdit->setText(QString::number(100));
    }
    if(m_iTransparent < 0)
    {
        m_iTransparent = 0;
        m_pTransparentEdit->setText(QString::number(0));
    }
    m_pTransparentSlider->setValue(m_iTransparent);
    // 重置画刷数据
    ChangeTransValue();

    emit changeDrawEvent();
}

void QDrawToolsBrushWidget::onPressTransparentButton()
{
    // 设置滚动条位置及大小
    m_pTransparentSlider->setGeometry(m_pTransparentLabel->geometry().x(),
                                m_pTransparentLabel->geometry().y() + 20, 80, 15);
    m_pTransparentSlider->raise();
    m_pTransparentSlider->show();
    m_pTransparentSlider->setFocus();
    m_pTransparentSlider->setValue(m_iTransparent);
}

void QDrawToolsBrushWidget::ReSetBrush(QBrush *pBrush)
{
    if(pBrush == NULL) return;
    if(m_pBrush != NULL)
    {
        delete m_pBrush;
    }
    m_pBrush = pBrush;
    emit changeDrawEvent();
}

void QDrawToolsBrushWidget::ChangeTransValue()
{
    m_brushColor.setAlpha(255 * m_iTransparent / 100);
    if(m_iPattern == 0)
    {
        m_pLinearGradient->setColorAt(0.001, m_brushColor);
        ReSetBrush(new QBrush(*m_pLinearGradient));
    }
    else if(m_iPattern == 1)
    {
        m_pRadialGradient->setColorAt(0, m_brushColor);
        ReSetBrush(new QBrush(*m_pRadialGradient));
    }
    else if(m_iPattern == 2)
    {
        m_pConicalGradient->setColorAt(0, m_brushColor);
        ReSetBrush(new QBrush(*m_pConicalGradient));
    }
    else
    {

    }
}
