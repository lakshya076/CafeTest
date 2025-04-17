#include "cardwidget.h"

CardWidget::CardWidget(int id,
                       const QString &name,
                       bool isVeg,
                       const QString &indicator1,
                       const QString &indicator2,
                       const QString &indicator3,
                       double price,
                       int availableQty,
                       QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CardWidget)
    , m_id(id)
    , m_price(price)
    , m_quantity(0)
    , m_availableQty(availableQty)
{
    ui->setupUi(this);

    // Set the icon based on whether the dish is vegetarian or not
    if (isVeg) {
        ui->vnCard->setPixmap(QIcon(":/icons/Icons/veg.png").pixmap(20, 20));
    } else {
        ui->vnCard->setPixmap(QIcon(":/icons/Icons/nonveg.png").pixmap(20, 20));
    }

    // Set other data
    ui->cartCard->setIcon(QIcon(":/icons/Icons/addcart.ico"));
    ui->nameCard->setText(name);
    ui->indicator1Card->setText(indicator1);
    ui->indicator2Card->setText(indicator2);
    ui->indicator3Card->setText(indicator3);
    ui->priceCard->setText(QString("Rs  %1").arg(price, 0, 'f', 2));
    ui->qtyCard->setText("0");

    // Connect signals and slots
    connect(ui->incrementCard, &QPushButton::clicked, this, &CardWidget::onIncrement);
    connect(ui->decrementCard, &QPushButton::clicked, this, &CardWidget::onDecrement);
    connect(ui->cartCard, &QPushButton::clicked, this, &CardWidget::onAddToCart);

    // Initial button states and visibility
    updateButtonStates();
    updateVisibility();
}

CardWidget::~CardWidget() {}

void CardWidget::onIncrement()
{
    if (m_quantity < m_availableQty) {
        m_quantity++;
        ui->qtyCard->setText(QString::number(m_quantity));

        updateButtonStates();
        emit quantityChanged(m_id, 1, m_price);
    } else {
        qDebug() << "Only" << m_availableQty << "items available";
    }
}

void CardWidget::onDecrement()
{
    if (m_quantity > 0) {
        m_quantity--;
        ui->qtyCard->setText(QString::number(m_quantity));

        updateButtonStates();
        updateVisibility(); // Check if we need to hide increment/decrement
        emit quantityChanged(m_id, -1, m_price);
    }
}

void CardWidget::onAddToCart()
{
    // When cart button is clicked, show quantity controls and add 1 to quantity
    m_quantity = 1;
    ui->qtyCard->setText(QString::number(m_quantity));
    updateButtonStates();
    updateVisibility();
    emit quantityChanged(m_id, 1, m_price);
    emit addToCart(m_id);
}

void CardWidget::setQuantity(int qty)
{
    if (qty >= 0 && qty <= m_availableQty) {
        int delta = qty - m_quantity;
        m_quantity = qty;
        ui->qtyCard->setText(QString::number(m_quantity));
        updateButtonStates();
        updateVisibility();
        if (delta != 0) {
            emit quantityChanged(m_id, delta, m_price);
        }
    }
}

void CardWidget::updateButtonStates()
{
    // Disable increment if at max available quantity
    ui->incrementCard->setEnabled(m_quantity < m_availableQty);

    // Disable decrement if quantity is zero
    ui->decrementCard->setEnabled(m_quantity > 0);

    // Show remaining quantity as tooltip
    int remaining = m_availableQty - m_quantity;
    ui->incrementCard->setToolTip(QString("%1 remaining").arg(remaining));
}

void CardWidget::updateVisibility()
{
    if (m_quantity > 0) {
        // Show quantity controls, hide cart button
        ui->cartCard->hide();
        ui->decrementCard->show();
        ui->qtyCard->show();
        ui->incrementCard->show();
    } else {
        // Hide quantity controls, show cart button
        ui->cartCard->show();
        ui->decrementCard->hide();
        ui->qtyCard->hide();
        ui->incrementCard->hide();
    }
}
