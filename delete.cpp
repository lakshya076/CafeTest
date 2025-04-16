#include "delete.h"
#include <QString>
#include "database.h"
#include "ui_delete.h"

QString deleteStyleSheet = R"(
    QComboBox {
        padding: 5px 10px 5px 10px;
        background-color: #1e1e1e;
    }
    QComboBox QAbstractItemView {
        padding: 0px;
        margin: 0px;
    }

    QPushButton {
        background-color: #3FA7D6;  /* Primary Button Color */
        color: #E0E0E0;  /* Text Color */
        border: 2px solid #252525;
        border-radius: 10px;
        padding: 6px 12px;
    }
    QPushButton:hover {
        background-color: #2D89B5;  /* Slightly darker blue for hover effect */
    }
    QPushButton:pressed {
        background-color: #1F6E96;  /* Even darker blue for pressed state */
    }
    QPushButton:disabled {
        background-color: #4A4F55;  /* Disabled Button Color */
        color: #7A828A;  /* Dimmed Text Color */
        border: 2px solid #383838;
    }
    QPushButton[danger="true"] {
        background-color: #D9534F;  /* Danger Button Color */
        color: #FFFAF0;
    }
    QPushButton[danger="true"]:hover {
        background-color: #B5423C;
    }
    QPushButton[secondary="true"] {
        background-color: #556B8D;  /* Secondary Button */
        color: #FFFAF0;
    }
    QPushButton[secondary="true"]:hover {
        background-color: #485A7A;
    })";

Delete::Delete(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Delete)
{
    ui->setupUi(this);

    setWindowTitle("Delete");
    this->setStyleSheet(deleteStyleSheet);

    Database::populateDD(ui->deleteDD);

    connect(ui->deleteItemButton, &QPushButton::clicked, this, &Delete::deleteItemFunction);
}

Delete::~Delete()
{
    delete ui;
}

void Delete::deleteItemFunction()
{
    int id = ui->deleteDD->currentData().toInt();

    Database::deleteItem(id, ui->deleteDD);
}

void Delete::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        qDebug() << "Escape key ignored!";
        return; // Ignore the Escape key press
    }
    QDialog::keyPressEvent(event);
}
