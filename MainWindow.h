#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPixmap>


#include "InitPage/InitPageWidget.h"
#include "EditPage/EditPageWidget.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onSelectInitPage();
    void onSelectEditPage();
    void onOpenFilesAct();
    void onOpenFolderAct();
    void onSelectTemplateAct();
    void onSavePhotoAct();

public slots:
    // 初始化页面   
    // 删除相片
    void onDeletePhotoImage(QList<int>);


private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    //
    void initWindows();
    void initLayout();
    void initSignals();
    // 添加相片
    void openPhotoImage(QStringList);
    
private:
    Ui::MainWindow *ui;
    QMenu *m_pFileMenu;
    QMenu *m_pEditMenu;
    QMenu *m_pViewMenu;
    QMenu *m_pHelpMenu;
    QToolBar *m_pFileToolBar;
    QToolBar *m_pEditToolBar;
    QAction *m_pOpenFilesAct;
    QAction *m_pOpenFolderAct;
    QAction *m_pSelectThemplateAct;
    QAction *m_pSavePhotoAct;
    QAction *m_pForwardAct;
    QAction *m_pUndoAct;
    QAction *m_pAboutHelpAct;
    // 分页
    InitPageWidget *m_pInitPageWidget;
    EditPageWidget *m_pEditPageWidget;
    QPushButton *m_pInitButton;
    QPushButton *m_pEditButton;
    QStackedWidget *m_pStackedWidget;
    // 数据层
    QList<QPixmap*> m_listPhotoPixmap;  // 相册源
    QStringList m_listPhotoPixmapPath;  // 相册源路径
    QList<QPixmap*> m_listComposePhotoPixmap;   // 合成预览效果

    // 模板库
    QString m_qstrPhotoTemplateLib; // 模板主题路径
};

#endif // MAINWINDOW_H
