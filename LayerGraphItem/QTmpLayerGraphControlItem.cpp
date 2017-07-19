#include "QTmpLayerGraphControlItem.h"
const qreal g_dPI = 3.1415926d;
const qreal g_dDoublePI = g_dPI * 2.0d;
////////////////////////////////////////////////////////////////////////////////
QTmpLayerGraphControlItem::QTmpLayerGraphControlItem(QRectF rectPixmap, QRectF rectLayer,
                                                     int iLayerType, QString qstrLayerID, QGraphicsItem* parent)
    : QTmpLayerGraphItem(rectPixmap, rectLayer, iLayerType, qstrLayerID, parent)
{
    //
    m_bLayerSelect = false;
    m_dPreRelativeScaleValue = m_dCurRelativeScaleValue = 1.0d;
    m_dPreRelativeAngeleValue = m_dCurRelativeAngleValue = 0.0d;
    //
    m_pPen = new QPen(QColor(255, 0, 0));
    m_pPen->setWidth(8);
    m_pPixmapPen = new QPen(QColor(0, 0, 255));
    m_pPixmapPen->setWidth(8);
    // 接受鼠标消息
  //  QGraphicsItem::setAcceptHoverEvents(true);
//    QWidget::setMouseTracking(true);
    setFlags(QGraphicsItem::ItemIsSelectable |
             QGraphicsItem::ItemSendsGeometryChanges |
             QGraphicsItem::ItemIsFocusable |
             QGraphicsItem::ItemIsMovable);
    //
    QGraphicsItem::setCursor(Qt::ArrowCursor);
    // 缩放
    m_drag = new QTmpLayerGraphDragItem(m_rectPixmap, 0, "-1", this);
    m_dDragScaleBoundWidth = m_drag->GetBoundWidth();
    m_dDragScaleBountHeight = m_drag->GetBoundHeight();
  //  connect(m_drag, SIGNAL(mouseDragLayerPos(bool, qreal,qreal)), this, SLOT(onDragLayerItem(bool, qreal,qreal)));
    connect(m_drag, SIGNAL(mouseRotateLayerPos(bool, qreal)), this, SLOT(onRotateLayerItem(bool, qreal)));
    connect(m_drag, SIGNAL(mouseReleaseRotate()), this, SLOT(onMouseReleaseRotate()));
}

QTmpLayerGraphControlItem::~QTmpLayerGraphControlItem()
{
    if(m_pPen != NULL)
    {
        delete m_pPen;
        m_pPen = NULL;
    }
}
void QTmpLayerGraphControlItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(m_bLayerSelect)
    {
        if(m_iLayerType)
        {// 客照
            painter->setPen(*m_pPixmapPen);
            painter->drawRect(m_rectLayer);
        }
    }
}

// 键盘按下事件处理函数，判断是否是向下方向键，如果是，则向下移动图形项
void QTmpLayerGraphControlItem::keyPressEvent(QKeyEvent *event)
{
    if(!m_iLayerType) return;   // 非客照图层不处理
    QPointF pointMoveGap = QPointF(0, 0);
    if(event->key() == Qt::Key_Up)
    {
        qDebug() << "key up";
        pointMoveGap.setY(-m_rectPixmap.height() / 100.0d);
    }
    else if(event->key() == Qt::Key_Down)
    {
        qDebug() << "key down";
        pointMoveGap.setY(m_rectPixmap.height() / 100.0d);
    }
    else if(event->key() == Qt::Key_Left)
    {
        qDebug() << "key left";
        pointMoveGap.setX(-m_rectPixmap.width() / 100.0d);
    }
    else if(event->key() == Qt::Key_Right)
    {
        qDebug() << "key right";
        pointMoveGap.setX(m_rectPixmap.width() / 100.0d);
    }
    else
    {
    }
    // 获取向量坐标和
    m_prePointRelativePos = m_curPointRelativePos;
    m_curPointRelativePos += pointMoveGap;
    // 外围框矩阵变换
    m_drag->SetDragTranslate(pointMoveGap.x(), pointMoveGap.y());
    // 碰撞检测
    bool bCollides = collidesWithItem(m_drag);
    if(bCollides)
    {// 存在碰撞
        // 此次数据还原
        qDebug() << "collides";
        m_curPointRelativePos = m_prePointRelativePos;
    }
    else
    {// 不存在碰撞则移动
        emit mouseMovePhotoPos(m_qstrLayerID.toInt(), m_curPointRelativePos);
    }
    m_drag->SetDragCollides(bCollides);
}
// 鼠标按下事件处理函数，设置被点击的图形项获得焦点，并改变光标外观
void QTmpLayerGraphControlItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{        
    emit mousePressLayerID(m_qstrLayerID);
    m_pointMousePress = event->pos();
    QGraphicsItem::setCursor(Qt::ClosedHandCursor);
    QGraphicsItem::update(m_rectLayer);

}
// 鼠标抬起事件处理函数，设置被点击的图形项获得焦点，并改变光标外观
void QTmpLayerGraphControlItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setFocus(Qt::MouseFocusReason);
    QGraphicsItem::setCursor(Qt::OpenHandCursor);
}

void QTmpLayerGraphControlItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //
    if(!m_iLayerType) return;   // 非客照图层不处理
    // 获取鼠标移动距离
    QPointF pointPos = event->pos();
    m_pointMoveGap = pointPos - m_pointMousePress;
    m_pointMousePress = pointPos;
    // 获取向量坐标和
    m_prePointRelativePos = m_curPointRelativePos;
    m_curPointRelativePos += m_pointMoveGap;
    // 外围框矩阵变换
    m_drag->SetDragTranslate(m_pointMoveGap.x(), m_pointMoveGap.y());
    // 碰撞检测
    bool bCollides = collidesWithItem(m_drag);
////    // 图层不存在碰撞
////    if(!m_iLayerType)
////    {
////        bCollides = false;
////    }
    if(bCollides)
    {// 存在碰撞
        // 此次数据还原
        qDebug() << "collides";
        m_curPointRelativePos = m_prePointRelativePos;
    }
    else
    {// 不存在碰撞则移动
        emit mouseMovePhotoPos(m_qstrLayerID.toInt(), m_curPointRelativePos);
    }
    m_drag->SetDragCollides(bCollides);
}

void QTmpLayerGraphControlItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    if(hasFocus())
    {
        int numDegrees = event->delta() / 8;
        qreal numSteps = (qreal)numDegrees / 750.0d;    // 每次0.01倍

        qDebug() << "wheel event" << numDegrees << numSteps;

        wheelScaleLayerItem(numSteps);

        event->accept();
    }
    else
    {
        event->ignore();
    }
}

// 右键菜单事件处理函数，为图形项添加一个右键菜单
void QTmpLayerGraphControlItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QGraphicsItem::update(m_rectLayer);
}
//
void QTmpLayerGraphControlItem::SetLayerVisable(int iLayerVisable)
{
    m_iLayerVisable = iLayerVisable;
    QGraphicsItem::update(m_rectLayer);
}
void QTmpLayerGraphControlItem::SetLayerSelect(bool bLayerSelect)
{
    m_bLayerSelect = bLayerSelect;
    // 缩放控件
    m_drag->SetItemSelectFlag(m_bLayerSelect);
    // 图层控件
    if(m_bLayerSelect && m_iLayerType)
 //   if(m_bLayerSelect)
    {
        QGraphicsItem::setCursor(Qt::OpenHandCursor);
        setFocus(Qt::MouseFocusReason);
        setZValue(99);
    }
    else
    {
        QGraphicsItem::setCursor(Qt::ArrowCursor);
        clearFocus();
        setZValue(m_iLayerType);
    }
    QGraphicsItem::update(m_rectLayer);
    //
}
int QTmpLayerGraphControlItem::GetLayerVisable()
{
    return m_iLayerVisable;
}

