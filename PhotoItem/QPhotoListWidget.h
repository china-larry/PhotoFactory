// 不可拖拽photo控件，用于首页初始化选片
/*
 * 自定义ListWidget，存放可以拖拽的图片List
 *
 */
#ifndef QPHOTOLISTWIDGET_H
#define QPHOTOLISTWIDGET_H

#include <QListWidget>
#include <QMouseEvent>
#include <QApplication>

class QPhotoListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit QPhotoListWidget(int iIconSize, QWidget *parent = 0);

protected:
    void keyPressEvent(QKeyEvent *event);
    QSize sizeHint() const;
public:
    void addPhotoItem(QPixmap pixmap, QString strName);
    void insertPhotoItem(int row, QPixmap pixmap, QString strName);

signals:
    void deletePhoto(QList<int>);

private:
    QPoint m_pointPressPos;
    int m_iIconSize;

};


#endif // QPHOTOLISTWIDGET_H
