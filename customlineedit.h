#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>

class CustomLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit CustomLineEdit(QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

};

#endif // CUSTOMLINEEDIT_H
