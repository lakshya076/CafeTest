#include "dotd.h"
#include <QDate>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QMessageBox>
#include "customlineedit.h"
#include "database.h"
#include "ui_dotd.h"

QString dotdStyleSheet = R"(
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

Dotd::Dotd(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dotd)
{
    ui->setupUi(this);

    this->setStyleSheet(dotdStyleSheet);

    QDate today = QDate::currentDate();
    int day = today.dayOfWeek();

    ui->dotdDD->addItem("Monday", 1);
    ui->dotdDD->addItem("Tuesday", 2);
    ui->dotdDD->addItem("Wednesday", 3);
    ui->dotdDD->addItem("Thursday", 4);
    ui->dotdDD->addItem("Friday", 5);
    ui->dotdDD->addItem("Saturday", 6);
    ui->dotdDD->addItem("Sunday", 7);

    ui->dotdDD->setCurrentIndex(day - 1);
    onDotdDDChanged();

    // Setting Validators
    ui->dotdNewPrice->setValidator(new QDoubleValidator(ui->dotdNewPrice));
    ui->dotdNewQty->setValidator(new QIntValidator(ui->dotdNewQty));

    // Signal Slotting
    connect(ui->dotdDD, &QComboBox::currentIndexChanged, this, &Dotd::onDotdDDChanged);
    connect(ui->updateDotdButton, &QPushButton::clicked, this, &Dotd::updateDotdFunction);
}

Dotd::~Dotd()
{
    delete ui;
}

void Dotd::onDotdDDChanged()
{
    QVariantMap dotd = Database::getDOTD(ui->dotdDD->currentData().toInt());
    qDebug() << dotd["name"] << dotd["is_vegetarian"] << dotd["indicator1"] << dotd["indicator2"]
             << dotd["indicator3"] << dotd["price"] << dotd["available_qty"];

    ui->dotdNameCI->setText(dotd["name"].toString());
    ui->dotdIndi1CI->setText(dotd["indicator1"].toString());
    ui->dotdIndi2CI->setText(dotd["indicator2"].toString());
    ui->dotdIndi3CI->setText(dotd["indicator3"].toString());
}

void Dotd::updateDotdFunction()
{
    int id = ui->dotdDD->currentData().toInt();
    dotdPriceNew = ui->dotdNewPrice->text().toDouble();
    dotdQtyNew = ui->dotdNewQty->text().toInt();

    if (ui->dotdNewPrice->text().length() == 0 && ui->dotdNewQty->text().length() == 0) {
        QMessageBox::critical(this, "Error", "Enter at least one value to update");
    } else if (ui->dotdNewPrice->text().length() != 0 && ui->dotdNewQty->text().length() == 0) {
        if (ui->dotdNewPrice->text().toDouble() == 0.0) {
            QMessageBox::warning(this, "Warning", "Updated Price cannot be zero");
        } else {
            qDebug() << "Only DOTD price will be updated";
            Database::updateItem(id, dotdPriceNew, QString::fromStdString("dotd"));

            QMessageBox::information(this, "Info", "Price updated successfully");
            ui->dotdNewPrice->clear();
        }
    } else if (ui->dotdNewPrice->text().length() == 0 && ui->dotdNewQty->text().length() != 0) {
        if (ui->dotdNewQty->text().toInt() == 0) {
            QMessageBox::warning(this, "Warning", "Updated Quantity cannot be zero");
        } else {
            qDebug() << "Only DOTD quantity will be updated";
            Database::updateItem(id, dotdQtyNew, QString::fromStdString("dotd"));

            QMessageBox::information(this, "Info", "Quantity updated successfully");
            ui->dotdNewQty->clear();
        }
    } else if (ui->dotdNewPrice->text().length() != 0 && ui->dotdNewQty->text().length() != 0) {
        if (ui->dotdNewPrice->text().toDouble() == 0 || ui->dotdNewQty->text().toInt() == 0) {
            QMessageBox::warning(this, "Warning", "Values cannot be zero");
        } else {
            qDebug() << "Both DOTD price and quantity will be updated";
            Database::updateItem(id, dotdPriceNew, dotdQtyNew, QString::fromStdString("dotd"));

            QMessageBox::information(this, "Info", "Price and Quantity updated successfully");
            ui->dotdNewPrice->clear();
            ui->dotdNewQty->clear();
        }
    }
}

void Dotd::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        qDebug() << "Escape key ignored!";
        return; // Ignore the Escape key press
    }
    QDialog::keyPressEvent(event);
}
