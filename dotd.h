#ifndef DOTD_H
#define DOTD_H

#include <QDate>
#include <QDialog>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QKeyEvent>
#include <QMessageBox>

namespace Ui {
class Dotd;
}

class Dotd : public QDialog
{
    Q_OBJECT

public:
    explicit Dotd(QWidget *parent = nullptr);
    ~Dotd();

private slots:
    void onDotdDDChanged();
    void updateDotdFunction();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::Dotd *ui;

    double dotdPriceNew;
    int dotdQtyNew;
};

#endif // DOTD_H
