#include "startup.h"
#include <QFile>
#include <QIntValidator>
#include <QLabel>
#include <QMessageBox>
#include "clickablelabel.h"
#include "database.h"
#include "ui_startup.h"
#include <regex>

const std::regex Startup::uid_regex(R"(^\d+$)");
const std::regex Startup::year_regex(R"(^\d{4}$)");
const std::regex Startup::phone_regex(R"(^\d{10}$)");
const std::regex Startup::pass_regex(
    R"(^[A-Za-z0-9!@#\$%\^&\*\(\)_\+\-=\{\}:\";'.,\/<>?\|\[\]]{8,20}$)");

QString error_css = "border: 2px solid #D9534F; font:12pt; border-radius:10px; padding:2 10px;";

QString startup_lineEdit_css = R"(
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
)";

QString button_css = R"(
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
}
)";

void passwordReset()
{
    QMessageBox msg;
    msg.setIcon(QMessageBox::Information);
    msg.setWindowTitle("Password Reset");
    msg.setText("Your password has been successfully reset.");
    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();
}

Startup::Startup(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Startup)
{
    ui->setupUi(this);

    ui->stack->setCurrentIndex(0);

    connect(ui->register_startup, &QPushButton::clicked, this, &Startup::redirect_register);
    connect(ui->login_startup, &QPushButton::clicked, this, &Startup::redirect_login);

    // Setting the password fields to not show the password
    ui->pfield_register->setEchoMode(QLineEdit::Password);
    ui->cpfield_register->setEchoMode(QLineEdit::Password);
    ui->pfield_login->setEchoMode(QLineEdit::Password);
    ui->pfield_reset->setEchoMode(QLineEdit::Password);
    ui->cpfield_reset->setEchoMode(QLineEdit::Password);

    css_reset(); // Adding (or Updating) CSS to every QLineEdit on Startup Page
    ui->register_startup->setStyleSheet(button_css);
    ui->login_startup->setStyleSheet(button_css);
    ui->register_button->setStyleSheet(button_css);
    ui->login_button->setStyleSheet(button_css);
    ui->reset_button->setStyleSheet(button_css);

    // Setting Validators
    ui->pnfield_register->setValidator(new QIntValidator(ui->pnfield_register));
    ui->yfield_register->setValidator(new QIntValidator(ui->yfield_register));

    // Connecting UI Elements (Register Page)
    connect(ui->uid_register, &QLineEdit::returnPressed, this, &Startup::register_enter_check);
    connect(ui->nfield_register, &QLineEdit::returnPressed, this, &Startup::register_enter_check);
    connect(ui->pnfield_register, &QLineEdit::returnPressed, this, &Startup::register_enter_check);
    connect(ui->yfield_register, &QLineEdit::returnPressed, this, &Startup::register_enter_check);
    connect(ui->bfield_register, &QLineEdit::returnPressed, this, &Startup::register_enter_check);
    connect(ui->pfield_register, &QLineEdit::returnPressed, this, &Startup::register_enter_check);
    connect(ui->cpfield_register, &QLineEdit::returnPressed, this, &Startup::register_enter_check);
    connect(ui->register_button, &QPushButton::clicked, this, &Startup::registerFunction);
    connect(ui->r_direct_l, &ClickableLabel::clicked, this, &Startup::redirect_login);

    //Connecting UI ELements (Login Page)
    connect(ui->uid_login, &QLineEdit::returnPressed, this, &Startup::login_enter_check);
    connect(ui->pfield_login, &QLineEdit::returnPressed, this, &Startup::login_enter_check);
    connect(ui->login_button, &QPushButton::clicked, this, &Startup::loginFunction);
    connect(ui->l_direct_r, &ClickableLabel::clicked, this, &Startup::redirect_register);
    connect(ui->l_direct_f, &ClickableLabel::clicked, this, &Startup::redirect_reset);

    //Connecting UI Elements (Reset Page)
    connect(ui->uid_reset, &QLineEdit::returnPressed, this, &Startup::reset_enter_check);
    connect(ui->pfield_reset, &QLineEdit::returnPressed, this, &Startup::reset_enter_check);
    connect(ui->cpfield_reset, &QLineEdit::returnPressed, this, &Startup::reset_enter_check);
    connect(ui->reset_button, &QPushButton::clicked, this, &Startup::resetFunction);
    connect(ui->r_direct_reg, &ClickableLabel::clicked, this, &Startup::redirect_register);
}

