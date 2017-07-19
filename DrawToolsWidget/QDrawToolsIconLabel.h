#ifndef QDRAWTOOLSICONLABEL_H
#define QDRAWTOOLSICONLABEL_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>

class QDrawToolsIconLabel : public QWidget
{
    Q_OBJECT
public:
    explicit QDrawToolsIconLabel(QWidget *parent = 0);
    
protected:
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);

signals:
    void sigMousePress();
public slots:

public:
    void SetPixmap(QPixmap *pPixmap);
    void SetSelect(bool bSelect);

private:
    QRect m_rectPixmap;
    QPixmap *m_pPixmap;
    bool m_bSelect;
    QRect m_rectSelect;     // 选中红色框矩形框，小于控件矩形如干个小像素
};

#endif // QDRAWTOOLSICONLABEL_H
