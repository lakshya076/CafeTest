#include "ripplebutton.h"

RippleButton::RippleButton(QWidget *parent)
    : QPushButton(parent)
{
    m_anim = new QPropertyAnimation(this, "rippleRadius", this);
    m_anim->setDuration(400);
    m_anim->setEasingCurve(QEasingCurve::OutCubic);
}

void RippleButton::setRippleRadius(qreal radius)
{
    m_rippleRadius = radius;
    update(); // Trigger repaint
}

void RippleButton::mousePressEvent(QMouseEvent *event)
{
    m_clickPos = event->pos();
    setRippleRadius(0);

    m_anim->stop();
    m_anim->setStartValue(0);
    m_anim->setEndValue(width() * 1.2); // ripple expands outward
    m_anim->start();

    QPushButton::mousePressEvent(event); // Still trigger click
}

void RippleButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event); // draw normal button

    if (m_rippleRadius > 0.0) {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        QColor rippleColor = QColor(0, 0, 0, 60); // semi-transparent black
        p.setBrush(rippleColor);
        p.setPen(Qt::NoPen);
        p.drawEllipse(m_clickPos,
                      static_cast<int>(m_rippleRadius),
                      static_cast<int>(m_rippleRadius));
    }
}
