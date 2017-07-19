#include "QPhotoItemIcon.h"
#include <QDebug>

QPhotoItemIcon::QPhotoItemIcon(QSize iconSize)
    : m_iconSize(iconSize)
{
    m_pPhotoPixmap = NULL;
    m_pPen = new QPen;
    m_pPen->setColor(QColor(255, 0, 0));

    m_numberSize.setWidth(20);
    m_numberSize.setHeight(20);

    m_qstrPhotoNumber = QString::number(1);
    m_bPhotoNumber = false;
}

QPhotoItemIcon::~QPhotoItemIcon()
{
    if(m_pPen != NULL)
    {
        delete m_pPen;
        m_pPen = NULL;
    }
}

void QPhotoItemIcon::paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state)
{
    painter->setPen(*m_pPen);

    m_rectPhotoPixmap.setLeft((rect.width() - m_pixmapSize.width())  / 2.0f + rect.left());
    m_rectPhotoPixmap.setTop((rect.height() - m_pixmapSize.height()) / 2.0f + rect.top());
    m_rectPhotoPixmap.setWidth(m_pixmapSize.width());
    m_rectPhotoPixmap.setHeight(m_pixmapSize.height());

    if(m_pPhotoPixmap != NULL)
    {
        painter->drawPixmap(m_rectPhotoPixmap, *m_pPhotoPixmap);
    }
    // 写序号
    if(m_bPhotoNumber)
    {
        m_rectPhotoNumber = rect;
        m_rectPhotoNumber.setWidth(m_numberSize.width());
        m_rectPhotoNumber.setHeight(m_numberSize.height());

        painter->drawText(m_rectPhotoNumber, m_qstrPhotoNumber);
    }
 //   painter->drawRect(rect);
}

QSize QPhotoItemIcon::actualSize(const QSize &size, QIcon::Mode mode, QIcon::State state)
{
    return m_iconSize;
}


void QPhotoItemIcon::setPhotoPixmap(QPixmap *pPixmap)
{
    m_pPhotoPixmap = pPixmap;
    // 默认绘制位置信息
    float fGeometryWidth = m_iconSize.width();
    float fGeometryHeight = m_iconSize.height();
    // 计算绘制图像位置，比例缩放
    float fPixmapWidth = m_pPhotoPixmap->width();
    float fPixmapHeight = m_pPhotoPixmap->height();
    float fViewImageScaleX = fPixmapWidth / fGeometryWidth;
    float fViewImageScsleY = fPixmapHeight / fGeometryHeight;
    float fViewImageScale = fViewImageScaleX > fViewImageScsleY ? fViewImageScaleX : fViewImageScsleY;
    fViewImageScale = 1.0f /fViewImageScale;

    m_pixmapSize.setWidth(fPixmapWidth*fViewImageScale);
    m_pixmapSize.setHeight(fPixmapHeight*fViewImageScale);
}

void QPhotoItemIcon::setPhotoNumberVisable(bool bVisable)
{
    m_bPhotoNumber = bVisable;
}

void QPhotoItemIcon::setPhotoNumber(int iNumber)
{
    m_qstrPhotoNumber = QString::number(iNumber);
}

int QPhotoItemIcon::getPhotoNumber()
{
    return m_qstrPhotoNumber.toInt();
}

