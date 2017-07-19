#include "QGraphDragScene.h"
#include <QMimeData>
#include <QDrag>
#include <QGraphicsSceneDragDropEvent>
#include <QDebug>
QGraphDragScene::QGraphDragScene(QObject *parent) :
    QGraphicsScene(parent)
{
//    setAcceptDrops(true);
    //背景
    setBackgroundBrush(QBrush(QColor(255, 0, 0, 200)));
    m_qstrCurLayerID = m_qstrPreLayerID = "0";
}

QGraphDragScene::~QGraphDragScene()
{
    qDeleteAll(m_listTmpLayerGraphItem);
}

void QGraphDragScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->accept();
}

void QGraphDragScene::AddGraphItem(QTmpLayerGraphItem *pTmpLayerGraphItem)
{
    m_listTmpLayerGraphItem.push_back(pTmpLayerGraphItem);
    this->addItem(pTmpLayerGraphItem);
    connect(pTmpLayerGraphItem, SIGNAL(mousePressLayerID(QString)), this, SLOT(onChangeGraphLayerID(QString)));
}

QList<QTmpLayerGraphItem *> &QGraphDragScene::GetGraphItemList()
{
    return m_listTmpLayerGraphItem;
}

void QGraphDragScene::SetViewPixmap(QPixmap *pPixmap)
{
    if(m_listTmpLayerGraphItem.size())
    {
        m_listTmpLayerGraphItem[0]->SetViewPixmap(pPixmap);
    }
}

QTmpLayerGraphItem *QGraphDragScene::GetGraphItemIndex(int iIndex)
{
    if(iIndex >=0 && iIndex < m_listTmpLayerGraphItem.size())
    {
        return m_listTmpLayerGraphItem[iIndex];
    }
    else
    {
        return NULL;
    }
}

int QGraphDragScene::GetGraphItemCount()
{
    return m_listTmpLayerGraphItem.size();
}

void QGraphDragScene::SetGraphItemScale(int iIndex, qreal dScale)
{
    if(iIndex >=0 && iIndex < m_listTmpLayerGraphItem.size())
    {
        m_listTmpLayerGraphItem[iIndex]->setScale(dScale);
    }
}

void QGraphDragScene::SetGraphItemSelect(QString qstrLayerID, bool bSelect)
{
    QList<QGraphicsItem*> itemList = items();
    QList<QGraphicsItem*>::iterator iter = itemList.begin();
    QTmpLayerGraphControlItem *pTmpLayerGraphlItem = (QTmpLayerGraphControlItem*)(*iter);
    for(; iter != itemList.end(); ++iter)
    {
        pTmpLayerGraphlItem = (QTmpLayerGraphControlItem*)(*iter);
        if(pTmpLayerGraphlItem->GetLayerID() == qstrLayerID)
        {
            pTmpLayerGraphlItem->SetLayerSelect(bSelect);
            if(bSelect)
            {
                setFocusItem(pTmpLayerGraphlItem);
                qDebug() << "item focus" << pTmpLayerGraphlItem->hasFocus();
            }
            break;
        }
    }
}

QString QGraphDragScene::GetCurLayerID()
{
    return m_qstrCurLayerID;
}

void QGraphDragScene::SetCurLayerID(QString iLayerID)
{
    if(m_qstrCurLayerID == iLayerID)
    {
        return;
    }
    m_qstrPreLayerID = m_qstrCurLayerID;
    m_qstrCurLayerID = iLayerID;
    SetGraphItemSelect(m_qstrPreLayerID, false);
    SetGraphItemSelect(m_qstrCurLayerID, true);
    update(this->sceneRect());
}

void QGraphDragScene::onChangeGraphLayerID(QString qstrLayerID)
{    
    emit sigChangeGraphLayerID(qstrLayerID);
    SetCurLayerID(qstrLayerID);
}
//void QGraphDragScene::dropEvent(QDropEvent *event)
//{
//    qDebug() << "secdrop event";
//    if(event->mimeData()->hasFormat("photo/x-imetto-compose"))
//    {
//        qDebug() << "phot format ok";
//        QByteArray pieceData = event->mimeData()->data("photo/x-imetto-compose");
//        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
//        QPixmap pixmap;
//        QString qstrName, qstrPath;
//        dataStream >> pixmap >> qstrName >> qstrPath;
//        // 响应拖拽进来的事件
//        emit sig_dropPhotoPath(qstrPath);
//        //
//        event->setDropAction(Qt::MoveAction);
//        event->accept();
//    }
//    else
//    {
//        event->ignore();
//    }
//}


