#ifndef QPHOTODRAGLISTWIDGET_H
#define QPHOTODRAGLISTWIDGET_H

/*
 * 自定义ListWidget，存放可以拖拽的图片List
 *
 */
#include <QListWidget>
#include <QMouseEvent>
#include <QApplication>


class QPhotoDragListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit QPhotoDragListWidget(int iIconSize, QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
 //   void dropEvent(QDropEvent *event);
    QSize sizeHint() const;

public:
    void addPhotoItem(QPixmap pixmap, QString qstrPath);
    void insertPhotoItem(int row, QPixmap pixmap, QString qstrPath);
    void deletePhotoItem(int iRow);

private:
    void startDrag();
private:
    QPoint m_pointPressPos;
    int m_iIconSize;
    QString m_qstrPhotoPath;    // photo路径
};

#endif // QPHOTODRAGLISTWIDGET_H


