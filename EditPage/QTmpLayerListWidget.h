#ifndef QTMPLAYERLISTWIDGET_H
#define QTMPLAYERLISTWIDGET_H

#include <QListWidget>

class QTmpLayerListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit QTmpLayerListWidget(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *event);
signals:
    
public slots:
    
};

#endif // QTMPLAYERLISTWIDGET_H
