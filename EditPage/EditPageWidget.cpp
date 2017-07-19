#include "EditPageWidget.h"
#include <QGridLayout>
#include <QFileInfo>

#include <QJsonDocument>
//#include <qjson_include/parser.h>
//#include <qjson_include/serializer.h>
#include <QDateTime>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/core/core_c.h>
#include <opencv2/imgproc/imgproc_c.h>


#include "LayerControlItem/QTmpLayerControlItem.h"

// 控件大小全局变量
const int g_iGraphViewWidth = 600;
const int g_iGrappViewHeight = 400;
const int g_iListWidgetWidth = 300;
const int g_iListWidgetHeight = 110;
const int g_iLayerListWidgetWidth = 300;
const int g_iLayerListWidgetHeight = 400;
const int g_iLayerControlItemHeight = 50;
const int g_iDrawToolsWidth = 500;
const int g_iDrawToolsHeight = 60;

// 图像数据转换
// vpCvImageToQImage
void vpCvImageToQImage(const IplImage* pIlpImageSrc, QPixmap *pPixmap)
{
    //
    if (!pIlpImageSrc)
    {
        return;
    }
    if(pIlpImageSrc->nChannels == 3)
    {
        // 使用24位
        // QImage to draw on paint event
        QImage qimage = QImage( QSize(pIlpImageSrc->width, pIlpImageSrc->height), QImage::Format_RGB888 );
        // IplImage * to work with OpenCV functions
        IplImage* _cvimage = cvCreateImageHeader( cvSize(pIlpImageSrc->width, pIlpImageSrc->height), 8, 3 );
        // Share the buffer between QImage and IplImage *
        _cvimage->imageData = (char *)qimage.bits();

        cvCopy(pIlpImageSrc, _cvimage, 0);
        // Convert it from BGR to RGB. QImage works with RGB and cvQueryFrame returns a BGR IplImage
        cvCvtColor(_cvimage, _cvimage, CV_BGR2RGB);
        //
        pPixmap->convertFromImage(qimage);
        cvReleaseImageHeader(&_cvimage);
    }
    if(pIlpImageSrc->nChannels == 4)//
    {
        // QImage to draw on paint event
        QImage qimage = QImage( QSize(pIlpImageSrc->width, pIlpImageSrc->height), QImage::Format_ARGB32 );
        // IplImage * to work with OpenCV functions
        IplImage* _cvimage = cvCreateImageHeader( cvSize(pIlpImageSrc->width, pIlpImageSrc->height), 8, 4 );
        // Share the buffer between QImage and IplImage *
        _cvimage->imageData = (char *)qimage.bits();
        cvCopy(pIlpImageSrc, _cvimage, 0);
        //
        pPixmap->convertFromImage(qimage);
        cvReleaseImageHeader(&_cvimage);
    }
}
// QImage to IplImage
IplImage * qImageToIplImage(const QImage & qImageSrc)
{
    int iWidth = qImageSrc.width();
    int iHeight = qImageSrc.height();
    CvSize cvSizeSrc;
    cvSizeSrc.height = iHeight;
    cvSizeSrc.width = iWidth;
    if(qImageSrc.hasAlphaChannel())
  //  if(qImageSrc.depth() == 32)
    {
        QImage qImageTmp = qImageSrc.convertToFormat(QImage::Format_ARGB32);
        IplImage *pIplImage = cvCreateImage(cvSizeSrc, IPL_DEPTH_8U, 4);
        memcpy(pIplImage->imageData, qImageTmp.bits(), qImageTmp.byteCount());
     //   cvCvtColor(pIplImage, pIplImage, CV_BGRA2RGBA);
        return pIplImage;
    }
    else
    {
        QImage qImageTmp = qImageSrc.convertToFormat(QImage::QImage::Format_RGB888);
        IplImage *pIplImage = cvCreateImage(cvSizeSrc, IPL_DEPTH_8U, 3);
        memcpy(pIplImage->imageData, qImageTmp.bits(), qImageTmp.byteCount());
        cvCvtColor(pIplImage, pIplImage, CV_BGR2RGB);
        return pIplImage;
    }
}

EditPageWidget::EditPageWidget(QWidget *parent) :
    QMainWindow(parent)
{
    m_pCurrentViewImage = NULL;
    m_pCurrentPhotoPage = NULL;    
    m_bBeginDrawMask = false;
    m_iCurMaskLayerID = 0;
    // 控件初始化
    InitPhotoListWidget();
    InitGraphView();
    InitTmpListWidget();
    InitOperateWidget();
    InitDrawToolsWidget();
    InitTmpLayerListView();
}

void EditPageWidget::resizeEvent(QResizeEvent *event)
{
    ResizeDragScene();
    // 蒙版场景
    if(m_bBeginDrawMask)
    {
        ResizeMaskScene();
    }
}
// 初始化客照控件
void EditPageWidget::InitPhotoListWidget()
{
    m_pPhotoDockWidget = new QDockWidget("photo", this);
    m_pPhotoDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_pPhotoListWidget = new QPhotoDragListWidget(g_iListWidgetHeight, m_pPhotoDockWidget);
    m_pPhotoDockWidget->setWidget(m_pPhotoListWidget);
    //
    setCorner(Qt::BottomLeftCorner,Qt::LeftDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, m_pPhotoDockWidget);
    // 默认隐藏
//    m_pPhotoDockWidget->hide();
}
// 初始化场景区域
void EditPageWidget::InitGraphView()
{
    // view
    m_pGraphView = new QGraphDragView(this);
    setCentralWidget(m_pGraphView);
    connect(m_pGraphView, SIGNAL(sig_dropPhotoPath(QString)), this, SLOT(onDropPhotoPath(QString)));
    QRectF rectGraphView = m_pGraphView->rect();
    rectGraphView.setWidth(rectGraphView.width() - 5);
    rectGraphView.setHeight(rectGraphView.height() - 5);
    m_pGraphView->setSceneRect(rectGraphView);
    // 蒙版场景
    m_pMaskDrawScene = new QGraphMaskScene(this);
    connect(m_pMaskDrawScene, SIGNAL(sigChangeMaskItemPixmap(QPixmap*)), this, SLOT(onChangeMaskLayerItem(QPixmap*)));

}
// 初始化合成页控件List
void EditPageWidget::InitTmpListWidget()
{
    m_pTmpDockWidget = new QDockWidget("page", this);
    m_pTmpDockWidget->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    m_pPhotoPageListWidget = new QPhotoListWidget(g_iListWidgetHeight, m_pTmpDockWidget);
    m_pPhotoPageListWidget->setMinimumSize(g_iGraphViewWidth, g_iListWidgetHeight);
    //
    m_pTmpDockWidget->setWidget(m_pPhotoPageListWidget);
    addDockWidget(Qt::BottomDockWidgetArea, m_pTmpDockWidget);
    connect(m_pPhotoPageListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(onChangeComposePage(int)));
    // 默认隐藏
 //   m_pTmpDockWidget->hide();
}
// 初始化图层控制控件List
void EditPageWidget::InitTmpLayerListView()
{
    //
    m_pTmpLayerDockWidget = new QDockWidget("laryer", this);
    m_pTmpLayerDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    //
    m_pTmpLayerListWidget = new QTmpLayerListWidget(m_pTmpLayerDockWidget);
    m_pTmpLayerListWidget->setResizeMode(QListView::Adjust);
    m_pTmpLayerListWidget->setViewMode(QListView::ListMode);
    m_pTmpLayerListWidget->setMovement(QListView::Static);
    m_pTmpLayerListWidget->setSpacing(10);
    m_pTmpLayerListWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_pTmpLayerListWidget->setMaximumWidth(g_iLayerListWidgetWidth);
    m_pTmpLayerListWidget->setMinimumWidth(g_iLayerListWidgetWidth);
    m_pTmpLayerListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTmpLayerButtonGroup = new QButtonGroup(this);
    //
    m_pTmpLayerDockWidget->setWidget(m_pTmpLayerListWidget);
    //
    setCorner(Qt::BottomRightCorner,Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, m_pTmpLayerDockWidget);
    // 默认隐藏图层
    //   m_pTmpLayerDockWidget->hide();
}
// 初始化操作面板（混合模式、透明度等）
void EditPageWidget::InitOperateWidget()
{
    m_pOperateDockWidget = new QDockWidget("operate", this);
    m_pOperateDockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
    m_pOperateDockWidget->setMaximumWidth(g_iLayerListWidgetWidth);
    m_pOperateLabel = new QOperateLabel(m_pOperateDockWidget);
    m_pOperateLabel->setMaximumWidth(g_iLayerListWidgetWidth);
    m_pOperateLabel->setMinimumWidth(g_iLayerListWidgetWidth);
    m_pOperateLabel->setMinimumHeight(55);
    m_pOperateLabel->setMaximumHeight(55);
    m_pOperateDockWidget->setWidget(m_pOperateLabel);
    addDockWidget(Qt::RightDockWidgetArea, m_pOperateDockWidget);
    //
    connect(m_pOperateLabel, SIGNAL(changeMixModeValue(int)), this, SLOT(onChangeLayerMixModel(int)));
    connect(m_pOperateLabel, SIGNAL(changeTransValue(int)), this, SLOT(onChangeLayerTransValue(int)));
}

