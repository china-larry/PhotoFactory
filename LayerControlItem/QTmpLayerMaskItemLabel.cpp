#include "QTmpLayerMaskItemLabel.h"

QTmpLayerMaskItemLabel::QTmpLayerMaskItemLabel(QWidget *parent)
    : QWidget(parent)
    , m_pPixmap(NULL)
{
    resize(50, 35);
    setMinimumWidth(50);
    setCursor(Qt::PointingHandCursor);
    m_rectSelect = QRect(0, 0, 49, 31);
    m_bSelectMask = false;
//    setAlignment(Qt::AlignCenter);
}

void QTmpLayerMaskItemLabel::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    if(m_pPixmap != NULL)
    {
        painter.drawPixmap(m_rectPixmap, *m_pPixmap);
    }
    if(m_bSelectMask)
    {
        painter.setPen(QColor(Qt::red));
        painter.drawRect(m_rectSelect);
    }
}

void QTmpLayerMaskItemLabel::mousePressEvent(QMouseEvent *ev)
{
    m_bSelectMask = true;
    emit mousePressMask();
}

void QTmpLayerMaskItemLabel::SetPixmap(QPixmap *pPixmap)
{
    if(pPixmap == NULL)
    {
        return;
    }
    // 计算缩放因子
    float fGraphicsViewX = 50.0f;
    float fGraphicsViewY = 35.0f;
    // 预览图缩放因子
    float fSrcImageWidth = pPixmap->width();
    float fSrcImageHeight = pPixmap->height();
    float fViewImageScaleX = fSrcImageWidth / fGraphicsViewX;
    float fViewImageScsleY = fSrcImageHeight / fGraphicsViewY;
    float fViewImageScale = fViewImageScaleX > fViewImageScsleY ? fViewImageScaleX : fViewImageScsleY;
    fViewImageScale = 1.0f / fViewImageScale;

    int iScaleImageWidth = fSrcImageWidth*fViewImageScale;
    int iScaleImageHeight = fSrcImageHeight*fViewImageScale;
    m_pPixmap = pPixmap;

    m_rectPixmap.setLeft((this->rect().width() - iScaleImageWidth) / 2);
    m_rectPixmap.setTop((this->rect().height() - iScaleImageHeight) / 2);
    m_rectPixmap.setWidth(iScaleImageWidth);
    m_rectPixmap.setHeight(iScaleImageHeight);
}

void QTmpLayerMaskItemLabel::SetSelectMask(bool bSelectMask)
{
    m_bSelectMask = bSelectMask;
    this->update();
}
