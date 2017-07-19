#include "QDrawToolsWidget.h"

#include <QGridLayout>
#include <QButtonGroup>

QDrawToolsWidget::QDrawToolsWidget(QWidget *parent)
    : QWidget(parent)
    , m_iDrawType(0)
{
    m_pBrush = new QBrush(QColor(Qt::red));
    m_pPen = new QPen(QColor(Qt::green));
    //
    m_pPenWidget = new QDrawToolsPenWidget(this);
    m_pBrushWidget = new QDrawToolsBrushWidget(this);
    //
    connect(m_pPenWidget, SIGNAL(changeDrawEvent()), this, SIGNAL(changeDrawTools()));
    connect(m_pBrushWidget, SIGNAL(changeDrawEvent()), this, SIGNAL(changeDrawTools()));
    //
    QButtonGroup *pTypeGroup = new QButtonGroup(this);
    m_pPenButton = new QRadioButton("画刷", this);
    m_pPenButton->setChecked(true);
    m_pBrushButton = new QRadioButton("渐变", this);
    pTypeGroup->addButton(m_pPenButton);
    pTypeGroup->addButton(m_pBrushButton);

    connect(m_pPenButton, SIGNAL(clicked()), this, SLOT(onSelectPen()));
    connect(m_pBrushButton, SIGNAL(clicked()), this, SLOT(onSelectBrush()));

    // Tab初始化
    m_pStackedWidget = new QStackedWidget;
    m_pStackedWidget->addWidget(m_pPenWidget);
    m_pStackedWidget->addWidget(m_pBrushWidget);

    // 布局
    QGridLayout *pGridLayout = new QGridLayout();
    pGridLayout->addWidget(m_pPenButton, 0, 0, 1, 1);
    pGridLayout->addWidget(m_pBrushButton, 1, 0, 1, 1);
    pGridLayout->addWidget(m_pStackedWidget, 0, 1, 2, 1);

    setLayout(pGridLayout);
}

void QDrawToolsWidget::onSelectPen()
{
    m_pStackedWidget->setCurrentIndex(0);
    m_iDrawType = 0;
    emit changeDrawTools();
}

void QDrawToolsWidget::onSelectBrush()
{
    m_pStackedWidget->setCurrentIndex(1);
    m_iDrawType = 1;
    emit changeDrawTools();
}

QPen *QDrawToolsWidget::GetPen()
{
    return m_pPenWidget->GetPen();
}

QBrush *QDrawToolsWidget::GetBrush()
{
    return m_pBrushWidget->GetBrush();
}

int QDrawToolsWidget::GetDrawType()
{
    return m_iDrawType;
}
