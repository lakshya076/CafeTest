#ifndef STARTUP_H
#define STARTUP_H

#include <regex>

#include <QWidget>
#include <QDialog>
#include <QKeyEvent>
#include <QDebug>
#include <QFile>
#include <QPushButton>
#include <QtUiTools/QUiLoader>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QLineEdit>
#include <QLabel>

namespace Ui {
class Startup;
}

class Startup : public QDialog{
    Q_OBJECT

public:
    explicit Startup(QWidget* parent = nullptr);
    ~Startup();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    Ui::Startup *ui;

    QString reg_uid;
    QString reg_name;
    QString reg_phone;
    QString reg_year;
    QString reg_batch;
    QString reg_password;
    QString reg_confirmPassword;
    QString log_uid;
    QString log_password;
    QString reset_uid;
    QString reset_password;
    QString reset_confirmPassword;

    static const std::regex uid_regex;
    static const std::regex year_regex;
    static const std::regex phone_regex;
    static const std::regex pass_regex;

    //Member Functions
    void redirect_register();
    void redirect_login();
    void redirect_reset();
    void register_enter_check();
    void login_enter_check();
    void reset_enter_check();
    void registerFunction();
    void loginFunction();
    void resetFunction();

    void css_reset();
};

#endif // STARTUP_H