void EditPageWidget::InitDrawToolsWidget()
{
    m_pDrawToolsDockWidget = new QDockWidget("draw tools", this);
    m_pDrawToolsDockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
 //   m_pDrawToolsDockWidget->setFloating(true);
    m_pDrawToolsWidget = new QDrawToolsWidget(m_pDrawToolsDockWidget);
    m_pDrawToolsWidget->setMinimumWidth(g_iDrawToolsWidth);
    m_pDrawToolsWidget->setMinimumHeight(g_iDrawToolsHeight);
    m_pDrawToolsDockWidget->setWidget(m_pDrawToolsWidget);
    addDockWidget(Qt::TopDockWidgetArea, m_pDrawToolsDockWidget);
    connect(m_pDrawToolsWidget, SIGNAL(changeDrawTools()), this, SLOT(onChangeDrawTools()));

    m_pDrawToolsDockWidget->hide();
}
// resize场景大小
void EditPageWidget::ResizeDragScene()
{
    if(m_pCurrentViewImage == NULL)
    {
        return;
    }
    int listTmpLayerGraphItemSize = m_pCurGraphScene->GetGraphItemCount();
    if(!listTmpLayerGraphItemSize)
    {
        return;
    }
    // 场景随view缩放
    QRectF rectGraphView = m_pGraphView->rect();
    rectGraphView.setWidth(rectGraphView.width() - 5);
    rectGraphView.setHeight(rectGraphView.height() - 5);
    m_pGraphView->setSceneRect(rectGraphView);
    // 计算缩放因子
    qreal fGraphicsViewX = rectGraphView.width();
    qreal fGraphicsViewY = rectGraphView.height();
    // 预览图缩放因子
    qreal fSrcImageWidth = m_pCurrentViewImage->width();
    qreal fSrcImageHeight = m_pCurrentViewImage->height();
    qreal fViewImageScaleX = fSrcImageWidth / fGraphicsViewX;
    qreal fViewImageScsleY = fSrcImageHeight / fGraphicsViewY;
    m_dViewImageScale = fViewImageScaleX > fViewImageScsleY ? fViewImageScaleX : fViewImageScsleY;
    m_dViewImageScale = 1.0f / m_dViewImageScale;
    // 第一个为预览图缩放
    m_pCurGraphScene->SetGraphItemScale(0, m_dViewImageScale);
    // 原图缩放因子
    if(m_pCurPsdTemplate == NULL) return;
    float fPsdImageWidthSrc = m_pCurPsdTemplate->GetPsdImageWidth();
    float fPsdImageHeightSrc = m_pCurPsdTemplate->GetPsdImageHeight();
    float fPsdImageScaleX = fPsdImageWidthSrc / fGraphicsViewX;
    float fPsdImageScaleY = fPsdImageHeightSrc / fGraphicsViewY;
    m_fPsdImageScale = fPsdImageScaleX > fPsdImageScaleY ? fPsdImageScaleX : fPsdImageScaleY;
    m_fPsdImageScale = 1.0f / m_fPsdImageScale;
    // 控件层缩放
    for(int i = 1; i != listTmpLayerGraphItemSize; ++i)
    {
        m_pCurGraphScene->SetGraphItemScale(i, m_fPsdImageScale);
    }
}

