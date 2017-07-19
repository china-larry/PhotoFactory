// 合成层数据类，记录那些层为客照层，合成的客照路径等信息
#ifndef CPHOTOCOMPOSELAYER_H
#define CPHOTOCOMPOSELAYER_H
#include <QString>

class CPhotoComposeLayer
{
public:
    CPhotoComposeLayer(int iLayerID, QString qstrPhotoSrcPath);

public:
    void SetLayerID(int iLayerID);
    void SetPhotoSrcPath(QString qstrPhotoSrcPath);
    //
    int GetLayerID();
    QString GetPhotoSrcPath();
private:
    int m_iLayerID; // 所在层ID
    QString m_qstrPhotoSrcPath; // 相册文件路径
    // Face *m_pFace;
};

#endif // CPHOTOCOMPOSELAYER_H
