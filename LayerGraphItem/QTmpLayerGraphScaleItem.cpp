#include "QTmpLayerGraphScaleItem.h"

// 缩放拖拽item
QTmpLayerGraphScaleItem::QTmpLayerGraphScaleItem(int iDragType, QRectF rectPixmap, int iLayerType,
                                               QString qstrLayerID,  QGraphicsItem* parent)
    : QTmpLayerGraphItem(rectPixmap, rectPixmap, iLayerType, qstrLayerID, parent)
    , m_iDragType(iDragType)
{
    m_bSelectItem = false;
    m_iLineWidth = 10;
    UpdataRectPos(rectPixmap);
    // 属性
    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemIsSelectable |
        QGraphicsItem::ItemSendsGeometryChanges |
        QGraphicsItem::ItemIsFocusable |
        QGraphicsItem::ItemIsMovable);
}
QRectF QTmpLayerGraphScaleItem::boundingRect() const
{
    if(m_iDragType == 1 || m_iDragType == 3)
    {
        return QRectF(0, 0, m_rect.width(), m_iLineWidth);// 上边缘在场景中坐标
    }
    else if(m_iDragType == 2 || m_iDragType == 4)
    {
        return QRectF(0, 0, m_iLineWidth, m_rect.height());// 右边缘在场景中坐标
    }
    else
    {
         return QRectF(0, 0, 0, 0);
    }
}
void QTmpLayerGraphScaleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *wid)
{
    if(m_bSelectItem)
    {
        painter->save();
        painter->setBrush(Qt::green);
        painter->drawRect(boundingRect());
        painter->restore();
    }
}
void QTmpLayerGraphScaleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_pointMousePress = mapToParent(event->pos());// 映射到父类view坐标系统
    QGraphicsItem::update(boundingRect());
}

void QTmpLayerGraphScaleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//    emit mouseScaleRelease();
    QGraphicsItem::update(boundingRect());
}

void QTmpLayerGraphScaleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // 获取鼠标移动距离
    QPointF pointEventPos = mapToParent(event->pos());
    m_pointMoveGap = pointEventPos - m_pointMousePress;

    //
    emit mouseScaleLayerPos(m_iDragType, 0.0d, m_pointMoveGap);
    m_pointMousePress = pointEventPos;
}

void QTmpLayerGraphScaleItem::SetItemSelectFlag(bool bSelect)
{
    m_bSelectItem = bSelect;
    if(m_bSelectItem)
    {
        if(m_iDragType == 1 || m_iDragType == 3)
        {
            QGraphicsItem::setCursor(Qt::SizeVerCursor);
        }
        else
        {
            QGraphicsItem::setCursor(Qt::SizeHorCursor);
        }
        setZValue(100);
    }
    else
    {
        QGraphicsItem::setCursor(Qt::ArrowCursor);
        setZValue(0);
    }
}
int QTmpLayerGraphScaleItem::GetDragType()
{
    return m_iDragType;
}

void QTmpLayerGraphScaleItem::SetDragType(int iDragType)
{
    m_iDragType = iDragType;
    if(m_bSelectItem)
    {
        if(m_iDragType == 1 || m_iDragType == 3)
        {
            QGraphicsItem::setCursor(Qt::SizeVerCursor);
        }
        else
        {
            QGraphicsItem::setCursor(Qt::SizeHorCursor);
        }
        setZValue(100);
    }
    else
    {
        QGraphicsItem::setCursor(Qt::ArrowCursor);
        setZValue(0);
    }
}

void QTmpLayerGraphScaleItem::UpdataRectPos(QRectF rect)
{
    m_rect = rect;
    m_dCircleRadii = qPow((qPow(m_rect.width(), 2.0f) + qPow(m_rect.height(), 2.0f)), 0.5f) * 0.5f;
    m_pointCircle = m_rect.topLeft() + QPointF(m_rect.width()/2.0f, m_rect.height()/2.0f);
    if(m_iDragType == 1)
    {
        setPos(m_rect.topLeft());
    }
    else if(m_iDragType == 2)
    {
        setPos(m_rect.topRight());
    }
    else if(m_iDragType == 3)
    {
        setPos(m_rect.bottomLeft());
    }
    else if(m_iDragType == 4)
    {
        setPos(m_rect.topLeft());
    }
    else
    {
    }
}

void QTmpLayerGraphScaleItem::SetDragTranslate(qreal x, qreal y)
{
    m_curTransformItem.translate(x, y);
    setTransform(m_curTransformItem);
}

void QTmpLayerGraphScaleItem::SetDragScale(qreal x, qreal y)
{
  //  m_scaleTransformTmp = m_curTransformItem;
    m_curTransformItem.scale(x, y);
    setTransform(m_curTransformItem);
}

void QTmpLayerGraphScaleItem::SetDragRotate(qreal rotate)
{
    m_curTransformItem.rotateRadians(rotate);
    setTransform(m_curTransformItem);
}
// 是否碰撞
void QTmpLayerGraphScaleItem::SetDragCollides(bool bCollides)
{
    if(bCollides)
    {// 碰撞，数据还原
        m_curTransformItem = m_preTransformItem;
        setTransform(m_curTransformItem);
    }
    else
    {// 不存在碰撞
        m_preTransformItem = m_curTransformItem;
    }
}

void QTmpLayerGraphScaleItem::ResetDragTransform(QRectF rectPixmap)
{
    UpdataRectPos(rectPixmap);
    m_pointMoveGap = m_curPointRelativePos = m_prePointRelativePos = QPointF(0, 0);
    m_preTransformItem.reset();
    m_curTransformItem.reset();
    setTransform(m_curTransformItem);
    this->update(boundingRect());
}

QPointF QTmpLayerGraphScaleItem::GetBoundTopLeftPoint()
{
    return mapToParent(boundingRect().topLeft());
}

QPointF QTmpLayerGraphScaleItem::GetBoundBottomRightPoint()
{
    return mapToParent(boundingRect().bottomRight());
}
