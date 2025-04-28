#include "userinterface.h"
#include "ripplebutton.h"
#include "ui_userinterface.h"
#include "history.h"

#include <regex>

QString main_css = R"(
        /*CentralWidget Stylesheet*/
        #centralwidget {
            background-color: #24292E;
            color: #FFFAF0;
        }

        /*Sidebar css*/
        #collapse {
            background-color: #313A46;
        }
        #collapse QPushButton {
            border:none;
        }
        #expand {
            background-color: #313A46;
        }
        #expand QPushButton {
            border:none;
            text-align: left;
            padding: 8px 0px 8px 15px;
            color: #BCD2F5;
            font:12pt \"Segoe UI\";
        }
        #expand QPushButton:checked {
            color: #fffaf0;;
        }

        /* Taste Profile CSS */
        #widget {
            background-color:#313a46;
            color:#fffaf0;
        }
        #taste {
            background-color:#313a46;
        }

        /* QLineEdit css */
        QLineEdit {
            padding-left: 10px;
            padding-right: 10px;
            border-radius: 20px;
            border-width: 1px;
            background-color: #fffaf0;
            color: #000;
        }

        /* QComboBox css */
        QComboBox {
            padding: 5px 10px 5px 10px;
            background-color: #1e1e1e;
        }
        QComboBox QAbstractItemView {
            padding: 0px;
            margin: 0px;
        })";

QString rippleButtonCSS = R"(
    RippleButton {
        background-color: #3FA7D6;
        color: #FFFAF0;
        font-weight: bold;
        border-radius: 6px;
        padding: 8px 20px;
        border: none;
    }

    RippleButton:pressed {
        background-color: #3498db;
    }

    RippleButton:checked {
        background-color: #3FA7D6;
    }

    RippleButton:focus {
        background-color: #3FA7D6;
        outline: none;
    }
)";

QString feedbackStyleSheet = R"(
    QLineEdit {
        background-color: #2F353C;
        selection-color: #2F353C;
        color: #E1E4E8;  /* Text Color */
        border-width: 1px;
        border-style: solid;
        border-color: #3D434A #3D434A #3D434A #3D434A;
        border-radius: 10px;
        padding: 2 10px;
        font: 12pt \"Segoe UI\";
    }
    QLineEdit:focus {
        border: 1px solid #3FA7D6;  /* Highlight border on focus */
    }

    QComboBox {
        padding: 5px 10px 5px 10px;
        background-color: #2F353C;
        color: #E1E4E8;
    }
    QComboBox QAbstractItemView {
        padding: 0px;
        margin: 0px;
    }
)";

QString uiButtonCSS = R"(
    QPushButton {
        background-color: #3FA7D6;
        color: #FFFAF0;
        border-radius: 6px;
        padding: 8px 20px;
        border: none;
    }

    QPushButton:pressed {
        background-color: #3498db;
    }

    QPushButton:checked {
        background-color: #3FA7D6;
    }

    QPushButton:focus {
        background-color: #3FA7D6;
        outline: none;
    }
)";

