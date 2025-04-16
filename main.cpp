#include <QApplication>
#include <QStyle>
#include <QMainWindow>
#include <QCoreApplication>

#include "startup.h"
#include "interfacechooser.h"
#include "database.h"
#include "CafeInterface.h"
#include "userinterface.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    // Sample id pass
    // 992401030010 , 12qw12qw

    // Connection to Database and pre-checks
    QString appDir = QCoreApplication::applicationDirPath();

    QDir dir(appDir);
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();

    QString fullPath = dir.absoluteFilePath("data/cafe.db");
    qDebug() << "Trying to open DB at:" << fullPath;

    Database::setupDatabase(QDir::cleanPath(fullPath));


    InterfaceChooser interfaceDialog;
    int result = interfaceDialog.exec();
    if (result == 1){

        // Load the startup QDialog
        if (Database::checkLogged()){
            QVariantMap user = Database::getUserData();
            qDebug() << user["uid"] << user["name"] << user["phone"] << user["year"] << user["batch"] << user["logged_in"];

            UserInterface mainWindow;

            mainWindow.show();
            return app.exec();
        }
        else {
            Startup startupDialog;
            int startupResult = startupDialog.exec();
            if (startupResult == 1) { // User Registered or Logged In
                UserInterface mainWindow;
                mainWindow.show();
                return app.exec();
            } else {
                return 0;
            }
        }
    }
    else if (result == 2) {
        CafeInterface cafe;
        cafe.show();
        return app.exec();
    }
    return 1;
}
