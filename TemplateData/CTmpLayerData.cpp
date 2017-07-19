#include "CTmpLayerData.h"
#include <QDebug>

// 色彩调整层全局Pximap
extern QPixmap *g_pNormalPixmap;
extern QPixmap *g_pLevelsPixmap;
extern QPixmap *g_pCurvesPixmap;
extern QPixmap *g_pBrightContrastPixmap;
extern QPixmap *g_pColorBalancePixmap;
extern QPixmap *g_pHueSaturationPixmap;
extern QPixmap *g_pSelectiveColorPixmap;
extern QPixmap *g_pGradientMapPixmap;
extern QPixmap *g_pOthersPixmap;

extern QString g_qstrTmpParsePath;

// 色彩调整层枚举
enum color_adjust{
    psd_layer_type_null = -1,                     // 无色彩调整层
    psd_layer_type_normal = 0,                  // 正常
    psd_layer_type_hidden,
    psd_layer_type_folder,
    psd_layer_type_solid_color,
    psd_layer_type_gradient_fill,
    psd_layer_type_pattern_fill,
    psd_layer_type_levels,                          // 色阶
    psd_layer_type_curves,                         // 曲线
    psd_layer_type_brightness_contrast,       // 亮度对比度
    psd_layer_type_color_balance,                // 色彩平衡
    psd_layer_type_hue_saturation,              // 色相/饱和度
    psd_layer_type_selective_color,              // 可选颜色
    psd_layer_type_threshold,                      // 阈值
    psd_layer_type_invert,                           // 反相
    psd_layer_type_posterize,                      // 色调分离
    psd_layer_type_channel_mixer,               //
    psd_layer_type_gradient_map,                // 渐变
    psd_layer_type_photo_filter,                   //
};

CTmpLayerData::CTmpLayerData()
{
    m_pLayerPixmap = NULL;
    m_pMaskPixmap = NULL;
    m_ptPhotoMoveGap = QPointF(0, 0);
    m_fPhotoScaleValue = 1.0f;
    m_fPhotoRotateValue = 0.0f;
    m_ptLayerMoveGap = QPointF(0, 0);
    m_fLayerScaleValue = 1.0f;
    m_fLayerRotateValue = 0.0f;
    m_rectPixmap = QRectF(0, 0, 0, 0);
    m_bMaskFlag = false;
}

CTmpLayerData::~CTmpLayerData()
{
    if(m_pLayerPixmap != NULL)
    {
        delete m_pLayerPixmap;
        m_pLayerPixmap = NULL;
    }
    if(m_pMaskPixmap != NULL)
    {
        delete m_pMaskPixmap;
        m_pMaskPixmap = NULL;
    }
}

void CTmpLayerData::setLayerID(QString qstrLayerID)
{
    m_qstrLayerID = qstrLayerID;
}

void CTmpLayerData::setBlandMode(int iBlandMode)
{
    m_iBlandMode = iBlandMode;
}

void CTmpLayerData::setClipp(int iClipp)
{
    m_iClipp = iClipp;
}

void CTmpLayerData::setBeClipped(int iBeClipped)
{
    m_iBeClipped = iBeClipped;
}

void CTmpLayerData::setColorAdjust(int iColorAdjust)
{
    m_iColorAdjust = iColorAdjust;
}

void CTmpLayerData::setWidth(int iWidth)
{
    m_iWidth = iWidth;
}

void CTmpLayerData::setHeight(int iHeight)
{
    m_iHeight = iHeight;
}

void CTmpLayerData::setLayerName(QString qstrLayerName)
{
    m_qstrLayerName = qstrLayerName;
}

void CTmpLayerData::setMaskWidth(int iWidth)
{
    m_iMaskWidth = iWidth;
}

void CTmpLayerData::setMaskHeight(int iHeight)
{
    m_iMaskHeight = iHeight;
}

void CTmpLayerData::setMaskOffsetX(int iOffsetX)
{
    m_iMaskOffsetX = iOffsetX;
}

void CTmpLayerData::setMaskOffsetY(int iOffsetY)
{
    m_iMaskOffsetY = iOffsetY;
}

void CTmpLayerData::setMaskSite(QString qstrMaskSite)
{
    m_qstrMaskSite = qstrMaskSite;
}

void CTmpLayerData::setOffsetX(int iOffsetX)
{
    m_iOffsetX = iOffsetX;
}