UserInterface::UserInterface(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UserInterface)
{
    ui->setupUi(this);

    setStyleSheet(main_css);

    // Get Current User Data
    user = Database::getUserData();
    dotd = Database::getDOTD();

    // Create a QVBoxLayout for cardsScrollArea
    cardsLayout = new QVBoxLayout(ui->cardsScrollAreaWidgetContents);
    cardsLayout->setSpacing(0);
    cardsLayout->setContentsMargins(10, 10, 10, 10);

    // Create a QVBoxLayout for historyScrollArea
    historyLayout = new QVBoxLayout(ui->historyScrollAreaWidgetContents);
    historyLayout->setSpacing(0);
    historyLayout->setContentsMargins(10, 10, 10, 10);

    // Initialize the total cost label
    updateTotalCostLabel();

    ui->cardsScrollArea->setWidgetResizable(true);
    ui->cardsScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->cardsScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Loading all the cards from the database
    loadCardsFromDatabase();
    loadOrderHistory();

    // Setting up Drop Down
    ui->optionDD->addItem("Dining");
    ui->optionDD->addItem("Takeaway");

    // Initial Checks
    ui->expand->hide();
    ui->home_collapse->setChecked(true);
    ui->stack->setCurrentIndex(0);
    ui->checkout->setStyleSheet(rippleButtonCSS);
    ui->submitFeedbackButton->setStyleSheet(rippleButtonCSS);
    ui->logout_settings->setStyleSheet(uiButtonCSS);
    ui->license_button->setStyleSheet(uiButtonCSS);
    ui->dotdAddToCart->setStyleSheet(uiButtonCSS);
    ui->feedback_page->setStyleSheet(feedbackStyleSheet);

    // Connecting buttons with function calls
    connect(ui->home_collapse, &QPushButton::clicked, this, &UserInterface::homeFunction);
    connect(ui->home_expand, &QPushButton::clicked, this, &UserInterface::homeFunction);

    connect(ui->feedback_collapse, &QPushButton::clicked, this, &UserInterface::feedbackFunction);
    connect(ui->feedback_expand, &QPushButton::clicked, this, &UserInterface::feedbackFunction);

    connect(ui->settings_collapse, &QPushButton::clicked, this, &UserInterface::settingsFunction);
    connect(ui->settings_expand, &QPushButton::clicked, this, &UserInterface::settingsFunction);

    connect(ui->license_button, &QPushButton::clicked, this, &UserInterface::openLicenseFunction);

    connect(ui->logout_collapse, &QPushButton::clicked, this, &UserInterface::logoutFunction);
    connect(ui->logout_expand, &QPushButton::clicked, this, &UserInterface::logoutFunction);
    connect(ui->logout_settings, &QPushButton::clicked, this, &UserInterface::logoutFunction);

    connect(ui->exit_collapse, &QPushButton::clicked, this, &UserInterface::exitFunction);
    connect(ui->exit_expand, &QPushButton::clicked, this, &UserInterface::exitFunction);

    connect(ui->license_cred_collapse, &QPushButton::clicked, this, &UserInterface::creditLicenseFunction);
    connect(ui->license_cred_expand, &QPushButton::clicked, this, &UserInterface::creditLicenseFunction);

    connect(ui->history_collapse, &QPushButton::clicked, this, &UserInterface::historyFunction);
    connect(ui->history_expand, &QPushButton::clicked, this, &UserInterface::historyFunction);

    connect(ui->checkout, &RippleButton::clicked, this, &UserInterface::checkoutFunction);

    connect(ui->submitFeedbackButton, &QPushButton::clicked, this, &UserInterface::submitFeedbackFunction);

    connect(ui->delete_acc, &QPushButton::clicked, this, &UserInterface::deleteAccountFunction);

    // Dish of The Day section
    qDebug();
    qDebug() << "Dish Of the Day";
    qDebug() << dotd["name"] << dotd["is_vegetarian"] << dotd["indicator1"] << dotd["indicator2"]
             << dotd["indicator3"] << dotd["price"] << dotd["available_qty"];
    ui->dotdName->setText(dotd["name"].toString());

    // Setting the veg/nonveg icon
    if (dotd["is_vegetarian"].toBool()) {
        ui->dotdIsVeg->setPixmap(QIcon(":/icons/Icons/veg.png").pixmap(20, 20));
    } else {
        ui->dotdIsVeg->setPixmap(QIcon(":/icons/Icons/nonveg.png").pixmap(20, 20));
    }

    ui->dotdIndi1->setText(dotd["indicator1"].toString());
    ui->dotdIndi2->setText(dotd["indicator2"].toString());
    ui->dotdIndi3->setText(dotd["indicator3"].toString());

    QString price = "Rs " + dotd["price"].toString();
    ui->dotdPrice->setText(price);

    connect(ui->dotdAddToCart, &QPushButton::clicked, this, &UserInterface::dotdAddToCartFunction);

    // Feedback Page
    ui->feedbackDD1->addItem("Very Poor", 1);
    ui->feedbackDD1->addItem("Poor", 2);
    ui->feedbackDD1->addItem("Good", 3);
    ui->feedbackDD1->addItem("Very Good", 4);
    ui->feedbackDD1->addItem("Excellent", 5);

    ui->feedbackDD2->addItem("1", 1);
    ui->feedbackDD2->addItem("2", 2);
    ui->feedbackDD2->addItem("3", 3);
    ui->feedbackDD2->addItem("4", 4);
    ui->feedbackDD2->addItem("5", 5);
    ui->feedbackDD2->addItem("6", 6);
    ui->feedbackDD2->addItem("7", 7);
    ui->feedbackDD2->addItem("8", 8);
    ui->feedbackDD2->addItem("9", 9);
    ui->feedbackDD2->addItem("10", 10);

    // Settings Page
    ui->uidSettings->setText(user["uid"].toString());
    ui->nameSettings->setText(user["name"].toString());
    ui->pnSettings->setText(user["phone"].toString());
    ui->yearSettings->setText(user["year"].toString());
    ui->batchSettings->setText(user["batch"].toString());
}