Startup::~Startup() {}

void Startup::redirect_register()
{
    setWindowTitle("Register");
    ui->uid_register->setText("");
    ui->nfield_register->setText("");
    ui->pnfield_register->setText("");
    ui->yfield_register->setText("");
    ui->bfield_register->setText("");
    ui->pfield_register->setText("");
    ui->cpfield_register->setText("");
    ui->error_register->setText("");
    css_reset();
    ui->stack->setCurrentIndex(1);
}

void Startup::redirect_login()
{
    setWindowTitle("Login");
    ui->uid_login->setText("");
    ui->pfield_login->setText("");
    ui->error_login->setText("");
    css_reset();
    ui->stack->setCurrentIndex(2);
}

void Startup::redirect_reset()
{
    setWindowTitle("Reset Password");
    ui->uid_reset->setText("");
    ui->pfield_reset->setText("");
    ui->cpfield_reset->setText("");
    ui->error_reset->setText("");
    css_reset();
    ui->stack->setCurrentIndex(3);
}

void Startup::register_enter_check()
{
    if (!(!(ui->uid_register->text() != "")) || !(ui->nfield_register->text() != "")
        || !(ui->pnfield_register->text() != "") || !(ui->yfield_register->text() != "")
        || !(ui->bfield_register->text() != "") || !(ui->pfield_register->text() != "")
        || !(ui->cpfield_register->text() != "")) {
        registerFunction();
    } else {
        ui->error_register->setText("Please fill in all the details");
    }
}

void Startup::login_enter_check()
{
    if (!(!(ui->uid_login->text() != "") || !(ui->pfield_login->text() != ""))) {
        loginFunction();
    } else {
        ui->error_login->setText("Please fill in all the details.");
    }
}

void Startup::reset_enter_check()
{
    if (!(!(ui->uid_reset->text() != "") || !(ui->pfield_reset->text() != "")
          || !(ui->cpfield_reset->text() != ""))) {
        resetFunction();
    } else {
        ui->error_reset->setText("Please fill in all the details.");
    }
}

void Startup::registerFunction()
{
    css_reset();
    reg_uid = ui->uid_register->text();
    reg_name = ui->nfield_register->text();
    reg_phone = ui->pnfield_register->text();
    reg_year = ui->yfield_register->text();
    reg_batch = ui->bfield_register->text();
    reg_password = ui->pfield_register->text();
    reg_confirmPassword = ui->cpfield_register->text();

    if (reg_uid.length() == 0 || reg_password.length() == 0 || reg_confirmPassword.length() == 0
        || reg_name.length() == 0 || reg_phone.length() == 0 || reg_year.length() == 0
        || reg_batch.length() == 0) {
        ui->error_register->setText("Please fill in all inputs.");
    } else if (!std::regex_match(reg_uid.toStdString(), uid_regex)) {
        ui->error_register->setText("Enter a valid Enrollment Number");
        ui->uid_register->setStyleSheet(error_css);
    } else if (reg_password.length() < 8) {
        ui->error_register->setText("Password cannot be less than 8 characters");
        ui->pfield_register->setStyleSheet(error_css);
        ui->cpfield_register->setStyleSheet(error_css);
    } else if (reg_password != reg_confirmPassword) {
        ui->error_register->setText("Passwords do not match");
        ui->pfield_register->setStyleSheet(error_css);
        ui->cpfield_register->setStyleSheet(error_css);
    } else if (!std::regex_match(reg_password.toStdString(), pass_regex)) {
        ui->error_register->setText("Enter a valid password");
        ui->pfield_register->setStyleSheet(error_css);
        ui->cpfield_register->setStyleSheet(error_css);
    } else if (!std::regex_match(reg_phone.toStdString(), phone_regex)) {
        ui->error_register->setText("Enter a valid phone number");
        ui->pnfield_register->setStyleSheet(error_css);
    } else if (!std::regex_match(reg_year.toStdString(), year_regex)) {
        ui->error_register->setText("Enter a valid year of passing");
        ui->yfield_register->setStyleSheet(error_css);
    } else {
        if (Database::uidValid(reg_uid)) {
            qDebug() << "uid exists in the database.";
            ui->error_register->setText("uid already exists");
            ui->uid_register->setStyleSheet(error_css);
        } else {
            qDebug() << "Registering";
            Database::insertUser(reg_uid, reg_name, reg_phone, reg_year, reg_batch, reg_password);

            //Checking if user exists in table
            QVariantMap user = Database::getUserData(reg_uid);
            qDebug() << user["uid"] << " " << user["name"] << " " << user["phone"] << user["year"]
                     << user["batch"] << user["logged_in"];

            qDebug() << "Registration done.";
            done(1);
        }
    }
}

