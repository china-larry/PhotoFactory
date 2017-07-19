#include "InitPageWidget.h"

#include <QFileDialog>
#include <QGridLayout>
#include <QDirIterator>
#include <QDebug>

#include "PhotoItem/QPhotoItemIcon.h"


InitPageWidget::InitPageWidget(QWidget *parent) :
    QWidget(parent)
{
    // 初始化预览控件列表
     InitPhotoViewWidget();
     // 删除相片
     connect(m_pPhotoViewListWidget, SIGNAL(deletePhoto(QList<int>)), this, SLOT(delectPhotoFilesSlot(QList<int>)));
     // 布局
     InitLayout();
}
// 初始化控件
void InitPageWidget::InitPhotoViewWidget()
{
    // 相片预览控件
    m_pPhotoViewListWidget =  new QPhotoListWidget(150, this);
}
// 布局
void InitPageWidget::InitLayout()
{
    // 相片
    QGridLayout *glPhotoLayout = new QGridLayout;
    glPhotoLayout->addWidget(m_pPhotoViewListWidget,      0, 0, 1, 1);
    //
    setLayout(glPhotoLayout);
}

void InitPageWidget::delectPhotoFilesSlot(QList<int> deleteList)
{
    emit deletePhotoFilesEmit(deleteList);
}

int InitPageWidget::getPhotoCount()
{
    return m_pPhotoViewListWidget->count();
}
// 更新当前相册预览
void InitPageWidget::updateOpenPhotoPixmap(int iIndex, QList<QPixmap *> &listPhotoPixmap, QStringList &listPhotoName)
{
    int iPhotoPixmapListSize = listPhotoPixmap.size();
    if(!iPhotoPixmapListSize) return;
    m_listPhotoPixmap = listPhotoPixmap;
    for(int i = iIndex; i < iPhotoPixmapListSize; ++i)
    {
        // 获取文件名
        QFile photoFile(listPhotoName[i-iIndex]);
        QFileInfo photoInfo(photoFile);
        QString qstrFileName = photoInfo.baseName();
        // 控件数据
        m_pPhotoViewListWidget->addPhotoItem(*listPhotoPixmap[i], qstrFileName);
    }
    this->update();
}
