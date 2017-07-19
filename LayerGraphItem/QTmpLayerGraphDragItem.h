/**
 * 用于旋转坐标变换控件，用于碰撞检测
 *
 */
#ifndef QTMPLAYERGRAPHDRAGITEM_H
#define QTMPLAYERGRAPHDRAGITEM_H

#include <QPainterPath>
#include <QPen>
#include "QTmpLayerGraphItem.h"

// 用于缩放拖拽的item，四条直线
class QTmpLayerGraphDragItem : public QTmpLayerGraphItem
{
    Q_OBJECT
public:
    QTmpLayerGraphDragItem(QRectF rectPixmap, int iLayerType,
                           QString qstrLayerID,  QGraphicsItem* parent = NULL);
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
signals:
    void mouseReleaseRotate();
    void mouseRotateLayerPos(bool, qreal);
    void mouseDragLayerPos(qreal); //
protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* wid);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
public:
    void SetItemSelectFlag(bool bSelect);
    void SetDragType(int iDragType);
    void UpdataRectPos(QRectF rect);
    // 坐标变换
    void SetDragTranslate(qreal x, qreal y);
    void SetDragScale(qreal x, qreal y);
    void SetDragRotate(qreal rotate);
    void SetDragCollides(bool bCollides);
    // 数据重置，缩放数据
    void ResetDragTransform(QRectF rectPixmap);
    // 获取外接矩形宽高
    qreal GetBoundWidth();
    qreal GetBoundHeight();
private:
    bool m_bSelectItem;
    QRectF m_rect;
    int m_iLineWidth;       // 绘制线宽
    QPen m_pen;
    bool m_bShiftKeyDown;
    //
    QPainterPath m_painterPath;
    QRectF m_rectShape;
    QRectF m_rectDraw;
    QRectF m_rectBound;
    QPointF m_pointCrossA;    // 与圆相交的2个点坐标
    QPointF m_pointCrossB;
    qreal m_dAngleCrossA;   // 起始点与X轴夹角
    qreal m_dAngleCrossB;
    //
    qreal m_dMoveAngle;     // 移动弧度
    qreal m_dMoveScale;     // 移动缩放距离
    bool m_bScale;      // 放大or缩小，true放大
};

#endif // QTMPLAYERGRAPHDRAGITEM_H
