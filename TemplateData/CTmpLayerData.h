#ifndef CTMPLAYERDATA_H
#define CTMPLAYERDATA_H

/**
 * PSD模板图层数据类
 *
 * liuqing 2012-12-24
 */

#include <QString>
#include <QPixmap>


class CTmpLayerData
{
public:
    CTmpLayerData();
    ~CTmpLayerData();
    bool operator<(const CTmpLayerData& b)const {return m_qstrLayerID.toInt() > b.m_qstrLayerID.toInt();}

public:
    void setLayerID(QString qstrLayerID);
    void setBlandMode(int iBlandMode);
    void setClipp(int iClipp);
    void setBeClipped(int iBeClipped);
    void setColorAdjust(int iColorAdjust);
    void setWidth(int iWidth);
    void setHeight(int iHeight);    
    void setOffsetX(int iOffsetX);
    void setOffsetY(int iOffsetY);
    void setOpacity(int iOpacity);
    void setLayerName(QString qstrLayerName);
    void setMaskWidth(int iWidth);
    void setMaskHeight(int iHeight);
    void setMaskOffsetX(int iOffsetX);
    void setMaskOffsetY(int iOffsetY);
    void setMaskSite(QString qstrMaskSite);
    void setPictureSite(QString qstrPictureSite);
    void setMaskSiteTemp(QString qstrMaskSiteTemp);
    void setPictureSiteTemp(QString qstrPictureSiteTemp);
    void setType(int iType);
    void setVisable(int iVisable);
    void setPotoVisable(int iVisable);
    // 蒙版
    void SetMaskFlag(bool bMask);
    void SetMaskPhotoPath(QString strMaskPhotoPath);
    void SetMaskPhotoPixmap(const QPixmap *pPixmap);
    // 客照
    void SetPhotoPath(QString strPhotoPath);
    void SetPhotoMoveGap(QPointF pointMoveGap);// 移动
    void SetPhotoScaleValue(float fScaleValue);
    void SetPhotoRotateValue(float fRotateValue);
    // 图层
    void SetLayerMoveGap(QPointF pointMoveGap);// 移动
    void SetLayerScaleValue(float fScaleValue);
    void SetLayerRotateValue(float fRotateValue);
    //
    QString getLayerID();
    int getBlandMode();
    int getClipp();
    int getBeClipped();
    int getColorAdjust();
    int getWidth();
    int getHeight();    
    int getOffsetX();
    int getOffsetY();
    int getOpacity();
    QString getLayerName();
    bool getMaskFlag();     // 是否存在蒙版
    int getMaskWidth();
    int getMaskHeight();
    int getMaskOffsetX();
    int getMaskOffsetY();
    QString getMaskSite();
    QString getPictureSite();
    QString getMaskSiteTemp();
    QString getPictureSiteTemp();
    QPixmap* getMaskPixmap();
    QPixmap* getLayerPixmap();
    int getType();
    int getVisable();
    int getPotoVisable();
    // 客照坐标变换
    QPointF GetPhotoMoveGap();
    float GetPhotoScaleValue();
    float GetPhotoRotateValue();
    // 图层坐标变换
    QPointF GetLayerMoveGap();
    float GetLayerScaleValue();
    float GetLayerRotateValue();
    //
    QRectF GetPixmapRect();

private:
    QString m_qstrLayerID;
    int m_iBlandMode;
    int m_iClipp;
    int m_iBeClipped;
    int m_iColorAdjust;
    int m_iWidth;
    int m_iHeight;    
    int m_iOffsetX;
    int m_iOffsetY;
    int m_iOpacity;
    QString m_qstrLayerName;
    bool m_bMaskFlag;
    int m_iMaskWidth;
    int m_iMaskHeight;
    int m_iMaskOffsetX;
    int m_iMaskOffsetY;
    QString m_qstrMaskSite;
    QString m_qstrPictureSite;
    QString m_qstrMaskSiteTemp;
    QString m_qstrPictureSiteTemp;
    QPixmap *m_pMaskPixmap;     // 蒙版
    QPixmap *m_pLayerPixmap;
    QRectF m_rectPixmap;
    int m_iType;
    int m_iVisable;
    int m_iPotoVisable;     // potoshop中是否可视，不可视在不显示在控件层，1不可视，0可视

    // 客照坐标变换
    QPointF m_ptPhotoMoveGap;
    float m_fPhotoScaleValue;
    float m_fPhotoRotateValue;
    // 图层坐标变换
    QPointF m_ptLayerMoveGap;
    float m_fLayerScaleValue;
    float m_fLayerRotateValue;
};

#endif // CTMPLAYERDATA_H
