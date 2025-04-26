#include "database.h"

#include <QJsonDocument>
#include <QJsonObject>

bool Database::setupDatabase(QString dbPath)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "Error opening database:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;

    if (!query.exec("PRAGMA foreign_keys = ON")) {
        qDebug() << "Failed to enable foreign keys:" << query.lastError().text();
        return false;
    }

    // Create tables if it doesn't exist
    if (!query.exec("CREATE TABLE IF NOT EXISTS users ("
                    "uid TEXT PRIMARY KEY NOT NULL UNIQUE,"
                    "name TEXT NOT NULL,"
                    "phone TEXT NOT NULL UNIQUE,"
                    "year TEXT,"
                    "batch TEXT,"
                    "password TEXT NOT NULL,"
                    "logged_in INTEGER DEFAULT 0 CHECK (logged_in IN (0, 1)));")) {
        qDebug() << "Error creating users table:" << query.lastError().text();
        return false;
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS items ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "name TEXT NOT NULL, "
                    "is_vegetarian BOOLEAN, "
                    "indicator1 TEXT, "
                    "indicator2 TEXT, "
                    "indicator3 TEXT, "
                    "price REAL NOT NULL, "
                    "available_qty INTEGER NOT NULL)")) {
        qDebug() << "Error creating items table:" << query.lastError().text();
        return false;
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS order_history ("
                    "order_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "uid TEXT NOT NULL, "
                    "order_date TEXT DEFAULT (datetime('now','localtime')), "
                    "total_amount REAL NOT NULL, "
                    "orderDetails TEXT NOT NULL, "
                    "FOREIGN KEY (uid) REFERENCES users(uid) ON DELETE CASCADE)")) {
        qDebug() << "Error creating order history table:" << query.lastError().text();
        return false;
    }

    qDebug() << "Database setup completed successfully.";
    return true;
}

bool Database::insertUser(const QString &uid,
                          const QString &name,
                          const QString &phone,
                          const QString &year,
                          const QString &batch,
                          const QString &password)
{
    // Only Calls when user registers
    QSqlQuery query;

    query.prepare("INSERT INTO users (uid, name, phone, year, batch, password, logged_in) "
                  "VALUES (:uid, :name, :phone, :year, :batch, :password, 1)");
    query.bindValue(":uid", uid);
    query.bindValue(":name", name);
    query.bindValue(":phone", phone);
    query.bindValue(":year", year);
    query.bindValue(":batch", batch);
    query.bindValue(":password", password);

    if (!query.exec()) {
        qDebug() << "Insert error:" << query.lastError().text();
        return false;
    }
    return true;
}

