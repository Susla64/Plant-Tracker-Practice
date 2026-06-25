#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

// Структура для зберігання даних про рослину
struct Plant {
    string name;
    int day;
    int month;
    int year;
    int waterFreq; // частота поливу в днях
};

// Функція для перетворення дати у дні (для зручного підрахунку)
int daysFromDate(int d, int m, int y) {
    tm time_in = { 0, 0, 0, d, m - 1, y - 1900 };
    time_t time_temp = mktime(&time_in);
    return time_temp / (60 * 60 * 24);
}

// Функція отримання поточної дати в днях
int getCurrentDays() {
    time_t now = time(0);
    return now / (60 * 60 * 24);
}

// Збереження у файл
void saveData(const vector<Plant>& plants) {
    ofstream file("plants_data.txt");
    for (size_t i = 0; i < plants.size(); i++) {
        file << plants[i].name << ";" 
             << plants[i].day << ";" 
             << plants[i].month << ";" 
             << plants[i].year << ";" 
             << plants[i].waterFreq << "\n";
    }
    file.close();
}

// Завантаження з файлу
void loadData(vector<Plant>& plants) {
    plants.clear();
    ifstream file("plants_data.txt");
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string item;
        Plant p;
        
        getline(ss, p.name, ';');
        
        getline(ss, item, ';'); p.day = stoi(item);
        getline(ss, item, ';'); p.month = stoi(item);
        getline(ss, item, ';'); p.year = stoi(item);
        getline(ss, item, ';'); p.waterFreq = stoi(item);
        
        plants.push_back(p);
    }
    file.close();
}

int main() {
    // Налаштування для коректного відображення тексту
    setlocale(LC_ALL, ""); 
    
    vector<Plant> myPlants;
    loadData(myPlants);

    int choice = -1;
    while (choice != 0) {
        cout << "\n=== ОБЛІК КІМНАТНИХ РОСЛИН ===\n";
        cout << "1. Додати нову рослину\n";
        cout << "2. Показати всі рослини\n";
        cout << "3. Показати рослини, які треба полити СЬОГОДНІ\n";
        cout << "4. Видалити рослину\n";
        cout << "0. Вийти\n";
        cout << "Ваш вибiр: ";
        
        if (!(cin >> choice)) {
            cout << "Помилка вводу! Введiть число.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 1) {
            Plant p;
            cout << "Введiть назву рослини: ";
            cin.ignore();
            getline(cin, p.name);
            cout << "Коли останнiй раз поливали?\n";
            cout << "День (наприклад, 25): "; cin >> p.day;
            cout << "Мiсяць (наприклад, 6): "; cin >> p.month;
            cout << "Рiк (наприклад, 2026): "; cin >> p.year;
            cout << "Частота поливу (у днях): "; cin >> p.waterFreq;
            
            myPlants.push_back(p);
            saveData(myPlants);
            cout << "[Успiшно] Рослину додано та збережено!\n";
        }
        else if (choice == 2) {
            cout << "\n--- Список усiх рослин ---\n";
            if (myPlants.empty()) cout << "Список порожнiй.\n";
            for (size_t i = 0; i < myPlants.size(); i++) {
                cout << i + 1 << ". " << myPlants[i].name 
                     << " | Останнiй полив: " << myPlants[i].day << "." 
                     << myPlants[i].month << "." << myPlants[i].year 
                     << " | Частота: кожнi " << myPlants[i].waterFreq << " днiв.\n";
            }
        }
        else if (choice == 3) {
            cout << "\n--- РОСЛИНИ, ЯКI ТРЕБА ПОЛИТИ СЬОГОДНІ ---\n";
            int currentDays = getCurrentDays();
            bool found = false;
            
            for (size_t i = 0; i < myPlants.size(); i++) {
                int lastWateredDays = daysFromDate(myPlants[i].day, myPlants[i].month, myPlants[i].year);
                int daysPassed = currentDays - lastWateredDays;
                
                if (daysPassed >= myPlants[i].waterFreq) {
                    cout << "-> " << myPlants[i].name << " (минуло " << daysPassed << " днiв з останнього поливу)\n";
                    found = true;
                }
            }
            if (!found) cout << "Сьогоднi поливати нiчого не треба!\n";
        }
        else if (choice == 4) {
            cout << "\n--- ВИДАЛЕННЯ РОСЛИНИ ---\n";
            if (myPlants.empty()) {
                cout << "Список порожнiй. Нiчого видаляти.\n";
            } else {
                // Спочатку показуємо список, щоб користувач бачив номери
                for (size_t i = 0; i < myPlants.size(); i++) {
                    cout << i + 1 << ". " << myPlants[i].name << "\n";
                }
                cout << "Введiть номер рослини для видалення: ";
                int deleteIdx;
                if (!(cin >> deleteIdx) || deleteIdx < 1 || deleteIdx > (int)myPlants.size()) {
                    cout << "Некоректний номер або помилка вводу!\n";
                    cin.clear();
                    cin.ignore(10000, '\n');
                } else {
                    string nameToRemove = myPlants[deleteIdx - 1].name;
                    // Видаляємо з вектора
                    myPlants.erase(myPlants.begin() + (deleteIdx - 1));
                    // Оновлюємо файл даних
                    saveData(myPlants);
                    cout << "[Успiшно] Рослину \"" << nameToRemove << "\" видалено!\n";
                }
            }
        }
    }

    return 0;
}
