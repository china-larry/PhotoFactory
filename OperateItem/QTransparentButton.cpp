#include "QTransparentButton.h"
#include <QHBoxLayout>

QSliderButtonEx::QSliderButtonEx(QLineEdit *pLineEdit)
    : QPushButton(pLineEdit)
{
    setMaximumSize(20, 20);
    setMinimumSize(20, 20);
    setFocusPolicy(Qt::NoFocus);
    setFlat(true);
    setCursor(QCursor(Qt::PointingHandCursor));
    //
    QPixmap pixmap(":/picture/layer_item/transparent.png");
    QIcon icon(pixmap);
    setIcon(icon);

    QHBoxLayout *pHBoxLayout = new QHBoxLayout();
    pHBoxLayout->setContentsMargins(0, 0, 0, 0);
    pHBoxLayout->addStretch();
    pHBoxLayout->addWidget(this);
    pLineEdit->setLayout(pHBoxLayout);
}
