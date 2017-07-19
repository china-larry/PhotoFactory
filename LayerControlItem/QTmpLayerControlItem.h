// 图层控制List中Item项
#ifndef QTMPLAYERCONTROLITEM_H
#define QTMPLAYERCONTROLITEM_H

#include <QPushButton>


#include <QWidget>
#include <QPushButton>
#include <QIcon>
#include <QLabel>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QImage>

#include "QTmpLayerItemLabel.h"
#include "QTmpLayerMaskItemLabel.h"


/**
 * @brief 控件层是否可视自定义Button，绘制眼睛
 */

class QTmpLayerControlItem : public QPushButton
{
    Q_OBJECT

public:
    explicit QTmpLayerControlItem();
    explicit QTmpLayerControlItem(int iVisable, int iClipp, int iType, bool bMask);
    ~QTmpLayerControlItem();

signals:
    /*
     * 是否可视与当前选中ID分开，可视发送此信号，在初始化控件是connect，ID由main中直接判定。
     */
    void changeLayerControlItemID(QString);
    void changeLayerControlItemVisable(QString, int);
    void beginDrawMask(int);
//
private slots:
    void checkLayerVisable(int);
    void changeLayerID();
public slots:
    void updateLayerType(QString, int);
    void updateLayerVisable(QString, int);
    void onBeginDrawMask();

public:
    void setLayerID(QString qstrLayerID);
    void setLayerIndex(QString qstrLayerIndex);
    void setLayerVisable(int iVisable);
    void setLayerPixmap(QPixmap *pPixmap);
    void SetMaskPixmap(QPixmap *pPixmap);
    void SetMaskSelect(bool bSelect);
    //
    QString getLayerID();
    int getLayerVisable();
private:
    // 控件
    QTmpLayerItemLabel *m_pVisableLabel;              ///< 是否可视
    QLabel *m_pImageViewLabel;                  ///< 预览图片
    QTmpLayerMaskItemLabel *m_pMaskViewLabel;                   ///< 蒙版预览
    QLabel *m_pLayerNameLabel;                  ///< 标签索引
    QLabel *m_pLayerTypeLabel;           ///< 图层类型    //

    // 布局管理器
    QHBoxLayout *m_pLayerLayout;
    QSpacerItem *m_pSpacerItem ;

    // 数据
    int m_iVisable;                    ///< 是否可视
    int m_iClipp;                       ///< 是否关联
    int m_iLayerType;
    bool m_bMaskFlag;       ///< 是否存在蒙版层
    QString m_qstrLayerID;          ///< 图层ID名称
    QPixmap *m_pPixmap;         ///< 预览图片
    QPixmap *m_pMaskPixmap;     ///< 蒙版预览
};
#endif // QTMPLAYERCONTROLITEM_H
