// cardwidget.h
#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>
#include <QIcon>
#include "ui_cardwidget.h"

namespace Ui {
class CardWidget;
}

class CardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CardWidget(int id,
                        const QString &name,
                        bool isVeg,
                        const QString &indicator1,
                        const QString &indicator2,
                        const QString &indicator3,
                        double price,
                        int availableQty,
                        QWidget *parent = nullptr);
    ~CardWidget();

    int getId() const { return m_id; }
    double getPrice() const { return m_price; }
    int getQuantity() const { return m_quantity; }
    void setQuantity(int qty);

signals:
    void quantityChanged(int id, int delta, double price);
    void addToCart(int id);

private slots:
    void onIncrement();
    void onDecrement();
    void onAddToCart();

private:
    Ui::CardWidget *ui;
    int m_id;
    double m_price;
    int m_quantity;
    int m_availableQty;
    void updateButtonStates();
    void updateVisibility();
};

#endif // CARDWIDGET_H
