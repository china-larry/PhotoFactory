/**
 * 编辑器页面控件
 *
 */
#ifndef EDITPAGEWIDGET_H
#define EDITPAGEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QButtonGroup>
#include <QDockWidget>
#include <QMainWindow>
#include "PhotoData/CEditorParameter.h"
#include "PhotoData/CPhotoComposePage.h"
#include "PhotoItem/QPhotoDragListWidget.h"
#include "PhotoItem/QPhotoListWidget.h"

#include "GraphViewScene/QGraphDragView.h"
#include "GraphViewScene/QGraphDragScene.h"
#include "GraphViewScene/QGraphMaskScene.h"

#include "OperateItem/QOperateLabel.h"
#include "DrawToolsWidget/QDrawToolsWidget.h"
#include "QTmpLayerListWidget.h"


class EditPageWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit EditPageWidget(QWidget *parent = 0);    
    void resizeEvent ( QResizeEvent * event );
signals:
    
private:
    void InitPhotoListWidget();
    void InitGraphView();
    void InitTmpListWidget();
    void InitTmpLayerListView();
    void InitOperateWidget();
    void InitDrawToolsWidget();
    //
    void ResizeDragScene();
    void ResizeMaskScene();
    // 更新视图区域
    bool UpdateGraphView();
    QGraphDragScene* CreateGraphScene(CPhotoComposePage *pPhotoComposePage);
    bool UpdateTmpLayerListView();
    // 坐标变换
    bool TranslatePhotoItem(int iLayerID);
    //
    void UpdateOperateWidget();
    // 蒙版选中更改，重新设定场景，重置蒙版选中状态
    void UpdateControlMaskLayer();

private slots:
    void onChangeComposePage(int);  // 切换页面
    void onDropPhotoPath(QString);  // 拖放客照
    void onBeginDrawMaskLayer(int);
public slots:
    void onChangeGraphLayerID(QString);
    void onMoveGraphPhotoItem(int, QPointF);
    void onSclaeGraphPhotoItem(int, qreal);
    void onRotateGraphPhotoItem(int, qreal);
    void onChangeControlLayerID(QString);
    void onChangeLayerVisable(QString, int);
    // 操作面板
    void onChangeLayerMixModel(int);
    void onChangeLayerTransValue(int);
    // 绘图工具
    void onChangeDrawTools();
    // 蒙版绘图数据
    void onChangeMaskLayerItem(QPixmap*);
public:
    void LoadPhotoPixmap(int iIndex, QList<QPixmap *> &listPhotoPixmap, QStringList &listPhotoName);
    void DeletePhotoPixmap(QList<int>& deleteList);
    void LoadEditWidget(QByteArray qbaComposeJson);
    void LoadComposePhotoPage();
    // 获取生成完成的模板相片json文件
    bool ReadComposeJson(QByteArray qbaComposeJson);
    // 合成相片
    bool ComposePhotoPage();
    // 获取当前合成完成图片保存
    QPixmap* GetCurrentPixmap();

public:
    // dock控件
    QDockWidget *m_pPhotoDockWidget;
    QDockWidget *m_pGraphDockWidget;
    QDockWidget *m_pTmpDockWidget;
    QDockWidget *m_pTmpLayerDockWidget;
    QDockWidget *m_pOperateDockWidget;
    QDockWidget *m_pDrawToolsDockWidget;

private:
    QPhotoDragListWidget *m_pPhotoListWidget;   // 相片库预览
    // 编辑视图
    QGraphDragView *m_pGraphView;
    QGraphDragScene *m_pCurGraphScene;
    QGraphMaskScene *m_pMaskDrawScene;       // 蒙版绘制场景
    // 模板
    QPhotoListWidget *m_pPhotoPageListWidget;          // 合成模板预览
    QTmpLayerListWidget *m_pTmpLayerListWidget;            // 模块图层控件
    QButtonGroup *m_pTmpLayerButtonGroup;
    // 操作区域
    QOperateLabel *m_pOperateLabel;
    // 绘图工具
    QDrawToolsWidget *m_pDrawToolsWidget;
    // 数据层
    QList<QPixmap*> m_listPhotoPixmap;  // 相册数据引用
    QList<CPhotoComposePage*> m_listPhotoComposePage; // 合成页数据
    // 模板库
    QPixmap* m_pCurrentViewImage;
    CPsdTemplate *m_pCurPsdTemplate;
    CPhotoComposePage* m_pCurrentPhotoPage;
    //
    qreal m_dViewImageScale;
    qreal m_fPsdImageScale;
    //
    bool m_bBeginDrawMask;
    int m_iCurMaskLayerID;
};

#endif // EDITPAGEWIDGET_H
