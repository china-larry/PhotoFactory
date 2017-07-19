#ifndef QTMPLAYERGRAPHSCALEITEM_H
#define QTMPLAYERGRAPHSCALEITEM_H

#include "QTmpLayerGraphItem.h"

// 用于缩放拖拽的item，四条直线
class QTmpLayerGraphScaleItem : public QTmpLayerGraphItem
{
    Q_OBJECT
public:
    QTmpLayerGraphScaleItem(int iDragType, QRectF rectPixmap, int iLayerType,
                           QString qstrLayerID,  QGraphicsItem* parent = NULL);
    virtual QRectF boundingRect() const;
signals:
    void mouseScaleLayerPos(int, qreal, QPointF);
protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* wid);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
public:
    void SetItemSelectFlag(bool bSelect);
    int GetDragType();
    void SetDragType(int iDragType);
    void UpdataRectPos(QRectF rect);
    // 坐标变换
    void SetDragTranslate(qreal x, qreal y);
    void SetDragScale(qreal x, qreal y);
    void SetDragRotate(qreal rotate);
    void SetDragCollides(bool bCollides);
    // 数据重置，缩放数据
    void ResetDragTransform(QRectF rectPixmap);
    //
    QPointF GetBoundTopLeftPoint();
    QPointF GetBoundBottomRightPoint();
private:
    bool m_bSelectItem;
    QRectF m_rect;
    int m_iDragType;      // 上，右，下，左顺序排列为1， 2， 3， 4
    int m_iLineWidth;       // 绘制线宽
    //
};

#endif // QTMPLAYERGRAPHSCALEITEM_H


