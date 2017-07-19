#include "QTmpLayerGraphDragItem.h"

const qreal g_dPI = 3.1415926d;
const qreal g_dDoublePI = g_dPI * 2.0d;
#include <Qpen>
// 缩放拖拽item
QTmpLayerGraphDragItem::QTmpLayerGraphDragItem(QRectF rectPixmap, int iLayerType,
                                               QString qstrLayerID,  QGraphicsItem* parent)
    : QTmpLayerGraphItem(rectPixmap, rectPixmap, iLayerType, qstrLayerID, parent)
{
    m_bSelectItem = false;
    m_iLineWidth = 10;
    m_pen = QPen(QBrush(Qt::green), m_iLineWidth);
    m_bShiftKeyDown = false;
    m_rectShape = QRectF(-50, -50, 100, 100);
    m_rectDraw = QRectF(-40, -40, 80, 80);
    UpdataRectPos(rectPixmap);
    // 属性
    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemIsSelectable |
        QGraphicsItem::ItemSendsGeometryChanges |
        QGraphicsItem::ItemIsFocusable |
        QGraphicsItem::ItemIsMovable);
}
QRectF QTmpLayerGraphDragItem::boundingRect() const
{
    return m_rectBound;
}
//
QPainterPath QTmpLayerGraphDragItem::shape() const
{
    return m_painterPath;
}

void QTmpLayerGraphDragItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *wid)
{
    if(m_bSelectItem)
    {        
        painter->setPen(m_pen);
        painter->setBrush(Qt::green);
        painter->drawPath(m_painterPath);
    }
}

void QTmpLayerGraphDragItem::keyPressEvent(QKeyEvent *event)
{
     if(event->key() == Qt::Key_Shift)
     {
             m_bShiftKeyDown = true;
             qDebug() << "shitf down";
     }
}

void QTmpLayerGraphDragItem::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Shift)
    {
            m_bShiftKeyDown = false;
            qDebug() << "shitf up";
    }
}
void QTmpLayerGraphDragItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_pointMousePress = mapToParent(event->pos());// 映射到父类view坐标系统
    QGraphicsItem::update(boundingRect());
}

void QTmpLayerGraphDragItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit mouseReleaseRotate();
    QGraphicsItem::update(boundingRect());    
}

void QTmpLayerGraphDragItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // 获取鼠标移动距离
    QPointF pointEventPos = mapToParent(event->pos());
    //
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
    emit mouseRotateLayerPos(m_bShiftKeyDown, m_dMoveAngle);
    m_pointMousePress = pointEventPos;
}

void QTmpLayerGraphDragItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    event->ignore();
}

void QTmpLayerGraphDragItem::SetItemSelectFlag(bool bSelect)
{
    m_bSelectItem = bSelect;
    if(m_bSelectItem)
    {
        QGraphicsItem::setCursor(Qt::CrossCursor);
        setZValue(100);
    }
    else
    {
        QGraphicsItem::setCursor(Qt::ArrowCursor);
        setZValue(0);
    }
}

void QTmpLayerGraphDragItem::UpdataRectPos(QRectF rect)
{
    m_rect.setTopLeft(QPointF(0, 0));
    m_rect.setWidth(rect.width());
    m_rect.setHeight(rect.height());
    setPos(rect.topLeft());
    // 计算参数
    m_dCircleRadii = qPow((qPow(rect.width(), 2.0f) + qPow(rect.height(), 2.0f)), 0.5f) * 0.5f;
    m_pointCircle = rect.topLeft() + QPointF(rect.width()/2.0f, rect.height()/2.0f);
    // 添加四个角矩形
    QPainterPath path1, path2, path3, path4;
    path1.moveTo(m_rect.topLeft());
    path1.lineTo(m_rect.topRight());
    path2.moveTo(m_rect.topRight());
    path2.lineTo(m_rect.bottomRight());
    path3.moveTo(m_rect.bottomRight());
    path3.lineTo(m_rect.bottomLeft());
    path4.moveTo(m_rect.bottomLeft());
    path4.lineTo(m_rect.topLeft());
    //
    m_painterPath = QPainterPath();
    m_painterPath.addPath(path1);
    m_painterPath.addPath(path2);
    m_painterPath.addPath(path3);
    m_painterPath.addPath(path4);

    m_painterPath.addRect(m_rect.left()+m_rectShape.left(), m_rect.top()+m_rectShape.top(), m_rectShape.width(), m_rectShape.height());
    m_painterPath.addRect(m_rect.right()+m_rectShape.left(), m_rect.top()+m_rectShape.top(), m_rectShape.width(), m_rectShape.height());
    m_painterPath.addRect(m_rect.right()+m_rectShape.left(), m_rect.bottom()+m_rectShape.top(), m_rectShape.width(), m_rectShape.height());
    m_painterPath.addRect(m_rect.left()+m_rectShape.left(), m_rect.bottom()+m_rectShape.top(), m_rectShape.width(), m_rectShape.height());
    //
    m_rectBound.setTopLeft(m_rect.topLeft() + m_rectShape.topLeft());
    m_rectBound.setWidth(m_rect.width() + m_rectShape.width());
    m_rectBound.setHeight(m_rect.height() + m_rectShape.height());
}

void QTmpLayerGraphDragItem::SetDragTranslate(qreal x, qreal y)
{
    m_curTransformItem.translate(x, y);
    setTransform(m_curTransformItem);
}

void QTmpLayerGraphDragItem::SetDragScale(qreal x, qreal y)
{
  //  m_scaleTransformTmp = m_curTransformItem;
    m_curTransformItem.scale(x, y);
    setTransform(m_curTransformItem);
}

void QTmpLayerGraphDragItem::SetDragRotate(qreal rotate)
{
    m_curTransformItem.rotateRadians(rotate);
    setTransform(m_curTransformItem);
}
// 是否碰撞
void QTmpLayerGraphDragItem::SetDragCollides(bool bCollides)
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

void QTmpLayerGraphDragItem::ResetDragTransform(QRectF rectPixmap)
{
    UpdataRectPos(rectPixmap);
    m_pointMoveGap = m_curPointRelativePos = m_prePointRelativePos = QPointF(0, 0);
    m_preTransformItem.reset();
    m_curTransformItem.reset();
    setTransform(m_curTransformItem);
    this->update(boundingRect());
}

qreal QTmpLayerGraphDragItem::GetBoundWidth()
{
    return m_rect.width();
}

qreal QTmpLayerGraphDragItem::GetBoundHeight()
{
    return m_rect.height();
}

