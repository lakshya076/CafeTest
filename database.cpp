#include "database.h"

bool Database::setupDatabase(QString dbPath)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "Error opening database:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;

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

    if (!query.exec("CREATE TABLE IF NOT EXISTS dotd ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "name TEXT NOT NULL, "
                    "is_vegetarian BOOLEAN, "
                    "indicator1 TEXT, "
                    "indicator2 TEXT, "
                    "indicator3 TEXT, "
                    "price REAL NOT NULL, "
                    "available_qty INTEGER NOT NULL)")) {
        qDebug() << "Error creating dotd table:" << query.lastError().text();
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

void Database::readData()
{
    QSqlQuery query("SELECT * FROM users");

    while (query.next()) {
        std::string uid = query.value("uid").toString().toStdString();
        std::string name = query.value("name").toString().toStdString();
        std::string phone = query.value("phone").toString().toStdString();
        std::string year = query.value("year").toString().toStdString();
        std::string batch = query.value("batch").toString().toStdString();

        qDebug() << "uid:" << uid << "| Name:" << name << "| Phone:" << phone << "| Year:" << year
                 << "| Batch:" << batch;
    }
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

void Database::checkDrivers()
{
    qDebug() << "Available drivers:" << QSqlDatabase::drivers();
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

void Database::updateItem(int id, double price, int qty, QString table)
{
    QSqlQuery query;

    if (table.toStdString() == "items") {
        query.prepare("UPDATE items SET price = :price, available_qty = :qty WHERE id = :id");
    } else if (table.toStdString() == "dotd") {
        query.prepare("UPDATE dotd SET price = :price, available_qty = :qty WHERE id = :id");
    }
    query.bindValue(":price", price);
    query.bindValue(":qty", qty);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error Updating Price and Quantity:" << query.lastError().text();
        return;
    }
    qDebug() << "Price and Quantity updated successfully.";
}

void Database::updateItem(int id, double price, QString table)
{
    QSqlQuery query;

    if (table.toStdString() == "items") {
        query.prepare("UPDATE items SET price = :price WHERE id = :id");
    } else if (table.toStdString() == "dotd") {
        query.prepare("UPDATE dotd SET price = :price WHERE id = :id");
    }
    query.bindValue(":price", price);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error Updating Price:" << query.lastError().text();
        return;
    }
    qDebug() << "Price updated successfully.";
}

void Database::updateItem(int id, int qty, QString table)
{
    QSqlQuery query;

    if (table.toStdString() == "items") {
        query.prepare("UPDATE items SET available_qty = :qty WHERE id = :id");
    } else if (table.toStdString() == "dotd") {
        query.prepare("UPDATE dotd SET available_qty = :qty WHERE id = :id");
    }
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
    query.prepare("SELECT name, is_vegetarian, indicator1, indicator2, indicator3, price, "
                  "available_qty FROM dotd WHERE :id = id");
    query.bindValue(":id", day);

    QVariantMap dotdData;

    if (query.exec()) {
        if (query.next()) {
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
    query.prepare("SELECT name, is_vegetarian, indicator1, indicator2, indicator3, price, "
                  "available_qty FROM dotd WHERE :id = id");
    query.bindValue(":id", id);

    QVariantMap dotdData;

    if (query.exec()) {
        if (query.next()) {
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
