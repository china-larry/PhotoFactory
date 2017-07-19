#include "QTmpLayerGraphItem.h"
QTmpLayerGraphItem::QTmpLayerGraphItem(QRectF rectPixmap, QRectF rectLayer,
                                       int iLayerType, QString qstrLayerID, QGraphicsItem* parent)
    : QGraphicsObject(parent)
    , m_rectPixmap(rectPixmap)
    , m_rectLayer(rectLayer)
    , m_iLayerType(iLayerType)
    , m_qstrLayerID(qstrLayerID)
    , m_curPointRelativePos(QPointF(0.0f, 0.0f))
{
    calculatePixmapRect();
}
QTmpLayerGraphItem::~QTmpLayerGraphItem()
{
}
QRectF QTmpLayerGraphItem::boundingRect() const
{
    return m_rectLayer;
}
void QTmpLayerGraphItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(m_pImageView != NULL)
    {
        painter->drawPixmap(0, 0, *m_pImageView);
    }
}
void QTmpLayerGraphItem::SetLayerID(QString qstrLayerID)
{
    m_qstrLayerID = qstrLayerID;
}
QString QTmpLayerGraphItem::GetLayerID()
{
    return m_qstrLayerID;
}
void QTmpLayerGraphItem::SetViewPixmap(QPixmap *pPixmap)
{
    m_pImageView = pPixmap;
    this->update(m_rectLayer);
}
QRectF QTmpLayerGraphItem::GetPixmapRect()
{
    return m_rectPixmap;
}

int QTmpLayerGraphItem::GetLayerType()
{
    return m_iLayerType;
}

void QTmpLayerGraphItem::calculatePixmapRect()
{
    if(m_iLayerType)
    {
        qreal fPixmapWidth = m_rectPixmap.width();
        qreal fPixmapHeight = m_rectPixmap.height();
        qreal fLayerWidth = m_rectLayer.width();
        qreal fLayerHeight = m_rectLayer.height();
        if(fPixmapWidth < fLayerWidth || fPixmapHeight < fLayerHeight)
        {// 图片小于图层，会缩放处理
            fLayerWidth += 20.0d;
            fLayerHeight += 20.0d;
            qreal fViewImageScaleX = fLayerWidth / fPixmapWidth;
            qreal fViewImageScsleY = fLayerHeight / fPixmapHeight;
            qreal fViewImageScale = fViewImageScaleX > fViewImageScsleY ? fViewImageScaleX : fViewImageScsleY;
            //
            qreal fPixmapScaleWidth = fPixmapWidth * fViewImageScale;
            qreal fPixmapScaleHeight = fPixmapHeight * fViewImageScale;
            m_rectPixmap.setLeft(m_rectLayer.left() - (fPixmapScaleWidth - m_rectLayer.width()) / 2.0f);
            m_rectPixmap.setTop(m_rectLayer.top() - (fPixmapScaleHeight - m_rectLayer.height()) / 2.0f);
            m_rectPixmap.setWidth(fPixmapScaleWidth);
            m_rectPixmap.setHeight(fPixmapScaleHeight);
        }
        else
        {// 客照大于图层，默认为中心放置
            m_rectPixmap.setLeft(m_rectLayer.left() - (fPixmapWidth - fLayerWidth) / 2.0f);
            m_rectPixmap.setTop(m_rectLayer.top() - (fPixmapHeight - fLayerHeight) / 2.0f);
            m_rectPixmap.setWidth(fPixmapWidth);
            m_rectPixmap.setHeight(fPixmapHeight);
        }
    }
    else
    {
        m_rectPixmap = m_rectLayer;
    }
}





