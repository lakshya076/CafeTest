#include "add.h"
#include "customlineedit.h"
#include "database.h"
#include "ui_add.h"


QString addError_css = "border: 2px solid #D9534F; font:12pt; border-radius:10px; padding:2 10px;";
QString textBoxStyleSheet = R"(
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
    })";
QString addStyleSheet = R"(
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

Add::Add(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Add)
{
    ui->setupUi(this);

    // Set dialog properties if needed
    setWindowTitle("Add");

    this->setStyleSheet(addStyleSheet);
    connect(ui->addItemButton, &QPushButton::clicked, this, &Add::addItemFunction);

    connect(ui->itemName, &QLineEdit::returnPressed, this, &Add::addItemFunction);
    connect(ui->itemPrice, &QLineEdit::returnPressed, this, &Add::addItemFunction);
    connect(ui->itemAvailQty, &QLineEdit::returnPressed, this, &Add::addItemFunction);
    connect(ui->itemIndiOne, &QLineEdit::returnPressed, this, &Add::addItemFunction);
    connect(ui->itemIndiTwo, &QLineEdit::returnPressed, this, &Add::addItemFunction);
    connect(ui->itemIndiThree, &QLineEdit::returnPressed, this, &Add::addItemFunction);

    //DropDown
    ui->itemDD->addItem("Vegetarian");
    ui->itemDD->addItem("Non-Vegetarian");

    // Setting Line Edit Validators
    ui->itemPrice->setValidator(new QDoubleValidator(ui->itemPrice));
    ui->itemAvailQty->setValidator(new QIntValidator(ui->itemAvailQty));
}

Add::~Add()
{
    delete ui;
}

void Add::addItemFunction()
{
    qDebug();

    css_reset();
    name = ui->itemName->text();

    if (ui->itemDD->currentIndex() == 0) {
        isVeg = true;
    } else if (ui->itemDD->currentIndex() == 1) {
        isVeg = false;
    }

    indicatorOne = ui->itemIndiOne->text();
    indicatorTwo = ui->itemIndiTwo->text();
    indicatorThree = ui->itemIndiThree->text();
    price = ui->itemPrice->text().toDouble();
    availqty = ui->itemAvailQty->text().toInt();

    if (name.length() == 0) {
        ui->itemName->setStyleSheet(addError_css);
        ui->error_add->setText("Item Name cannot be blank");
    } else if (ui->itemDD->currentIndex() == -1) {
        ui->error_add->setText("Choose an option from the dropdown");
    } else if (ui->itemPrice->text().length() == 0) {
        ui->itemPrice->setStyleSheet(addError_css);
        ui->error_add->setText("Enter a price");
    } else if (ui->itemPrice->text().toDouble() == 0.0) {
        ui->itemPrice->setStyleSheet(addError_css);
        ui->error_add->setText("Price cannot be zero");
    } else if (ui->itemAvailQty->text().length() == 0) {
        ui->itemAvailQty->setStyleSheet(addError_css);
        ui->error_add->setText("Enter available quantity");
    } else if (ui->itemAvailQty->text().toInt() == 0) {
        ui->itemAvailQty->setStyleSheet(addError_css);
        ui->error_add->setText("Available Quantity cannot be zero");
    } else {
        qDebug() << "Name:" << name << "Vegetarian:" << isVeg << "Indicator 1:" << indicatorOne
                 << "Indicator 2:" << indicatorTwo << "Indicator 3:" << indicatorThree
                 << "Price:" << price << "Available Quantity:" << availqty;

        Database::addItem(name, isVeg, indicatorOne, indicatorTwo, indicatorThree, price, availqty);

        QMessageBox::information(this, "Success", "Item inserted successfully!");
        reset();
    }

    qDebug();
}

void Add::css_reset()
{
    this->setStyleSheet(addStyleSheet);
    ui->error_add->clear();
    ui->itemName->setStyleSheet(textBoxStyleSheet);
    ui->itemPrice->setStyleSheet(textBoxStyleSheet);
    ui->itemAvailQty->setStyleSheet(textBoxStyleSheet);
    ui->itemIndiOne->setStyleSheet(textBoxStyleSheet);
    ui->itemIndiTwo->setStyleSheet(textBoxStyleSheet);
    ui->itemIndiThree->setStyleSheet(textBoxStyleSheet);
}

void Add::reset()
{
    ui->itemName->clear();
    ui->itemDD->setCurrentIndex(-1);
    ui->itemPrice->clear();
    ui->itemAvailQty->clear();
    ui->itemIndiOne->clear();
    ui->itemIndiTwo->clear();
    ui->itemIndiThree->clear();
    ui->error_add->clear();
}

void Add::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        return;
    }
    QDialog::keyPressEvent(event);
}
