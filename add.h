// add.h
#ifndef ADD_H
#define ADD_H

#include <QDialog>
#include <QKeyEvent>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QMessageBox>

namespace Ui {
class Add;
}

class Add : public QDialog
{
    Q_OBJECT

public:
    explicit Add(QWidget *parent = nullptr);
    ~Add();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void addItemFunction();
    void reset();
    void css_reset();

private:
    Ui::Add *ui;

    QString name;
    bool isVeg;
    double price;
    int availqty;
    QString indicatorOne;
    QString indicatorTwo;
    QString indicatorThree;
};

#endif // ADD_H
