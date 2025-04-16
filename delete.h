#ifndef DELETE_H
#define DELETE_H

#include <QDialog>
#include <QKeyEvent>

namespace Ui {
class Delete;
}

class Delete : public QDialog
{
    Q_OBJECT

public:
    explicit Delete(QWidget *parent = nullptr);
    ~Delete();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void deleteItemFunction();

private:
    Ui::Delete *ui;
};

#endif // DELETE_H
