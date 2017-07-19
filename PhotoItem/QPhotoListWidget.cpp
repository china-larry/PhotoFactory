#include "QPhotoListWidget.h"
#include <QDebug>
#include <QMimeData>
#include <QDrag>
QPhotoListWidget::QPhotoListWidget(int iIconSize, QWidget *parent)
    : QListWidget(parent)
    , m_iIconSize(iIconSize)
{
    setViewMode(QListView::IconMode);
    setResizeMode(QListView::Adjust);
    setFlow(QListView::LeftToRight);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setIconSize(QSize(m_iIconSize, m_iIconSize-15));// 20为文字高度空间
    setSpacing(10);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

void QPhotoListWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
    {// 删除相片动作
        // 数据层删除
        QList<int> selectIndexList;
        // 视图删除
        QList<QListWidgetItem*> selectItemsList = selectedItems();
        for(int i = 0; i < selectItemsList.size(); ++i)
        {
            selectIndexList.push_back(row(selectItemsList[i]));
            delete selectItemsList[i];
        }
        emit deletePhoto(selectIndexList);
    }
    // ctrl + a 全选
    if(event->modifiers() == Qt::ControlModifier)
    {
        if(event->key() == Qt::Key_A)
        {
            selectAll();
        }
    }
}
// 默认大小
QSize QPhotoListWidget::sizeHint() const
{
    return QSize(600, 150);
}

void QPhotoListWidget::addPhotoItem(QPixmap pixmap, QString strName)
{
    QListWidgetItem *item = new QListWidgetItem(this);
    item->setTextAlignment(Qt::AlignCenter);
    item->setSizeHint(QSize(m_iIconSize, m_iIconSize));
    item->setText(strName);
    item->setIcon(QIcon(pixmap));
    item->setData(Qt::UserRole, QVariant(pixmap));
    item->setData(Qt::UserRole+1, strName);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable
                        | Qt::ItemIsDragEnabled);
}

void QPhotoListWidget::insertPhotoItem(int row, QPixmap pixmap, QString strName)
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setTextAlignment(Qt::AlignCenter);
    item->setSizeHint(QSize(m_iIconSize, m_iIconSize));
    item->setIcon(QIcon(pixmap));
    item->setText(strName);
    item->setData(Qt::UserRole, QVariant(pixmap));
    item->setData(Qt::UserRole+1, strName);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable
                        | Qt::ItemIsDragEnabled);

    insertItem(row, item);
}