bool QTmpLayerGraphControlItem::GetLayerSelect()
{
    return m_bLayerSelect;
}
void QTmpLayerGraphControlItem::SetViewPixmap(QPixmap *pPixmap, QRectF rectPixmap)
{
    m_pImageView = pPixmap;
    m_rectPixmap = rectPixmap;
    // 重置数据
    calculatePixmapRect();  // 计算m_rectPixmap
    // 移动数据重置
    m_curPointRelativePos = m_prePointRelativePos = QPointF(0, 0);
    m_dPreRelativeScaleValue = m_dCurRelativeScaleValue = 1.0d;
    m_dPreRelativeAngeleValue = m_dCurRelativeAngleValue = 0.0d;
    // 缩放数据重置
    m_drag->ResetDragTransform(m_rectPixmap);
    m_dDragScaleBoundWidth = m_drag->GetBoundWidth();
    m_dDragScaleBountHeight = m_drag->GetBoundHeight();
    this->update(m_rectLayer);
}
// 鼠标中键缩放
void QTmpLayerGraphControlItem::wheelScaleLayerItem(qreal dScaleValue)
{
    if(!m_iLayerType) return;   // 非客照图层不处理
    m_dMoveScaleValue = dScaleValue;
    m_dPreRelativeScaleValue = m_dCurRelativeScaleValue;
    m_dCurRelativeScaleValue += m_dMoveScaleValue;
    //
//    fPhotoMoveScaleValue = m_dCurRelativeScaleValue;
    // 计算fPhotoMoveScaleValue的数据来源为通过photo缩放比例得到，而非通过鼠标距离获得
    // 100-98-96-94，与上次缩放数据相关
    //
    qreal fPhotoMoveScaleValue = m_dCurRelativeScaleValue / m_dPreRelativeScaleValue;
    // 移动
    float fScaleTranslateGapWidth, fScaleTranslateGapHeight;
    if(fPhotoMoveScaleValue > 1)
    {
        fScaleTranslateGapWidth = (fPhotoMoveScaleValue - 1.0f) * m_dDragScaleBoundWidth * 0.5f;
        fScaleTranslateGapHeight = (fPhotoMoveScaleValue - 1.0f) * m_dDragScaleBountHeight * 0.5f;
        //
        m_drag->SetDragTranslate(-fScaleTranslateGapWidth, -fScaleTranslateGapHeight);
    }
    else
    {
        fScaleTranslateGapWidth = (1.0f - fPhotoMoveScaleValue) * m_dDragScaleBoundWidth * 0.5f;
        fScaleTranslateGapHeight = (1.0f - fPhotoMoveScaleValue) * m_dDragScaleBountHeight * 0.5f;
        //
        m_drag->SetDragTranslate(fScaleTranslateGapWidth, fScaleTranslateGapHeight);
    }
    m_drag->SetDragScale(fPhotoMoveScaleValue, fPhotoMoveScaleValue);
    // 碰撞检测
    bool bCollides = collidesWithItem(m_drag) && dScaleValue <= 0.0d;
    if(bCollides)
    {// 存在碰撞
        // 此次数据还原
        m_dCurRelativeScaleValue = m_dPreRelativeScaleValue;
    }
    else
    {// 不存在碰撞则移动
        emit mouseScalePhotoPos(m_qstrLayerID.toInt(), m_dCurRelativeScaleValue);
    }
    m_drag->SetDragCollides(bCollides);
}

// 缩放
void QTmpLayerGraphControlItem::onMouseReleaseRotate()
{
    setFocus(Qt::MouseFocusReason);
}

void QTmpLayerGraphControlItem::onRotateLayerItem(bool bShiftKeyDown, qreal dMoveAngle)
{
    if(!m_iLayerType) return;   // 非客照图层不处理
    // 旋转
    QPointF pointMove1 = m_rectPixmap.center() - m_rectPixmap.topLeft();
    m_dCurRelativeAngleValue += dMoveAngle;
    if(m_dCurRelativeAngleValue >= g_dDoublePI)
    {
        m_dCurRelativeAngleValue -= g_dDoublePI;
    }
    if(m_dCurRelativeAngleValue <= -g_dDoublePI)
    {
        m_dCurRelativeAngleValue += g_dDoublePI;
    }
    //
//    if(bShiftKeyDown)
//    {// 45度整数旋转
//        if(m_dCurRelativeAngleValue >= 0.0d && m_dCurRelativeAngleValue < g_dPI * 0.25d)
//        {
//            m_dCurRelativeAngleValue = g_dPI * 0.25d;
//        }
//        else if(m_dCurRelativeAngleValue >= g_dPI * 0.25d && m_dCurRelativeAngleValue < g_dPI * 0.5d)
//        {
//            m_dCurRelativeAngleValue = g_dPI * 0.5d;
//        }
//        else if(m_dCurRelativeAngleValue >= g_dPI * 0.5d && m_dCurRelativeAngleValue < g_dPI * 0.75d)
//        {
//            m_dCurRelativeAngleValue = g_dPI * 0.75d;
//        }
//        else if(m_dCurRelativeAngleValue >= g_dPI * 0.75d && m_dCurRelativeAngleValue < g_dPI)
//        {
//            m_dCurRelativeAngleValue = g_dPI;
//        }
//        else if(m_dCurRelativeAngleValue >= g_dPI && m_dCurRelativeAngleValue < g_dPI * 1.25d)
//        {
//            m_dCurRelativeAngleValue = g_dPI * 1.25d;
//        }
//        else if(m_dCurRelativeAngleValue >= g_dPI * 1.25d && m_dCurRelativeAngleValue < g_dPI * 1.5d)
//        {
//            m_dCurRelativeAngleValue = g_dPI * 1.5d;
//        }
//        else if(m_dCurRelativeAngleValue >= g_dPI * 1.5d && m_dCurRelativeAngleValue < g_dPI * 1.75d)
//        {
//            m_dCurRelativeAngleValue = g_dPI * 1.75d;
//        }
//        else if(m_dCurRelativeAngleValue >= g_dPI * 1.75d && m_dCurRelativeAngleValue < g_dDoublePI)
//        {
//            m_dCurRelativeAngleValue = g_dDoublePI;
//        }
//    }
//    else
//    {
//        m_drag->SetDragTranslate(pointMove1.x(), pointMove1.y());
//        m_drag->SetDragRotate(dMoveAngle);
//        m_drag->SetDragTranslate(-pointMove1.x(), -pointMove1.y());
//    }

    m_drag->SetDragTranslate(pointMove1.x(), pointMove1.y());
    m_drag->SetDragRotate(dMoveAngle);
    m_drag->SetDragTranslate(-pointMove1.x(), -pointMove1.y());

    //
    bool bCollides = collidesWithItem(m_drag);
    if(bCollides)
    {// 存在碰撞
        // 此次数据还原
        m_dCurRelativeAngleValue = m_dPreRelativeAngeleValue;
    }
    else
    {// 不存在碰撞则移动
        m_dPreRelativeAngeleValue = m_dCurRelativeAngleValue;
        emit mouseRotatePhotoPos(m_qstrLayerID.toInt(), m_dCurRelativeAngleValue);
    }
    m_drag->SetDragCollides(bCollides);
}

