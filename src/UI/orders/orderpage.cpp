#include "orderpage.h"
#include <QScrollBar>
#include <QApplication>
#include <QHeaderView>
#include "../cars/catalog/carcatalogpage.h"

OrderPage::OrderPage(QWidget *parent) : QWidget(parent) {
    setupUI();
    populateOrders();
}

void OrderPage::setupUI() {
    headerWidget = new HeaderWidget(this);
    ordersTable = new QTableWidget(this);
    mainLayout = new QVBoxLayout(this);

    ordersTable->setStyleSheet(
        "QTableWidget { border: 1px solid #000; background-color: #fff; }");
    ordersTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ordersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ordersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ordersTable->setAlternatingRowColors(true);
    ordersTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ordersTable->setColumnCount(6);

    QStringList headers = {
        "Идентификатор заказа", "Начало", "Конец", "Идентификатор машины", "Идентификатор клиента", "Цена"
    };
    ordersTable->setHorizontalHeaderLabels(headers);
    ordersTable->horizontalHeader()->setStyleSheet("background-color: #ccc;");

    QScreen *screen = QGuiApplication::primaryScreen();
    int tableWidth = screen->geometry().width();
    for (int i = 0; i < 6; ++i) {
        ordersTable->setColumnWidth(i, tableWidth / 6);
    }

    mainLayout->addWidget(headerWidget);
    mainLayout->addWidget(ordersTable);

    setLayout(mainLayout);

    connect(headerWidget, &HeaderWidget::catalogClicked, this, &OrderPage::showCarCatalogPage);
    connect(headerWidget, &HeaderWidget::chartClicked, this, &OrderPage::showTransmissionChartPage);


    priceRangeCheckbox = new QCheckBox("Диапазон цен", this);
    lowestPriceLabel = new QLabel("Самая низкая цена:", this);
    lowestPriceEdit = new QLineEdit(this);
    highestPriceLabel = new QLabel("Самая высокая цена:", this);
    highestPriceEdit = new QLineEdit(this);
    QHBoxLayout *priceRangeLayout = new QHBoxLayout();
    priceRangeLayout->addWidget(priceRangeCheckbox);
    priceRangeLayout->addWidget(lowestPriceLabel);
    priceRangeLayout->addWidget(lowestPriceEdit);
    priceRangeLayout->addWidget(highestPriceLabel);
    priceRangeLayout->addWidget(highestPriceEdit);
    priceRangeLayout->addStretch();

    dateRangeCheckbox = new QCheckBox("Диапазон дат", this);
    startDateLabel = new QLabel("Дата начала:", this);
    startDateCalendar = new QCalendarWidget(this);
    endDateLabel = new QLabel("Дата окончания:", this);
    endDateCalendar = new QCalendarWidget(this);
    QHBoxLayout *dateRangeLayout = new QHBoxLayout();
    dateRangeLayout->addWidget(dateRangeCheckbox);
    dateRangeLayout->addWidget(startDateLabel);
    dateRangeLayout->addWidget(startDateCalendar);
    dateRangeLayout->addWidget(endDateLabel);
    dateRangeLayout->addWidget(endDateCalendar);
    dateRangeLayout->addStretch();

    QVBoxLayout *bottomLayout = new QVBoxLayout();
    bottomLayout->addLayout(priceRangeLayout);
    bottomLayout->addLayout(dateRangeLayout);
    bottomLayout->addStretch();

    mainLayout->addLayout(bottomLayout);

    connect(priceRangeCheckbox, &QCheckBox::stateChanged, this, &OrderPage::onPriceRangeCheckboxStateChanged);
    connect(dateRangeCheckbox, &QCheckBox::stateChanged, this, &OrderPage::onDateRangeCheckboxStateChanged);
    connect(startDateCalendar, &QCalendarWidget::selectionChanged, this, &OrderPage::onStartDateSelectionChanged);
    connect(endDateCalendar, &QCalendarWidget::selectionChanged, this, &OrderPage::onEndDateSelectionChanged);
    connect(headerWidget, &HeaderWidget::addCarClicked, this, &OrderPage::showAddCarDialog);

    deleteButton = new QPushButton("Удалить заказ", this);
    deleteButton->setStyleSheet(
        "QPushButton { background-color: #e74c3c; color: white; border: 2px solid #c0392b; padding: 8px 16px; border-radius: 5px; }"
        "QPushButton:hover { background-color: #c0392b; }"
        "QPushButton:pressed { background-color: #922b21; }");
    mainLayout->addWidget(deleteButton);

    connect(deleteButton, &QPushButton::clicked, this, &OrderPage::onDeleteButtonClicked);
}

void OrderPage::populateTableWithOrders(const std::vector<Order> &orders) {
    ordersTable->clearContents();
    ordersTable->setRowCount(0);

    for (const auto &order: orders) {
        int row = ordersTable->rowCount();
        ordersTable->insertRow(row);

        QTableWidgetItem *itemId = new QTableWidgetItem(order.getId());
        QTableWidgetItem *itemStart = new QTableWidgetItem(TimeHelper::tmToQString(order.getStartDate()));
        QTableWidgetItem *itemEnd = new QTableWidgetItem(TimeHelper::tmToQString(order.getEndDate()));
        QTableWidgetItem *itemCarId = new QTableWidgetItem(order.getCarId());
        QTableWidgetItem *itemClientId = new QTableWidgetItem(order.getClientId());
        QTableWidgetItem *itemPrice = new QTableWidgetItem(QString::number(order.getPrice()));

        ordersTable->setItem(row, 0, itemId);
        ordersTable->setItem(row, 1, itemStart);
        ordersTable->setItem(row, 2, itemEnd);
        ordersTable->setItem(row, 3, itemCarId);
        ordersTable->setItem(row, 4, itemClientId);
        ordersTable->setItem(row, 5, itemPrice);
    }
}

