/**
 * PSD模板属性类
 *
 */
#ifndef CPSDTEMPLATE_H
#define CPSDTEMPLATE_H
/**
 * PSD模板类
 *
 * liuqing 2012-12-24
 */
#include <QList>
#include <QPixmap>
#include <QString>
#include <QFileInfo>
#include <QVariantMap>
#include <QDebug>


#include "CTmpLayerData.h"

class CPsdTemplate
{
public:
    CPsdTemplate();
    ~CPsdTemplate();     
    /**
     * @brief CPsdTemplate
     * @param qstrPsdFilePath为解析后的PSD文件路径，通过此路径获得预览图片，
     * json描述文件，和图层图片数据
     * 2012-12-25暂时此路径为不可变默认死路径F:\imetto profession\thumbnail.jpg
     * 后期会修改此项，且模板名称不固定为随机系统生成项（md5值）
     */
    CPsdTemplate(QString qstrPsdFilePath);

public:
    /*
     * 返回图层数据数组的一个引用，对象存在，为有效引用
     */
    QList<CTmpLayerData*> & GetTmpLayerDataList();   // 已经按照LayerID排序

    // 设置数据
    bool SetTmpLayerVisable(QString qstrLayerID, int iVisable);
    bool SetTmpLayerType(QString qstrLayerID, int iLayerType);
    bool SetTmpLayerPicture(int iLayerID, QString qstrLayerPicture);
    bool SetTmpLayerBlendMode(int iLayerID, int iBlendMode);
    bool SetTmpLayerTransValue(int iLayerID, int iTransValue);
    bool SetTmpLayerMaskPixmap(int iLayerID, const QPixmap* pPixmap);
    //
    bool SetTmpLayerMoveGap(int iLayerID, QPointF pointMoveGap);
    bool SetTmpLayerScaleValue(int iLayerID, float fScaleValue);
    bool SetTmpLayerRotateValue(int iLayerID, float fRotateValue);

public:
    int GetPsdImageWidth();
    int GetPsdImageHeight();
    QString GetTmpViewPath();
    QString GetTmpParsePath();
    qreal GetPsdScaleValue();

private:
    void setPsdImageWidth(int iWidth);
    void setPsdImageHeight(int iHeight);
    void setTmpViewPath(QString qstrTmpViewPath);
    void setPsdScaleValue(qreal dScaleValue);


private:
    QList<CTmpLayerData*> m_listTmpLayerData; ///< 模板图层数据
    QString m_qstrTmpParsePath;         ///< 模板解析路径
    QString m_qstrTmpViewPath;          ///< 预览图
    QString m_qStrTmpName;              ///< 模板名称
    QString m_qstrTmpDscePath;          ///< 描述文件


    int m_iPsdImageWidthSrc;           ///< 原始PSD文件宽高
    int m_iPsdImageHeightSrc;
    qreal m_dPsdScaleValue;         ///< 使用预览处理数据缩放比率
};

#endif // CPSDTEMPLATE_H
