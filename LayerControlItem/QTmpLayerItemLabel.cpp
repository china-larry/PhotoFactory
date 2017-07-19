#include "QTmpLayerItemLabel.h"

#include <QDebug>

QTmpLayerItemLabel::QTmpLayerItemLabel(QWidget *parent)
    : QLabel(parent)
{

}

QTmpLayerItemLabel::QTmpLayerItemLabel(int iLabelFlag, QPixmap *pNormal, QPixmap *pActive, QWidget *parent)
    : m_iLabelFlag(iLabelFlag)
    , m_pNormalPixmap(pNormal)
    , m_pActivePixmap(pActive)
    , QLabel(parent)
{
    setCursor(Qt::PointingHandCursor);
    updateLabelStatus();
}

QTmpLayerItemLabel::~QTmpLayerItemLabel()
{

}

void QTmpLayerItemLabel::mousePressEvent(QMouseEvent *ev)
{
    if(m_iLabelFlag)
    {
        m_iLabelFlag = 0;
        this->setPixmap(*m_pNormalPixmap);
    }
    else
    {
        m_iLabelFlag = 1;
        this->setPixmap(*m_pActivePixmap);
    }
    emit checkLabelStatus(m_iLabelFlag);
}

void QTmpLayerItemLabel::mouseMoveEvent(QMouseEvent *ev)
{
}

void QTmpLayerItemLabel::mouseReleaseEvent(QMouseEvent *ev)
{
}

void QTmpLayerItemLabel::setLabelStatus(int iLabelFlag)
{
    m_iLabelFlag = iLabelFlag;
    updateLabelStatus();
}

void QTmpLayerItemLabel::updateLabelStatus()
{
    if(m_iLabelFlag)
    {
        this->setPixmap(*m_pActivePixmap);
    }
    else
    {
        this->setPixmap(*m_pNormalPixmap);
    }
}



