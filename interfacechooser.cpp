#include "interfacechooser.h"
#include "ui_interfacechooser.h"

InterfaceChooser::InterfaceChooser(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::InterfaceChooser)
{
    ui->setupUi(this);
    setWindowTitle("Choose Interface");

    connect(ui->userInterfaceButton, &QPushButton::clicked, this, &InterfaceChooser::userInterfaceFunction);
    connect(ui->cafeInterfaceButton, &QPushButton::clicked, this, &InterfaceChooser::cafeInterfaceFucntion);
}

InterfaceChooser::~InterfaceChooser()
{
    delete ui;
}

void InterfaceChooser::userInterfaceFunction(){
    done(1);
}

void InterfaceChooser::cafeInterfaceFucntion(){
    done(2);
}

void InterfaceChooser::keyPressEvent(QKeyEvent* event){
    if (event->key() == Qt::Key_Escape) {
        qDebug() << "Escape key ignored!";
        return; // Ignore the Escape key press
    }
    QDialog::keyPressEvent(event);
}
