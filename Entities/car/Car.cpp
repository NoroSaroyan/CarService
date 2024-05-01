#include "Car.h"
#include <utility>

Car::Car(QString id, QString brand, QString model, QString volume, BodyType bodyType, Transmission transmission,
         bool hasCarSeat) {
    this->setId(id);
    this->setBrand(brand);
    this->setVolume(volume);
    this->setBodyType(bodyType);
    this->setTransmission(transmission);
    this->setHasCarSeat(hasCarSeat);
}

const QString &Car::getBrand() const {
    return this->brand;
}


void Car::setBrand(const QString &brand) {
    this->brand = brand;
}

const QString &Car::getModel() const {
    return this->model;
}

void Car::setModel(const QString &model) {
    this->model = model;
}

const QString &Car::getVolume() const {
    return volume;
}

void Car::setVolume(const QString &volume) {
    this->volume = volume;
}

BodyType Car::getBodyType() const {
    return this->bodyType;
}

void Car::setBodyType(BodyType bodyType) {
    this->bodyType = bodyType;
}

Transmission Car::getTransmission() const {
    return this->transmission;
}

void Car::setTransmission(Transmission transmission) {
    this->transmission = transmission;
}

bool Car::isHasCarSeat() const {
    return this->hasCarSeat;
}

void Car::setHasCarSeat(bool hasCarSeat) {
    this->hasCarSeat = hasCarSeat;
}

Car::Car() {}

const QString &Car::getId() const {
    return id;
}

void Car::setId(const QString &id) {
    Car::id = id;
}

double Car::getRate() const {
    return rate;
}

void Car::setRate(double rate) {
    Car::rate = rate;
}

Car::~Car() {
}

// пока не уверен, что нужен в такой реализации, но пусть будет
//ostream &operator<<(ostream &os, const Car &car) {
//    os << "brand: " << car.brand << " model: " << car.model << " volume: " << car.volume << " bodyType: "
//       << car.bodyType << " transmission: " << car.transmission << " hasCarSeat: " << car.hasCarSeat;
//    return os;
//}
