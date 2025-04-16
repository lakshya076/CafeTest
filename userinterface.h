#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <QWidget>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPropertyAnimation>
#include <QTimer>
#include <QFile>
#include <QMessageBox>
#include <QProcess>
#include <QStandardPaths>
#include <QDir>
#include "cardwidget.h"

namespace Ui {
class UserInterface;
}

class UserInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserInterface(QWidget *parent = nullptr);
    ~UserInterface();

    void addHorizontalDivider();

private slots:
    void homeFunction();
    void feedbackFunction();
    void settingsFunction();
    void openLicenseFunction();
    void logoutFunction();
    void exitFunction();
    void creditLicenseFunction();
    void checkoutFunction();
    void dotdAddToCartFunction();
    void submitFeedbackFunction();

    // Card functions
    void onCardQuantityChanged(int id, int delta, double price);
    void onAddToCart(int id);

private:
    Ui::UserInterface *ui;
    QVBoxLayout *cardsLayout;
    QMap<int, CardWidget*> cardWidgets; // Map to store cards by ID

    double totalCost;
    QVariantMap dotd;

    void loadCardsFromDatabase();
    void updateTotalCostLabel();

    void addVerticalSpacer(int height) {
        QSpacerItem* spacer = new QSpacerItem(20, height, QSizePolicy::Minimum, QSizePolicy::Fixed);
        cardsLayout->addItem(spacer);
    }
};

#endif // USERINTERFACE_H
