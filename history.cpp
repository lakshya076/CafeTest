#include "history.h"
#include "ui_history.h"

History::History(const Database::OrderInfo& order, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::History)
    , m_orderId(order.orderId)
{
    ui->setupUi(this);

    // Set order details
    ui->historyDate->setText(order.orderDate.toString("yyyy-MM-dd hh:mm:ss"));
    ui->historyAmount->setText("Rs " + QString::number(order.totalAmount, 'f', 2));

    QString details;
    QMapIterator<int, int> it(order.orderDetails);
    while (it.hasNext()) {
        it.next();

        int dishID = it.key();
        dishDetails = Database::getItem(dishID);

        details += QString("%1 : Quantity %2\n").arg(dishDetails["name"].toString()).arg(it.value());
    }

    ui->historyDetails->setText(details);
    m_orderDetails = details;

    // Storing this info for opening in notepad
    m_orderDetails = QString("Order #%1\n").arg(m_orderId) +
                     QString("Date: %1\n").arg(order.orderDate.toString("yyyy-MM-dd hh:mm:ss")) +
                     QString("Total Amount: Rs %1\n\n").arg(order.totalAmount, 0, 'f', 2) +
                     "Order Details:\n" + details;
}

History::~History() {
    delete ui;
}

void History::onClicked()
{
    // Create a temporary file
    QString tempFileName = QString("order_%1.txt").arg(m_orderId);
    QFile file(tempFileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << m_orderDetails;
        file.close();

    // Open with notepad
    #ifdef Q_OS_WIN
            QProcess::startDetached("notepad.exe", QStringList() << tempFileName);
    #else
            QProcess::startDetached("xdg-open", QStringList() << tempFileName);
    #endif
    }
}

void History::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        onClicked();
    }
    QWidget::mousePressEvent(event);
}
