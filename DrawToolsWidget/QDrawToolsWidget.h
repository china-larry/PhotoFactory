// 绘图工具面板
#ifndef QDRAWTOOLSWIDGET_H
#define QDRAWTOOLSWIDGET_H

#include <QWidget>
#include <QPen>
#include <QBrush>
#include <QLabel>
#include <QRadioButton>
#include <QStackedWidget>

#include "QDrawToolsPenWidget.h"
#include "QDrawToolsBrushWidget.h"

class QDrawToolsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QDrawToolsWidget(QWidget *parent = 0);
    
signals:
    void changeDrawTools();
public slots:

private slots:
    void onSelectPen();
    void onSelectBrush();

public:
    QPen* GetPen();
    QBrush* GetBrush();
    int GetDrawType();

private:
    // 分页    
    QRadioButton *m_pPenButton;
    QRadioButton *m_pBrushButton;
    QStackedWidget *m_pStackedWidget;
    QDrawToolsPenWidget *m_pPenWidget;
    QDrawToolsBrushWidget *m_pBrushWidget;

    QPen *m_pPen;
    QBrush *m_pBrush;    
    int m_iDrawType;    // 画笔or画刷
};

#endif // QDRAWTOOLSWIDGET_H
