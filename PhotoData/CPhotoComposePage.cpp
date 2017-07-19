#include "CPhotoComposePage.h"

CPhotoComposePage::CPhotoComposePage(QObject *parent)
    : QObject(parent)
{
    m_pPsdContext = NULL;
    m_pPsdTemplate = NULL;
    m_pPhotoPagePixmap = NULL;
    m_pGraphDragScene = NULL;
}

CPhotoComposePage::~CPhotoComposePage()
{
    qDeleteAll(m_listPhotoComposeLayer);
    m_listPhotoComposeLayer.clear();
    if(m_pPsdContext != NULL)
    {
        delete m_pPsdContext;
        m_pPsdContext = NULL;
    }
    if(m_pPsdTemplate != NULL)
    {
        delete m_pPsdTemplate;
        m_pPsdTemplate = NULL;
    }
    if(m_pPhotoPagePixmap != NULL)
    {
        delete m_pPhotoPagePixmap;
        m_pPhotoPagePixmap = NULL;
    }
    if(m_pGraphDragScene != NULL)
    {
        delete m_pGraphDragScene;
        m_pGraphDragScene = NULL;
    }
}

void CPhotoComposePage::SetTemplatePath(QString qstrTemplatePaht)
{
    m_qstrTemplatePath = qstrTemplatePaht;
}

void CPhotoComposePage::AddPhoto(int iLayerID, QString qstrPhotoPath)
{
    CPhotoComposeLayer *pPhotoComposeLayer = new CPhotoComposeLayer(iLayerID, qstrPhotoPath);
    m_listPhotoComposeLayer.push_back(pPhotoComposeLayer);
}

QString CPhotoComposePage::GetTemplatePath()
{
    return m_qstrTemplatePath;
}

QList<CPhotoComposeLayer *> &CPhotoComposePage::GetPhotoComposeLayer()
{
    return m_listPhotoComposeLayer;
}

void CPhotoComposePage::SetPsdTemplate(CPsdTemplate *pPsdTemplate)
{
    if(m_pPsdTemplate != NULL)
        delete m_pPsdTemplate;
    m_pPsdTemplate = pPsdTemplate;
}

CPsdTemplate *CPhotoComposePage::GetPsdTemplate()
{
    return m_pPsdTemplate;
}

void CPhotoComposePage::SetPsdContext(_psd_context *pPsdContext)
{
    if(m_pPsdContext != NULL)
    {
        image_free(m_pPsdContext);
    }
    m_pPsdContext = pPsdContext;
}

_psd_context *CPhotoComposePage::GetPsdContext()
{
    return m_pPsdContext;
}

void CPhotoComposePage::SetPhotoPagePixmap(QPixmap *pPixmap)
{
    if(m_pPhotoPagePixmap != NULL)
        delete m_pPhotoPagePixmap;
    m_pPhotoPagePixmap = pPixmap;
}

QPixmap *CPhotoComposePage::GetPhotoPagePixmap()
{
    return m_pPhotoPagePixmap;
}

void CPhotoComposePage::SetGraphDragScene(QGraphDragScene *pGraphDragScene)
{
    if(m_pGraphDragScene != NULL)
    {
        delete m_pGraphDragScene;
    }
    m_pGraphDragScene = pGraphDragScene;
    connect(pGraphDragScene, SIGNAL(sigChangeGraphLayerID(QString)), this, SIGNAL(mouseChangePageLayerID(QString)));
}

QGraphDragScene *CPhotoComposePage::GetGraphDragScene()
{
    return m_pGraphDragScene;
}

