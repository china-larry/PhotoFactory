#include "CPsdTemplate.h"
#include <QJsonDocument>
//#include <qjson_include\parser.h>
//#include <qjson_include\serializer.h>

CPsdTemplate::CPsdTemplate()
{

}

CPsdTemplate::~CPsdTemplate()
{
    // list释放
    qDeleteAll(m_listTmpLayerData);
    m_listTmpLayerData.clear();
}

/**
 * @brief CPsdTemplate
 * @param qstrPsdFilePath为解析后的PSD文件路径，通过此路径获得预览图片，
 * @json描述文件，和图层图片数据
 */
CPsdTemplate::CPsdTemplate(QString qstrPsdFilePath)
{
    // 获取描述文件
    m_qstrTmpParsePath = qstrPsdFilePath;
    m_qstrTmpDscePath = m_qstrTmpParsePath + "\\psd_release.desc";
    QFile fTmpDescFile(m_qstrTmpDscePath);
    if(!fTmpDescFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    fTmpDescFile.seek(0);
    QByteArray qFileByteArray = fTmpDescFile.readAll();
    //    qt5 qjson
    QJsonDocument jsonDoc = QJsonDocument::fromJson(qFileByteArray);
    QVariantMap result = jsonDoc.toVariant().toMap();
    // qt4.8 libqjson
//    QJson::Parser parser;
//    QVariantMap result = parser.parse(qFileByteArray).toMap();

    // 解析扩展信息
    QVariantMap psdAdditon =  result["psd_addition"].toMap();
    setPsdImageWidth(psdAdditon["srcwidth"].toInt());
    setPsdImageHeight(psdAdditon["srcheight"].toInt());
    setPsdScaleValue(psdAdditon["zoom_rate"].toDouble());
    // 预览图
    m_qstrTmpViewPath = psdAdditon["thumbnail_site"].toString();

    // 解析层数据
    QVariantMap psdDataMap = result["psd"].toMap();
    //
    int iListTmpLayerDataSize = 0;

    foreach(QString qstrLayerID, psdDataMap.keys())
    {
        CTmpLayerData *pTmpLayerData = new CTmpLayerData;
        // layer层ID
        pTmpLayerData->setLayerID(qstrLayerID);
        // layer数据
        QVariantMap layerDataMap = psdDataMap[qstrLayerID].toMap();
        pTmpLayerData->setBlandMode(layerDataMap["blandmode"].toInt());
        pTmpLayerData->setClipp(layerDataMap["clipp"].toInt());
        pTmpLayerData->setBeClipped(layerDataMap["be_clipped"].toInt());
        pTmpLayerData->setColorAdjust(layerDataMap["color_adjust_layer"].toInt());
        pTmpLayerData->setWidth(layerDataMap["width"].toInt());
        pTmpLayerData->setHeight(layerDataMap["height"].toInt());
        pTmpLayerData->setLayerName(layerDataMap["layername"].toString());        
        pTmpLayerData->setOffsetX(layerDataMap["offset_x"].toInt());
        pTmpLayerData->setOffsetY(layerDataMap["offset_y"].toInt());
        pTmpLayerData->setOpacity(layerDataMap["opacity"].toInt());

        pTmpLayerData->setMaskWidth(layerDataMap["mask_width"].toInt());
        pTmpLayerData->setMaskHeight(layerDataMap["mask_height"].toInt());
        pTmpLayerData->setMaskOffsetX(layerDataMap["mask_offset_x"].toInt());
        pTmpLayerData->setMaskOffsetY(layerDataMap["mask_offset_y"].toInt());
        pTmpLayerData->setMaskSite(layerDataMap["mask_site"].toString());
        pTmpLayerData->setPictureSite(layerDataMap["picture_site"].toString());
        QString qstrMaskSiteTemp = layerDataMap["temp_mask_site"].toString();
        pTmpLayerData->setMaskSiteTemp(qstrMaskSiteTemp);
        QString qstrPictureSiteTemp = layerDataMap["temp_picture_site"].toString();
        pTmpLayerData->setPictureSiteTemp(qstrPictureSiteTemp);

        pTmpLayerData->setType(layerDataMap["type"].toInt());        
        int iVisable = layerDataMap["visable"].toInt();
        pTmpLayerData->setVisable(iVisable);
        pTmpLayerData->setPotoVisable(iVisable);
        // 蒙版预览图
        if(qstrMaskSiteTemp == "")
        {// 无蒙版，为空
            pTmpLayerData->SetMaskFlag(false);
        }
        else
        {
            pTmpLayerData->SetMaskFlag(true);
            pTmpLayerData->SetMaskPhotoPath(m_qstrTmpParsePath + qstrMaskSiteTemp);
        }
        // 预览图片
        pTmpLayerData->SetPhotoPath(m_qstrTmpParsePath + qstrPictureSiteTemp);
        //        
        // 从大到小顺序插入
        iListTmpLayerDataSize = m_listTmpLayerData.size();
        for(int i = 0; i <= iListTmpLayerDataSize; ++i)
        {
            if(i == iListTmpLayerDataSize)
            {
                m_listTmpLayerData.push_back(pTmpLayerData);
                break;
            }
            //
            if(m_listTmpLayerData[i]->getLayerID().toInt() < pTmpLayerData->getLayerID().toInt())
            {
                continue;
            }
            else
            {
                m_listTmpLayerData.insert(i, pTmpLayerData);
                break;
            }
        }
    }
}


QList<CTmpLayerData*> &CPsdTemplate::GetTmpLayerDataList()
{
    return m_listTmpLayerData;
}

bool CPsdTemplate::SetTmpLayerVisable(QString qstrLayerID, int iVisable)
{
    int iLayerID = qstrLayerID.toInt();
    if(iLayerID < 0 || iLayerID >= m_listTmpLayerData.size())
    {
        return false;
    }
    m_listTmpLayerData[iLayerID]->setVisable(iVisable);
}

bool CPsdTemplate::SetTmpLayerType(QString qstrLayerID, int iLayerType)
{
    int iLayerID = qstrLayerID.toInt();
    if(iLayerID < 0 || iLayerID >= m_listTmpLayerData.size())
    {
        return false;
    }
    m_listTmpLayerData[iLayerID]->setType(iLayerType);
}

bool CPsdTemplate::SetTmpLayerPicture(int iLayerID, QString qstrLayerPicture)
{
    if(iLayerID < 0 || iLayerID >= m_listTmpLayerData.size())
    {
        return false;
    }
    m_listTmpLayerData[iLayerID]->SetPhotoPath(qstrLayerPicture);
}

bool CPsdTemplate::SetTmpLayerBlendMode(int iLayerID, int iBlendMode)
{
    if(iLayerID < 0 || iLayerID >= m_listTmpLayerData.size())
    {
        return false;
    }
    m_listTmpLayerData[iLayerID]->setBlandMode(iBlendMode);
}

bool CPsdTemplate::SetTmpLayerTransValue(int iLayerID, int iTransValue)
{
    if(iLayerID < 0 || iLayerID >= m_listTmpLayerData.size())
    {
        return false;
    }
    m_listTmpLayerData[iLayerID]->setOpacity(iTransValue);
}

bool CPsdTemplate::SetTmpLayerMaskPixmap(int iLayerID, const QPixmap *pPixmap)
{
    if(iLayerID < 0 || iLayerID >= m_listTmpLayerData.size())
    {
        return false;
    }
    m_listTmpLayerData[iLayerID]->SetMaskPhotoPixmap(pPixmap);
}


bool CPsdTemplate::SetTmpLayerMoveGap(int iLayerID, QPointF pointMoveGap)
{
    if(iLayerID < 0 || iLayerID >= m_listTmpLayerData.size())
    {
        return false;
    }
    m_listTmpLayerData[iLayerID]->SetPhotoMoveGap(pointMoveGap);
}

bool CPsdTemplate::SetTmpLayerScaleValue(int iLayerID, float fScaleValue)
{
    if(iLayerID < 0 || iLayerID >= m_listTmpLayerData.size())
    {
        return false;
    }
    m_listTmpLayerData[iLayerID]->SetPhotoScaleValue(fScaleValue);
}

bool CPsdTemplate::SetTmpLayerRotateValue(int iLayerID, float fRotateValue)
{
    if(iLayerID < 0 || iLayerID >= m_listTmpLayerData.size())
    {
        return false;
    }
    m_listTmpLayerData[iLayerID]->SetPhotoRotateValue(fRotateValue);
}

int CPsdTemplate::GetPsdImageWidth()
{
    return m_iPsdImageWidthSrc;
}

int CPsdTemplate::GetPsdImageHeight()
{
    return m_iPsdImageHeightSrc;
}

QString CPsdTemplate::GetTmpViewPath()
{
    return m_qstrTmpViewPath;
}

QString CPsdTemplate::GetTmpParsePath()
{
    return m_qstrTmpParsePath;
}

qreal CPsdTemplate::GetPsdScaleValue()
{
    return m_dPsdScaleValue;
}

void CPsdTemplate::setPsdImageWidth(int iWidth)
{
    m_iPsdImageWidthSrc = iWidth;
}

void CPsdTemplate::setPsdImageHeight(int iHeight)
{
    m_iPsdImageHeightSrc = iHeight;
}

void CPsdTemplate::setTmpViewPath(QString qstrTmpViewPath)
{
    m_qstrTmpViewPath = qstrTmpViewPath;
}

void CPsdTemplate::setPsdScaleValue(qreal dScaleValue)
{
    m_dPsdScaleValue = dScaleValue;
}



