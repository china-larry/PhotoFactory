/**
 * 场景中基础控制控件，主要用于绘制外围矩形框，用于坐标变换
 *
 */
#ifndef QTMPLAYERGRAPHITEM_H
#define QTMPLAYERGRAPHITEM_H

#include <QWidget>
#include <QGraphicsObject>
#include <QPainter>
#include <QCursor>
#include <QString>
#include <QPixmap>
#include <QPointF>
#include <QRectF>
#include <QPen>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRotation>
#include <QtCore/qmath.h>
#include <QDebug>

// 视图中基础控件item
class QTmpLayerGraphItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit QTmpLayerGraphItem(QRectF rectPixmap, QRectF rectLayer, int iLayerType,
                                QString qstrLayerID,  QGraphicsItem* parent = NULL);
    ~QTmpLayerGraphItem();
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:
    void mousePressLayerID(QString);
public:
    void SetLayerID(QString qstrLayerID);
    QString GetLayerID();
    void SetViewPixmap(QPixmap *pPixmap);
    QRectF GetPixmapRect();
    int GetLayerType();
protected:
    void calculatePixmapRect();   // 获取pixmap矩形，缩放
protected:
    QPixmap  *m_pImageView;       ///< 图片，非自有资源不需要释放
    QRectF m_rectPixmap;            ///< pixmap矩形，只有宽高有意义
    QRectF m_rectLayer;                 ///< 图片所在图层矩形框
    int m_iLayerType;                   ///< 图层类型
    //
    QString m_qstrLayerID;
    QPointF m_pointMousePress;         ///< 相对中心点移动距离
    QPointF m_pointMoveGap;
    QPointF m_prePointRelativePos;     ///< 相对图层中心点坐标，上一次值记录 ，碰撞后还原此值
    QPointF m_curPointRelativePos;     ///< 相对图层中心点坐标
    QTransform m_preTransformItem;
    QTransform m_curTransformItem;
    QTransform m_scaleTransformTmp;
    QPointF m_pointCircle;  // 圆中心点坐标
    qreal m_dCircleRadii;   // 圆半径
};


#endif // QTMPLAYERGRAPHITEM_H
