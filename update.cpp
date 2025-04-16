#include "update.h"
#include "customlineedit.h"
#include "database.h"
#include "ui_update.h"

QString updateStyleSheet = R"(
    QLineEdit {
        background-color: #2D2D2D;
        selection-color: #2D2D2D;
        color: #E0E0E0;  /* Text Color */
        border-width: 1px;
        border-style: solid;
        border-color: #383838 #383838 #383838 #383838;
        border-radius: 10px;
        padding: 2 10px;
        font: 12pt \"Segoe UI\";
    }
    QLineEdit:focus {
        border: 1px solid #3FA7D6;  /* Highlight border on focus */
    }

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

Update::Update(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Update)
{
    ui->setupUi(this);

    setWindowTitle("Update");
    this->setStyleSheet(updateStyleSheet);

    Database::populateDD(ui->updateDD);

    ui->updateNewPrice->setValidator(new QDoubleValidator(ui->updateNewPrice));
    ui->updateNewQty->setValidator(new QIntValidator(ui->updateNewQty));

    connect(ui->updateItemButton, &QPushButton::clicked, this, &Update::updateItemFunction);
}

Update::~Update()
{
    delete ui;
}

void Update::updateItemFunction()
{
    int id = ui->updateDD->currentData().toInt();
    priceNew = ui->updateNewPrice->text().toDouble();
    qtyNew = ui->updateNewQty->text().toInt();

    if (ui->updateNewPrice->text().length() == 0 && ui->updateNewQty->text().length() == 0) {
        QMessageBox::critical(this, "Error", "Enter at least one value");
    } else if (ui->updateNewPrice->text().length() != 0 && ui->updateNewQty->text().length() == 0) {
        if (ui->updateNewPrice->text().toDouble() == 0.0) {
            QMessageBox::warning(this, "Warning", "Updated Price cannot be zero");
        } else {
            qDebug() << "Only price will be updated";
            Database::updateItem(id, priceNew, QString::fromStdString("items"));
            ui->updateDD->setCurrentIndex(-1);

            QMessageBox::information(this, "Info", "Price updated successfully");
            ui->updateNewPrice->clear();
        }
    } else if (ui->updateNewPrice->text().length() == 0 && ui->updateNewQty->text().length() != 0) {
        if (ui->updateNewQty->text().toInt() == 0) {
            QMessageBox::warning(this, "Warning", "Updated Quantity cannot be zero");
        } else {
            qDebug() << "Only quantity will be updated";
            Database::updateItem(id, qtyNew, QString::fromStdString("items"));
            ui->updateDD->setCurrentIndex(-1);

            QMessageBox::information(this, "Info", "Quantity updated successfully");
            ui->updateNewQty->clear();
        }
    } else if (ui->updateNewPrice->text().length() != 0 && ui->updateNewQty->text().length() != 0) {
        if (ui->updateNewPrice->text().toDouble() == 0 || ui->updateNewQty->text().toInt() == 0) {
            QMessageBox::warning(this, "Warning", "Values cannot be zero");
        } else {
            qDebug() << "Both price and quantity will be updated";
            Database::updateItem(id, priceNew, qtyNew, QString::fromStdString("items"));
            ui->updateDD->setCurrentIndex(-1);

            QMessageBox::information(this, "Info", "Price and Quantity updated successfully");
            ui->updateNewPrice->clear();
            ui->updateNewQty->clear();
        }
    }
}

void Update::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        qDebug() << "Escape key ignored!";
        return; // Ignore the Escape key press
    }
    QDialog::keyPressEvent(event);
}
