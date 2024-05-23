#include "headerwidget.h"

HeaderWidget::HeaderWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *holderWidget = new QWidget(this);
    holderWidget->setStyleSheet(
        "background-color: #fff; border-bottom: 2px solid #ccc; padding: 10px; border-radius: 10px;");
    mainLayout->addWidget(holderWidget);

    QVBoxLayout *holderLayout = new QVBoxLayout(holderWidget);
    holderLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *headerLabel = new QLabel("Сервис Аренды Машин", holderWidget);
    QFont font("Arial", 24, QFont::Bold);
    headerLabel->setFont(font);
    headerLabel->setAlignment(Qt::AlignLeft);
    headerLabel->setStyleSheet("background-color: none;");

    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->addWidget(headerLabel);

    searchLineEdit = new QLineEdit(holderWidget);
    searchLineEdit->setPlaceholderText("Поиск...");
    searchLineEdit->setStyleSheet(
        "border: 1px solid #ccc; border-radius: 5px; padding: 5px; min-width: 400px; max-width: 400px;");
    headerLayout->addWidget(searchLineEdit);

    searchButton = new QPushButton(holderWidget);
    searchButton->setIcon(
        QIcon("/Users/noriksaroyan/CLionProjects/CarService/UI/static/icons/sIcon.png").pixmap(32, 32).copy());
    searchButton->setStyleSheet("QPushButton { border: none; background-color: transparent; }");
    searchButton->setCursor(Qt::PointingHandCursor);
    searchButton->setFixedWidth(40);
    headerLayout->addWidget(searchButton);

    QLabel *catalogLabel = new QLabel("<a href=\"#\">Каталог</a>", holderWidget);
    QLabel *ordersLabel = new QLabel("<a href=\"#\">Заказы</a>", holderWidget);
    catalogLabel->setTextFormat(Qt::RichText);
    ordersLabel->setTextFormat(Qt::RichText);
    catalogLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ordersLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    catalogLabel->setOpenExternalLinks(false);
    ordersLabel->setOpenExternalLinks(false);
    catalogLabel->setStyleSheet("color: #000;");
    ordersLabel->setStyleSheet("color: #000;");

    headerLayout->addWidget(catalogLabel);
    headerLayout->addWidget(ordersLabel);

    holderLayout->addLayout(headerLayout);

    searchResultsListView = new QListView(this);
    searchResultsListView->setStyleSheet(
        "border: 1px solid #ccc; border-radius: 5px; padding: 10px; background-color: #fff;");
    searchResultsListView->setVisible(false);
    searchResultsListView->setFixedWidth(412);
    searchResultsListView->setMaximumHeight(200); // Adjust the height as needed
    searchResultsListView->setWindowFlags(Qt::Popup);

    searchResultsModel = new QStringListModel(this);
    searchResultsListView->setModel(searchResultsModel);

    connect(catalogLabel, &QLabel::linkActivated, this, &HeaderWidget::catalogClicked);
    connect(ordersLabel, &QLabel::linkActivated, this, &HeaderWidget::ordersClicked);
    connect(searchButton, &QPushButton::clicked, this, &HeaderWidget::performSearch);
    connect(searchResultsListView, &QListView::clicked, this, &HeaderWidget::handleSearchResultClick);
}

void HeaderWidget::performSearch() {
    QString keyword = searchLineEdit->text();
    if (keyword.isEmpty()) {
        clearSearchResults();
        searchResultsListView->setVisible(false);
        return;
    }

    std::string keywordStr = keyword.toStdString();
    auto results = searchService.searchByKeyword(keywordStr);
    updateSearchResults(results);
}

void HeaderWidget::updateSearchResults(const std::pair<std::vector<Order>, std::vector<Car> > &results) {
    QStringList searchResults;

    for (const auto &car: results.second) {
        QString carInfo = "Cars->" + car.getId() + ": " + car.getBrand() + " " + car.getModel()
                          + " - " + QString::number(car.getRate()) + " USD/day";
        searchResults.append(carInfo);
    }

    for (const auto &order: results.first) {
        QString orderInfo = "Orders->" + order.getId() + ": Order ID: " +
                            order.getId() + " - Car ID: " +
                            order.getCarId() + " - Price: " + QString::number(order.getPrice());
        searchResults.append(orderInfo);
    }

    searchResultsModel->setStringList(searchResults);

    searchResultsListView->setVisible(true);
    QPoint globalPos = searchLineEdit->mapToGlobal(QPoint(0, searchLineEdit->height()));
    searchResultsListView->move(globalPos.x(), globalPos.y() + 5);
}

void HeaderWidget::clearSearchResults() {
    searchResultsModel->setStringList(QStringList());
}

void HeaderWidget::handleSearchResultClick(const QModelIndex &index) {
    QString result = searchResultsModel->data(index, Qt::DisplayRole).toString();
    emit searchResultClicked(result);
}
