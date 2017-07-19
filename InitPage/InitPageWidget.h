/**
 * 初始化页面，用于选客照
 *
 */
#ifndef INITPAGEWIDGET_H
#define INITPAGEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>

#include "PhotoItem/QPhotoListWidget.h"

class InitPageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InitPageWidget(QWidget *parent = 0);    

private:
    void InitPhotoViewWidget();
    void InitLayout();

private slots:
    // 删除相片文件
    void delectPhotoFilesSlot(QList<int>);
signals:
    void deletePhotoFilesEmit(QList<int>);

public:
    int getPhotoCount();
    void updateOpenPhotoPixmap(int iIndex, QList<QPixmap *> &listPhotoPixmap, QStringList &listPhotoName);
private:
    // 相片模块
    QPhotoListWidget *m_pPhotoViewListWidget;    
    // 数据层
    QList<QPixmap*> m_listPhotoPixmap;  // 相册数据引用
};

#endif // INITPAGEWIDGET_H