QVariantMap Database::getUserData(const QString &uid, const QString &password)
{
    QSqlQuery query;
    query.prepare("SELECT uid, password, email, logged_in FROM users WHERE uid = :uid AND password "
                  "= :password");
    query.bindValue(":uid", uid);
    query.bindValue(":password", password);

    QVariantMap userData;

    if (query.exec()) {
        if (query.next()) {
            userData["uid"] = query.value("uid").toString();
            userData["password"] = query.value("password").toString();
            userData["email"] = query.value("email").toString();
            userData["logged_in"] = query.value("logged_in").toInt();
        } else {
            qDebug() << "No user found with uid: " << uid;
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    return userData;
}

QVariantMap Database::getUserData()
{
    QSqlQuery query;
    query.prepare("SELECT uid, name, phone, year, batch, logged_in FROM users WHERE logged_in=1");

    QVariantMap userData;

    if (query.exec()) {
        if (query.next()) {
            userData["uid"] = query.value("uid").toString();
            userData["name"] = query.value("name").toString();
            userData["phone"] = query.value("phone").toString();
            userData["year"] = query.value("year").toString();
            userData["batch"] = query.value("batch").toString();
            userData["logged_in"] = query.value("logged_in").toInt();
        } else {
            qDebug() << "No user logged in";
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    return userData;
}

QVariantMap Database::getUserData(const QString &uid)
{
    QSqlQuery query;
    query.prepare("SELECT uid, name, phone, year, batch, logged_in FROM users WHERE uid = :uid");
    query.bindValue(":uid", uid);

    QVariantMap userData;

    if (query.exec()) {
        if (query.next()) {
            userData["uid"] = query.value("uid").toString();
            userData["name"] = query.value("name").toString();
            userData["phone"] = query.value("phone").toString();
            userData["year"] = query.value("year").toString();
            userData["batch"] = query.value("batch").toString();
            userData["logged_in"] = query.value("logged_in").toInt();
        } else {
            qDebug() << "No user found with uid: " << uid;
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    return userData;
}

bool Database::uidValid(const QString &uid)
{
    QSqlQuery query;
    query.prepare("SELECT 1 FROM users WHERE uid = :uid LIMIT 1");
    query.bindValue(":uid", uid);

    if (!query.exec() || !query.next()) {
        return false; // User does not exist
    }
    return true;
}

bool Database::uidValid(const QString &uid, const QString &password)
{
    QSqlQuery query;

    query.prepare("SELECT 1 FROM users WHERE uid = :uid AND password = :password LIMIT 1");
    query.bindValue(":uid", uid);
    query.bindValue(":password", password);

    return query.exec() && query.next();
}

bool Database::changePassword(const QString &uid, const QString &password)
{
    QSqlQuery query;

    // Check if the user exists before updating the password
    query.prepare("SELECT 1 FROM users WHERE uid = :uid LIMIT 1");
    query.bindValue(":uid", uid);

    if (!query.exec() || !query.next()) {
        qDebug() << "User does not exist or query failed:" << query.lastError().text();
        return false;
    }

    // Update password for the user
    query.prepare("UPDATE users SET password = :newPassword WHERE uid = :uid");
    query.bindValue(":newPassword", password); // Consider hashing before storing
    query.bindValue(":uid", uid);

    if (!query.exec()) {
        qDebug() << "Failed to update password:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0; // True if the password was updated
}

bool Database::logout()
{
    QSqlQuery query;

    // Update all users to logged_out (logged_in = 0)
    if (!query.exec("UPDATE users SET logged_in = 0")) {
        qDebug() << "Failed to log out users:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

void Database::setUserLoggedIn(const QString &uid)
{
    QSqlQuery query;

    if (!query.exec("UPDATE users SET logged_in = 0")) { // Safeguard condition
        qDebug() << "Failed to reset logged_in status for all users:" << query.lastError().text();
        return;
    }

    // Step 2: Set logged_in = 1 for the specific user
    query.prepare("UPDATE users SET logged_in = 1 WHERE uid = :uid");
    query.bindValue(":uid", uid);

    if (query.exec()) {
        qDebug() << "Set the current user to logged_in=1";
        return;
    } else {
        qDebug() << "Failed to update logged_in status for user:" << uid
                 << "Error:" << query.lastError().text();
        return;
    }
}

bool Database::checkLogged()
{
    QSqlQuery query("SELECT * FROM users WHERE logged_in = 1");

    if (query.exec()) {
        if (query.next()) {
            return true;
        }
    } else {
        qDebug() << "Query execution failed:" << query.lastError().text();
    }

    return false;
}

bool Database::deleteUser(const QString& uid) {
    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE uid = :uid");
    query.bindValue(":uid", uid);

    if (!query.exec()) {
        qDebug() << "Error deleting user:" << query.lastError().text();
        return false;
    }
    return true;
}

QVariantMap Database::getItem(const int &id)
{
    QSqlQuery query;
    query.prepare("SELECT name, available_qty FROM items where id = :id");
    query.bindValue(":id", id);

    QVariantMap itemData;

    if (query.exec()) {
        if (query.next()) {
            itemData["name"] = query.value("name").toString();
            itemData["available_qty"] = query.value("available_qty").toInt();
        } else {
            qDebug() << "No item found with uid: " << id;
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    return itemData;
}

bool Database::insertOrder(const QString& uid, const QMap<int, int>& orderDetails, double totalAmount) {
    QSqlQuery query;
    query.prepare("INSERT INTO order_history (uid, total_amount, orderDetails) "
                  "VALUES (:uid, :total_amount, :orderDetails)");

    // Convert orderDetails map to JSON string
    QJsonObject orderDetailsJson;
    for (auto it = orderDetails.constBegin(); it != orderDetails.constEnd(); ++it) {
        orderDetailsJson.insert(QString::number(it.key()), it.value());
    }
    QJsonDocument doc(orderDetailsJson);
    QString orderDetailsStr = doc.toJson(QJsonDocument::Compact);

    query.bindValue(":uid", uid);
    query.bindValue(":total_amount", totalAmount);
    query.bindValue(":orderDetails", orderDetailsStr);

    return query.exec();
}

QList<Database::OrderInfo> Database::getOrderHistory(const QString& uid) {

    QList<OrderInfo> orderHistory;
    QSqlQuery query;
    query.prepare("SELECT order_id, order_date, total_amount, orderDetails "
                  "FROM order_history WHERE uid = :uid ORDER BY order_date DESC");
    query.bindValue(":uid", uid);

    if (query.exec()) {
        while (query.next()) {
            OrderInfo order;
            order.orderId = query.value("order_id").toInt();
            order.orderDate = QDateTime::fromString(query.value("order_date").toString(),
                                                    "yyyy-MM-dd hh:mm:ss");
            order.totalAmount = query.value("total_amount").toDouble();

            // Parse orderDetails JSON back to QMap<int, int>
            QString orderDetailsStr = query.value("orderDetails").toString();
            QJsonDocument doc = QJsonDocument::fromJson(orderDetailsStr.toUtf8());
            QJsonObject orderDetailsJson = doc.object();

            for (auto it = orderDetailsJson.constBegin(); it != orderDetailsJson.constEnd(); ++it) {
                // Convert string key back to int
                order.orderDetails.insert(it.key().toInt(), it.value().toInt());
            }

            orderHistory.append(order);
        }
    }

    return orderHistory;
}

// Cafe Interface Functions
void Database::addItem(const QString &name,
                       const bool &is_vegetarian,
                       const QString &indicator1,
                       const QString &indicator2,
                       const QString &indicator3,
                       const double &price,
                       const int &available_qty)
{
    QSqlQuery query;
    query.prepare("INSERT INTO items (name, is_vegetarian, indicator1, indicator2, indicator3, "
                  "price, available_qty) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?)");

    query.bindValue(0, name);
    query.bindValue(1, is_vegetarian);
    query.bindValue(2, indicator1);
    query.bindValue(3, indicator2);
    query.bindValue(4, indicator3);
    query.bindValue(5, price);
    query.bindValue(6, available_qty);

    if (!query.exec()) {
        qDebug() << "Error inserting data:" << query.lastError().text();
        return;
    }
    qDebug() << "Item inserted successfully";
}

void Database::populateDD(QComboBox *DD)
{
    QSqlQuery query;

    if (query.exec("SELECT id, name FROM items")) {
        while (query.next()) {
            QString name = query.value(1).toString();
            int id = query.value(0).toInt();
            DD->addItem(name, id);
        }
    } else {
        qDebug() << "Database query failed:" << query.lastError().text();
    }
}

void Database::deleteItem(int id, QComboBox *deleteDD)
{
    QSqlQuery query;

    query.prepare("DELETE FROM items WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error Deleting Item:" << query.lastError().text();
        return;
    }
    qDebug() << "Item deleted successfully.";
    deleteDD->removeItem(deleteDD->currentIndex());
    deleteDD->setCurrentIndex(-1);
}

void Database::updateItem(int id, double price, int qty)
{
    QSqlQuery query;
    query.prepare("UPDATE items SET price = :price, available_qty = :qty WHERE id = :id");
    query.bindValue(":price", price);
    query.bindValue(":qty", qty);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error Updating Price and Quantity:" << query.lastError().text();
        return;
    }
    qDebug() << "Price and Quantity updated successfully.";
}

void Database::updateItem(int id, double price)
{
    QSqlQuery query;
    query.prepare("UPDATE items SET price = :price WHERE id = :id");
    query.bindValue(":price", price);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error Updating Price:" << query.lastError().text();
        return;
    }
    qDebug() << "Price updated successfully.";
}

void Database::updateItem(int id, int qty)
{
    QSqlQuery query;
    query.prepare("UPDATE items SET available_qty = :qty WHERE id = :id");
    query.bindValue(":qty", qty);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error Updating Quantity:" << query.lastError().text();
        return;
    }
    qDebug() << "Quantity updated successfully.";
}

QVariantMap Database::getDOTD()
{
    QDate today = QDate::currentDate();
    int day = today.dayOfWeek();

    QSqlQuery query;
    query.prepare("SELECT id, name, is_vegetarian, indicator1, indicator2, indicator3, price, "
                  "available_qty FROM items WHERE id = :id");
    query.bindValue(":id", day+10000);

    QVariantMap dotdData;

    if (query.exec()) {
        if (query.next()) {
            dotdData["id"] = query.value("id").toInt();
            dotdData["name"] = query.value("name").toString();
            dotdData["is_vegetarian"] = query.value("is_vegetarian").toBool();
            dotdData["indicator1"] = query.value("indicator1").toString();
            dotdData["indicator2"] = query.value("indicator2").toString();
            dotdData["indicator3"] = query.value("indicator3").toString();
            dotdData["price"] = query.value("price").toDouble();
            dotdData["available_qty"] = query.value("available_qty").toInt();
        } else {
            qDebug() << "No data found for day";
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    return dotdData;
}

QVariantMap Database::getDOTD(int id)
{
    QSqlQuery query;
    query.prepare("SELECT id, name, is_vegetarian, indicator1, indicator2, indicator3, price, "
                  "available_qty FROM items WHERE id = :id");
    query.bindValue(":id", id);

    QVariantMap dotdData;

    if (query.exec()) {
        if (query.next()) {
            dotdData["id"] = query.value("id").toInt();
            dotdData["name"] = query.value("name").toString();
            dotdData["is_vegetarian"] = query.value("is_vegetarian").toBool();
            dotdData["indicator1"] = query.value("indicator1").toString();
            dotdData["indicator2"] = query.value("indicator2").toString();
            dotdData["indicator3"] = query.value("indicator3").toString();
            dotdData["price"] = query.value("price").toDouble();
            dotdData["available_qty"] = query.value("available_qty").toInt();
        } else {
            qDebug() << "No data found for day";
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    return dotdData;
}

bool Database::insertDotdItems()
{
    QSqlQuery query;
    query.prepare("INSERT INTO items (id, name, is_vegetarian, indicator1, indicator2, indicator3, price, available_qty) "
                  "VALUES (:id, :name, :is_vegetarian, :indicator1, :indicator2, :indicator3, :price, :available_qty)");

    struct DotdItem {
        int id;
        QString name;
        bool isVegetarian;
        QString indicator1;
        QString indicator2;
        QString indicator3;
        int price;
        int availableQty;
    };

    QList<DotdItem> items = {
        {10001, "Dosa Idli Sambhar Chutney", true,  "400kcal", "South Indian", "20-25Min", 150, 20},
        {10002, "Chhole Bhature",            true,  "450kcal", "Fried",        "15-20Min", 80,  20},
        {10003, "Dal Makhani & Naan",        true,  "350kcal", "Creamy",       "10-15Min", 80,  20},
        {10004, "Pav Bhaji",                 true,  "390kcal", "Oily",         "10-15Min", 85,  20},
        {10005, "Matar Kulcha",              true,  "598kcal", "Baked Kulcha", "10-15Min", 70,  20},
        {10006, "Paneer Tikka Masala",       true,  "390kcal", "Tandoor",      "15-20min", 150, 20},
        {10007, "Chicken Malai Tikka",       false, "497kcal", "Tandoor",      "20-25Min", 220, 20}
    };

    for (const auto& item : items) {
        query.bindValue(":id", item.id);
        query.bindValue(":name", item.name);
        query.bindValue(":is_vegetarian", item.isVegetarian);
        query.bindValue(":indicator1", item.indicator1);
        query.bindValue(":indicator2", item.indicator2);
        query.bindValue(":indicator3", item.indicator3);
        query.bindValue(":price", item.price);
        query.bindValue(":available_qty", item.availableQty);

        if (!query.exec()) {
            qDebug() << "Insert failed for ID" << item.id << ":" << query.lastError().text();
            return false;
        }
    }

    return true;
}