void OrderPage::onDeleteButtonClicked() {
    if (ordersTable->selectedItems().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select an order to delete.");
        return;
    }

    int selectedRow = ordersTable->selectedItems().first()->row();

    QString orderId = ordersTable->item(selectedRow, 0)->text();

    QMessageBox::StandardButton confirmation;
    confirmation = QMessageBox::question(this, "Confirmation", "Are you sure you want to delete this order?",
                                         QMessageBox::Yes | QMessageBox::No);
    if (confirmation == QMessageBox::Yes) {
        orderService.deleteOrder(orderId.toStdString());
        populateOrders();
    }
}

void OrderPage::populateOrders() {
    ordersTable->clearContents();
    auto orders = orderService.getAllOrders();
    ordersTable->setRowCount(0);

    for (const auto &order: orders) {
        int row = ordersTable->rowCount();
        ordersTable->insertRow(row);

        QTableWidgetItem *itemId = new QTableWidgetItem(order.getId());
        QTableWidgetItem *itemStart = new QTableWidgetItem(TimeHelper::tmToQString(order.getStartDate()));
        QTableWidgetItem *itemEnd = new QTableWidgetItem(TimeHelper::tmToQString(order.getEndDate()));
        QTableWidgetItem *itemCarId = new QTableWidgetItem(order.getCarId());
        QTableWidgetItem *itemClientId = new QTableWidgetItem(order.getClientId());
        QTableWidgetItem *itemPrice = new QTableWidgetItem(QString::number(order.getPrice()));

        ordersTable->setItem(row, 0, itemId);
        ordersTable->setItem(row, 1, itemStart);
        ordersTable->setItem(row, 2, itemEnd);
        ordersTable->setItem(row, 3, itemCarId);
        ordersTable->setItem(row, 4, itemClientId);
        ordersTable->setItem(row, 5, itemPrice);
    }
}

void OrderPage::onPriceRangeCheckboxStateChanged(int state) {
    lowestPriceLabel->setEnabled(state == Qt::Checked);
    lowestPriceEdit->setEnabled(state == Qt::Checked);
    highestPriceLabel->setEnabled(state == Qt::Checked);
    highestPriceEdit->setEnabled(state == Qt::Checked);

    if (state == Qt::Checked) {
        double lowestPrice = lowestPriceEdit->text().toDouble();
        double highestPrice = highestPriceEdit->text().toDouble();
        auto filteredOrders = orderService.findOrdersByPriceRange(lowestPrice, highestPrice);

        populateTableWithOrders(filteredOrders);
    } else {
        populateOrders();
    }
}

void OrderPage::onDateRangeCheckboxStateChanged(int state) {
    startDateLabel->setEnabled(state == Qt::Checked);
    startDateCalendar->setEnabled(state == Qt::Checked);
    endDateLabel->setEnabled(state == Qt::Checked);
    endDateCalendar->setEnabled(state == Qt::Checked);

    if (state == Qt::Unchecked) {
        startDateCalendar->setSelectedDate(QDate::currentDate());
        endDateCalendar->setSelectedDate(QDate::currentDate());
    }

    if (state == Qt::Checked) {
        startDateCalendar->setHidden(false);
        endDateCalendar->setHidden(false);
    } else {
        startDateCalendar->setHidden(true);
        endDateCalendar->setHidden(true);
        populateOrders();
    }
}

void OrderPage::onStartDateSelectionChanged() {
    if (dateRangeCheckbox->isChecked()) {
        QDate startDate = startDateCalendar->selectedDate();

        if (endDateCalendar->selectedDate().isValid()) {
            QDate endDate = endDateCalendar->selectedDate();
            filterOrdersByDateRange(startDate, endDate);
        }
    }
}

void OrderPage::onEndDateSelectionChanged() {
    if (dateRangeCheckbox->isChecked()) {
        QDate endDate = endDateCalendar->selectedDate();

        if (startDateCalendar->selectedDate().isValid()) {
            QDate startDate = startDateCalendar->selectedDate();
            filterOrdersByDateRange(startDate, endDate);
        }
    }
}

void OrderPage::filterOrdersByDateRange(const QDate &startDate, const QDate &endDate) {
    QDateTime startDateTime = QDateTime(startDate, QTime(0, 0, 0));
    QDateTime endDateTime = QDateTime(endDate, QTime(23, 59, 59));

    std::vector<Order> filteredOrders;
    for (const auto &order: orderService.getAllOrders()) {
        QDateTime orderStartDateTime = TimeHelper::tmToQDateTime(order.getStartDate());
        QDateTime orderEndDateTime = TimeHelper::tmToQDateTime(order.getEndDate());

        if (orderStartDateTime >= startDateTime && orderEndDateTime <= endDateTime) {
            filteredOrders.push_back(order);
        }
    }
    populateTableWithOrders(filteredOrders);
}


void OrderPage::showCarCatalogPage() {
    auto catalogPage = new CarCatalogPage(new QMainWindow);
    QMainWindow *parent = dynamic_cast<QMainWindow *>(this->parentWidget());
    parent->setCentralWidget(catalogPage);
    this->parentWidget()->show();
}

void OrderPage::showTransmissionChartPage() {
    TransmissionChart *chartPage = new TransmissionChart(this);
    QMainWindow *parent = dynamic_cast<QMainWindow *>(this->parentWidget());
    parent->setCentralWidget(chartPage);
}

void OrderPage::showAddCarDialog() {
    QAddCarDialog *addCarDialog = new QAddCarDialog(this);
    if (addCarDialog->exec() == QDialog::Accepted) {
        CarCatalogPage *catalog_page = new CarCatalogPage;
        this->close();
        catalog_page->show();
    }
}
