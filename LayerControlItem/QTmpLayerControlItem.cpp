#include "QTmpLayerControlItem.h"

#include <QDebug>

extern QPixmap *g_pClippPixmap;
extern QPixmap *g_pIsVisablePixmap;
extern QPixmap *g_pNotVisablePixmap;
extern QPixmap *g_pLayerImagePixmap;
extern QPixmap *g_pLayerFodderPixmap;

QTmpLayerControlItem::QTmpLayerControlItem()
{

}

QTmpLayerControlItem::QTmpLayerControlItem(int iVisable, int iClipp, int iType, bool bMask)
    : m_iVisable(iVisable)
    , m_iClipp(iClipp)
    , m_iLayerType(iType)
    , m_bMaskFlag(bMask)
    , m_pMaskViewLabel(NULL)
{
    connect(this, SIGNAL(clicked()), this, SLOT(changeLayerID()));
    // 是否可视
    m_pVisableLabel = new QTmpLayerItemLabel(m_iVisable, g_pIsVisablePixmap, g_pNotVisablePixmap,this);
    m_pVisableLabel->setMinimumWidth(20);
    m_pVisableLabel->setAlignment(Qt::AlignCenter);
    connect(m_pVisableLabel, SIGNAL(checkLabelStatus(int)), this, SLOT(checkLayerVisable(int)));
    // 图层预览
    m_pImageViewLabel = new QLabel(this);
    m_pImageViewLabel->resize(50, 35);
 //   m_pImageViewLabel->setFrameStyle(QFrame::Box);
    m_pImageViewLabel->setMinimumWidth(50);
    m_pImageViewLabel->setAlignment(Qt::AlignCenter);
    // 图层名称
    m_pLayerNameLabel = new QLabel(this);
    m_pLayerNameLabel->resize(20, 15);
    m_pLayerNameLabel->setMinimumWidth(20);
    // 图层类型
    m_pLayerTypeLabel = new QLabel(this);
    m_pLayerTypeLabel->setMinimumWidth(30);
    if(m_iLayerType)
    {
        m_pLayerTypeLabel->setPixmap(*g_pLayerImagePixmap);
    }
    else
    {
        m_pLayerTypeLabel->setPixmap(*g_pLayerFodderPixmap);
    }
    // 布局
    m_pLayerLayout = new QHBoxLayout(this);
    m_pLayerLayout->addWidget(m_pVisableLabel);
    // 是否具有管理层
    int iSpacerItemWidth = 200; // 空白布局宽度
    if(m_iClipp)
    {
        // 是否关联
        QLabel *pLayerClipp = new QLabel(this);
        pLayerClipp->setPixmap(*g_pClippPixmap);
        pLayerClipp->setMinimumWidth(25);
        m_pLayerLayout->addWidget(pLayerClipp);

        iSpacerItemWidth -= 40;
    }
    // 图像预览
    m_pLayerLayout->addWidget(m_pImageViewLabel);
    // 是否具有蒙版层
    if(m_bMaskFlag)
    {
        m_pMaskViewLabel = new QTmpLayerMaskItemLabel(this);
        connect(m_pMaskViewLabel, SIGNAL(mousePressMask()), this, SLOT(onBeginDrawMask()));
        //
        m_pLayerLayout->addWidget(m_pMaskViewLabel);
        //
        iSpacerItemWidth -= 50;
    }
    //
    m_pSpacerItem = new QSpacerItem(iSpacerItemWidth, 20);

    m_pLayerLayout->addSpacerItem(m_pSpacerItem);
    m_pLayerLayout->addWidget(m_pLayerTypeLabel);
    m_pLayerLayout->addWidget(m_pLayerNameLabel);
    m_pLayerLayout->setSpacing(5);
}

QTmpLayerControlItem::~QTmpLayerControlItem()
{

}


void QTmpLayerControlItem::setLayerID(QString qstrLayerID)
{
    m_qstrLayerID = qstrLayerID;
}

void QTmpLayerControlItem::setLayerIndex(QString qstrLayerIndex)
{
    m_pLayerNameLabel->setText(qstrLayerIndex);
}

void QTmpLayerControlItem::setLayerVisable(int iVisable)
{
    m_iVisable = iVisable;
}

void QTmpLayerControlItem::setLayerPixmap(QPixmap *pPixmap)
{
    if(pPixmap == NULL)
    {
        return;
    }
    m_pPixmap = pPixmap;
    m_pImageViewLabel->setPixmap(*m_pPixmap);
}

void QTmpLayerControlItem::SetMaskPixmap(QPixmap *pPixmap)
{
    if(pPixmap == NULL)
    {
        return;
    }
    m_pMaskPixmap = pPixmap;
    if(m_pMaskViewLabel != NULL)
    {
        m_pMaskViewLabel->SetPixmap(m_pMaskPixmap);
    }
    this->update();
}

void QTmpLayerControlItem::SetMaskSelect(bool bSelect)
{
    if(m_pMaskViewLabel != NULL)
    {
        m_pMaskViewLabel->SetSelectMask(bSelect);
    }
}

QString QTmpLayerControlItem::getLayerID()
{
    return m_qstrLayerID;
}

int QTmpLayerControlItem::getLayerVisable()
{
    return m_iVisable;
}

void QTmpLayerControlItem::checkLayerVisable(int iVisable)
{
    m_iVisable = iVisable;
    emit changeLayerControlItemVisable(m_qstrLayerID, m_iVisable);
}

void QTmpLayerControlItem::changeLayerID()
{
    emit changeLayerControlItemID(m_qstrLayerID);
}

void QTmpLayerControlItem::updateLayerType(QString qstrLayerID, int iLayerType)
{
    if(qstrLayerID == m_qstrLayerID)
    {
        m_iLayerType = iLayerType;
    }
}

void QTmpLayerControlItem::updateLayerVisable(QString qstrLayerID, int iLayerVisable)
{
    if(qstrLayerID == m_qstrLayerID)
    {
        m_iVisable = iLayerVisable;
        m_pVisableLabel->setLabelStatus(m_iVisable);
    }
}

void QTmpLayerControlItem::onBeginDrawMask()
{
    emit beginDrawMask(m_qstrLayerID.toInt());
}
