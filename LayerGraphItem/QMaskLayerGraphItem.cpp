#include "QMaskLayerGraphItem.h"
#include <QDebug>
#include <qmath.h>
QMaskLayerGraphItem::QMaskLayerGraphItem(QRectF rectMask, QGraphicsItem* parent)
    : QGraphicsObject(parent)
    , m_rectMask(rectMask)
    , m_pPen(NULL)
    , m_pBrush(NULL)
    , m_iDrawType(0)
{
    m_pMaskDrawPainter = new QPainter();
    m_pMaskPixmap = NULL;
    QGraphicsItem::setCursor(Qt::ForbiddenCursor);
    //
    m_bStartDraw = false;
}

QMaskLayerGraphItem::~QMaskLayerGraphItem()
{
    if(m_pMaskDrawPainter != NULL)
    {
        delete m_pMaskDrawPainter;
        m_pMaskDrawPainter = NULL;
    }
}

QRectF QMaskLayerGraphItem::boundingRect() const
{
    return m_rectMask;
}

void QMaskLayerGraphItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::white, 3));
    painter->drawRect(m_rectMask);

    if(m_iDrawType)
    {// 画刷
        if(m_bStartDraw)
        {
            painter->drawLine(m_pointPress, m_pointMove);
        }
    }
}

void QMaskLayerGraphItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_bStartDraw = true;
    m_pointMove = m_pointPress = event->pos();
    m_pointRelaMove = m_pointRelaPress = m_pointMove - m_rectMask.topLeft();
    // pixmap绘图
    BeginDrawMaskPicture();
    if(!m_iDrawType)
    {// 画笔，绘制点
        if(m_pPen->brush().style() == Qt::RadialGradientPattern)
        {// 渐变画笔，重置圆心
            QRadialGradient *pRadialGradient = (QRadialGradient *)m_pPen->brush().gradient();
            pRadialGradient->setCenter(m_pointMove);
            pRadialGradient->setFocalPoint(m_pointMove);
            pRadialGradient->setRadius(m_pPen->width() / 2);
            QBrush brush(*pRadialGradient);
            m_pPen->setBrush(brush);
        }
        // pixmap绘制
        m_pMaskDrawPainter->setPen(*m_pPen);
        m_pMaskDrawPainter->drawPoint(m_pointRelaPress);
    }
}

void QMaskLayerGraphItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_bStartDraw = false;
    // 绘制画刷
    if(m_iDrawType)
    {
        m_pMaskDrawPainter->setPen(QPen(QColor(0, 0, 0, 0)));
        // 计算绘制矩形相对坐标
        QRectF rectDraw;
        if(m_pointRelaPress.x() < m_pointRelaMove.x())
        {
            rectDraw.setLeft(m_pointRelaPress.x());
        }
        else
        {
            rectDraw.setLeft(m_pointRelaMove.x());
        }
        if(m_pointRelaPress.y() < m_pointRelaMove.y())
        {
            rectDraw.setTop(m_pointRelaPress.y());
        }
        else
        {
            rectDraw.setTop(m_pointRelaMove.y());
        }
        rectDraw.setWidth(qAbs(m_pointRelaPress.x() - m_pointRelaMove.x()));
        rectDraw.setHeight(qAbs(m_pointRelaPress.y() - m_pointRelaMove.y()));
        // 画刷类型判定画刷距离坐标参数
        if(m_pBrush->style() == Qt::LinearGradientPattern)
        {            
            QLinearGradient *pLinearGradient = (QLinearGradient*)m_pBrush->gradient();
            pLinearGradient->setStart(m_pointRelaPress);
            pLinearGradient->setFinalStop(m_pointRelaMove);
            QBrush brush(*pLinearGradient);
            //
            m_pMaskDrawPainter->setBrush(brush);
            m_pMaskDrawPainter->drawRect(m_rectMask);

        }
        else if(m_pBrush->style() == Qt::RadialGradientPattern)
        {
            QRadialGradient *pRadialGradient = (QRadialGradient*)m_pBrush->gradient();
            pRadialGradient->setCenter(m_pointRelaPress);
            pRadialGradient->setFocalPoint(m_pointRelaPress);
            // 计算圆半径
            qreal dRadialRadius = qPow(qPow(rectDraw.width(), 2.0d) + qPow(rectDraw.height(), 2.0d), 0.5d);
            pRadialGradient->setRadius(dRadialRadius);
            // 绘制圆
            QBrush brush(*pRadialGradient);
            m_pMaskDrawPainter->setBrush(brush);
            m_pMaskDrawPainter->drawEllipse(m_pointRelaPress, dRadialRadius, dRadialRadius);
        }
        else if(m_pBrush->style() == Qt::ConicalGradientPattern)
        {
            QConicalGradient *pConicalGradient = (QConicalGradient*)m_pBrush->gradient();
            pConicalGradient->setCenter(m_pointRelaPress);
            // 计算圆半径
            qreal dRadialRadius = qPow(qPow(rectDraw.width(), 2.0d) + qPow(rectDraw.height(), 2.0d), 0.5d);
            // 计算角度，坐标系不同，注意计算数值正负
            qreal dAngle = qAtan2(m_pointRelaPress.y() - m_pointRelaMove.y(), m_pointRelaMove.x() - m_pointRelaPress.x());
            pConicalGradient->setAngle(dAngle * 180.d / 3.1415926d);
            // 绘制圆
            QBrush brush(*pConicalGradient);
            m_pMaskDrawPainter->setBrush(brush);
            m_pMaskDrawPainter->drawEllipse(m_pointRelaPress, dRadialRadius, dRadialRadius);
        }
        else
        {
        }
    }
    EndDrawMaskPicture();
    update(boundingRect());
}

void QMaskLayerGraphItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{    
    m_pointMove = event->pos();
    m_pointRelaMove = m_pointMove - m_rectMask.topLeft();
    if(m_bStartDraw)
    {
        if(!m_iDrawType)
        {
            // 画笔
            if(m_pPen->brush().style() == Qt::RadialGradientPattern)
            {// 渐变画笔，重置圆心
                QRadialGradient *pRadialGradient = (QRadialGradient *)m_pPen->brush().gradient();
                pRadialGradient->setCenter(m_pointMove);
                pRadialGradient->setFocalPoint(m_pointMove);
                pRadialGradient->setCenterRadius(m_pPen->width());
                QBrush brush(*pRadialGradient);
                m_pPen->setBrush(brush);
            }
            // pixmap绘制
            m_pMaskDrawPainter->setPen(*m_pPen);
       //     m_pMaskDrawPainter->drawLine(m_pointPress, m_pointMove);
            m_pMaskDrawPainter->drawPoint(m_pointRelaMove);
            m_pointPress = m_pointMove;
        }
    }
    update(boundingRect());
}

void QMaskLayerGraphItem::SetMaskPixmap(QPixmap *pPixmap)
{
    if(pPixmap != NULL)
    {
        m_pMaskPixmap = pPixmap;
    }
}

void QMaskLayerGraphItem::BeginDrawMaskPicture()
{
    m_pMaskDrawPainter->begin(m_pMaskPixmap);
}

void QMaskLayerGraphItem::EndDrawMaskPicture()
{
    m_pMaskDrawPainter->end();
    //   m_pMaskPixmap->save("C:\\mask.png");
    emit sigChangePixmap(m_pMaskPixmap);
}

void QMaskLayerGraphItem::SetPen(QPen *pPen)
{
    m_pPen = pPen;
}

void QMaskLayerGraphItem::SetBrush(QBrush *pBrush)
{
    m_pBrush = pBrush;
}

void QMaskLayerGraphItem::SetDrawType(int iType)
{
    m_iDrawType = iType;
}

