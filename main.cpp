#include "UI/cars/catalog/carcatalogpage.h"
#include <QApplication>
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    CarCatalogPage home;
    home.show();
    return QApplication::exec();
}
