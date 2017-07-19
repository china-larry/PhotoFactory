#include "CPhotoComposeLayer.h"

CPhotoComposeLayer::CPhotoComposeLayer(int iLayerID, QString qstrPhotoSrcPath)
    : m_iLayerID(iLayerID)
    , m_qstrPhotoSrcPath(qstrPhotoSrcPath)
{
}

void CPhotoComposeLayer::SetLayerID(int iLayerID)
{
    m_iLayerID = iLayerID;
}

void CPhotoComposeLayer::SetPhotoSrcPath(QString qstrPhotoSrcPath)
{
    m_qstrPhotoSrcPath = qstrPhotoSrcPath;
}

int CPhotoComposeLayer::GetLayerID()
{
    return m_iLayerID;
}

QString CPhotoComposeLayer::GetPhotoSrcPath()
{
    return m_qstrPhotoSrcPath;
}
