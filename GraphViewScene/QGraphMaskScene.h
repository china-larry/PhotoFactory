// 此场景为专属绘制蒙版使用，一个场景有且只有一个蒙版控件和背景控件
#ifndef QGRAPHMASKSCENE_H
#define QGRAPHMASKSCENE_H

#include <QGraphicsScene>
#include <QPainter>

#include "LayerGraphItem/QMaskLayerGraphItem.h"
#include "LayerGraphItem/QTmpLayerGraphItem.h"

class QGraphMaskScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit QGraphMaskScene(QObject *parent = 0);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
//    void drawForeground(QPainter *painter, const QRectF &rect);
    
signals:
    void sigChangeMaskItemPixmap(QPixmap*);

public slots:
    

public:
    void ResetScene(QRectF rectMask, QPixmap *pBackPixmap, QPixmap *pMaskPixmap);
    QRectF GetMaskRect();
    void SetGraphBackItemScale(qreal dScale);
    void SetGraphMaskItemScale(qreal dScale);

    // 绘图
    void SetPen(QPen *pPen);
    void SetBrush(QBrush *pBrush);
    void SetDrawType(int iType);
    //
    void SetBackGraphPixmap(QPixmap *pPixmap);

private:
    QTmpLayerGraphItem *m_pBackGraphItem;
    QMaskLayerGraphItem *m_pMaskDrawItem;

    QRectF m_rectMask;
};

#endif // QGRAPHMASKSCENE_H