UserInterface::~UserInterface()
{
    delete ui;
}

void UserInterface::homeFunction()
{
    ui->stack->setCurrentIndex(0);
}

void UserInterface::feedbackFunction()
{
    ui->stack->setCurrentIndex(1);
}

void UserInterface::settingsFunction()
{
    ui->stack->setCurrentIndex(2);
}

void UserInterface::creditLicenseFunction()
{
    ui->stack->setCurrentIndex(3);
}

void UserInterface::historyFunction() {
    ui->stack->setCurrentIndex(4);
}

void UserInterface::logoutFunction()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  "Logout",
                                  "Are you sure you want to logout?",
                                  QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (Database::logout()) {
            qDebug() << "Logout Done";
        } else {
            qDebug() << "No users were logged out (maybe none were logged in).";
        }
        close();
    } else {
        qDebug() << "Logout Aborted";
    }
}

void UserInterface::exitFunction()
{
    qDebug() << "Exiting";
    close();
}

void UserInterface::checkoutFunction()
{
    qDebug();
    qDebug() << "Starting Checkout";

    std::string priceStr = ui->totalCostLabel->text().toStdString();
    std::regex numberRegex(R"([\d\.]+)");
    std::smatch match;
    double price;

    // Checking if there is a number in totalCostLabel
    if (std::regex_search(priceStr, match, numberRegex)) {
        price = std::stod(match.str());
    } else {
        qDebug() << "No number found in totalCostLabel";
    }

    if (ui->optionDD->currentIndex() != -1) {
        // Check if there are any items in the cart
        bool hasItems = false;
        bool isDotd = false;

        for (auto &card : cardWidgets) {
            if (card->getQuantity() > 0) {
                hasItems = true;
                break;
            }
        }

        if (!ui->dotdAddToCart->isChecked()) {
            hasItems = true;
            isDotd = true;
            ui->dotdAddToCart->setChecked(true);
            ui->dotdAddToCart->setText("Add To Cart");
        }

        if (!hasItems) {
            qDebug() << "No items in the cart";
            QMessageBox::warning(this, "Error", "No items in the cart");
            return;
        } else {
            // Clearing orderDetails map with values 0
            QMutableMapIterator<int, int> it(orderDetails);
            while (it.hasNext()) {
                it.next();
                if (it.value() == 0) {
                    it.remove();
                }
            }

            qDebug() << "Option:" << ui->optionDD->currentText()
                     << "Price:" << price;
            qDebug() << "Order Details:";

            // Write to File
            QFile file("order.txt");

            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);

                out << "User Details: " << user["uid"].toString() << ", " << user["name"].toString()
                    << "\n\n";
                out << "Option: " << ui->optionDD->currentText() << "\n";
                out << "Price: Rs " << price << "\n\n";
                out << "Order Details: " << "\n";

                for (auto it = orderDetails.constBegin(); it != orderDetails.constEnd(); ++it) {
                    QVariantMap tempData = Database::getItem(it.key());
                    qDebug() << tempData["name"].toString() << ": Quantity" << it.value();

                    out << tempData["name"].toString() << " : Quantity " << it.value()
                        << "\n"; // Writing to File
                }

                // DOTD Section
                if (isDotd) {
                    out << dotd["name"].toString() << " : Quantity 1" << "\n";
                    orderDetails[dotd["id"].toInt()] += 1;
                }

                file.close();
                qDebug() << "Order written to file successfully.";

                Database::insertOrder(user["uid"].toString(), orderDetails, totalCost);

                QMessageBox::information(this, "Info", "Order placed succesfully.");

                // Order History Page
                clearCards(historyLayout);
                loadOrderHistory();

                // Home Page
                updateDatabaseQuantities(); // Updating the available quantites for items in the cart
                clearCards(cardsLayout); // Clearing cards to reload the cards with new updated quantities
                loadCardsFromDatabase(); // Loading cards from the database (again)

            } else {
                qDebug() << "Error opening order.txt for writing!";
            }
        }

        // Add Animation
        QPropertyAnimation *animation = new QPropertyAnimation(ui->checkout, "geometry");
        animation->setDuration(200);
        animation->setStartValue(ui->checkout->geometry());
        animation->setEndValue(ui->checkout->geometry()); // slight shrink
        animation->setEasingCurve(QEasingCurve::OutBounce);
        animation->start();

    } else {
        qDebug() << "No Option selected.";
        QMessageBox::critical(this, "Error", "Select an option (Dining/Takeaway)");
    }

    orderDetails.clear(); // Clearing the order for the next one
    qDebug();
}

