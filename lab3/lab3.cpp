#include <iostream>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
struct LogEntry {
    long timestamp;
    std::string level;
    std::string message;

    // Перевантаження оператора < для сортування за часом
    bool operator<(const LogEntry& other) const {
        return timestamp < other.timestamp;
    }

    // Перевантаження оператора << для виводу
    friend std::ostream& operator<<(std::ostream& os, const LogEntry& log) {
        os << "[" << log.timestamp << "] " << log.level << " - " << log.message;
        return os;
    }
};
template <class T>
class DataBuffer {
private:
    T* data;
    size_t capacity;
    size_t current_size;

    // Метод для розширення динамічного масиву
    void resize() {
        size_t new_capacity = (capacity == 0) ? 2 : capacity * 2;
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < current_size; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }

public:
    DataBuffer() : data(nullptr), capacity(0), current_size(0) {}
    ~DataBuffer() {
        delete[] data;
    }

    // Конструктор копіювання (необхідний для повернення DataBuffer з методу filter)
    DataBuffer(const DataBuffer& other) : capacity(other.capacity), current_size(other.current_size) {
        if (capacity > 0) {
            data = new T[capacity];
            for (size_t i = 0; i < current_size; ++i) {
                data[i] = other.data[i];
            }
        }
        else {
            data = nullptr;
        }
    }

    // Оператор присвоювання
    DataBuffer& operator=(const DataBuffer& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            current_size = other.current_size;
            if (capacity > 0) {
                data = new T[capacity];
                for (size_t i = 0; i < current_size; ++i) {
                    data[i] = other.data[i];
                }
            }
            else {
                data = nullptr;
            }
        }
        return *this;
    }
    void add(const T& item) {
        if (current_size >= capacity) {
            resize();
        }
        data[current_size++] = item;
    }

    // Шаблонний метод фільтрації
    template <typename Predicate>
    DataBuffer<T> filter(Predicate p) const {
        DataBuffer<T> result;
        for (size_t i = 0; i < current_size; ++i) {
            if (p(data[i])) {
                result.add(data[i]);
            }
        }
        return result;
    }

    // Вивід усіх елементів
    void printAll() const {
        for (size_t i = 0; i < current_size; ++i) {
            std::cout << data[i] << "\n";
        }
    }

    // Ітератори для підтримки STL алгоритмів (std::sort, std::for_each тощо)
    T* begin() { return data; }
    T* end() { return data + current_size; }
    const T* begin() const { return data; }
    const T* end() const { return data + current_size; }
};
int main() {
    DataBuffer<LogEntry> logs;
    logs.add({ 1690000050, "INFO", "System startup initiated." });
    logs.add({ 1690000062, "WARNING", "High memory usage detected." });
    logs.add({ 1690000010, "INFO", "Configuration loaded." });
    logs.add({ 1690000095, "ERROR", "Failed to connect to database." });
    logs.add({ 1690000102, "INFO", "User 'admin' logged in." });
    logs.add({ 1690000115, "ERROR", "Null pointer exception in module X." });
    logs.add({ 1690000030, "INFO", "Service status: OK." });
    logs.add({ 1690000150, "WARNING", "Disk space below 15%." });
    logs.add({ 1690000180, "ERROR", "Failed to connect to database." }); // Повторюване повідомлення
    logs.add({ 1690000200, "INFO", "Nightly backup completed." });
    std::cout << "--- Оригінальні логи ---\n";
    logs.printAll();
    //Сортування за часом
    std::sort(logs.begin(), logs.end());
    std::cout << "\n--- Відсортовані логи (за часом) ---\n";
    logs.printAll();

    //Підрахунок статистики
    std::map<std::string, int> stats;
    for (const auto& log : logs) {
        stats[log.level]++;
    }
    std::cout << "\n--- Статистика логів ---\n";
    for (const auto& pair : stats) {
        std::cout << pair.first << ": " << pair.second << "\n";
    }

    //Фільтрація логів з рівнем "ERROR"
    auto errorLogs = logs.filter([](const LogEntry& log) {
        return log.level == "ERROR";
        });

    std::cout << "\n--- Відфільтровані логи (ERROR) за допомогою std::for_each ---\n";
    std::for_each(errorLogs.begin(), errorLogs.end(), [](const LogEntry& log) {
        std::cout << ">> " << log << "\n";
        });

    //Збір унікальних повідомлень
    std::set<std::string> uniqueMessages;
    for (const auto& log : logs) {
        uniqueMessages.insert(log.message);
    }
    std::cout << "\n--- Унікальні повідомлення ---\n";
    for (const auto& msg : uniqueMessages) {
        std::cout << "- " << msg << "\n";
    }
    //Зклейка повідомлень ERROR за допомогою
    std::string errorReport = std::accumulate(
        errorLogs.begin(),
        errorLogs.end(),
        std::string("Звіт про критичні помилки:"),
        [](std::string acc, const LogEntry& log) {
            return acc + "\n* [" + std::to_string(log.timestamp) + "] " + log.message;
        }
    );
    std::cout << "\n--- Згенерований звіт (std::accumulate) ---\n";
    std::cout << errorReport << "\n";

    return 0;
}