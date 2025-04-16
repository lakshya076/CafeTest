#ifndef UPDATE_H
#define UPDATE_H

#include <QDialog>
#include <QKeyEvent>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QMessageBox>

namespace Ui {
class Update;
}

class Update : public QDialog
{
    Q_OBJECT

public:
    explicit Update(QWidget *parent = nullptr);
    ~Update();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateItemFunction();

private:
    Ui::Update *ui;

    double priceNew;
    int qtyNew;
};

#endif // UPDATE_H
