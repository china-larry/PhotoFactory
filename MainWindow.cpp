#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QDirIterator>
#include <QToolBar>
#include <QDebug>
#include <QDateTime>


#include "IntelligentDesignDLL/intelligentdesigndll.h"

const int g_iSeletButtonWidth = 80;
const int g_iSeletButtonHeight = 60;
const int g_iPhotoViewWidth = 200; // 预览photo宽高
const int g_iPhotoViewHeight = 200;

// 图层控件资源
QPixmap *g_pClippPixmap = NULL;
QPixmap *g_pIsVisablePixmap = NULL;
QPixmap *g_pNotVisablePixmap = NULL;
QPixmap *g_pLayerImagePixmap = NULL;
QPixmap *g_pLayerFodderPixmap = NULL;

// 色彩调整层全局Pximap
QPixmap *g_pNormalPixmap = NULL;
QPixmap *g_pLevelsPixmap = NULL;
QPixmap *g_pCurvesPixmap = NULL;
QPixmap *g_pBrightContrastPixmap = NULL;
QPixmap *g_pColorBalancePixmap = NULL;
QPixmap *g_pHueSaturationPixmap = NULL;
QPixmap *g_pSelectiveColorPixmap = NULL;
QPixmap *g_pGradientMapPixmap = NULL;
QPixmap *g_pOthersPixmap = NULL;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this); 
    //
    m_qstrPhotoTemplateLib = "C:";
    //
    initWindows();
//    // 布局
    initLayout();
//    // 信号
    initSignals();
}

