#ifndef QTMPLAYERGRAPHROTATEITEM_H
#define QTMPLAYERGRAPHROTATEITEM_H
#include "QTmpLayerGraphItem.h"
// 用于旋转的item，四个角矩形
class QTmpLayerGraphRotateItem : public QTmpLayerGraphItem
{
    Q_OBJECT
public:
    QTmpLayerGraphRotateItem(int iDragType, QRectF rectPixmap, int iLayerType,
                           QString qstrLayerID,  QGraphicsItem* parent = NULL);
    virtual QRectF boundingRect() const;
signals:
    void mouseRotateLayerPos(qreal);
protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* wid);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
public:
    void SetItemSelectFlag(bool bSelect);
    void UpdataRectPos(QRectF rect);
    void SetRotateItemPos(QPointF point);
    // 坐标变换
    void SetRotateTranslate(qreal x, qreal y);
    void SetRotateScale(qreal x, qreal y);
    void SetRotateRotate(qreal rotate);
    void SetRotateCollides(bool bCollides);
    // 数据重置，缩放数据
    void ResetRotateTransform(QRectF rectPixmap);
private:
    bool m_bSelectItem;
    QRectF m_rect;
    int m_iDragType;      // 上，右，下，左顺序排列为1， 2， 3， 4
    int m_iLineWidth;       // 绘制线宽

    QPointF m_pointCrossA;    // 与圆相交的2个点坐标
    QPointF m_pointCrossB;

    qreal m_dAngleCrossA;   // 起始点与X轴夹角
    qreal m_dAngleCrossB;
    //
    qreal m_dMoveAngle;     // 移动弧度
};
#endif // QTMPLAYERGRAPHROTATEITEM_H
