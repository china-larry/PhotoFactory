#include "QPhotoDragListWidget.h"

#include <QDebug>
#include <QMimeData>
#include <QDrag>
#include <QFileInfo>
QPhotoDragListWidget::QPhotoDragListWidget(int iIconSize, QWidget *parent)
    : QListWidget(parent)
    , m_iIconSize(iIconSize)
{    
    setViewMode(QListView::IconMode);
    setResizeMode(QListView::Adjust);
    setFlow(QListView::LeftToRight);
    setIconSize(QSize(m_iIconSize, m_iIconSize-15));// 20为文字高度空间
    setSpacing(10);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    // 拖拽
    setDragEnabled(true);
    setAcceptDrops(false);
    setDropIndicatorShown(false);
}

void QPhotoDragListWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_pointPressPos = event->pos();
    }
    QListWidget::mousePressEvent(event);
}

void QPhotoDragListWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        int iDestance = (event->pos() - m_pointPressPos).manhattanLength();
        if(iDestance >= QApplication::startDragDistance())
        {
            startDrag();
        }
    }
    QListWidget::mouseMoveEvent(event);
}

void QPhotoDragListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("photo/x-imetto-compose"))
    {
        event->accept();
    }
    else
        event->ignore();
}

void QPhotoDragListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("photo/x-imetto-compose"))
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else
        event->ignore();
}

// 自身控件drop
//void QPhotoDragListWidget::dropEvent(QDropEvent *event)
//{
//    if(event->mimeData()->hasFormat("photo/x-imetto-compose"))
//    {
//        QByteArray pieceData = event->mimeData()->data("photo/x-imetto-compose");
//        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
//        QPixmap pixmap;
//        QString strName;
//        dataStream >> pixmap >> strName;

//        // 响应拖拽进来的事件
//        QPoint point = event->pos();
//        QListWidgetItem *item = itemAt(point);

//        insertPhotoItem(row(item), pixmap, strName);
//        //
//        event->setDropAction(Qt::MoveAction);
//        event->accept();
//    }
//    else
//    {
//        event->ignore();
//    }
//}
// 默认大小
QSize QPhotoDragListWidget::sizeHint() const
{
    return QSize(300, 400);
}

void QPhotoDragListWidget::addPhotoItem(QPixmap pixmap, QString qstrPath)
{
    QListWidgetItem *item = new QListWidgetItem(this);
    item->setTextAlignment(Qt::AlignCenter);
    item->setSizeHint(QSize(m_iIconSize, m_iIconSize));    
    item->setIcon(QIcon(pixmap));
    // 获取文件名
    QFile photoFile(qstrPath);
    QFileInfo photoInfo(photoFile);
    QString qstrName = photoInfo.baseName();
    item->setText(qstrName);

    // 拖拽数据
    item->setData(Qt::UserRole, QVariant(pixmap));
    item->setData(Qt::UserRole+1, qstrName);
    item->setData(Qt::UserRole+2, qstrPath);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable
                        | Qt::ItemIsDragEnabled);
}

void QPhotoDragListWidget::insertPhotoItem(int row, QPixmap pixmap, QString qstrPath)
{
    QListWidgetItem *item = new QListWidgetItem(this);
    item->setTextAlignment(Qt::AlignCenter);
    item->setSizeHint(QSize(m_iIconSize, m_iIconSize));
    item->setIcon(QIcon(pixmap));
    // 获取文件名
    QFile photoFile(qstrPath);
    QFileInfo photoInfo(photoFile);
    QString qstrName = photoInfo.baseName();
    item->setText(qstrName);

    // 拖拽数据
    item->setData(Qt::UserRole, QVariant(pixmap));
    item->setData(Qt::UserRole+1, qstrName);
    item->setData(Qt::UserRole+2, qstrPath);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable
                        | Qt::ItemIsDragEnabled);

    insertItem(row, item);
}

void QPhotoDragListWidget::deletePhotoItem(int iRow)
{
    delete takeItem(iRow);
}

void QPhotoDragListWidget::startDrag()
{
    QListWidgetItem *item = currentItem();
    if(item == NULL) return;

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    QPixmap pixmap = qvariant_cast<QPixmap>(item->data(Qt::UserRole));
    QString qstrName = item->data(Qt::UserRole+1).toString();
    QString qstrPath = item->data(Qt::UserRole+2).toString();
    //
    dataStream << pixmap << qstrName << qstrPath;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("photo/x-imetto-compose", itemData);

    //
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    // 计算缩放时候缩放图片比例
    float fPixmapWidth = pixmap.width();
    float fPixmapHeight = pixmap.height();
//    float fViewImageScaleX = fPixmapWidth / m_iIconSize;
//    float fViewImageScsleY = fPixmapHeight / (m_iIconSize-15);
//    float fViewImageScale = fViewImageScaleX > fViewImageScsleY ? fViewImageScaleX : fViewImageScsleY;
//    fViewImageScale = 1.0f /fViewImageScale;
//    fPixmapWidth *= fViewImageScale;
//    fPixmapHeight *= fViewImageScale;
//    drag->setPixmap(pixmap.scaled(fPixmapWidth, fPixmapHeight));
    drag->setPixmap(pixmap);
    //
    QPoint hotPoint = drag->hotSpot();
    drag->setHotSpot(hotPoint + QPoint(fPixmapWidth / 2.0f, fPixmapHeight / 2.0f));

    if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
    {
 //       delete takeItem(row(item));
    }
}
