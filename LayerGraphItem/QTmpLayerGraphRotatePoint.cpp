#include "QTmpLayerGraphRotatePoint.h"


QTmpLayerGraphRotatePoint::QTmpLayerGraphRotatePoint(QRectF rectPixmap, QRectF rectLayer,
                                                     int iLayerType, QString qstrLayerID, QGraphicsItem *parent)
    : QTmpLayerGraphItem(rectPixmap, rectLayer, iLayerType, qstrLayerID, parent)
{
    qreal dLayerHalfWidth = rectLayer.width() / 2.0d;
    QPointF pointRotate(0.0d, -dLayerHalfWidth);
    m_pDrawPath = new QPainterPath(QPointF(0.0d, 0.0d));
    m_pDrawPath->lineTo(pointRotate);
    m_pDrawPath->addEllipse(pointRotate, 50, 50);
    //
    m_pShapePath = new QPainterPath(pointRotate);
    m_pShapePath->addEllipse(pointRotate, 50, 50);
}

QTmpLayerGraphRotatePoint::~QTmpLayerGraphRotatePoint()
{
}

QRectF QTmpLayerGraphRotatePoint::boundingRect() const
{
    return m_pDrawPath->boundingRect();
}

QPainterPath QTmpLayerGraphRotatePoint::shape() const
{
    return *m_pShapePath;
}

void QTmpLayerGraphRotatePoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *wid)
{
    if(m_bSelectItem)
    {
        painter->setBrush(Qt::green);
        painter->drawPath(*m_pDrawPath);
    }
}

void QTmpLayerGraphRotatePoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}

void QTmpLayerGraphRotatePoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
}

void QTmpLayerGraphRotatePoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
}

void QTmpLayerGraphRotatePoint::SetLayerSelect(bool bSelect)
{
    m_bSelectItem = bSelect;
    if(m_bSelectItem)
    {
        QGraphicsItem::setCursor(Qt::CrossCursor);
        setZValue(101);
    }
    else
    {
        QGraphicsItem::setCursor(Qt::ArrowCursor);
        setZValue(0);
    }
}
