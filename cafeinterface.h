#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class CafeInterface;
}

class CafeInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit CafeInterface(QWidget *parent = nullptr);
    ~CafeInterface();

private slots:
    void redirectAdd();
    void redirectDelete();
    void redirectDOTD();
    void redirectUpdate();

private:
    Ui::CafeInterface *ui;
};
#endif // MAINWINDOW_H
