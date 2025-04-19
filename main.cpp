#include <QApplication>
#include <QCoreApplication>
#include <QMainWindow>
#include <QStyle>

#include "CafeInterface.h"
#include "database.h"
#include "interfacechooser.h"
#include "startup.h"
#include "userinterface.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // Sample id pass
    // 992401030010 , 12qw12qw

    QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();

    // Set size relative to the screen
    int width = screenGeometry.width() * 0.9;
    int height = screenGeometry.height() * 0.9;

    // Center the window
    int x = screenGeometry.x() + (screenGeometry.width() - width) / 2;
    int y = screenGeometry.y() + (screenGeometry.height() - height) / 2;

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
    if (result == 1) {
        // Load the startup QDialog
        if (Database::checkLogged()) {
            QVariantMap user = Database::getUserData();
            qDebug();
            qDebug() << user["uid"] << user["name"] << user["phone"] << user["year"]
                     << user["batch"] << user["logged_in"];

            UserInterface mainWindow;
            mainWindow.setGeometry(x, y, width, height);
            mainWindow.show();

            return app.exec();
        } else {
            Startup startupDialog;
            int startupResult = startupDialog.exec();
            if (startupResult == 1) { // User Registered or Logged In
                UserInterface mainWindow;
                mainWindow.setGeometry(x, y, width, height);
                mainWindow.show();

                return app.exec();
            } else {
                return 0;
            }
        }
    } else if (result == 2) {
        CafeInterface cafe;
        cafe.setGeometry(x, y, width, height);
        cafe.show();

        return app.exec();
    }
    return 1;
}