void UserInterface::openLicenseFunction()
{
    QString resourcePath = ":/license.txt";

    QString targetDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QDir().mkpath(targetDir);

    QString targetPath = QDir(targetDir).filePath("license.txt");
    QFile::remove(targetPath);

    // Copy from resources to disk
    if (!QFile::copy(resourcePath, targetPath)) {
        QMessageBox::warning(this, "Error", "Failed to copy license.txt to disk.");
        return;
    }

    QFile::setPermissions(targetPath, QFile::ReadOwner | QFile::WriteOwner); // Allow overwriting

    // Open with notepad
    #ifdef Q_OS_WIN
        QProcess::startDetached("notepad.exe", QStringList() << targetPath);
    #else
        QProcess::startDetached("xdg-open", QStringList() << targetPath);
    #endif
}

void UserInterface::deleteAccountFunction() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  "Delete Account",
                                  "Are you sure you want to delete your account?",
                                  QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (!Database::deleteUser(user["uid"].toString())) {
            QMessageBox::critical(this, "Error", "Your account could not be deleted. Try Again later");
        } else {
            QMessageBox::information(this, "Info", "Your account has been deleted successfully.\nCafe++ will now close");
            close();
        }
    } else {
        qDebug() << "Account will not be deleted";
    }
}

// Card Events (Order History Cards and Dish Cards)
void UserInterface::addHorizontalDivider(QLayout* layout)
{
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setLineWidth(1);
    line->setStyleSheet("background-color: #555555;"); // Dark gray line for dark mode

    // Add vertical spacers before and after the line for padding
    addVerticalSpacer(5);
    layout->addWidget(line);
    addVerticalSpacer(5);
}

