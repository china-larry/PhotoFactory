#ifndef QTMPLAYERMASKITEMLABEL_H
#define QTMPLAYERMASKITEMLABEL_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>

class QTmpLayerMaskItemLabel : public QWidget
{
    Q_OBJECT
public:
    QTmpLayerMaskItemLabel(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
  //  void mouseMoveEvent(QMouseEvent *ev);
   // void mouseReleaseEvent(QMouseEvent *ev);
signals:
    void mousePressMask();      // 绘制模板
public:
    void SetPixmap(QPixmap *pPixmap);
    void SetSelectMask(bool bSelectMask);

private:
    QPixmap *m_pPixmap;
    bool m_bSelectMask;     // 选中绘制蒙版
    QRect m_rectSelect;     // 选中红色框矩形框，小于控件矩形如干个小像素
    QRect m_rectPixmap;     // pixmap绘制矩形，居中绘制

};

#endif // QTMPLAYERMASKITEMLABEL_H
