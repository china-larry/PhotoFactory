#include "QDrawToolsPenWidget.h"

QDrawToolsPenWidget::QDrawToolsPenWidget(QWidget *parent)
    : QWidget(parent)
    , m_iType(0)
    , m_iWidth(1)
    , m_iTransparent(100)
    , m_iHardness(0)
{
    m_penColor = QColor(0, 0, 0, 255);

    m_pPen = new QPen(m_penColor);
    m_pPen->setCapStyle(Qt::RoundCap);
    m_pPen->setJoinStyle(Qt::RoundJoin);
    //
    m_pPen->setBrush(QBrush(m_penColor));
    //
    m_pRadialGradient = new QRadialGradient();
    m_pRadialGradient->setColorAt(0, QColor(0, 0, 0, 255));
    m_pRadialGradient->setColorAt(1, QColor(0, 0, 0, 0));
    // 图标
    m_pSolidTypeIcon = new QPixmap(":/picture/image/solid_pen.png");
    m_pRadialTypeIcon = new QPixmap(":/picture/image/radial_pen.png");
    //
    InitPenType();
    InitPenColor();
    InitPenWidth();
    InitPenTransparent();
    InitPenHardness();
    //
    InitLayout();
}

QDrawToolsPenWidget::~QDrawToolsPenWidget()
{
    if(m_pRadialGradient != NULL)
    {
        delete m_pRadialGradient;
        m_pRadialGradient = NULL;
    }
    if(m_pPen != NULL)
    {
        delete m_pPen;
        m_pPen = NULL;
    }
    if(m_pSolidTypeIcon != NULL)
    {
        delete m_pSolidTypeIcon;
        m_pSolidTypeIcon = NULL;
    }
    if(m_pRadialTypeIcon != NULL)
    {
        delete m_pRadialTypeIcon;
        m_pRadialTypeIcon = NULL;
    }
}
// 初始化画笔类型控件
void QDrawToolsPenWidget::InitPenType()
{
    // 样式
    m_pTypeLabel = new QLabel("样式", this);
    m_pSolidTypeButton = new QDrawToolsIconLabel(this);
    m_pSolidTypeButton->SetPixmap(m_pSolidTypeIcon);
    m_pSolidTypeButton->SetSelect(true);
    m_pRadialTypeButton = new QDrawToolsIconLabel(this);
    m_pRadialTypeButton->SetPixmap(m_pRadialTypeIcon);

    connect(m_pSolidTypeButton, SIGNAL(sigMousePress()), this, SLOT(onSelectSolidType()));
    connect(m_pRadialTypeButton, SIGNAL(sigMousePress()), this, SLOT(onSelectRadialType()));
}

void QDrawToolsPenWidget::InitPenColor()
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
// 初始化画笔宽度控件
void QDrawToolsPenWidget::InitPenWidth()
{
    m_pWidthLabel = new QLabel("笔宽", this);

    //
    m_pWidthSlider = new QAutoHideSlider(Qt::Horizontal, this);

    m_pWidthSlider->setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    m_pWidthSlider->hide();
    m_pWidthSlider->setRange(1, 500);
    m_pWidthSlider->setValue(1);
    connect(m_pWidthSlider, SIGNAL(lossFocus()), this, SLOT(onWidthLossFocus()));
    connect(m_pWidthSlider, SIGNAL(valueChanged(int)), this, SLOT(onChangeWidthSliderValue(int)));
    //
    m_pWidthEdit = new QLineEdit("1", this);
    m_pWidthEdit->setMinimumHeight(20);
    m_pWidthEdit->setMaximumWidth(50);
    m_pWidthButton = new QSliderButtonEx(m_pWidthEdit);
    //
    connect(m_pWidthEdit, SIGNAL(textEdited(QString)), this, SLOT(onChangeWidthEditValue(QString)));
    connect(m_pWidthButton, SIGNAL(clicked()), this, SLOT(onPressWidthButton()));
}

void QDrawToolsPenWidget::InitPenTransparent()
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