void EditPageWidget::ResizeMaskScene()
{
    if(!m_bBeginDrawMask)
    {// 没有进行蒙版场景绘制
        return;
    }
    // 场景随view缩放
    QRectF rectGraphView = m_pGraphView->rect();
    rectGraphView.setWidth(rectGraphView.width() - 5);
    rectGraphView.setHeight(rectGraphView.height() - 5);
    m_pGraphView->setSceneRect(rectGraphView);
    // 计算缩放因子
    qreal fGraphicsViewX = rectGraphView.width();
    qreal fGraphicsViewY = rectGraphView.height();
    // 预览图缩放因子
    qreal fSrcImageWidth = m_pCurrentViewImage->width();
    qreal fSrcImageHeight = m_pCurrentViewImage->height();
    qreal fViewImageScaleX = fSrcImageWidth / fGraphicsViewX;
    qreal fViewImageScsleY = fSrcImageHeight / fGraphicsViewY;
    qreal dViewImageScale = fViewImageScaleX > fViewImageScsleY ? fViewImageScaleX : fViewImageScsleY;
    dViewImageScale = 1.0f / dViewImageScale;
    // 背景控件
    m_pMaskDrawScene->SetGraphBackItemScale(dViewImageScale);
    // 原图缩放因子
    m_pMaskDrawScene->SetGraphMaskItemScale(dViewImageScale);
}
// 场景数据改变，更新场景
bool EditPageWidget::UpdateGraphView()
{
    //// 清空原有数据**************************
    if(m_pCurrentPhotoPage == NULL)
        return false;
    m_pCurrentViewImage = m_pCurrentPhotoPage->GetPhotoPagePixmap();
    m_pCurPsdTemplate = m_pCurrentPhotoPage->GetPsdTemplate();
    m_pCurGraphScene = m_pCurrentPhotoPage->GetGraphDragScene();
    ResizeDragScene();
    m_pGraphView->setScene(m_pCurGraphScene);
    return true;
}
// 场景新的场景，即添加新的页的时候创建
QGraphDragScene* EditPageWidget::CreateGraphScene(CPhotoComposePage *pPhotoComposePage)
{
    if(pPhotoComposePage == NULL)
        return NULL;
    CPsdTemplate *pPsdCurrentTemplate = pPhotoComposePage->GetPsdTemplate();
    if(pPsdCurrentTemplate == NULL)
    {
        return NULL;
    }
    QGraphDragScene *pGraphDragScene = new QGraphDragScene(this);
    // 背景图片
    QPixmap *pCurrentViewImage = pPhotoComposePage->GetPhotoPagePixmap();
    // 场景背景控件
    QTmpLayerGraphItem *pTmpLayerGraphItem = new QTmpLayerGraphItem(QRectF(0, 0, pCurrentViewImage->width(), pCurrentViewImage->height()),
                                                                    QRectF(0, 0, pCurrentViewImage->width(), pCurrentViewImage->height()), 0, "-1");
    pTmpLayerGraphItem->SetViewPixmap(pCurrentViewImage);
    pGraphDragScene->AddGraphItem(pTmpLayerGraphItem);
    // 获得当前选中模板数据
    QList<CTmpLayerData*> vTmpLayerData = pPsdCurrentTemplate->GetTmpLayerDataList();
    int iTmpLayerDataCount = vTmpLayerData.size();
    // 获得数据
    QPixmap *pLayerImage = NULL;
    QRectF rectPixmap;
    QString qstrLayerID;
    int iLayerType;
    // 循环插入图层控件
    for(int i = 0; i != iTmpLayerDataCount; ++i)
    {
        pLayerImage = vTmpLayerData[i]->getLayerPixmap();
        rectPixmap = vTmpLayerData[i]->GetPixmapRect();
        QRectF rectLayer;
        rectLayer.setLeft(vTmpLayerData[i]->getOffsetX());
        rectLayer.setTop(vTmpLayerData[i]->getOffsetY());
        rectLayer.setWidth(vTmpLayerData[i]->getWidth());
        rectLayer.setHeight(vTmpLayerData[i]->getHeight());
        qstrLayerID = vTmpLayerData[i]->getLayerID();
        iLayerType = vTmpLayerData[i]->getType();
        // 新的GraphItem
        QTmpLayerGraphControlItem *pTmpLayerControlGraphItem =
                new QTmpLayerGraphControlItem(rectPixmap, rectLayer, iLayerType, qstrLayerID);
        connect(pTmpLayerControlGraphItem, SIGNAL(mouseMovePhotoPos(int, QPointF)), this, SLOT(onMoveGraphPhotoItem(int, QPointF)));
        connect(pTmpLayerControlGraphItem, SIGNAL(mouseScalePhotoPos(int, qreal)), this, SLOT(onSclaeGraphPhotoItem(int, qreal)));
        connect(pTmpLayerControlGraphItem, SIGNAL(mouseRotatePhotoPos(int, qreal)), this, SLOT(onRotateGraphPhotoItem(int,qreal)));
        //
        if(iLayerType)
        {// 客照层在上层
            pTmpLayerControlGraphItem->setZValue(1);
        }
        pGraphDragScene->AddGraphItem(pTmpLayerControlGraphItem);
    }
    return pGraphDragScene;
}
// 更新图层控制控件，切换页的时候，所用原控件clear后创建新的控件
bool EditPageWidget::UpdateTmpLayerListView()
{
    // 清空控件数据
    m_pTmpLayerListWidget->clear();
    // 获得当前选中模板数据    
    if(m_pCurrentPhotoPage == NULL)
        return false;
    CPsdTemplate *pPsdTemplate = m_pCurrentPhotoPage->GetPsdTemplate();
    if(pPsdTemplate == NULL)
        return false;
    QList<CTmpLayerData*> vTmpLayerData = pPsdTemplate->GetTmpLayerDataList();
    int iTmpLayerDataCount = vTmpLayerData.size();
    if(!iTmpLayerDataCount)
    {
        return false;
    }
    // 数据初始化
    QString qstrLayerID, qstrLayerIndex;
    int iVisable, iPotoVisable, iClipp, iLayerType;
    bool bMask;
    QPixmap *pLayerPixmap = NULL;
    QPixmap *pMaskPixmap = NULL;
    // 遍历图层数据，创建控件
    for(int i = iTmpLayerDataCount-1; i != -1; --i)
    {
        // 获取数据
        iPotoVisable = vTmpLayerData[i]->getPotoVisable();
        if(iPotoVisable) // potoshop中不可见，控件层不显示
        {
            continue;
        }
        qstrLayerID = vTmpLayerData[i]->getLayerID();
        qstrLayerIndex.setNum(i);
        iVisable = vTmpLayerData[i]->getVisable();
        iClipp = vTmpLayerData[i]->getClipp();
        iLayerType = vTmpLayerData[i]->getType();
        bMask = vTmpLayerData[i]->getMaskFlag();
        pLayerPixmap = vTmpLayerData[i]->getLayerPixmap();
        pMaskPixmap = vTmpLayerData[i]->getMaskPixmap();
//        // 初始化控件
        QTmpLayerControlItem *pTmpControlItem = new QTmpLayerControlItem(iVisable, iClipp, iLayerType, bMask);
        pTmpControlItem->setLayerID(qstrLayerID);
        // 序列名称
        pTmpControlItem->setLayerIndex(qstrLayerIndex);
        pTmpControlItem->setLayerPixmap(pLayerPixmap);
        if(bMask && pMaskPixmap != NULL)
        {
            pTmpControlItem->SetMaskPixmap(pMaskPixmap);
        }
        pTmpControlItem->setCheckable(true);

        m_pTmpLayerButtonGroup->addButton(pTmpControlItem);
        /*
         * 是否可视，在此connect信号QTmpControlItem发来的visable和id，多个信号，一个槽
         */
        connect(pTmpControlItem, SIGNAL(changeLayerControlItemID(QString)), this, SLOT(onChangeControlLayerID(QString)));
        connect(pTmpControlItem, SIGNAL(changeLayerControlItemVisable(QString, int)), this, SLOT(onChangeLayerVisable(QString, int)));
        connect(pTmpControlItem, SIGNAL(beginDrawMask(int)), this, SLOT(onBeginDrawMaskLayer(int)));

        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(QSize(0, g_iLayerControlItemHeight));
        m_pTmpLayerListWidget->addItem(item);
        m_pTmpLayerListWidget->setItemWidget(item, pTmpControlItem);
    }
    return true;
}
// 图层or客照坐标变换操作
bool EditPageWidget::TranslatePhotoItem(int iLayerID)
{
    if(m_pCurrentPhotoPage == NULL) return false;
    CPsdTemplate* pPsdTemplate = m_pCurrentPhotoPage->GetPsdTemplate();
    if(pPsdTemplate == NULL) return false;
    QList<CTmpLayerData*> listTmpLayerData = pPsdTemplate->GetTmpLayerDataList();
    int iListTmpLayerDataSize = listTmpLayerData.size();
    if(iLayerID <= 0 || iLayerID >= iListTmpLayerDataSize) return false;
    // 获取模板当前层信息
    CTmpLayerData *pTmpLayerData = listTmpLayerData.at(iLayerID);
    if(pTmpLayerData->getColorAdjust() > 0) return false;   // 色彩调整层不予处理
    //
    _psd_context *pPsdContext = m_pCurrentPhotoPage->GetPsdContext();
    if(pPsdContext == NULL) return false;

    // 相片信息初始化
    CEditorParameter cEditorParameter;
    cEditorParameter.set_layer_num(iLayerID);
    cEditorParameter.set_layer_or_photo(1);  // 对客照进行坐标变换

//    cEditorParameter.set_blend_width(pPsdTemplate->GetPsdImageWidth());
//    cEditorParameter.set_blend_height(pPsdTemplate->GetPsdImageHeight());
    cEditorParameter.set_blend_offset_x(pTmpLayerData->getOffsetX());
    cEditorParameter.set_blend_offset_y(pTmpLayerData->getOffsetY());
    cEditorParameter.set_blend_width(pTmpLayerData->getWidth());
    cEditorParameter.set_blend_height(pTmpLayerData->getHeight());


    cEditorParameter.set_blend_mode(pTmpLayerData->getBlandMode());
    cEditorParameter.set_opacity(pTmpLayerData->getOpacity());
    cEditorParameter.set_load_customer_image(0);// 非客照
    // 获取visable数组
    int *iVisableContextAarry = new int[iListTmpLayerDataSize];
    for(int i = 0; i != iListTmpLayerDataSize; ++i)
    {
        iVisableContextAarry[i] = listTmpLayerData[i]->getVisable();
    }
    cEditorParameter.set_visable(iVisableContextAarry);
    // 图层操作or客照操作
    if(pTmpLayerData->getType())
    {// 客照
        // 移动
        cEditorParameter.set_x_move(listTmpLayerData[iLayerID]->GetPhotoMoveGap().x());
        cEditorParameter.set_y_move(listTmpLayerData[iLayerID]->GetPhotoMoveGap().y());
        // 缩放
        cEditorParameter.set_zoom_scale(listTmpLayerData[iLayerID]->GetPhotoScaleValue());
        // 旋转
        cEditorParameter.set_rotation(listTmpLayerData[iLayerID]->GetPhotoRotateValue());
        //
    }
    else
    {// 图层，暂不处理
//        // 移动
//        cEditorParameter.set_layer_x_move(listTmpLayerData[iLayerID]->GetMoveGap().x());
//        cEditorParameter.set_layer_y_move(listTmpLayerData[iLayerID]->GetMoveGap().y());
//        // 缩放
//        cEditorParameter.set_zoom_scale(listTmpLayerData[iLayerID]->GetScaleValue());
    }

    _editor_parameter *p_editor_parameter = create_editorpata(cEditorParameter.layer_num, cEditorParameter.layer_or_photo,
                                                              cEditorParameter.x_move, cEditorParameter.y_move, cEditorParameter.layer_x_move,
                                                              cEditorParameter.layer_y_move, cEditorParameter.blend_offset_y, cEditorParameter.blend_offset_x,
                                                              cEditorParameter.blend_width, cEditorParameter.blend_height, cEditorParameter.zoom_scale,
                                                              cEditorParameter.rotation, cEditorParameter.blend_mode, cEditorParameter.opacity,
                                                              cEditorParameter.load_customer_image, cEditorParameter.customer_image, cEditorParameter.visable,
                                                              cEditorParameter.mask_edit, cEditorParameter.mask_image);
    //
    QString qstrTemplatePath = m_pCurrentPhotoPage->GetTemplatePath();
    char cTemplaetPath[512];
    strcpy(cTemplaetPath, qstrTemplatePath.QString::toUtf8());
    //
    qDebug() << "imag_blend is begin";
    IplImage *plImage = image_blend(pPsdContext, cTemplaetPath, p_editor_parameter);
    qDebug() << "image_blend is good__" ;
    delete []iVisableContextAarry;
    free_editorpata(p_editor_parameter);
    if(plImage == NULL) return false;
    // 转换为QPixmap
    QPixmap *pPixmap = new QPixmap;
    vpCvImageToQImage(plImage, pPixmap);
    m_pCurrentPhotoPage->SetPhotoPagePixmap(pPixmap);
    m_pCurrentViewImage = m_pCurrentPhotoPage->GetPhotoPagePixmap();
    // 更新视图
    m_pCurGraphScene->SetViewPixmap(pPixmap);
    m_pCurGraphScene->update(m_pCurGraphScene->sceneRect());
    return true;
}
// 更新操作面板，再切换图层时候或页的时候更新数据
void EditPageWidget::UpdateOperateWidget()
{
    if(m_pCurrentPhotoPage == NULL) return;
    CPsdTemplate* pPsdTemplate = m_pCurrentPhotoPage->GetPsdTemplate();
    if(pPsdTemplate == NULL) return;
    QList<CTmpLayerData*> listTmpLayerData = pPsdTemplate->GetTmpLayerDataList();
    if(m_pCurGraphScene == NULL) return;
    int iLayerID = m_pCurGraphScene->GetCurLayerID().toInt();
    m_pOperateLabel->SetMixModel(listTmpLayerData[iLayerID]->getBlandMode());
    m_pOperateLabel->SetTransparent(listTmpLayerData[iLayerID]->getOpacity());
}

