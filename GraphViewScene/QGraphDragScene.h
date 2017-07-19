/**
 * 自定义场景类
 *
 */
#ifndef QGRAPHDRAGSCENE_H
#define QGRAPHDRAGSCENE_H

#include <QGraphicsScene>
#include <QDropEvent>
#include "LayerGraphItem/QTmpLayerGraphControlItem.h"
class QGraphDragScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit QGraphDragScene(QObject *parent = 0);
    ~QGraphDragScene();
    
protected:
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
//    void dropEvent(QDropEvent *event);
signals:
//    void sig_dropPhotoPath(QString);
    void sigChangeGraphLayerID(QString);
public slots:
    void onChangeGraphLayerID(QString);
public:
    void AddGraphItem(QTmpLayerGraphItem* pTmpLayerGraphItem);
    QList<QTmpLayerGraphItem*> & GetGraphItemList();
    void SetViewPixmap(QPixmap *pPixmap);       // 设置第一个控件即背景控件pixmap
    QTmpLayerGraphItem* GetGraphItemIndex(int iIndex);
    int GetGraphItemCount();
    void SetGraphItemScale(int iIndex, qreal dScale);
    void SetGraphItemSelect(QString qstrLayerID, bool bSelect);    
    QString GetCurLayerID();
    void SetCurLayerID(QString iLayerID);
private:
    QList<QTmpLayerGraphItem*> m_listTmpLayerGraphItem;
    QString m_qstrCurLayerID;
    QString m_qstrPreLayerID;
};

#endif // QGRAPHDRAGSCENE_H