void CTmpLayerData::setOffsetY(int iOffsetY)
{
    m_iOffsetY = iOffsetY;
}

void CTmpLayerData::setOpacity(int iOpacity)
{
    m_iOpacity = iOpacity;
}

void CTmpLayerData::setPictureSite(QString qstrPictureSite)
{
    m_qstrPictureSite = qstrPictureSite;
}

void CTmpLayerData::setMaskSiteTemp(QString qstrMaskSiteTemp)
{
    m_qstrMaskSiteTemp = qstrMaskSiteTemp;
}

void CTmpLayerData::setPictureSiteTemp(QString qstrPictureSiteTemp)
{
    m_qstrPictureSiteTemp = qstrPictureSiteTemp;    
}

void CTmpLayerData::setType(int iType)
{
    m_iType  = iType;
}

void CTmpLayerData::setVisable(int iVisable)
{
    m_iVisable = iVisable;
}

void CTmpLayerData::setPotoVisable(int iVisable)
{
    m_iPotoVisable = iVisable;
}

void CTmpLayerData::SetMaskFlag(bool bMask)
{
    m_bMaskFlag = bMask;
}

void CTmpLayerData::SetMaskPhotoPath(QString strMaskPhotoPath)
{
    if(strMaskPhotoPath == "")
    {// 不存在蒙版
        m_bMaskFlag = false;
        return;
    }
    if(m_pMaskPixmap != NULL)
    {
        delete m_pMaskPixmap;
        m_pMaskPixmap = NULL;
    }
    //
    QPixmap pixmap(strMaskPhotoPath);
    if(pixmap.width() == 0)
    {
        m_bMaskFlag = false;
        return;
    }

    m_pMaskPixmap = new QPixmap(pixmap.width(), pixmap.height());
    *m_pMaskPixmap = pixmap.copy(0, 0, pixmap.width(), pixmap.height());
    m_bMaskFlag = true;
}

void CTmpLayerData::SetMaskPhotoPixmap(const QPixmap *pPixmap)
{
    if(pPixmap == NULL || pPixmap == m_pMaskPixmap)
    {
        qDebug() << "same pixmap";
        return;
    }
    //
    if(m_pMaskPixmap != NULL)
    {
        delete m_pMaskPixmap;
    }
    m_pMaskPixmap = new QPixmap(pPixmap->width(), pPixmap->height());
    *m_pMaskPixmap = pPixmap->copy(0, 0, pPixmap->width(), pPixmap->height());
    m_bMaskFlag = true;
}

