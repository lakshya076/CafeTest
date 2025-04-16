#include "customlineedit.h"

// Custom Line Edit class because when a QDoubleValidator is used, 'e' key is noted but we dont want that

CustomLineEdit::CustomLineEdit(QWidget *parent)
    : QLineEdit(parent)
{}

void CustomLineEdit::keyPressEvent(QKeyEvent *event)
{
    QString keyText = event->text();
    if (keyText.toLower() == "e") {
        return;
    }
    QLineEdit::keyPressEvent(event);
}