MainWindow::~MainWindow()
{
    // 资源释放
    qDeleteAll(m_listComposePhotoPixmap);
    m_listComposePhotoPixmap.clear();

    delete ui;
}
// 菜单和工具栏action
void MainWindow::createActions()
{
    // 文件    
    //
    m_pOpenFilesAct = new QAction(QIcon(":/picture/image/open_files.png"), "&导入文件", this);
    m_pOpenFilesAct->setShortcuts(QKeySequence::Open);
    connect(m_pOpenFilesAct, SIGNAL(triggered()), this,  SLOT(onOpenFilesAct()));

    m_pOpenFolderAct = new QAction(QIcon(":/picture/image/open_folder.png"), "&Input Folder", this);
    m_pOpenFolderAct->setShortcuts(QKeySequence::Open);
    connect(m_pOpenFolderAct, SIGNAL(triggered()), this,  SLOT(onOpenFolderAct()));

    m_pSelectThemplateAct = new QAction(QIcon(":/picture/image/template.png"), "&Template Lib", this);
    connect(m_pSelectThemplateAct, SIGNAL(triggered()), this,  SLOT(onSelectTemplateAct()));

    m_pSavePhotoAct = new QAction(QIcon(":/picture/image/save_files.png"), "&保存", this);
    connect(m_pSavePhotoAct, SIGNAL(triggered()), this,  SLOT(onSavePhotoAct()));

    // 编辑
    m_pForwardAct = new QAction(QIcon(":/picture/image/forward.png"), "&前进", this);
    m_pForwardAct->setShortcuts(QKeySequence::Forward);
    m_pUndoAct = new QAction(QIcon(":/picture/image/undo.png"), "&后退", this);
    m_pUndoAct->setShortcuts(QKeySequence::Undo);

    // 帮助
    m_pAboutHelpAct = new QAction("&关于", this);
}
// 菜单
void MainWindow::createMenus()
{
    // 文件
    m_pFileMenu = menuBar()->addMenu("&文件");
    m_pFileMenu->addAction(m_pOpenFilesAct);
    m_pFileMenu->addAction(m_pOpenFolderAct);
    m_pFileMenu->addAction(m_pSelectThemplateAct);
    m_pFileMenu->addAction(m_pSavePhotoAct);
    // 编辑
    m_pEditMenu = menuBar()->addMenu("&编辑");
    m_pEditMenu->addAction(m_pForwardAct);
    m_pEditMenu->addAction(m_pUndoAct);
    // 视图
    m_pViewMenu = menuBar()->addMenu("&视图");
    m_pViewMenu->addAction(m_pEditPageWidget->m_pPhotoDockWidget->toggleViewAction());
    m_pViewMenu->addAction(m_pEditPageWidget->m_pTmpDockWidget->toggleViewAction());
    m_pViewMenu->addAction(m_pEditPageWidget->m_pTmpLayerDockWidget->toggleViewAction());
    m_pViewMenu->addAction(m_pEditPageWidget->m_pOperateDockWidget->toggleViewAction());
    m_pViewMenu->addAction(m_pEditPageWidget->m_pDrawToolsDockWidget->toggleViewAction());
    // 帮助
    m_pHelpMenu = menuBar()->addMenu("&帮助");
    m_pHelpMenu->addAction(m_pAboutHelpAct);
}
// 工具栏
void MainWindow::createToolBars()
{
    m_pFileToolBar = addToolBar("&文件");
    m_pFileToolBar->addAction(m_pOpenFilesAct);
    m_pFileToolBar->addAction(m_pOpenFolderAct);
    m_pFileToolBar->addAction(m_pSelectThemplateAct);
    m_pFileToolBar->addAction(m_pSavePhotoAct);

    m_pEditToolBar = addToolBar("&编辑");
    m_pEditToolBar->addAction(m_pForwardAct);
    m_pEditToolBar->addAction(m_pUndoAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage("状态栏");
}

void MainWindow::initWindows()
{
    // 资源加载
    // 图层控件资源初始化
    g_pClippPixmap = new QPixmap(":/picture/layer_item/clipp.png");
    g_pIsVisablePixmap = new QPixmap(":/picture/layer_item/open_eye.png");
    g_pNotVisablePixmap = new QPixmap(":/picture/layer_item/close_eye.png");
    g_pLayerImagePixmap = new QPixmap(":/picture/layer_item/psd_image.png");
    g_pLayerFodderPixmap = new QPixmap(":/picture/layer_item/psd_star.png");
    // 色彩调整层资源初始化
    g_pNormalPixmap = new QPixmap(":/picture/color_adjust_layer/normal_color.png");
    g_pLevelsPixmap = new QPixmap(":/picture/color_adjust_layer/levels.png");
    g_pCurvesPixmap = new QPixmap(":/picture/color_adjust_layer/curves.png");
    g_pBrightContrastPixmap = new QPixmap(":/picture/color_adjust_layer/brightness_contrast.png");
    g_pColorBalancePixmap = new QPixmap(":/picture/color_adjust_layer/color_balance.png");
    g_pHueSaturationPixmap = new QPixmap(":/picture/color_adjust_layer/hue_saturation.png");
    g_pSelectiveColorPixmap = new QPixmap(":/picture/color_adjust_layer/selective_color.png");
    g_pGradientMapPixmap = new QPixmap(":/picture/color_adjust_layer/gradient_map.png");
    g_pOthersPixmap = new QPixmap(":/picture/color_adjust_layer/others.png");
    this->setWindowTitle("IMETTO");
 //   this->setWindowIcon(QIcon(":/picture/image/win.png"));
  //  this->resize(1200, 750);
    // 分页面初始化
    m_pInitPageWidget = new InitPageWidget(this);
    m_pEditPageWidget = new EditPageWidget(this);
    // 菜单工具栏等
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    // 选择按钮
    m_pInitButton = new QPushButton(this);
    m_pInitButton->setMaximumSize(g_iSeletButtonWidth, g_iSeletButtonHeight);
    m_pInitButton->setText("Start");

    m_pEditButton = new QPushButton(this);
    m_pEditButton->setMaximumSize(g_iSeletButtonWidth, g_iSeletButtonHeight);
    m_pEditButton->setText("Edit");

    // Tab初始化
    m_pStackedWidget = new QStackedWidget;
    m_pStackedWidget->addWidget(m_pInitPageWidget);
    m_pStackedWidget->addWidget(m_pEditPageWidget);
}

void MainWindow::initLayout()
{
    // 选择按钮
    QHBoxLayout *pSelectLayout = new QHBoxLayout;
    pSelectLayout->addStretch(1);
    pSelectLayout->addWidget(m_pInitButton);
    pSelectLayout->addWidget(m_pEditButton);
    // 主窗口
    QVBoxLayout *pMainLayout = new QVBoxLayout;
    pMainLayout->addWidget(m_pStackedWidget);
    pMainLayout->addLayout(pSelectLayout);
    // 布局
    QWidget *widget = new QWidget();
    setCentralWidget(widget);
    widget->setLayout(pMainLayout);
}

void MainWindow::initSignals()
{
    // 切换页面
    connect(m_pInitButton, SIGNAL(clicked()), this, SLOT(onSelectInitPage()));
    connect(m_pEditButton, SIGNAL(clicked()), this, SLOT(onSelectEditPage()));
    // 初始化页面
    connect(m_pInitPageWidget, SIGNAL(deletePhotoFilesEmit(QList<int>)), this, SLOT(onDeletePhotoImage(QList<int>)));
}

void MainWindow::onSelectInitPage()
{
    m_pStackedWidget->setCurrentIndex(0);
}

void MainWindow::onSelectEditPage()
{
    uint iStartTime = QDateTime::currentDateTime().toTime_t();
    qDebug() << "on selet edit";
    // 获取智能匹配路径
   qDebug() << m_listPhotoPixmapPath.size() << m_qstrPhotoTemplateLib;
   QByteArray qstrIntelligentByte = "tst";//IntelligentDesign(m_listPhotoPixmapPath, m_qstrPhotoTemplateLib);
   if(qstrIntelligentByte == "") return;
   qDebug() << qstrIntelligentByte;
   qDebug() << "good";
   uint iEndTime = QDateTime::currentDateTime().toTime_t();
   uint uTime = iEndTime - iStartTime;
   qDebug() <<   "_____time____" << uTime;
      //
    m_pStackedWidget->setCurrentIndex(1);
    m_pEditPageWidget->LoadEditWidget(qstrIntelligentByte);
}

void MainWindow::onDeletePhotoImage(QList<int> deleteList)
{
    int iDeletListSize = deleteList.size();
    if(!iDeletListSize) return;
    for(int i = 0; i < iDeletListSize; ++i)
    {
        // 删除预览数据
        m_listPhotoPixmap.removeAt(deleteList[i]);
        // 删除记录客照路径数据
        m_listPhotoPixmapPath.removeAt(deleteList[i]);
    }
    // 编辑页面数据删除
    m_pEditPageWidget->DeletePhotoPixmap(deleteList);

}

void MainWindow::openPhotoImage(QStringList qslPhotoList)
{
    uint iStartTime = QDateTime::currentDateTime().toTime_t();

    int iPhotoListSize = qslPhotoList.size();
    if(!iPhotoListSize) return;
    int iPhotoListWidgetCount = m_pInitPageWidget->getPhotoCount();
    // 加载新相册
    for(int i = 0; i != iPhotoListSize; ++i)
    {
        QPixmap pixmap(qslPhotoList[i]);
        QPixmap *pPixmap = new QPixmap(g_iPhotoViewWidth, g_iPhotoViewHeight);
        *pPixmap = pixmap.scaled(g_iPhotoViewWidth, g_iPhotoViewHeight, Qt::KeepAspectRatio);
        m_listPhotoPixmap.push_back(pPixmap);
    }
    m_listPhotoPixmapPath = qslPhotoList;
    //
    m_pInitPageWidget->updateOpenPhotoPixmap(iPhotoListWidgetCount, m_listPhotoPixmap, m_listPhotoPixmapPath);
    m_pEditPageWidget->LoadPhotoPixmap(iPhotoListWidgetCount, m_listPhotoPixmap, m_listPhotoPixmapPath);
    this->update();

    uint iEndTime = QDateTime::currentDateTime().toTime_t();
    uint uTime = iEndTime - iStartTime;
    qDebug() <<   "_____time____" << uTime;
}

void MainWindow::onOpenFilesAct()
{
    QString strDialog = "打开文件";
    QString strDir = "F:\\picture\\test\\";
    QString strFileFormat("Image (*.png *.jpg *bmp *gif)");
    QStringList strFilePathListTemp = QFileDialog::getOpenFileNames(this, strDialog, strDir, strFileFormat);
    if(!strFilePathListTemp.size())
    {// 没有模板数据
        qDebug() << "open file null";
        return;
    }
    openPhotoImage(strFilePathListTemp);
}

void MainWindow::onOpenFolderAct()
{
    QString strDialog = "open file";
    QString strDir = "F:\\picture\\test\\";
    QString str = QFileDialog::getExistingDirectory(this, strDialog, strDir, QFileDialog::ShowDirsOnly);
    //
    QStringList qslPhotoImageList;
    // 遍历目录中的图片文件
    QString strFilePath, strFileName, strsuffix;
    QStringList slFilePath;
    QDirIterator dirIter(str, QDir::Files|QDir::NoSymLinks,QDirIterator::Subdirectories);
    while(dirIter.hasNext())
    {
        strFilePath = dirIter.next();
        QFileInfo fileInfo(strFilePath);
        strsuffix = fileInfo.suffix().toLower();
        if(strsuffix == "jpg" || strsuffix == "png" || strsuffix == "bmp" || strsuffix == "gif")
        {
            qslPhotoImageList.push_back(strFilePath);
        }
    }
    openPhotoImage(qslPhotoImageList);
}

void MainWindow::onSelectTemplateAct()
{
    QString strDialog = "Template";
    QString strDir = "F:\\imetto\\";
    QString strFileFormat("Theme (*.theme)");
    QString strSelectedFilter("");
    QString strFilePathTemp = QFileDialog::getExistingDirectory(this, strDialog, strDir, QFileDialog::ShowDirsOnly);
    if(!strFilePathTemp.size())
    {// 没有模板数据
        qDebug() << "open file null";
        return;
    }
    m_qstrPhotoTemplateLib = strFilePathTemp;
    // 控件预览
}

void MainWindow::onSavePhotoAct()
{
    QPixmap *pPixmap = m_pEditPageWidget->GetCurrentPixmap();
    if(pPixmap == NULL) return;
    QString strDialog = "保存文件";
    QString strDir = "F:\\picture\\test\\";
    QString strFileFormat("Image (*.png *.jpg *bmp *gif)");
    QString strFilePath = QFileDialog::getSaveFileName(this, strDialog, strDir, strFileFormat);
    if(strFilePath == "")
    {// 没有模板数据
        qDebug() << "open file null";
        return;
    }
    pPixmap->save(strFilePath);
}
