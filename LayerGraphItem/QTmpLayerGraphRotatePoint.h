#ifndef QTMPLAYERGRAPHROTATEPOINT_H
#define QTMPLAYERGRAPHROTATEPOINT_H

#include "QTmpLayerGraphItem.h"

class QTmpLayerGraphRotatePoint : public QTmpLayerGraphItem
{
    Q_OBJECT
public:
    explicit QTmpLayerGraphRotatePoint(QRectF rectPixmap, QRectF rectLayer, int iLayerType,
                                       QString qstrLayerID,  QGraphicsItem* parent = NULL);
    ~QTmpLayerGraphRotatePoint();
public:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* wid);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
public:
    void SetLayerSelect(bool bSelect);
signals:
    void mouseRotateLayerPos(qreal);
    
signals:
    
public slots:

private:
    QPainterPath* m_pDrawPath;
    QPainterPath* m_pShapePath;

    bool m_bSelectItem;
    
};

#endif // QTMPLAYERGRAPHROTATEPOINT_H
