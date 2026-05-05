#include <iostream>
#include <string>
#include <random>
#include <cstring>
using namespace std;
class ISensor {
public:
    virtual double getMeasurement() = 0;
    virtual ~ISensor() = default;
};
class IControl {
public:
    virtual void execute(int value) = 0;
    virtual ~IControl() = default;
};
class Device {
protected:
    char* modelName;
    string id;

public:
    Device(const char* name, string id) : id(id) {
        modelName = new char[strlen(name) + 1];
        strcpy(modelName, name);
    };
     // Конструктор копіювання
    Device(const Device& other) : id(other.id) {
        modelName = new char[strlen(other.modelName) + 1];
        strcpy(modelName, other.modelName);
    }
    // Оператор присвоєння (Алгоритм із завдання 3)
    Device& operator=(const Device& other) {
        if (this == &other) { 
            return *this;
        }
        delete[] modelName; 
        id = other.id; 
        modelName = new char[strlen(other.modelName) + 1];
        strcpy(modelName, other.modelName);
        return *this;
    }
    virtual ~Device() {
        delete[] modelName;
    }
    virtual void print() const = 0;
    // Метод для зміни імені (щоб довести глибоке копіювання)
    void setModelName(const char* newName) {
        delete[] modelName;
        modelName = new char[strlen(newName) + 1];
        strcpy(modelName, newName);
    }
    const char* getModelName() const { return modelName; }
};

//Множинне наслідування
class SmartStation : public Device, public ISensor, public IControl {
public:
    SmartStation(const char* name, string id) : Device(name, id) {}
    void print() const override {
        cout << "[SmartStation] Model: " << modelName << " | ID: " << id << endl;
    }
    double getMeasurement() override {
        static random_device rd;
        static mt19937 gen(rd());
        uniform_real_distribution<> dis(15.0, 30.0);
        return dis(gen);
    }
    void execute(int value) override {
        cout << "Executing control command with value: " << value << " on " << modelName << endl;
    }
};

int main() {
    cout << "--- Демонстрація глибокого копіювання ---" << endl;
    SmartStation original("Meteo-100", "ID-001");
    SmartStation copy = original; // Виклик конструктора копіювання
    // Змінюємо ім'я у копії
    copy.setModelName("Meteo-200_COPY");
    cout << "Original name: ";
    original.print();
    cout << "Copy name: ";
    copy.print();
    cout << "(Як бачимо, оригінал не змінився, глибоке копіювання працює успішно)\n\n";
    cout << "--- Робота з масивом вказівників (Поліморфізм) ---" << endl;
    // Створення динамічного масиву вказівників
    Device** inventory = new Device * [5];
    // Заповнення масиву
    inventory[0] = new SmartStation("Sensor-Alpha", "A1");
    inventory[1] = new SmartStation("Sensor-Beta", "B2");
    inventory[2] = new SmartStation("Sensor-Gamma", "G3");
    inventory[3] = new SmartStation("Sensor-Delta", "D4");
    inventory[4] = new SmartStation("Sensor-Epsilon", "E5");
    // Проходження циклом
    for (int i = 0; i < 5; ++i) {
        inventory[i]->print();
        ISensor* sensor = dynamic_cast<ISensor*>(inventory[i]);
        if (sensor != nullptr) {
            cout << "  -> Measurement: " << sensor->getMeasurement() << " C" << endl;
        }
        else {
            cout << "  -> This device is not a sensor." << endl;
        }
    }
    for (int i = 0; i < 5; ++i) {
        delete inventory[i]; 
    }
    delete[] inventory;

    cout << "\nПам'ять успішно очищена." << endl;

    return 0;
}