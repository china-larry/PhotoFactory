#include "QDrawToolsIconLabel.h"
#include <QDebug>

QDrawToolsIconLabel::QDrawToolsIconLabel(QWidget *parent)
    : QWidget(parent)
    , m_pPixmap(NULL)
{
   // resize(35, 35);
    setMinimumSize(18, 18);
    setMaximumSize(18, 18);
    setCursor(Qt::PointingHandCursor);
    m_rectSelect = QRect(0, 0, 17, 17);
    m_bSelect = false;
}

void QDrawToolsIconLabel::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    if(m_pPixmap != NULL)
    {
        painter.drawPixmap(m_rectPixmap, *m_pPixmap);
    }

    if(m_bSelect)
    {
        painter.setPen(QColor(Qt::red));
    }
    else
    {
        painter.setPen(QColor(0, 0, 0, 100));
    }
    painter.drawRect(m_rectSelect);
}


void QDrawToolsIconLabel::mousePressEvent(QMouseEvent *ev)
{
    m_bSelect = true;
    emit sigMousePress();
}


void QDrawToolsIconLabel::SetSelect(bool bSelect)
{
    m_bSelect = bSelect;
    this->update();
}


void QDrawToolsIconLabel::SetPixmap(QPixmap *pPixmap)
{
    if(pPixmap == NULL)
    {
        return;
    }
    // 计算缩放因子
    float fGraphicsViewX = 18.0f;
    float fGraphicsViewY = 18.0f;
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

    qDebug() << m_rectPixmap << "lia rect";
}