void Startup::loginFunction()
{
    css_reset();
    log_uid = ui->uid_login->text();
    log_password = ui->pfield_login->text();

    if (log_uid.length() == 0 || log_password.length() == 0) {
        ui->error_login->setText("Please fill in all the inputs.");
        ui->uid_login->setStyleSheet(error_css);
        ui->pfield_login->setStyleSheet(error_css);
    } else {
        if (Database::uidValid(log_uid, log_password)) {
            Database::setUserLoggedIn(log_uid);

            QVariantMap user = Database::getUserData(log_uid);
            qDebug() << user["uid"] << " " << user["name"] << " " << user["phone"] << user["year"]
                     << user["batch"] << user["logged_in"];

            qDebug() << "Login done.";
            done(1);
        } else {
            ui->error_login->setText("Incorrect Credentials.");
            ui->uid_login->setStyleSheet(error_css);
            ui->pfield_login->setStyleSheet(error_css);
        }
    }
}

void Startup::resetFunction()
{
    css_reset();
    ui->error_reset->setText("");
    reset_uid = ui->uid_reset->text();
    reset_password = ui->pfield_reset->text();
    reset_confirmPassword = ui->cpfield_reset->text();

    if (reset_uid.length() == 0 || reset_password.length() == 0
        || reset_confirmPassword.length() == 0) {
        ui->error_reset->setText("Please fill in all the inputs.");
    } else if (reset_password.length() < 8) {
        ui->error_reset->setText("Password too short.");
        ui->pfield_reset->setStyleSheet(error_css);
    } else if (reset_password != reset_confirmPassword) {
        ui->error_reset->setText("Passwords do not match.");
        ui->pfield_reset->setStyleSheet(error_css);
        ui->cpfield_reset->setStyleSheet(error_css);
    } else {
        if (Database::uidValid(reset_uid)) {
            if (Database::changePassword(reset_uid, reset_password)) {
                qDebug() << "Password changed successfully!"; //Change the credentials in DB

                passwordReset(); //Popup to show password has been reset
                redirect_login();

            } else {
                qDebug() << "Failed to change password.";
            }
        } else {
            ui->error_reset->setText("User does not exist");
            ui->uid_reset->setStyleSheet(error_css);
        }
    }
}

void Startup::css_reset()
{
    ui->uid_register->setStyleSheet(startup_lineEdit_css);
    ui->nfield_register->setStyleSheet(startup_lineEdit_css);
    ui->pnfield_register->setStyleSheet(startup_lineEdit_css);
    ui->yfield_register->setStyleSheet(startup_lineEdit_css);
    ui->bfield_register->setStyleSheet(startup_lineEdit_css);
    ui->pfield_register->setStyleSheet(startup_lineEdit_css);
    ui->cpfield_register->setStyleSheet(startup_lineEdit_css);
    ui->uid_login->setStyleSheet(startup_lineEdit_css);
    ui->pfield_login->setStyleSheet(startup_lineEdit_css);
    ui->uid_reset->setStyleSheet(startup_lineEdit_css);
    ui->pfield_reset->setStyleSheet(startup_lineEdit_css);
    ui->cpfield_reset->setStyleSheet(startup_lineEdit_css);
}

void Startup::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        qDebug() << "Escape key ignored!";
        return; // Ignore the Escape key press
    }
    QDialog::keyPressEvent(event);
}
