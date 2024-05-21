// orderspage.h

#ifndef ORDERSPAGE_H
#define ORDERSPAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include "../../../Entities/order/order.h"
#include "../static/header/headerwidget.h"
#include <QScrollBar>

class OrderPage : public QWidget {
Q_OBJECT
public:
    explicit OrderPage(QWidget *parent = nullptr);
signals:
    void navigateToCatalog();

private:
    HeaderWidget *headerWidget;
    QTableWidget *ordersTable;
    QVBoxLayout *mainLayout;

    void setupUI();
    void populateOrders();
};

#endif // ORDERSPAGE_H