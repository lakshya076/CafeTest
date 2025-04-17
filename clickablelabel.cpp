#include "clickablelabel.h"

// Custom QLabel class to make QLable clickable (used in Startup window)

ClickableLabel::ClickableLabel(QWidget *parent)
    : QLabel(parent)
{}

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
}
