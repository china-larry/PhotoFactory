/**
 * 自定义绘制图像类，使用必须resize控件大小，设置pixmap和labelname
 *
 */
#ifndef QPHOTOITEMWIDGET_H
#define QPHOTOITEMWIDGET_H


#include <QPixmap>
#include <QPainter>
#include <QRect>
#include <QPen>
#include <QIconEngine>

class QPhotoItemIcon : public QIconEngine
{

public:
    explicit QPhotoItemIcon(QSize iconSize);
    ~QPhotoItemIcon();

signals:
    
public slots:


protected:
    void paint ( QPainter * painter,
            const QRect & rect, QIcon::Mode mode, QIcon::State state );

    QSize	 actualSize ( const QSize & size, QIcon::Mode mode, QIcon::State state );

public:
    void setPhotoPixmap(QPixmap *pPixmap);
    void setPhotoNumberVisable(bool bVisable);
    void setPhotoNumber(int iNumber);

    int getPhotoNumber();
private:
    QPixmap *m_pPhotoPixmap;        // 非自有资源，不需要释放
    QString m_qstrPhotoNumber;      // 被使用次数
    bool m_bPhotoNumber;
    //
    QPen *m_pPen;       // 自有资源，释放
    //
    QRect m_rectPhotoPixmap;
    QRect m_rectPhotoNumber;

    QSize m_iconSize;
    QSize m_pixmapSize;
    QSize m_numberSize;
};

#endif // QPHOTOITEMWIDGET_H
