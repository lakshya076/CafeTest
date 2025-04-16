#ifndef INTERFACECHOOSER_H
#define INTERFACECHOOSER_H

#include <QWidget>
#include <QDialog>
#include <QKeyEvent>
#include <QFile>

namespace Ui {
class InterfaceChooser;
}

class InterfaceChooser : public QDialog {
    Q_OBJECT

public:
    explicit InterfaceChooser(QWidget* parent = nullptr);
    ~InterfaceChooser();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void userInterfaceFunction();
    void cafeInterfaceFucntion();

private:
    Ui::InterfaceChooser *ui;
};

#endif // INTERFACECHOOSER_H
