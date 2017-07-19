/**
 * 控制控件，处理一起场景中控件操作信号，与场景交互
 */
#ifndef QTMPLAYERGRAPHCONTROLITEM_H
#define QTMPLAYERGRAPHCONTROLITEM_H
#include "QTmpLayerGraphItem.h"
#include "QTmpLayerGraphDragItem.h"
// 绘制图层item
class QTmpLayerGraphControlItem : public QTmpLayerGraphItem
{
    Q_OBJECT
public:
    explicit QTmpLayerGraphControlItem(QRectF rectPixmap, QRectF rectLayer, int iLayerType,
                                       QString qstrLayerID, QGraphicsItem* parent = NULL);
    ~QTmpLayerGraphControlItem();
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
  //  bool event(QEvent *ev);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);    
signals:
    void mouseMovePhotoPos(int, QPointF);
    void mouseScalePhotoPos(int , qreal);
    void mouseRotatePhotoPos(int, qreal);
    //
public slots:
    void onMouseReleaseRotate();     // 缩放移动边类型，图层缩放绝对比率
    void onRotateLayerItem(bool, qreal);  // 是否有shift按下，45度旋转，绝对旋转角度，相对旋转角度和相对移动向量
    void onDragLayerItem(bool, qreal, qreal); // 缩放距离和旋转距离
public:
    void SetLayerVisable(int iLayerVisable);
    void SetLayerSelect(bool bLayerSelect);
    int GetLayerVisable();
    bool GetLayerSelect();
    // 更换photo，重置所有数据
    void SetViewPixmap(QPixmap *pPixmap, QRectF rectPixmap);
    // 鼠标缩放
    void wheelScaleLayerItem(qreal dScaleValue);

private:
    int m_iLayerVisable;
    bool m_bLayerSelect;
    QPen *m_pPen;                       ///< 画笔
    QPen *m_pPixmapPen;             ///< 图片画笔
    // 缩放
    QTmpLayerGraphDragItem* m_drag;
    qreal m_dMoveScaleValue;        // 相对移动缩放比
    qreal m_dCurRelativeScaleValue;    // 绝对移动缩放比
    qreal m_dPreRelativeScaleValue;
    // 缩放后外接矩形宽高
    qreal m_dDragScaleBoundWidth;
    qreal m_dDragScaleBountHeight;
    //
    qreal m_dPreRelativeAngeleValue;    // 前值
    qreal m_dCurRelativeAngleValue; // 累积移动绝对弧度
};
#endif // QTMPLAYERGRAPHCONTROLITEM_H