void UserInterface::loadCardsFromDatabase()
{
    QSqlQuery query;
    query.prepare("SELECT id, name, is_vegetarian, indicator1, indicator2, indicator3, price, "
                  "available_qty FROM items");

    if (!query.exec()) {
        qDebug() << "Item Query failed:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        bool isVeg = query.value(2).toBool();
        QString indicator1 = query.value(3).toString();
        QString indicator2 = query.value(4).toString();
        QString indicator3 = query.value(5).toString();
        double price = query.value(6).toDouble();
        int availableQty = query.value(7).toInt();

        // Cards with availableQty=0 dont load in home screen
        if (availableQty == 0) {
            continue;
        }

        // Create a new card widget
        CardWidget *card = new CardWidget(id,
                                          name,
                                          isVeg,
                                          indicator1,
                                          indicator2,
                                          indicator3,
                                          price,
                                          availableQty,
                                          this);

        connect(card, &CardWidget::addToCart, this, &UserInterface::onAddToCart);
        connect(card, &CardWidget::quantityChanged, this, &UserInterface::onCardQuantityChanged);

        cardsLayout->addWidget(card);
        addHorizontalDivider(cardsLayout); // Add spacing between cards
        cardWidgets[id] = card; // Store the card in the map
    }

    cardsLayout->addStretch();
}

void UserInterface::onCardQuantityChanged(int id, int delta, double price)
{
    // Update the total cost
    totalCost += delta * price;
    updateTotalCostLabel();
    orderDetails[id] += delta;
    qDebug() << "Quantity for" << id << ":" << orderDetails[id];
}

void UserInterface::onAddToCart(int id)
{
    if (cardWidgets.contains(id)) {
        CardWidget *card = cardWidgets[id];
        if (card->getQuantity() > 0) {
            qDebug() << "Added" << id << "to order";
        }
    }
}

void UserInterface::updateTotalCostLabel()
{
    ui->totalCostLabel->setText(QString("Total Cost: Rs %1").arg(totalCost, 0, 'f', 2));
    ui->totalCostLabel->setStyleSheet(
        "QLabel { color: #50E3C2; font-weight: bold; padding: 5px; }");

    // Return to normal style after a short delay
    QTimer::singleShot(500, this, [this]() {
        ui->totalCostLabel->setStyleSheet(
            "QLabel { color: #FFFFFF; font-weight: bold; padding: 5px; }");
    });
}

void UserInterface::clearCards(QLayout* layout)
{
    // Function to clear cards in user interface from cardsScrollArea
    for (auto &card : cardWidgets) {
        layout->removeWidget(card);
        delete card;
    }

    cardWidgets.clear();

    // Function to remove children in cardsLayout
    remove(layout);

    totalCost = 0.0;
    updateTotalCostLabel();
}

void UserInterface::updateDatabaseQuantities()
{
    // Function to update available quantities of the items in the order in the database
    QSqlDatabase::database().transaction();

    for (auto &card : cardWidgets) {
        int id = card->getId();
        int quantity = card->getQuantity();

        if (quantity > 0) {
            // Get current available quantity from database
            QSqlQuery selectQuery;
            selectQuery.prepare("SELECT available_qty FROM items WHERE id = ?");
            selectQuery.bindValue(0, id);

            if (selectQuery.exec() && selectQuery.next()) {
                int currentAvailableQty = selectQuery.value(0).toInt();
                int newAvailableQty = currentAvailableQty - quantity;

                // Make sure we don't go below zero
                newAvailableQty = qMax(0, newAvailableQty);

                // Update the database
                QSqlQuery updateQuery;
                updateQuery.prepare("UPDATE items SET available_qty = ? WHERE id = ?");
                updateQuery.bindValue(0, newAvailableQty);
                updateQuery.bindValue(1, id);

                if (!updateQuery.exec()) {
                    qDebug() << "Error updating quantity for item" << id << ":"
                             << updateQuery.lastError().text();
                    QSqlDatabase::database().rollback();
                    return;
                }
            } else {
                qDebug() << "Error fetching current quantity for item" << id << ":"
                         << selectQuery.lastError().text();
                QSqlDatabase::database().rollback();
                return;
            }
        }
    }

    QSqlDatabase::database().commit();
    qDebug() << "Available quantities updated for items";
    qDebug();
}

