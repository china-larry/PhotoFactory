#ifndef QMASKLAYERGRAPHITEM_H
#define QMASKLAYERGRAPHITEM_H

#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>
#include <QPixmap>
#include <QPicture>
#include <QPainter>
#include <QCursor>
#include <QPainterPath>
#include <QPen>
#include <QBrush>

class QMaskLayerGraphItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit QMaskLayerGraphItem(QRectF rectMask, QGraphicsItem* parent = NULL);
    ~QMaskLayerGraphItem();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

signals:
    void sigChangePixmap(QPixmap*);

public:
    //
    QPixmap* GetMaskPicture();
    void SetMaskPixmap(QPixmap *pPixmap);
    //
    void BeginDrawMaskPicture();
    void EndDrawMaskPicture();
    // 绘图
    void SetPen(QPen *pPen);
    void SetBrush(QBrush *pBrush);
    void SetDrawType(int iType);
    void SetItemScale(qreal dScale);
private:
    QPixmap *m_pMaskPixmap;          // 非自有资源，不须释放，资源为蒙版层数据
    QPainter *m_pMaskDrawPainter;

    QRectF m_rectMask;
    // 绘图
    QPen *m_pPen;           // 非自有资源，不需释放
    QBrush *m_pBrush;
    int m_iDrawType;    // 0为画笔，1为画刷
    //
    bool m_bStartDraw;
    //
    QPointF m_pointPress;
    QPointF m_pointMove;
    QPointF m_pointRelaPress;   // 相对模板自己坐标
    QPointF m_pointRelaMove;
};

#endif // QMASKLAYERGRAPHITEM_H