void CTmpLayerData::SetPhotoPath(QString strPhotoPath)
{
    if(m_pLayerPixmap != NULL)
    {
        delete m_pLayerPixmap;
    }
    //
    QPixmap pixmap(strPhotoPath);
    // 计算缩放因子
    float fGraphicsViewX = 50.0f;
    float fGraphicsViewY = 35.0f;
    // 预览图缩放因子
    float fSrcImageWidth = pixmap.width();
    float fSrcImageHeight = pixmap.height();
    float fViewImageScaleX = fSrcImageWidth / fGraphicsViewX;
    float fViewImageScsleY = fSrcImageHeight / fGraphicsViewY;
    float fViewImageScale = fViewImageScaleX > fViewImageScsleY ? fViewImageScaleX : fViewImageScsleY;
    fViewImageScale = 1.0f / fViewImageScale;

    m_pLayerPixmap = new QPixmap(50, 35);
    *m_pLayerPixmap = QPixmap(strPhotoPath).scaled(fSrcImageWidth*fViewImageScale, fSrcImageHeight*fViewImageScale, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    m_rectPixmap.setWidth(fSrcImageWidth);
    m_rectPixmap.setHeight(fSrcImageHeight);
}

void CTmpLayerData::SetPhotoMoveGap(QPointF pointMoveGap)
{
    m_ptPhotoMoveGap = pointMoveGap;
}

void CTmpLayerData::SetPhotoScaleValue(float fScaleValue)
{
    m_fPhotoScaleValue = fScaleValue;
}

void CTmpLayerData::SetPhotoRotateValue(float fRotateValue)
{
    m_fPhotoRotateValue = fRotateValue;
}

void CTmpLayerData::SetLayerMoveGap(QPointF pointMoveGap)
{
    m_ptLayerMoveGap = pointMoveGap;
}

void CTmpLayerData::SetLayerScaleValue(float fScaleValue)
{
    m_fLayerScaleValue = fScaleValue;
}

void CTmpLayerData::SetLayerRotateValue(float fRotateValue)
{
    m_fLayerRotateValue = fRotateValue;
}

QString CTmpLayerData::getLayerID()
{
    return m_qstrLayerID;
}

int CTmpLayerData::getBlandMode()
{
    return m_iBlandMode;
}

int CTmpLayerData::getClipp()
{
    return m_iClipp;
}

int CTmpLayerData::getBeClipped()
{
    return m_iBeClipped;
}

int CTmpLayerData::getColorAdjust()
{
    return m_iColorAdjust;
}

int CTmpLayerData::getWidth()
{
    return m_iWidth;
}

int CTmpLayerData::getHeight()
{
    return m_iHeight;
}

QString CTmpLayerData::getLayerName()
{
    return m_qstrLayerName;
}

bool CTmpLayerData::getMaskFlag()
{
    return m_bMaskFlag;
}

int CTmpLayerData::getMaskWidth()
{
    return m_iMaskWidth;
}

int CTmpLayerData::getMaskHeight()
{
    return m_iMaskHeight;
}

int CTmpLayerData::getMaskOffsetX()
{
    return m_iMaskOffsetX;
}

int CTmpLayerData::getMaskOffsetY()
{
    return m_iMaskOffsetY;
}

QString CTmpLayerData::getMaskSite()
{
    return m_qstrMaskSite;
}

int CTmpLayerData::getOffsetX()
{
    return m_iOffsetX;
}

int CTmpLayerData::getOffsetY()
{
    return m_iOffsetY;
}

int CTmpLayerData::getOpacity()
{
    return m_iOpacity;
}

QString CTmpLayerData::getPictureSite()
{
    return m_qstrPictureSite;
}

QString CTmpLayerData::getMaskSiteTemp()
{
    return m_qstrMaskSiteTemp;
}

QString CTmpLayerData::getPictureSiteTemp()
{
    return m_qstrPictureSiteTemp;
}

QPixmap *CTmpLayerData::getMaskPixmap()
{
    return m_pMaskPixmap;
}

QPixmap *CTmpLayerData::getLayerPixmap()
{
    switch (m_iColorAdjust) {
    case psd_layer_type_null:
        return m_pLayerPixmap;
    case psd_layer_type_normal:
        return g_pNormalPixmap;
    case psd_layer_type_hidden:
    case psd_layer_type_folder:
    case psd_layer_type_solid_color:
    case psd_layer_type_gradient_fill:
    case psd_layer_type_pattern_fill:
         return g_pOthersPixmap;
    case psd_layer_type_levels:
         return g_pLevelsPixmap;
    case psd_layer_type_curves:
         return g_pCurvesPixmap;
    case psd_layer_type_brightness_contrast:
         return g_pBrightContrastPixmap;
    case psd_layer_type_color_balance:
         return g_pColorBalancePixmap;
    case psd_layer_type_hue_saturation:
         return g_pHueSaturationPixmap;
    case psd_layer_type_selective_color:
         return g_pSelectiveColorPixmap;
    case psd_layer_type_threshold:
    case psd_layer_type_invert:
    case psd_layer_type_posterize:
    case psd_layer_type_channel_mixer:
         return g_pOthersPixmap;
    case psd_layer_type_gradient_map:
         return g_pGradientMapPixmap;
    case psd_layer_type_photo_filter:
         return g_pOthersPixmap;
    default:
         return m_pLayerPixmap;
    }
}

int CTmpLayerData::getType()
{
    return m_iType;
}

int CTmpLayerData::getVisable()
{
    return m_iVisable;
}

int CTmpLayerData::getPotoVisable()
{
    return m_iPotoVisable;
}

QPointF CTmpLayerData::GetPhotoMoveGap()
{
    return m_ptPhotoMoveGap;
}

float CTmpLayerData::GetPhotoScaleValue()
{
    return m_fPhotoScaleValue;
}

float CTmpLayerData::GetPhotoRotateValue()
{
    return m_fPhotoRotateValue;
}

QPointF CTmpLayerData::GetLayerMoveGap()
{
    return m_ptLayerMoveGap;
}

float CTmpLayerData::GetLayerScaleValue()
{
    return m_fLayerScaleValue;
}

float CTmpLayerData::GetLayerRotateValue()
{
    return m_fLayerRotateValue;
}

QRectF CTmpLayerData::GetPixmapRect()
{
    return m_rectPixmap;
}


