// 图层控制List中每个Button中的Label，自定义绘制和鼠标事件
// 主要用于眼睛开关visable和图层类型控制
#ifndef QTMPLAYERITEMLABEL_H
#define QTMPLAYERITEMLABEL_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>

class QTmpLayerItemLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QTmpLayerItemLabel(QWidget *parent=0);
    explicit QTmpLayerItemLabel(int iLabelFlag, QPixmap *pNormal, QPixmap *pActive, QWidget *parent=0);
    ~QTmpLayerItemLabel();
signals:
    void checkLabelStatus(int);
public slots:

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
public:
    void setLabelStatus(int iLabelFlag);
protected:
    void updateLabelStatus();

protected:
    QPixmap *m_pNormalPixmap;
    QPixmap *m_pActivePixmap;
    int m_iLabelFlag;
};


#endif // QTMPLAYERITEMLABEL_H
