#include "QGraphDragView.h"

#include <QMimeData>
#include <QDrag>
#include <QDebug>

QGraphDragView::QGraphDragView(QWidget *parent) :
    QGraphicsView(parent)
{
    //
    setCacheMode(QGraphicsView::CacheBackground);
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);

    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::TextAntialiasing, false);

    setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);

    setDragMode(QGraphicsView::RubberBandDrag);
    setRubberBandSelectionMode(Qt::ContainsItemBoundingRect);
    //

    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    // 禁用滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 拖拽
 //   setDragEnabled(false);
    setAcceptDrops(true);
//    setDropIndicatorShown(true);
}


// 自身控件drop
void QGraphDragView::dropEvent(QDropEvent *event)
{
    qDebug() << "vie drop event";
    if(event->mimeData()->hasFormat("photo/x-imetto-compose"))
    {

        QByteArray pieceData = event->mimeData()->data("photo/x-imetto-compose");
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
        QPixmap pixmap;
        QString qstrName, qstrPath;
        dataStream >> pixmap >> qstrName >> qstrPath;
        // 响应拖拽进来的事件
        emit sig_dropPhotoPath(qstrPath);
        qDebug() << "phot format ok" << qstrPath;
        //
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}
