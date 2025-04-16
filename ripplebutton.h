#ifndef RIPPLEBUTTON_H
#define RIPPLEBUTTON_H

#include <QPropertyAnimation>
#include <QPushButton>
#include <QTimer>
#include <QMouseEvent>
#include <QPainter>

class RippleButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(qreal rippleRadius READ rippleRadius WRITE setRippleRadius)

public:
    RippleButton(QWidget *parent = nullptr);

    qreal rippleRadius() const { return m_rippleRadius; }
    void setRippleRadius(qreal radius);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QPoint m_clickPos;
    qreal m_rippleRadius = 0;
    QPropertyAnimation *m_anim = nullptr;
};

#endif // RIPPLEBUTTON_H