void EditPageWidget::UpdateControlMaskLayer()
{
    if(m_bBeginDrawMask)
    {
        m_pGraphView->setScene(m_pCurGraphScene);
        // 找到当前layerlist中的ID，蒙版设为非选中
        QListWidgetItem* pListWidgetItem = NULL;
        QTmpLayerControlItem *pTmpControlItem = NULL;
        int iLayerListWidgetCount = m_pTmpLayerListWidget->count();
        for(int i = 0; i != iLayerListWidgetCount; ++i)
        {
            pListWidgetItem = m_pTmpLayerListWidget->item(i);
            pTmpControlItem = (QTmpLayerControlItem *)m_pTmpLayerListWidget->itemWidget(pListWidgetItem);
            if(pTmpControlItem->getLayerID().toInt() == m_iCurMaskLayerID)
            {
                pTmpControlItem->SetMaskSelect(false);
                break;
            }
        }
        m_bBeginDrawMask = false;
        m_pDrawToolsDockWidget->hide();
    }
}
// 当前选择页改变
void EditPageWidget::onChangeComposePage(int iCurSelectRow)
{
    if(iCurSelectRow < 0 || iCurSelectRow >= m_listPhotoComposePage.size())
        return;
    m_pCurrentPhotoPage = m_listPhotoComposePage[iCurSelectRow];
    UpdateGraphView();
    UpdateTmpLayerListView();
}
// 拖拽新的客照到当前页的当前图层
void EditPageWidget::onDropPhotoPath(QString qstrPhotoPath)
{
    if(m_pCurrentPhotoPage == NULL) return;
    int iLayerID = m_pCurGraphScene->GetCurLayerID().toInt();
    CPsdTemplate* pPsdTemplate = m_pCurrentPhotoPage->GetPsdTemplate();
    if(pPsdTemplate == NULL) return;
    QList<CTmpLayerData*> listTmpLayerData = pPsdTemplate->GetTmpLayerDataList();
    int iListTmpLayerDataSize = listTmpLayerData.size();
    if(iLayerID <= 0 || iLayerID >= iListTmpLayerDataSize) return;
    if(listTmpLayerData[iLayerID]->getType() != 1) return;    // 非客照层，不替换
    // 修改原模板中客照数据
    pPsdTemplate->SetTmpLayerPicture(iLayerID, qstrPhotoPath);
    // 移动
    listTmpLayerData[iLayerID]->SetPhotoMoveGap(QPointF(0, 0));
    // 缩放
    listTmpLayerData[iLayerID]->SetPhotoScaleValue(1.0d);
    // 旋转
    listTmpLayerData[iLayerID]->SetPhotoRotateValue(0.0d);
    //
    _psd_context *pPsdContext = m_pCurrentPhotoPage->GetPsdContext();
    if(pPsdContext == NULL) return;
    // 获取模板当前层信息
    CTmpLayerData *pTmpLayerData = listTmpLayerData.at(iLayerID);
    // 相片信息初始化
    CEditorParameter cEditorParameter;
    cEditorParameter.set_layer_num(iLayerID);
    cEditorParameter.set_layer_or_photo(pTmpLayerData->getType());
//    cEditorParameter.set_blend_width(pPsdTemplate->GetPsdImageWidth());
//    cEditorParameter.set_blend_height(pPsdTemplate->GetPsdImageHeight());

    cEditorParameter.set_blend_offset_x(pTmpLayerData->getOffsetX());
    cEditorParameter.set_blend_offset_y(pTmpLayerData->getOffsetY());
    cEditorParameter.set_blend_width(pTmpLayerData->getWidth());
    cEditorParameter.set_blend_height(pTmpLayerData->getHeight());


    cEditorParameter.set_blend_mode(pTmpLayerData->getBlandMode());
    cEditorParameter.set_opacity(pTmpLayerData->getOpacity());
    cEditorParameter.set_load_customer_image(1);// 非客照
    // 客照
    IplImage* pPhotoImage = qImageToIplImage(QImage(qstrPhotoPath));
    cEditorParameter.set_customer_image(pPhotoImage);
    // 获取visable数组
    int *iVisableContextAarry = new int[iListTmpLayerDataSize];
    for(int i = 0; i != iListTmpLayerDataSize; ++i)
    {
        iVisableContextAarry[i] = listTmpLayerData[i]->getVisable();
    }
    cEditorParameter.set_visable(iVisableContextAarry);
    _editor_parameter *p_editor_parameter = create_editorpata(cEditorParameter.layer_num, cEditorParameter.layer_or_photo,
                                                              cEditorParameter.x_move, cEditorParameter.y_move, cEditorParameter.layer_x_move,
                                                              cEditorParameter.layer_y_move, cEditorParameter.blend_offset_y, cEditorParameter.blend_offset_x,
                                                              cEditorParameter.blend_width, cEditorParameter.blend_height, cEditorParameter.zoom_scale,
                                                              cEditorParameter.rotation, cEditorParameter.blend_mode, cEditorParameter.opacity,
                                                              cEditorParameter.load_customer_image, cEditorParameter.customer_image, cEditorParameter.visable,
                                                              cEditorParameter.mask_edit, cEditorParameter.mask_image);
    //
    QString qstrTemplatePath = m_pCurrentPhotoPage->GetTemplatePath();
    char cTemplaetPath[512];
    strcpy(cTemplaetPath, qstrTemplatePath.QString::toUtf8());
    //
    qDebug() << "imag_blend is begin";
    IplImage *plImage = image_blend(pPsdContext, cTemplaetPath, p_editor_parameter);
    qDebug() << "image_blend is good__" ;
    cvReleaseImage(&pPhotoImage);
    delete []iVisableContextAarry;
    free_editorpata(p_editor_parameter);
    if(plImage == NULL) return;
    // 转换为QPixmap
    QPixmap *pPhotoPixmap = new QPixmap;
    vpCvImageToQImage(plImage, pPhotoPixmap);
    m_pCurrentPhotoPage->SetPhotoPagePixmap(pPhotoPixmap);
    m_pCurrentViewImage = m_pCurrentPhotoPage->GetPhotoPagePixmap();
    // 更新视图
    m_pCurGraphScene->SetViewPixmap(pPhotoPixmap);
    // 更新替换客照数据
    QTmpLayerGraphControlItem* pTmpLayerGraphItem =  dynamic_cast<QTmpLayerGraphControlItem* >(m_pCurGraphScene->GetGraphItemIndex(iLayerID+1));
    if(pTmpLayerGraphItem != NULL)
    {
        pTmpLayerGraphItem->SetViewPixmap(listTmpLayerData[iLayerID]->getLayerPixmap(), listTmpLayerData[iLayerID]->GetPixmapRect());
    }
    m_pCurGraphScene->update(m_pCurGraphScene->sceneRect());
    // 图层控件更新
    // 找到当前layerlist中的ID，setcheck为tur
    QListWidgetItem* pListWidgetItem = NULL;
    QTmpLayerControlItem *pTmpControlItem = NULL;
    int iLayerListWidgetCount = m_pTmpLayerListWidget->count();
    for(int i = 0; i != iLayerListWidgetCount; ++i)
    {
        pListWidgetItem = m_pTmpLayerListWidget->item(i);
        pTmpControlItem = (QTmpLayerControlItem *)m_pTmpLayerListWidget->itemWidget(pListWidgetItem);
        if(pTmpControlItem->getLayerID().toInt() == iLayerID)
        {
            pTmpControlItem->setLayerPixmap(listTmpLayerData[iLayerID]->getLayerPixmap());
            break;
        }
    }
}
// 绘制蒙版
void EditPageWidget::onBeginDrawMaskLayer(int iLayerID)
{    
    if(m_iCurMaskLayerID == iLayerID && m_bBeginDrawMask) return;
    // 重置蒙版状态
    UpdateControlMaskLayer();
    //
    qreal dPsdScaleValue = m_pCurPsdTemplate->GetPsdScaleValue();
    QList<CTmpLayerData*> listTmpLayerData = m_pCurPsdTemplate->GetTmpLayerDataList();
    int iListTmpLayerDataSize = listTmpLayerData.size();
    if(iLayerID <= 0 || iLayerID >= iListTmpLayerDataSize) return;  //  第0层蒙版暂不允许操作
    //
    m_bBeginDrawMask = true;
    m_iCurMaskLayerID = iLayerID;
    //
    m_pMaskDrawScene->ResetScene(QRectF(listTmpLayerData[iLayerID]->getMaskOffsetX() * dPsdScaleValue,
                                        listTmpLayerData[iLayerID]->getMaskOffsetY() * dPsdScaleValue,
                                        listTmpLayerData[iLayerID]->getMaskWidth() * dPsdScaleValue,
                                        listTmpLayerData[iLayerID]->getMaskHeight() * dPsdScaleValue),
                                 m_pCurrentPhotoPage->GetPhotoPagePixmap(), listTmpLayerData[iLayerID]->getMaskPixmap());
    // 绘图
    m_pMaskDrawScene->SetDrawType(m_pDrawToolsWidget->GetDrawType());
    m_pMaskDrawScene->SetPen(m_pDrawToolsWidget->GetPen());
    m_pMaskDrawScene->SetBrush(m_pDrawToolsWidget->GetBrush());
    //
    ResizeMaskScene();
    m_pGraphView->setScene(m_pMaskDrawScene);
    // 绘图工具
    m_pDrawToolsDockWidget->raise();
    m_pDrawToolsDockWidget->show();
    m_pDrawToolsDockWidget->setFloating(true);
    QWidget* pTopWidget = (QWidget*)this->parent()->parent()->parent();
    m_pDrawToolsDockWidget->setGeometry(
                pTopWidget->geometry().x() + (pTopWidget->geometry().width() - g_iDrawToolsWidth - 120),
                pTopWidget->geometry().y(), g_iDrawToolsWidth, g_iDrawToolsHeight);
}
// 当前场景图层改变
void EditPageWidget::onChangeGraphLayerID(QString qstrLayerID)
{
    // 找到当前layerlist中的ID，setcheck为ture
    QListWidgetItem* pListWidgetItem = NULL;
    QTmpLayerControlItem *pTmpControlItem = NULL;
    int iLayerListWidgetCount = m_pTmpLayerListWidget->count();
    for(int i = 0; i != iLayerListWidgetCount; ++i)
    {
        pListWidgetItem = m_pTmpLayerListWidget->item(i);
        pTmpControlItem = (QTmpLayerControlItem *)m_pTmpLayerListWidget->itemWidget(pListWidgetItem);
        if(pTmpControlItem->getLayerID() == qstrLayerID)
        {
            pTmpControlItem->setChecked(true);
         //   m_pTmpLayerListWidget->setCurrentRow(i);
            m_pTmpLayerListWidget->clearFocus();
            //
            m_pCurGraphScene->setFocus();
            break;
        }
    }
    // 设置操作面板数据
    UpdateOperateWidget();
}
// 当前控件图层改变
void EditPageWidget::onChangeControlLayerID(QString qstrLayerID)
{
    // 重置蒙版状态
    UpdateControlMaskLayer();
    //
    QString qstrCurGraphLayerID = m_pCurGraphScene->GetCurLayerID();
    if(qstrCurGraphLayerID == qstrLayerID)
        return;
    /*
     * 更行预览区选中框，更新属性面板
     */    
    m_pCurGraphScene->SetCurLayerID(qstrLayerID);
    // 更新操作面板
    UpdateOperateWidget();
}
// 移动客照控件
void EditPageWidget::onMoveGraphPhotoItem(int iLayerID, QPointF pointMoveGap)
{
    //
    if(!pointMoveGap.x() && !pointMoveGap.y()) return;
    if(m_pCurrentPhotoPage == NULL || m_pCurPsdTemplate == NULL) return;
    QList<CTmpLayerData*> listTmpLayerData = m_pCurPsdTemplate->GetTmpLayerDataList();
    int iListTmpLayerDataSize = listTmpLayerData.size();
    if(iLayerID <= 0 || iLayerID >= iListTmpLayerDataSize) return;
    //
    m_pCurPsdTemplate->SetTmpLayerMoveGap(iLayerID, pointMoveGap);
    //
    TranslatePhotoItem(iLayerID);
}
// 缩放客照控件
void EditPageWidget::onSclaeGraphPhotoItem(int iLayerID, qreal dScaleValue)
{
    if(dScaleValue == 1.0d && dScaleValue == 0.0d) return;
    if(m_pCurrentPhotoPage == NULL) return;
    CPsdTemplate* pPsdTemplate = m_pCurrentPhotoPage->GetPsdTemplate();
    if(pPsdTemplate == NULL) return;
    QList<CTmpLayerData*> listTmpLayerData = pPsdTemplate->GetTmpLayerDataList();
    int iListTmpLayerDataSize = listTmpLayerData.size();
    if(iLayerID <= 0 || iLayerID >= iListTmpLayerDataSize) return;
    //
    pPsdTemplate->SetTmpLayerScaleValue(iLayerID, dScaleValue);
    //
    TranslatePhotoItem(iLayerID);
}
// 旋转客照控件
void EditPageWidget::onRotateGraphPhotoItem(int iLayerID, qreal dRelativeAngle)
{
    qreal dRotateAngle = dRelativeAngle * 180.0d / 3.1415926d;
    if(!dRotateAngle) return;
    if(m_pCurrentPhotoPage == NULL) return;
    CPsdTemplate* pPsdTemplate = m_pCurrentPhotoPage->GetPsdTemplate();
    if(pPsdTemplate == NULL) return;
    QList<CTmpLayerData*> listTmpLayerData = pPsdTemplate->GetTmpLayerDataList();
    int iListTmpLayerDataSize = listTmpLayerData.size();
    if(iLayerID <= 0 || iLayerID >= iListTmpLayerDataSize) return;
    //
    pPsdTemplate->SetTmpLayerRotateValue(iLayerID, dRotateAngle);
    //
    TranslatePhotoItem(iLayerID);
}
// 设定某层是否可视
void EditPageWidget::onChangeLayerVisable(QString qstrLayerID, int iVisable)
{
    int iLayerID = qstrLayerID.toInt();
    if(m_pCurrentPhotoPage == NULL) return;
    CPsdTemplate* pPsdTemplate = m_pCurrentPhotoPage->GetPsdTemplate();
    if(pPsdTemplate == NULL) return;
    QList<CTmpLayerData*> listTmpLayerData = pPsdTemplate->GetTmpLayerDataList();
    int iListTmpLayerDataSize = listTmpLayerData.size();
    if(iLayerID <= 0 || iLayerID >= iListTmpLayerDataSize) return;
    // 设置当前层可视
    pPsdTemplate->SetTmpLayerVisable(qstrLayerID, iVisable);
    //
    _psd_context *pPsdContext = m_pCurrentPhotoPage->GetPsdContext();
    if(pPsdContext == NULL) return;
    //
    // 获取模板当前层信息
    CTmpLayerData *pTmpLayerData = listTmpLayerData.at(iLayerID);
    // 相片信息初始化
    CEditorParameter cEditorParameter;
    cEditorParameter.set_layer_num(iLayerID);
    cEditorParameter.set_layer_or_photo(0);// 图层操作
    cEditorParameter.set_blend_width(pPsdTemplate->GetPsdImageWidth());
    cEditorParameter.set_blend_height(pPsdTemplate->GetPsdImageHeight());
    cEditorParameter.set_blend_mode(pTmpLayerData->getBlandMode());
    cEditorParameter.set_opacity(pTmpLayerData->getOpacity());
    cEditorParameter.set_load_customer_image(0);// 非客照
    // 获取visable数组
    int *iVisableContextAarry = new int[iListTmpLayerDataSize];
    for(int i = 0; i != iListTmpLayerDataSize; ++i)
    {
        iVisableContextAarry[i] = listTmpLayerData[i]->getVisable();
    }
    cEditorParameter.set_visable(iVisableContextAarry);
    // 移动
    cEditorParameter.set_x_move(listTmpLayerData[iLayerID]->GetLayerMoveGap().x());
    cEditorParameter.set_y_move(listTmpLayerData[iLayerID]->GetLayerMoveGap().y());
    // 缩放
    cEditorParameter.set_zoom_scale(listTmpLayerData[iLayerID]->GetLayerScaleValue());
    // 旋转
    cEditorParameter.set_rotation(listTmpLayerData[iLayerID]->GetLayerRotateValue());
    //
    _editor_parameter *p_editor_parameter = create_editorpata(cEditorParameter.layer_num, cEditorParameter.layer_or_photo,
                                                              cEditorParameter.x_move, cEditorParameter.y_move, cEditorParameter.layer_x_move,
                                                              cEditorParameter.layer_y_move, cEditorParameter.blend_offset_y, cEditorParameter.blend_offset_x,
                                                              cEditorParameter.blend_width, cEditorParameter.blend_height, cEditorParameter.zoom_scale,
                                                              cEditorParameter.rotation, cEditorParameter.blend_mode, cEditorParameter.opacity,
                                                              cEditorParameter.load_customer_image, cEditorParameter.customer_image, cEditorParameter.visable,
                                                              cEditorParameter.mask_edit, cEditorParameter.mask_image);
    //
    QString qstrTemplatePath = m_pCurrentPhotoPage->GetTemplatePath();
    char cTemplaetPath[512];
    strcpy(cTemplaetPath, qstrTemplatePath.QString::toUtf8());
    //
    qDebug() << "imag_blend is begin";
    IplImage *plImage = image_blend(pPsdContext, cTemplaetPath, p_editor_parameter);
    qDebug() << "image_blend is good__";
    delete []iVisableContextAarry;
    free_editorpata(p_editor_parameter);
    if(plImage == NULL) return;
    // 转换为QPixmap
    QPixmap *pPixmap = new QPixmap;
    vpCvImageToQImage(plImage, pPixmap);
    m_pCurrentPhotoPage->SetPhotoPagePixmap(pPixmap);
    m_pCurrentViewImage = m_pCurrentPhotoPage->GetPhotoPagePixmap();
    // 更新视图
    m_pCurGraphScene->SetViewPixmap(pPixmap);
    m_pCurGraphScene->update(m_pCurGraphScene->sceneRect());
}
// 更改当前图层的混合模式
void EditPageWidget::onChangeLayerMixModel(int iLayerMixModel)
{
    if(m_pCurGraphScene == NULL) return;
    int iLayerID = m_pCurGraphScene->GetCurLayerID().toInt();
    if(m_pCurrentPhotoPage == NULL) return;
    CPsdTemplate* pPsdTemplate = m_pCurrentPhotoPage->GetPsdTemplate();
    if(pPsdTemplate == NULL) return;
    QList<CTmpLayerData*> listTmpLayerData = pPsdTemplate->GetTmpLayerDataList();
    int iListTmpLayerDataSize = listTmpLayerData.size();
    if(iLayerID <= 0 || iLayerID >= iListTmpLayerDataSize) return;// 第0层不予操作
    // 获取模板当前层信息
    CTmpLayerData *pTmpLayerData = listTmpLayerData.at(iLayerID);
    // 混合模式相同则不合成
    if(pTmpLayerData->getBlandMode() == iLayerMixModel) return;
    // 设置混合模式
    pPsdTemplate->SetTmpLayerBlendMode(iLayerID, iLayerMixModel);
    _psd_context *pPsdContext = m_pCurrentPhotoPage->GetPsdContext();
    if(pPsdContext == NULL) return;
    // 相片信息初始化
    CEditorParameter cEditorParameter;
    cEditorParameter.set_layer_num(iLayerID);
    cEditorParameter.set_layer_or_photo(0); // 混合模式是对图层操作
    cEditorParameter.set_blend_width(pPsdTemplate->GetPsdImageWidth());
    cEditorParameter.set_blend_height(pPsdTemplate->GetPsdImageHeight());
    cEditorParameter.set_blend_mode(pTmpLayerData->getBlandMode());
    cEditorParameter.set_opacity(pTmpLayerData->getOpacity());
    cEditorParameter.set_load_customer_image(0);// 非客照
    // 获取visable数组
    int *iVisableContextAarry = new int[iListTmpLayerDataSize];
    for(int i = 0; i != iListTmpLayerDataSize; ++i)
    {
        iVisableContextAarry[i] = listTmpLayerData[i]->getVisable();
    }
    cEditorParameter.set_visable(iVisableContextAarry);
    // 移动
    cEditorParameter.set_x_move(listTmpLayerData[iLayerID]->GetLayerMoveGap().x());
    cEditorParameter.set_y_move(listTmpLayerData[iLayerID]->GetLayerMoveGap().y());
    // 缩放
    cEditorParameter.set_zoom_scale(listTmpLayerData[iLayerID]->GetLayerScaleValue());
    // 旋转
    cEditorParameter.set_rotation(listTmpLayerData[iLayerID]->GetLayerRotateValue());
    //
    _editor_parameter *p_editor_parameter = create_editorpata(cEditorParameter.layer_num, cEditorParameter.layer_or_photo,
                                                              cEditorParameter.x_move, cEditorParameter.y_move, cEditorParameter.layer_x_move,
                                                              cEditorParameter.layer_y_move, cEditorParameter.blend_offset_y, cEditorParameter.blend_offset_x,
                                                              cEditorParameter.blend_width, cEditorParameter.blend_height, cEditorParameter.zoom_scale,
                                                              cEditorParameter.rotation, cEditorParameter.blend_mode, cEditorParameter.opacity,
                                                              cEditorParameter.load_customer_image, cEditorParameter.customer_image, cEditorParameter.visable,
                                                              cEditorParameter.mask_edit, cEditorParameter.mask_image);
    //
    QString qstrTemplatePath = m_pCurrentPhotoPage->GetTemplatePath();
    char cTemplaetPath[512];
    strcpy(cTemplaetPath, qstrTemplatePath.QString::toUtf8());
    //
    qDebug() << "imag_blend is begin";
    IplImage *plImage = image_blend(pPsdContext, cTemplaetPath, p_editor_parameter);
    qDebug() << "image_blend is good__";
    delete []iVisableContextAarry;
    free_editorpata(p_editor_parameter);
    if(plImage == NULL) return;
    // 转换为QPixmap
    QPixmap *pPixmap = new QPixmap;
    vpCvImageToQImage(plImage, pPixmap);
    m_pCurrentPhotoPage->SetPhotoPagePixmap(pPixmap);
    m_pCurrentViewImage = m_pCurrentPhotoPage->GetPhotoPagePixmap();
    // 更新视图
    m_pCurGraphScene->SetViewPixmap(pPixmap);
    m_pCurGraphScene->update(m_pCurGraphScene->sceneRect());
}
// 更改当前图层透明度
void EditPageWidget::onChangeLayerTransValue(int iLayerTransValue)
{
    if(m_pCurGraphScene == NULL) return;
    int iLayerID = m_pCurGraphScene->GetCurLayerID().toInt();
    if(m_pCurrentPhotoPage == NULL) return;
    CPsdTemplate* pPsdTemplate = m_pCurrentPhotoPage->GetPsdTemplate();
    if(pPsdTemplate == NULL) return;
    QList<CTmpLayerData*> listTmpLayerData = pPsdTemplate->GetTmpLayerDataList();
    int iListTmpLayerDataSize = listTmpLayerData.size();
    if(iLayerID <= 0 || iLayerID >= iListTmpLayerDataSize) return;
    // 获取模板当前层信息
    CTmpLayerData *pTmpLayerData = listTmpLayerData.at(iLayerID);
    if(pTmpLayerData->getOpacity() == iLayerTransValue) return;
    // 设置透明度值
    pPsdTemplate->SetTmpLayerTransValue(iLayerID, iLayerTransValue);
    _psd_context *pPsdContext = m_pCurrentPhotoPage->GetPsdContext();
    if(pPsdContext == NULL) return;
    // 相片信息初始化
    CEditorParameter cEditorParameter;
    cEditorParameter.set_layer_num(iLayerID);
    cEditorParameter.set_layer_or_photo(0); // 透明度是对图层操作
    cEditorParameter.set_blend_width(pPsdTemplate->GetPsdImageWidth());
    cEditorParameter.set_blend_height(pPsdTemplate->GetPsdImageHeight());
    cEditorParameter.set_blend_mode(pTmpLayerData->getBlandMode());
    cEditorParameter.set_opacity(pTmpLayerData->getOpacity());
    cEditorParameter.set_load_customer_image(0);// 非客照
    // 获取visable数组
    int *iVisableContextAarry = new int[iListTmpLayerDataSize];
    for(int i = 0; i != iListTmpLayerDataSize; ++i)
    {
        iVisableContextAarry[i] = listTmpLayerData[i]->getVisable();
    }
    cEditorParameter.set_visable(iVisableContextAarry);
    // 移动
    cEditorParameter.set_x_move(listTmpLayerData[iLayerID]->GetLayerMoveGap().x());
    cEditorParameter.set_y_move(listTmpLayerData[iLayerID]->GetLayerMoveGap().y());
    // 缩放
    cEditorParameter.set_zoom_scale(listTmpLayerData[iLayerID]->GetLayerScaleValue());
    // 旋转
    cEditorParameter.set_rotation(listTmpLayerData[iLayerID]->GetLayerRotateValue());
    //
    _editor_parameter *p_editor_parameter = create_editorpata(cEditorParameter.layer_num, cEditorParameter.layer_or_photo,
                                                              cEditorParameter.x_move, cEditorParameter.y_move, cEditorParameter.layer_x_move,
                                                              cEditorParameter.layer_y_move, cEditorParameter.blend_offset_y, cEditorParameter.blend_offset_x,
                                                              cEditorParameter.blend_width, cEditorParameter.blend_height, cEditorParameter.zoom_scale,
                                                              cEditorParameter.rotation, cEditorParameter.blend_mode, cEditorParameter.opacity,
                                                              cEditorParameter.load_customer_image, cEditorParameter.customer_image, cEditorParameter.visable,
                                                              cEditorParameter.mask_edit, cEditorParameter.mask_image);
    //
    QString qstrTemplatePath = m_pCurrentPhotoPage->GetTemplatePath();
    char cTemplaetPath[512];
    strcpy(cTemplaetPath, qstrTemplatePath.QString::toUtf8());
    //
    qDebug() << "imag_blend is begin";
    IplImage *plImage = image_blend(pPsdContext, cTemplaetPath, p_editor_parameter);
    qDebug() << "image_blend is good__";
    delete []iVisableContextAarry;
    free_editorpata(p_editor_parameter);
    if(plImage == NULL) return;
    // 转换为QPixmap
    QPixmap *pPixmap = new QPixmap;
    vpCvImageToQImage(plImage, pPixmap);
    m_pCurrentPhotoPage->SetPhotoPagePixmap(pPixmap);
    m_pCurrentViewImage = m_pCurrentPhotoPage->GetPhotoPagePixmap();
    // 更新视图
    m_pCurGraphScene->SetViewPixmap(pPixmap);
    m_pCurGraphScene->update(m_pCurGraphScene->sceneRect());
}