void QDrawToolsPenWidget::InitPenHardness()
{
    m_pHardnessLabel = new QLabel("硬度", this);
    //
    m_pHardnessSlider = new QAutoHideSlider(Qt::Horizontal, this);

    m_pHardnessSlider->setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    m_pHardnessSlider->hide();
    m_pHardnessSlider->setRange(0, 100);
    m_pHardnessSlider->setValue(0);
    connect(m_pHardnessSlider, SIGNAL(lossFocus()), this, SLOT(onHardnessLossFocus()));
    connect(m_pHardnessSlider, SIGNAL(valueChanged(int)), this, SLOT(onChangeHardnessSliderValue(int)));
    //
    m_pHardnessEdit = new QLineEdit("0", this);
    m_pHardnessEdit->setMinimumHeight(20);
    m_pHardnessEdit->setMaximumWidth(50);
    m_pHardnessButton = new QSliderButtonEx(m_pHardnessEdit);
    //
    connect(m_pHardnessEdit, SIGNAL(textEdited(QString)), this, SLOT(onChangeHardnessEditValue(QString)));
    connect(m_pHardnessButton, SIGNAL(clicked()), this, SLOT(onPressHardnessButton()));
}

void QDrawToolsPenWidget::InitLayout()
{
    QGridLayout *pGridLayout = new QGridLayout;
    // 样式
    pGridLayout->addWidget(m_pTypeLabel, 0, 0, 1, 1);
    pGridLayout->addWidget(m_pSolidTypeButton, 0, 1, 1, 1);
    pGridLayout->addWidget(m_pRadialTypeButton, 0, 2, 1, 1);
    // 颜色
    pGridLayout->addWidget(m_pColorWhiteButton, 0, 3, 1, 1);
    pGridLayout->addWidget(m_pColorBlackButton, 0, 4, 1, 1);
    //
    pGridLayout->addWidget(m_pWidthLabel, 0, 5, 1, 1);
    pGridLayout->addWidget(m_pWidthEdit, 0, 6, 1, 1);
    //
    pGridLayout->addWidget(m_pTransparentLabel, 0, 7, 1, 1);
    pGridLayout->addWidget(m_pTransparentEdit, 0, 8, 1, 1);
    //
    pGridLayout->addWidget(m_pHardnessLabel, 0, 9, 1, 1);
    pGridLayout->addWidget(m_pHardnessEdit, 0, 10, 1, 1);

    setLayout(pGridLayout);
}

void QDrawToolsPenWidget::onSelectSolidType()
{
    m_iType = 0;
    //
    m_pSolidTypeButton->SetSelect(true);
    m_pRadialTypeButton->SetSelect(false);
    //
    m_pPen->setBrush(QBrush(m_penColor, Qt::SolidPattern));
    emit changeDrawEvent();
}

void QDrawToolsPenWidget::onSelectRadialType()
{
    m_iType = 1;
    m_pSolidTypeButton->SetSelect(false);
    m_pRadialTypeButton->SetSelect(true);
    //
    m_pPen->setBrush(QBrush(*m_pRadialGradient));
    emit changeDrawEvent();
}

void QDrawToolsPenWidget::onSelectBlackColor()
{
    //
    m_penColor = QColor(0, 0, 0, 255 * m_iTransparent / 100);
    m_pRadialGradient->setColorAt(0, m_penColor);
    m_pRadialGradient->setColorAt(1, QColor(0, 0, 0, 0));
    //
    if(m_iType)
    {
        m_pPen->setBrush(QBrush(*m_pRadialGradient));
    }
    else
    {
        m_pPen->setBrush(QBrush(m_penColor, Qt::SolidPattern));
    }
    emit changeDrawEvent();
}

void QDrawToolsPenWidget::onSelectWhiteColor()
{
    //
    m_penColor = QColor(255, 255, 255, 255 * m_iTransparent / 100);
    m_pRadialGradient->setColorAt(0, m_penColor);
    m_pRadialGradient->setColorAt(1, QColor(255, 255, 255, 0));
    //
    if(m_iType)
    {
        m_pPen->setBrush(QBrush(*m_pRadialGradient));
    }
    else
    {
        m_pPen->setBrush(QBrush(m_penColor, Qt::SolidPattern));
    }
    emit changeDrawEvent();
}

void QDrawToolsPenWidget::onWidthLossFocus()
{
    m_pWidthSlider->hide();
}

void QDrawToolsPenWidget::onChangeWidthSliderValue(int iWidth)
{
    m_iWidth = iWidth;
    QString strWidth = QString::number(m_iWidth);
    m_pWidthEdit->setText(strWidth);
    //
    m_pPen->setWidth(m_iWidth);
    if(m_iType)
    {
        m_pRadialGradient->setRadius(m_iWidth);
        m_pPen->setBrush(QBrush(*m_pRadialGradient));
    }
    emit changeDrawEvent();
}

