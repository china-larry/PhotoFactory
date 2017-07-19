#include "QGraphMaskScene.h"
#include <QDebug>

QGraphMaskScene::QGraphMaskScene(QObject *parent)
    : QGraphicsScene(parent)
{
    m_pMaskDrawItem = NULL;
    m_rectMask = QRectF(0, 0, 0, 0);
}

void QGraphMaskScene::drawBackground(QPainter *painter, const QRectF &rect)
{

}


void QGraphMaskScene::ResetScene(QRectF rectMask, QPixmap *pBackPixmap, QPixmap *pMaskPixmap)
{
    if(pBackPixmap == NULL) return;
    this->clear();
    m_rectMask = rectMask;
    // 背景item
    m_pBackGraphItem = new QTmpLayerGraphItem(QRectF(0, 0, pBackPixmap->width(), pBackPixmap->height()),
                                              QRectF(0, 0, pBackPixmap->width(), pBackPixmap->height()), 0, "-1");
    m_pBackGraphItem->setZValue(0);
    m_pBackGraphItem->SetViewPixmap(pBackPixmap);
    this->addItem(m_pBackGraphItem);
    // 蒙版item
    m_pMaskDrawItem = new QMaskLayerGraphItem(m_rectMask);
    m_pMaskDrawItem->setZValue(1);
    m_pMaskDrawItem->SetMaskPixmap(pMaskPixmap);
    this->addItem(m_pMaskDrawItem);
    connect(m_pMaskDrawItem, SIGNAL(sigChangePixmap(QPixmap*)), this, SIGNAL(sigChangeMaskItemPixmap(QPixmap*)));
}

QRectF QGraphMaskScene::GetMaskRect()
{
    return m_rectMask;
}

void QGraphMaskScene::SetGraphBackItemScale(qreal dScale)
{
    m_pBackGraphItem->setScale(dScale);    
}


void QGraphMaskScene::SetGraphMaskItemScale(qreal dScale)
{
    m_pMaskDrawItem->setScale(dScale);
}

void QGraphMaskScene::SetPen(QPen *pPen)
{
    m_pMaskDrawItem->SetPen(pPen);
}

void QGraphMaskScene::SetBrush(QBrush *pBrush)
{
    m_pMaskDrawItem->SetBrush(pBrush);
}

void QGraphMaskScene::SetDrawType(int iType)
{
    m_pMaskDrawItem->SetDrawType(iType);
}

void QGraphMaskScene::SetBackGraphPixmap(QPixmap *pPixmap)
{
    m_pBackGraphItem->SetViewPixmap(pPixmap);
}