void EditPageWidget::onChangeDrawTools()
{
    // 绘图
    m_pMaskDrawScene->SetDrawType(m_pDrawToolsWidget->GetDrawType());
    m_pMaskDrawScene->SetPen(m_pDrawToolsWidget->GetPen());
    m_pMaskDrawScene->SetBrush(m_pDrawToolsWidget->GetBrush());
}

void EditPageWidget::onChangeMaskLayerItem(QPixmap *pPixmap)
{
    qDebug() << "say hello" << pPixmap->depth() << pPixmap->hasAlpha();
    int iLayerID = m_iCurMaskLayerID;
    if(m_pCurrentPhotoPage == NULL) return;
    CPsdTemplate* pPsdTemplate = m_pCurrentPhotoPage->GetPsdTemplate();
    if(pPsdTemplate == NULL) return;
    QList<CTmpLayerData*> listTmpLayerData = pPsdTemplate->GetTmpLayerDataList();
    int iListTmpLayerDataSize = listTmpLayerData.size();
    if(iLayerID <= 0 || iLayerID >= iListTmpLayerDataSize) return; //  第0层蒙版暂不允许操作
    //
    _psd_context *pPsdContext = m_pCurrentPhotoPage->GetPsdContext();
    if(pPsdContext == NULL) return;
    //
    // 获取模板当前层信息
    CTmpLayerData *pTmpLayerData = listTmpLayerData.at(iLayerID);
    // 相片信息初始化
    CEditorParameter cEditorParameter;
    cEditorParameter.set_layer_num(iLayerID);
    cEditorParameter.set_layer_or_photo(0);// 图层操作
    cEditorParameter.set_blend_width(pPsdTemplate->GetPsdImageWidth());
    cEditorParameter.set_blend_height(pPsdTemplate->GetPsdImageHeight());
    cEditorParameter.set_blend_mode(pTmpLayerData->getBlandMode());
    cEditorParameter.set_opacity(pTmpLayerData->getOpacity());
    cEditorParameter.set_load_customer_image(0);// 非客照
    cEditorParameter.set_mask_edit(1);// 蒙版编辑1
    // pixmap转IplImage
    IplImage *pMaskImage = qImageToIplImage(pPixmap->toImage());
    if(pMaskImage == NULL) return;
    cvSaveImage("C:\\new.jpg", pMaskImage);
    cEditorParameter.set_mask_image(pMaskImage);
    // 获取visable数组
    int *iVisableContextAarry = new int[iListTmpLayerDataSize];
    for(int i = 0; i != iListTmpLayerDataSize; ++i)
    {
        iVisableContextAarry[i] = listTmpLayerData[i]->getVisable();
    }
    cEditorParameter.set_visable(iVisableContextAarry);
    _editor_parameter *p_editor_parameter = create_editorpata(cEditorParameter.layer_num, cEditorParameter.layer_or_photo,
                                                              cEditorParameter.x_move, cEditorParameter.y_move, cEditorParameter.layer_x_move,
                                                              cEditorParameter.layer_y_move, cEditorParameter.blend_offset_y, cEditorParameter.blend_offset_x,
                                                              cEditorParameter.blend_width, cEditorParameter.blend_height, cEditorParameter.zoom_scale,
                                                              cEditorParameter.rotation, cEditorParameter.blend_mode, cEditorParameter.opacity,
                                                              cEditorParameter.load_customer_image, cEditorParameter.customer_image, cEditorParameter.visable,
                                                              cEditorParameter.mask_edit, cEditorParameter.mask_image);
    //
    QString qstrTemplatePath = m_pCurrentPhotoPage->GetTemplatePath();
    char cTemplaetPath[512];
    strcpy(cTemplaetPath, qstrTemplatePath.QString::toUtf8());
    //
    qDebug() << "imag_blend is begin";
    IplImage *plImage = image_blend(pPsdContext, cTemplaetPath, p_editor_parameter);
    qDebug() << "image_blend is good__";
    delete []iVisableContextAarry;
    free_editorpata(p_editor_parameter);
    cvReleaseImage(&pMaskImage);
    if(plImage == NULL) return;
    // 转换为QPixmap
    QPixmap *pNewPixmap = new QPixmap;
    vpCvImageToQImage(plImage, pNewPixmap);
    m_pCurrentPhotoPage->SetPhotoPagePixmap(pNewPixmap);
    m_pCurrentViewImage = m_pCurrentPhotoPage->GetPhotoPagePixmap();
    //更新蒙版场景
    m_pMaskDrawScene->SetBackGraphPixmap(pNewPixmap);
    m_pMaskDrawScene->update(m_pMaskDrawScene->sceneRect());
    // 更新视图
    m_pCurGraphScene->SetViewPixmap(pNewPixmap);
    m_pCurGraphScene->update(m_pCurGraphScene->sceneRect());
    // 图层控件更新
    QListWidgetItem* pListWidgetItem = NULL;
    QTmpLayerControlItem *pTmpControlItem = NULL;
    int iLayerListWidgetCount = m_pTmpLayerListWidget->count();
    for(int i = 0; i != iLayerListWidgetCount; ++i)
    {
        pListWidgetItem = m_pTmpLayerListWidget->item(i);
        pTmpControlItem = (QTmpLayerControlItem *)m_pTmpLayerListWidget->itemWidget(pListWidgetItem);
        if(pTmpControlItem->getLayerID().toInt() == iLayerID)
        {
            pTmpControlItem->SetMaskPixmap(listTmpLayerData[iLayerID]->getMaskPixmap());
            break;
        }
    }
}
// 获取并加载客照数据
void EditPageWidget::LoadPhotoPixmap(int iIndex, QList<QPixmap *> &listPhotoPixmap, QStringList &listPhotoName)
{
    int iPhotoPixmapListSize = listPhotoPixmap.size();
    if(!iPhotoPixmapListSize) return;
    m_listPhotoPixmap = listPhotoPixmap;
    for(int i = iIndex; i < iPhotoPixmapListSize; ++i)
    {
        // 控件数据
        m_pPhotoListWidget->addPhotoItem(*m_listPhotoPixmap[i], listPhotoName[i-iIndex]);
    }
    this->update();
}
// 删除客照数据
void EditPageWidget::DeletePhotoPixmap(QList<int>& deleteList)
{
    int iSize = deleteList.size();
    for(int i = 0; i != iSize; ++i)
    {
        m_pPhotoListWidget->deletePhotoItem(deleteList[i]);
    }
    this->update();
}
// 假的编辑页面数据
void EditPageWidget::LoadEditWidget(QByteArray qbaComposeJson)
{
    uint iStartTime = QDateTime::currentDateTime().toTime_t();
    ReadComposeJson(qbaComposeJson);
    ComposePhotoPage();
    uint iEndTime = QDateTime::currentDateTime().toTime_t();

    uint uTime = iEndTime - iStartTime;

    qDebug() <<   "_____time____" << uTime;
}
// 获取合成页面数据
void EditPageWidget::LoadComposePhotoPage()
{
    int iComposePageSize = m_listPhotoComposePage.size();
    if(!iComposePageSize)
        return;
    for(int i = 0; i != iComposePageSize; ++i)
    {
        QString qstrI = QString::number(i);
        QPixmap *pPixmap = m_listPhotoComposePage[i]->GetPhotoPagePixmap();
        m_pPhotoPageListWidget->addPhotoItem(*pPixmap, qstrI);
    }
    this->update();
}