void UserInterface::loadOrderHistory()
{
    orderHistory.clear(); // If Order history consists something beforehand
    orderHistory = Database::getOrderHistory(user["uid"].toString());

    if (orderHistory.isEmpty()) {
        QLabel* noHistoryLabel = new QLabel("No order history found", this);
        noHistoryLabel->setFont(QFont("Segoe UI", 16));
        noHistoryLabel->setAlignment(Qt::AlignCenter);
        historyLayout->addWidget(noHistoryLabel);
    } else {
        // Sort by most recent first (if not already sorted)
        std::sort(orderHistory.begin(), orderHistory.end(),
                  [](const Database::OrderInfo& a, const Database::OrderInfo& b) {
                      return a.orderDate > b.orderDate;
                  });

        // Create a card for each order
        for (int i = 0; i < orderHistory.size(); ++i) {
            const Database::OrderInfo& order = orderHistory.at(i);
            History* historyCard = new History(order, this);

            historyLayout->addWidget(historyCard);
            addHorizontalDivider(historyLayout);
        }
    }

    historyLayout->addStretch();
}


// Dish Of The Day
void UserInterface::dotdAddToCartFunction()
{
    if (!ui->dotdAddToCart->isChecked()) {
        qDebug() << "Added Dish of The Day to Cart";
        ui->dotdAddToCart->setChecked(false);
        ui->dotdAddToCart->setText("Remove From Cart");

        totalCost += dotd["price"].toDouble();
        updateTotalCostLabel();
    } else {
        qDebug() << "Removed Dish of The Day to Cart";
        ui->dotdAddToCart->setChecked(true);
        ui->dotdAddToCart->setText("Add To Cart");

        totalCost -= dotd["price"].toDouble();
        updateTotalCostLabel();
    }
}

// Feedback
void UserInterface::submitFeedbackFunction()
{
    int feed1 = ui->feedbackDD1->currentData().toInt();
    int feed2 = ui->feedbackDD2->currentData().toInt();

    QString text1 = ui->feedbackLE1->text();
    QString text2 = ui->feedbackLE2->text();
    QString text3 = ui->feedbackLE2->text();

    bool opt1 = false, opt2 = false;

    if (ui->feedbackRadioYes1->isChecked()) {
        opt1 = true;
    } else if (ui->feedbackRadioNo1->isChecked()) {
        opt1 = false;
    }

    if (ui->feedbackRadioYes2->isChecked()) {
        opt2 = true;
    } else if (ui->feedbackRadioNo2->isChecked()) {
        opt2 = false;
    }

    qDebug() << "Feedback is:" << feed1 << opt1 << text1 << feed2 << opt2 << text2 << text3;

    // Write to File
    QFile file("feedback.txt");

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);

        QVariantMap user = Database::getUserData();

        out << "Feedback for user: " << user["uid"].toString() << "\n\n";
        out << "How easy is it to navigate the app: " << feed1 << "\n";
        out << "Did you encounter any difficulties finding what you needed: " << opt1 << "\n";
        out << "Are there features you wish the app had: " << text1 << "\n";
        out << "On a scale of 1–10, how satisfied are you with the app overall: " << feed2 << "\n";
        out << "Would you recommend this app to others: " << opt2 << "\n";
        out << "What would you change or improve in the app: " << text2 << "\n";
        out << "Any additional feedback you'd like to share: " << text3 << "\n";

        file.close();
        qDebug() << "Feedback written to file successfully.";

        QMessageBox::information(this, "Info", "Feedback has been submitted successfully");

        ui->feedbackDD1->clear();
        ui->feedbackDD2->clear();
        ui->feedbackLE1->clear();
        ui->feedbackLE2->clear();
        ui->feedbackLE3->clear();

    } else {
        qDebug() << "Error opening feedback.txt for writing!";
    }
}


// Helper function to remove children from a qlayout
void UserInterface::remove(QLayout* layout)
{
    QLayoutItem* child;
    while(layout->count()!=0) {
        child = layout->takeAt(0);

        if(child->layout() != 0) {
            remove(child->layout());
        }
        else if(child->widget() != 0) {
            delete child->widget();
        }

        delete child;
    }
}
