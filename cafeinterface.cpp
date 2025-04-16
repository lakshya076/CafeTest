#include "cafeinterface.h"
#include "ui_cafeinterface.h"
#include "add.h"
#include "delete.h"
#include "dotd.h"
#include "update.h"

CafeInterface::CafeInterface(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CafeInterface)
{
    ui->setupUi(this);

    connect(ui->addButton, &QPushButton::clicked, this, &CafeInterface::redirectAdd);
    connect(ui->deleteButton, &QPushButton::clicked, this, &CafeInterface::redirectDelete);
    connect(ui->dotdButton, &QPushButton::clicked, this, &CafeInterface::redirectDOTD);
    connect(ui->updateButton, &QPushButton::clicked, this, &CafeInterface::redirectUpdate);
}

CafeInterface::~CafeInterface()
{
    delete ui;
}

void CafeInterface::redirectAdd(){
    Add *addDialog = new Add(this);
    addDialog->exec();
}

void CafeInterface::redirectDelete(){
    Delete *deleteDialog = new Delete(this);
    deleteDialog->exec();
}

void CafeInterface::redirectDOTD(){
    Dotd *dotdDialog = new Dotd(this);
    dotdDialog->exec();
}

void CafeInterface::redirectUpdate(){
    Update *updateDialog = new Update(this);
    updateDialog->exec();
}
