#ifndef HISTORY_H
#define HISTORY_H

#include <QWidget>
#include <QMouseEvent>
#include <QFile>
#include <QTextStream>
#include <QDesktopServices>
#include <QUrl>
#include <QMouseEvent>
#include <QProcess>
#include "database.h"

namespace Ui {
class History;
}

class History : public QWidget
{
    Q_OBJECT

public:
    explicit History(const Database::OrderInfo& order, QWidget *parent = nullptr);
    ~History();

protected:
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void onClicked();

private:
    Ui::History *ui;
    int m_orderId;
    QString m_orderDetails;

    QVariantMap dishDetails;
};

#endif // HISTORY_H