// 读取合成json文件，生成最终pixmap图像
bool EditPageWidget::ReadComposeJson(QByteArray qbaComposeJson)
{
    if(!qbaComposeJson.size()) return false;
    /////////////////////////////////////////////////////////////////////////////////////////
    // 获取json数据
    QFile fComposeJsonFile("F:\\imetto\\test.theme");
    if(!fComposeJsonFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "open theme json error";
        return false;
    }
    fComposeJsonFile.seek(0);
    QByteArray qFileByteArray = fComposeJsonFile.readAll();
    // qt5.0
    QJsonDocument jsonDoc = QJsonDocument::fromJson(qFileByteArray);
    QVariantMap rootMap = jsonDoc.toVariant().toMap();
    /////////////////////////////////////////////////////////////////////////////////////////
    // qt4.8 libqjson
//    QJson::Parser parser;
//    QVariantMap rootMap = parser.parse(qFileByteArray).toMap();
    //////////////////////////////////////////////////////////////////////////////////////////
    // 解析层数据
    QVariantMap composePhotoMap = rootMap["theme"].toMap();

    foreach(QString qstrPhotoPageIndex, composePhotoMap.keys())
    {// 获取每个相片信息
        CPhotoComposePage *pPhotoComposePage = new CPhotoComposePage;
        QVariantMap photoPageMap = composePhotoMap[qstrPhotoPageIndex].toMap();
        pPhotoComposePage->SetTemplatePath(photoPageMap["template_path"].toString());
        connect(pPhotoComposePage, SIGNAL(mouseChangePageLayerID(QString)), this, SLOT(onChangeGraphLayerID(QString)));
        // 获取每层数据
        QVariantMap photoLayerMap = photoPageMap["layer_data"].toMap();
        foreach(QString qstrLayerID, photoLayerMap.keys())
        {
            QVariantMap photoPathMap = photoLayerMap[qstrLayerID].toMap();
            pPhotoComposePage->AddPhoto(qstrLayerID.toInt(), photoPathMap["path"].toString());
        }
        m_listPhotoComposePage.push_back(pPhotoComposePage);
    }
    if(m_listPhotoComposePage.size())
    {
        m_pCurrentPhotoPage = m_listPhotoComposePage[0];
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////
//    QVariantMap rootMap, themeMap;
//    for(int i = 0; i < iPageSize; ++i)
//    {
//        QVariantMap pageMap;
//        pageMap.insert("template_path", "F:\\imetto\\61.5x43.7cm-06.psd_5FFADB7D2FF2970E6168304265A02633");

//        QVariantMap layerDataMap;
//        for(int j = 0; j < layerDataSize; ++j)
//        {
//            QVariantMap pathMap;
//            pathMap.insert("path", "F:\\picture\\test\\1.jpg");
//            //
//            layerDataMap.insert(layerID, pahtMap);
//        }
//        pageMap.insert("layer_data", layerDataMap);
//        //
//        themeMap.insert(iPageID, pageMap);
//    }
//    rootMap.insert("theme", themeMap);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
// 模板与相片合成为最终pixmap
bool EditPageWidget::ComposePhotoPage()
{
    // 合成pixmap
    int iPageCount = m_listPhotoComposePage.count();
    qDebug() << "compose photo page" << iPageCount;
    for(int i = 0; i < iPageCount; ++i)
    {
        // 最终图片数据
        IplImage *plImage = NULL;
        // 获取合成数据
        QString qstrTemplatePath = m_listPhotoComposePage[i]->GetTemplatePath();
        QList<CPhotoComposeLayer*> listPhotoComposeLayer = m_listPhotoComposePage[i]->GetPhotoComposeLayer();
        // 解析当前模板
        CPsdTemplate* pPsdTemplate = new CPsdTemplate(qstrTemplatePath);
        QList<CTmpLayerData*> listTmpLayerData = pPsdTemplate->GetTmpLayerDataList();
        int iListTmpLayerDataSize = listTmpLayerData.size();        
        // 构造模板内容
        char cTemplaetPath[512];
        strcpy(cTemplaetPath, qstrTemplatePath.QString::toUtf8());
        qDebug() << "cont beg" << cTemplaetPath;
        _psd_context *pPsdContext = quickrebuild(cTemplaetPath);
        qDebug() << "cont good";
        m_listPhotoComposePage[i]->SetPsdContext(pPsdContext);
        // 获取相片所在层数据信息，并生成图片
        int iPhotoLayerCount = listPhotoComposeLayer.count();
        for(int j = 0; j < iPhotoLayerCount; ++j)
        {
            int iLayerID = listPhotoComposeLayer[j]->GetLayerID();
            QString qstrPhotoSrcPath = listPhotoComposeLayer[j]->GetPhotoSrcPath();
            // 修改原模板中客照数据
            pPsdTemplate->SetTmpLayerPicture(iLayerID, qstrPhotoSrcPath);
            // 获取模板当前层信息
            CTmpLayerData *pTmpLayerData = listTmpLayerData.at(iLayerID);
            // 相片信息初始化
            CEditorParameter cEditorParameter;
            cEditorParameter.set_layer_num(iLayerID);
            cEditorParameter.set_layer_or_photo(pTmpLayerData->getType());
            if(j != iPhotoLayerCount-1)
            {
                cEditorParameter.set_blend_width(pTmpLayerData->getWidth());
                cEditorParameter.set_blend_height(pTmpLayerData->getHeight());
//                cEditorParameter.set_blend_offset_x(pTmpLayerData->getOffsetX());
//                cEditorParameter.set_blend_offset_y(pTmpLayerData->getOffsetY());

            }
            else
            {
                cEditorParameter.set_blend_width(pPsdTemplate->GetPsdImageWidth());
                cEditorParameter.set_blend_height(pPsdTemplate->GetPsdImageHeight());
            }

//            cEditorParameter.set_blend_width(pPsdTemplate->GetPsdImageWidth());
//            cEditorParameter.set_blend_height(pPsdTemplate->GetPsdImageHeight());


            cEditorParameter.set_blend_mode(pTmpLayerData->getBlandMode());
            cEditorParameter.set_opacity(pTmpLayerData->getOpacity());
            cEditorParameter.set_load_customer_image(1);// 客照
            // 客照
            IplImage* pPhotoImage = qImageToIplImage(QImage(listPhotoComposeLayer[j]->GetPhotoSrcPath()));
            cEditorParameter.set_customer_image(pPhotoImage);
            // 获取visable数组
            int *iVisableContextAarry = new int[iListTmpLayerDataSize];
            for(int i = 0; i != iListTmpLayerDataSize; ++i)
            {
                iVisableContextAarry[i] = listTmpLayerData[i]->getVisable();
            }
            cEditorParameter.set_visable(iVisableContextAarry);
            //
            _editor_parameter *p_editor_parameter = create_editorpata(cEditorParameter.layer_num, cEditorParameter.layer_or_photo,
                                                                      cEditorParameter.x_move, cEditorParameter.y_move, cEditorParameter.layer_x_move,
                                                                      cEditorParameter.layer_y_move, cEditorParameter.blend_offset_y, cEditorParameter.blend_offset_x,
                                                                      cEditorParameter.blend_width, cEditorParameter.blend_height, cEditorParameter.zoom_scale,
                                                                      cEditorParameter.rotation, cEditorParameter.blend_mode, cEditorParameter.opacity,
                                                                      cEditorParameter.load_customer_image, cEditorParameter.customer_image, cEditorParameter.visable,
                                                                      cEditorParameter.mask_edit, cEditorParameter.mask_image);
            //
            qDebug() << "imag_blend is begin" << cTemplaetPath;
            plImage = image_blend(pPsdContext, cTemplaetPath, p_editor_parameter);
            qDebug() << "image_blend is good__" ;
            cvReleaseImage(&pPhotoImage);
            delete []iVisableContextAarry;
            free_editorpata(p_editor_parameter);
        }
        // 转换为QPixmap
        QPixmap *pPixmap = new QPixmap;
        vpCvImageToQImage(plImage, pPixmap);
        m_listPhotoComposePage[i]->SetPhotoPagePixmap(pPixmap);
        // 模板数据
        m_listPhotoComposePage[i]->SetPsdTemplate(pPsdTemplate);
        // 场景创建
        m_listPhotoComposePage[i]->SetGraphDragScene(CreateGraphScene(m_listPhotoComposePage[i]));
    }
    // page页加载到控件显示
    LoadComposePhotoPage();
    //
    UpdateGraphView();
    UpdateTmpLayerListView();
    return true;

}
// 获取当前也pixmap数据
QPixmap *EditPageWidget::GetCurrentPixmap()
{
    return m_pCurrentViewImage;
}


