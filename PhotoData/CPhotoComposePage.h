/**
 * 合成页数据信息，包含当前页所有属性信息及管理
 *
 */
#ifndef CPHOTOCOMPOSEPAGE_H
#define CPHOTOCOMPOSEPAGE_H

#include <QObject>
#include <QList>
#include "CPhotoComposeLayer.h"
#include <QPixmap>
#include "TemplateData/CPsdTemplate.h"
#include "template_editor_lib/template_editor.h"
#include "GraphViewScene/QGraphDragScene.h"

class CPhotoComposePage : public QObject
{
    Q_OBJECT
public:
    explicit CPhotoComposePage(QObject *parent = 0);
    ~CPhotoComposePage();

signals:

    void mouseChangePageLayerID(QString);
public slots:


public:
    void SetTemplatePath(QString qstrTemplatePaht);
    void AddPhoto(int iLayerID, QString qstrPhotoPath);
    //
    QString GetTemplatePath();
    QList<CPhotoComposeLayer*> &GetPhotoComposeLayer();
    //
    void SetPsdTemplate(CPsdTemplate* pPsdTemplate);
    CPsdTemplate* GetPsdTemplate();
    void SetPsdContext(_psd_context* pPsdContext);
    _psd_context* GetPsdContext();
    //
    void SetPhotoPagePixmap(QPixmap *pPixmap);
    QPixmap* GetPhotoPagePixmap();
    //
    void SetGraphDragScene(QGraphDragScene *pGraphDragScene);
    QGraphDragScene* GetGraphDragScene();
private:
    QString m_qstrTemplatePath;
    QList<CPhotoComposeLayer *> m_listPhotoComposeLayer;
    //
    CPsdTemplate* m_pPsdTemplate;
    _psd_context* m_pPsdContext;
     QPixmap* m_pPhotoPagePixmap;
     QGraphDragScene* m_pGraphDragScene;
};

#endif // CPHOTOCOMPOSEPAGE_H