void QDrawToolsPenWidget::onChangeWidthEditValue(QString qstrWidth)
{
    m_iWidth = qstrWidth.toInt();
    if(m_iWidth > 500)
    {
        m_iWidth = 500;
        m_pWidthEdit->setText(QString::number(500));
    }
    if(m_iWidth < 0)
    {
        m_iWidth = 0;
        m_pWidthEdit->setText(QString::number(0));
    }
    m_pWidthSlider->setValue(m_iWidth);
    //
    m_pPen->setWidth(m_iWidth);
    if(m_iType)
    {
        m_pRadialGradient->setRadius(m_iWidth);
        m_pPen->setBrush(QBrush(*m_pRadialGradient));
    }
    emit changeDrawEvent();
}

void QDrawToolsPenWidget::onPressWidthButton()
{
    // 设置滚动条位置及大小
    m_pWidthSlider->setGeometry(m_pWidthLabel->geometry().x(),
                                m_pWidthLabel->geometry().y() + 20, 80, 15);
    m_pWidthSlider->raise();
    m_pWidthSlider->show();
    m_pWidthSlider->setFocus();
    m_pWidthSlider->setValue(m_iWidth);
}

void QDrawToolsPenWidget::onTransparentLossFocus()
{
    m_pTransparentSlider->hide();
}

void QDrawToolsPenWidget::onChangeTransparentSliderValue(int iTransparent)
{
    m_iTransparent = iTransparent;
    QString strTransparent = QString::number(m_iTransparent);
    m_pTransparentEdit->setText(strTransparent);
    //
    m_penColor.setAlpha(255 * m_iTransparent / 100);
    m_pRadialGradient->setColorAt(0, m_penColor);
    //
    if(m_iType)
    {
        m_pPen->setBrush(QBrush(*m_pRadialGradient));
    }
    else
    {
        m_pPen->setBrush(QBrush(m_penColor, Qt::SolidPattern));
    }
    emit changeDrawEvent();
}

void QDrawToolsPenWidget::onChangeTransparentEditValue(QString qstrTransparent)
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
    //
    m_penColor.setAlpha(255 * m_iTransparent / 100);
    m_pRadialGradient->setColorAt(0, m_penColor);
    //
    if(m_iType)
    {
        m_pPen->setBrush(QBrush(*m_pRadialGradient));
    }
    else
    {
        m_pPen->setBrush(QBrush(m_penColor, Qt::SolidPattern));
    }
    emit changeDrawEvent();
}

void QDrawToolsPenWidget::onPressTransparentButton()
{
    // 设置滚动条位置及大小
    m_pTransparentSlider->setGeometry(m_pTransparentLabel->geometry().x(),
                                m_pTransparentLabel->geometry().y() + 20, 80, 15);
    m_pTransparentSlider->raise();
    m_pTransparentSlider->show();
    m_pTransparentSlider->setFocus();
    m_pTransparentSlider->setValue(m_iTransparent);
}

void QDrawToolsPenWidget::onHardnessLossFocus()
{
    m_pHardnessSlider->hide();
}

void QDrawToolsPenWidget::onChangeHardnessSliderValue(int iHardness)
{
    m_iHardness = iHardness;
    QString strHardness = QString::number(m_iHardness);
    m_pHardnessEdit->setText(strHardness);
    emit changeDrawEvent();
}

void QDrawToolsPenWidget::onChangeHardnessEditValue(QString qstrHardness)
{
    m_iHardness = qstrHardness.toInt();
    if(m_iHardness > 100)
    {
        m_iHardness = 100;
        m_pHardnessEdit->setText(QString::number(100));
    }
    if(m_iHardness < 0)
    {
        m_iHardness = 0;
        m_pHardnessEdit->setText(QString::number(0));
    }
    m_pHardnessSlider->setValue(m_iHardness);
    //
    emit changeDrawEvent();
}

void QDrawToolsPenWidget::onPressHardnessButton()
{
    // 设置滚动条位置及大小
    m_pHardnessSlider->setGeometry(m_pHardnessLabel->geometry().x(),
                                m_pHardnessLabel->geometry().y() + 20, 80, 15);
    m_pHardnessSlider->raise();
    m_pHardnessSlider->show();
    m_pHardnessSlider->setFocus();
    m_pHardnessSlider->setValue(m_iHardness);
}

QPen *QDrawToolsPenWidget::GetPen()
{
    return m_pPen;
}