void QTmpLayerGraphControlItem::onDragLayerItem(bool bScale, qreal dMoveScale, qreal dMoveAngle)
{
    if(!m_iLayerType) return;   // 非客照图层不处理
    // 缩放
    float fPhotoMoveScaleValue = 0.0f;   // 相对缩放大小
    m_dMoveScaleValue = dMoveScale;
    m_dPreRelativeScaleValue = m_dCurRelativeScaleValue;
    m_dCurRelativeScaleValue += m_dMoveScaleValue;
//    fPhotoMoveScaleValue = m_dCurRelativeScaleValue;
    // 计算fPhotoMoveScaleValue的数据来源为通过photo缩放比例得到，而非通过鼠标距离获得
    // 100-98-96-94，与上次缩放数据相关
    //
    fPhotoMoveScaleValue = m_dCurRelativeScaleValue / m_dPreRelativeScaleValue;
    // 移动
    float fScaleTranslateGapWidth, fScaleTranslateGapHeight;
    if(fPhotoMoveScaleValue > 1)
    {
        fScaleTranslateGapWidth = (fPhotoMoveScaleValue - 1.0f) * m_dDragScaleBoundWidth * 0.5f;
        fScaleTranslateGapHeight = (fPhotoMoveScaleValue - 1.0f) * m_dDragScaleBountHeight * 0.5f;
        //
        m_drag->SetDragTranslate(-fScaleTranslateGapWidth, -fScaleTranslateGapHeight);
    }
    else
    {
        fScaleTranslateGapWidth = (1.0f - fPhotoMoveScaleValue) * m_dDragScaleBoundWidth * 0.5f;
        fScaleTranslateGapHeight = (1.0f - fPhotoMoveScaleValue) * m_dDragScaleBountHeight * 0.5f;
        //
        m_drag->SetDragTranslate(fScaleTranslateGapWidth, fScaleTranslateGapHeight);
    }
    m_drag->SetDragScale(fPhotoMoveScaleValue, fPhotoMoveScaleValue);
    // 碰撞检测
    bool bCollides = collidesWithItem(m_drag);
    if(bCollides)
    {// 存在碰撞
        // 此次数据还原
        m_dCurRelativeScaleValue = m_dPreRelativeScaleValue;
    }
    else
    {// 不存在碰撞则移动
        emit mouseScalePhotoPos(m_qstrLayerID.toInt(), m_dCurRelativeScaleValue);
    }
    m_drag->SetDragCollides(bCollides);
    //
    // 旋转
    QPointF pointMove1 = m_rectPixmap.center() - m_rectPixmap.topLeft();
    m_dCurRelativeAngleValue += dMoveAngle;
    if(m_dCurRelativeAngleValue >= g_dDoublePI)
    {
        m_dCurRelativeAngleValue -= g_dDoublePI;
    }
    if(m_dCurRelativeAngleValue <= -g_dDoublePI)
    {
        m_dCurRelativeAngleValue += g_dDoublePI;
    }
    //
    m_drag->SetDragTranslate(pointMove1.x(), pointMove1.y());
    m_drag->SetDragRotate(dMoveAngle);
    m_drag->SetDragTranslate(-pointMove1.x(), -pointMove1.y());
    //
    bCollides = collidesWithItem(m_drag);
    if(bCollides)
    {// 存在碰撞
        // 此次数据还原
        m_dCurRelativeAngleValue = m_dPreRelativeAngeleValue;
    }
    else
    {// 不存在碰撞则移动
        m_dPreRelativeAngeleValue = m_dCurRelativeAngleValue;
        emit mouseRotatePhotoPos(m_qstrLayerID.toInt(), m_dCurRelativeAngleValue);
    }
    m_drag->SetDragCollides(bCollides);
}
