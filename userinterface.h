#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QDir>
#include <QFile>
#include <QMainWindow>
#include <QMessageBox>
#include <QProcess>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QSpacerItem>
#include <QStandardPaths>
#include <QTimer>
#include <QUuid>
#include <QVBoxLayout>
#include <QWidget>
#include "cardwidget.h"
#include "database.h"

namespace Ui {
class UserInterface;
}

class UserInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserInterface(QWidget *parent = nullptr);
    ~UserInterface();

    void addHorizontalDivider(QLayout* layout);

private slots:
    void homeFunction();
    void feedbackFunction();
    void settingsFunction();
    void openLicenseFunction();
    void logoutFunction();
    void exitFunction();
    void creditLicenseFunction();
    void historyFunction();
    void checkoutFunction();
    void dotdAddToCartFunction();
    void submitFeedbackFunction();
    void deleteAccountFunction();

    // Card functions
    void onCardQuantityChanged(int id, int delta, double price);
    void onAddToCart(int id);

private:
    Ui::UserInterface *ui;
    QVBoxLayout *cardsLayout;
    QVBoxLayout *historyLayout;
    QMap<int, CardWidget *> cardWidgets; // Map to store cards by ID

    double totalCost;
    QMap<int, int> orderDetails;
    QList<Database::OrderInfo> orderHistory;
    QVariantMap dotd;
    QVariantMap user;

    void remove(QLayout* layout);
    void clearCards(QLayout* layout);
    void updateDatabaseQuantities();

    void loadOrderHistory();
    void loadCardsFromDatabase();
    void updateTotalCostLabel();

    void addVerticalSpacer(int height)
    {
        QSpacerItem *spacer = new QSpacerItem(20, height, QSizePolicy::Minimum, QSizePolicy::Fixed);
        cardsLayout->addItem(spacer);
    }
};

#endif // USERINTERFACE_H
