/**
 * 自定义场景视图。接受拖拽数据Data
 *
 */
#ifndef QGRAPHDRAGVIEW_H
#define QGRAPHDRAGVIEW_H

#include <QGraphicsView>
#include <QDropEvent>

class QGraphDragView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit QGraphDragView(QWidget *parent = 0);
protected:
    void dropEvent(QDropEvent *event);
signals:
    void sig_dropPhotoPath(QString);
public slots:

private:
    QString m_qstrPhotoPath;    // photo路径
};

#endif // QGRAPHDRAGVIEW_H
