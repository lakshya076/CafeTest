#ifndef INTERFACECHOOSER_H
#define INTERFACECHOOSER_H

#include <QDialog>
#include <QFile>
#include <QKeyEvent>
#include <QWidget>

namespace Ui {
class InterfaceChooser;
}

class InterfaceChooser : public QDialog
{
    Q_OBJECT

public:
    explicit InterfaceChooser(QWidget *parent = nullptr);
    ~InterfaceChooser();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void userInterfaceFunction();
    void cafeInterfaceFucntion();

private:
    Ui::InterfaceChooser *ui;
};

#endif // INTERFACECHOOSER_H
