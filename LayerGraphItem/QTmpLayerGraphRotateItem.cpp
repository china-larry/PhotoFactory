#include "QTmpLayerGraphRotateItem.h"

const qreal g_dPI = 3.1415926d;
const qreal g_dDoublePI = g_dPI * 2.0d;
////////////////////////////////////////////////////////////////////////////////
// 旋转item
QTmpLayerGraphRotateItem::QTmpLayerGraphRotateItem(int iDragType, QRectF rectPixmap, int iLayerType,
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
QRectF QTmpLayerGraphRotateItem::boundingRect() const
{
    return QRect(-40, -40, 80, 80);
}
void QTmpLayerGraphRotateItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *wid)
{
    if(m_bSelectItem && m_iLayerType)
    {
        painter->save();
        painter->setBrush(Qt::green);
        painter->drawRect(boundingRect());
        painter->restore();
    }
}
void QTmpLayerGraphRotateItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_pointMousePress = mapToParent(event->pos());// 映射到父类view坐标系统
    QGraphicsItem::update(boundingRect());
}

void QTmpLayerGraphRotateItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::update(boundingRect());
}

void QTmpLayerGraphRotateItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // 获取鼠标移动距离
    QPointF pointEventPos = mapToParent(event->pos());
    m_pointCrossA = m_pointMousePress - m_pointCircle;
    m_pointCrossB = pointEventPos - m_pointCircle;
    m_dAngleCrossA = qAtan2(m_pointCrossA.y(), m_pointCrossA.x());
    m_dAngleCrossB = qAtan2(m_pointCrossB.y(), m_pointCrossB.x());
    // 计算移动弧度
    m_dMoveAngle = m_dAngleCrossB - m_dAngleCrossA;
    if(m_dMoveAngle > g_dPI)
    {
        m_dMoveAngle = g_dDoublePI - m_dMoveAngle;
    }
    if(m_dMoveAngle < -g_dPI)
    {
        m_dMoveAngle = g_dDoublePI + m_dMoveAngle;
    }
    emit mouseRotateLayerPos(m_dMoveAngle);
    m_pointMousePress = pointEventPos;
}

void QTmpLayerGraphRotateItem::SetItemSelectFlag(bool bSelect)
{
    m_bSelectItem = bSelect;
    if(m_bSelectItem)
    {
        QGraphicsItem::setCursor(Qt::SizeAllCursor);
        setZValue(101);
    }
    else
    {
        QGraphicsItem::setCursor(Qt::ArrowCursor);
        setZValue(0);
    }
}

void QTmpLayerGraphRotateItem::UpdataRectPos(QRectF rect)
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
        setPos(m_rect.bottomRight());
    }
    else if(m_iDragType == 4)
    {
        setPos(m_rect.bottomLeft());
    }
    else
    {
    }
}

void QTmpLayerGraphRotateItem::SetRotateItemPos(QPointF point)
{
    if(m_iDragType == 1)
    {
        setPos(point);
    }
    else if(m_iDragType == 2)
    {
        setPos(point);
    }
    else if(m_iDragType == 3)
    {
        setPos(point);
    }
    else if(m_iDragType == 4)
    {
        setPos(point);
    }
    else
    {
    }
    this->update(boundingRect());
}

void QTmpLayerGraphRotateItem::SetRotateTranslate(qreal x, qreal y)
{
    m_curTransformItem.translate(x, y);
    setTransform(m_curTransformItem);
}

void QTmpLayerGraphRotateItem::SetRotateScale(qreal x, qreal y)
{
    m_curTransformItem.scale(x, y);
    setTransform(m_curTransformItem);
}

void QTmpLayerGraphRotateItem::SetRotateRotate(qreal rotate)
{
    m_curTransformItem.rotate(rotate);
    setTransform(m_curTransformItem);
}
// 是否碰撞
void QTmpLayerGraphRotateItem::SetRotateCollides(bool bCollides)
{
    if(bCollides)
    {// 碰撞，数据还原
        m_curPointRelativePos = m_prePointRelativePos;
        m_curTransformItem = m_preTransformItem;
        setTransform(m_curTransformItem);
    }
    else
    {// 不存在碰撞
        m_preTransformItem = m_curTransformItem;
    }
}

void QTmpLayerGraphRotateItem::ResetRotateTransform(QRectF rectPixmap)
{
    UpdataRectPos(rectPixmap);
    m_dMoveAngle = 0.0d;
    m_preTransformItem.reset();
    m_curTransformItem.reset();
    setTransform(m_curTransformItem);
    this->update(boundingRect());
}
