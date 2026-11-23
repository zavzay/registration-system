#include <windows.h> // <--- ОБЯЗАТЕЛЬНО ПЕРВЫМ, до using namespace std;
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstdlib>
#include <conio.h>

#include "system.hpp"
#include "list.hpp"

using namespace std;

// ============================================================
//  Клавиши и экран
// ============================================================
enum Key { KEY_UP, KEY_DOWN, KEY_ENTER, KEY_ESC, KEY_OTHER };

Key readKey() {
    int c = _getch();
    if (c == 0 || c == 224) {
        int c2 = _getch();
        if (c2 == 72) return KEY_UP;
        if (c2 == 80) return KEY_DOWN;
        return KEY_OTHER;
    }
    if (c == 13) return KEY_ENTER;
    if (c == 27) return KEY_ESC;
    return KEY_OTHER;
}

void clearScreen() { system("cls"); }

// ============================================================
//  Меню (все пункты сразу, без скроллинга)
// ============================================================
void drawMenu(const char** items, int count, int selected) {
    clearScreen();
    cout << "════════════ МЕНЮ ════════════\n\n";

    for (int i = 0; i < count; ++i) {
        cout << (i == selected ? " ► " : "   ") << items[i] << "\n";
    }

    cout << "\n ↑/↓ — навигация   Enter — выбор   Esc — выход\n";
}

int runMenu(const char** items, int count, int& selected) {
    while (true) {
        drawMenu(items, count, selected);
        Key k = readKey();
        
        if (k == KEY_UP && selected > 0) {
            --selected;
        } else if (k == KEY_DOWN && selected < count - 1) {
            ++selected;
        } else if (k == KEY_ENTER) {
            return selected;
        } else if (k == KEY_ESC) {
            return -1;
        }
    }
}

void waitForKey() {
    cout << "\nНажмите любую клавишу...";
    readKey();
}

// ============================================================
//  Действия меню
// ============================================================
void showAccounts(const Client& client, const Master& master, const Administrator& admin) {
    cout << "=== Аккаунты ===\n";
    cout << client << "\n" << master << "\n" << admin << "\n";
}

void showWarehouse(const Warehouse& wh) {
    wh.printAll(cout);
}

void auditWarehouse(Administrator& admin, const Warehouse& wh) {
    admin.auditWarehouse(wh);
}

void createAppointment(Schedule& schedule, Client& client, const Service& haircut) {
    DateTime start(2026, 11, 25, 10, 0);
    DateTime end(2026, 11, 25, 11, 0);
    Appointment* a = schedule.book(&client, haircut, start, end);
    if (a) cout << "Создана запись #" << a->getId() << "\n";
}

void showSchedule(const Schedule& schedule) {
    schedule.printAll(cout);
}

void completeAppointment(Schedule& schedule, Warehouse& wh) {
    bool done = false;
    for (int i = 0; i < schedule.getCount(); ++i) {
        Appointment* a = schedule.getAppointment(i);
        if (a && a->getStatus() == PLANNED) {
            if (a->complete(wh))
                cout << "Запись #" << a->getId() << " выполнена\n";
            else
                cout << "Запись #" << a->getId()
                     << " отклонена: не хватает расходников\n";
            done = true;
            break;
        }
    }
    if (!done) cout << "Нет запланированных записей\n";
}

void demoOperators(const Client& client) {
    cout << "=== Демонстрация операторов ===\n";
    Money a(1500.0), b(4000.0);
    cout << "a=" << a << "  b=" << b << "\n";
    cout << "a+b=" << (a + b) << "  b-a=" << (b - a) << "\n";
    cout << "a<b? " << (a < b ? "да" : "нет") << "   -b=" << (-b) << "\n";
    cout << "++a=" << (++a) << "\n";
    DateTime t(2026, 11, 25, 23, 0);
    cout << "t=" << t << "  ++t=" << (++t) << "  t+30=" << (t + 30) << "\n";
    Client c2 = client;
    c2++;
    cout << "клиент после ++: " << c2 << "\n";
}

void demoList() {
    cout << "=== Демонстрация List<T> ===\n";
    List<int> nums;
    nums.push_back(20);
    nums.push_back(30);
    nums.push_front(10);
    nums.push_back(40);
    cout << "прямой:   ";
    for (List<int>::iterator it = nums.begin(); it != nums.end(); ++it)
        cout << *it << ' ';
    cout << "\nобратный: ";
    for (List<int>::reverse_iterator r = nums.rbegin(); r != nums.rend(); ++r)
        cout << *r << ' ';
    cout << "\n";
    List<Money> prices;
    prices.push_back(Money(1500.0));
    prices.push_back(Money(4000.0));
    Money total(0.0);
    for (int i = 0; i < prices.size(); ++i) total += prices[i];
    cout << "сумма List<Money>: " << total << "\n";
}

// ============================================================
//  main
// ============================================================
int main() {
    system("chcp 65001 > nul");

    Client client(1, "Иван Иванов", "+7-900-111-22-33", "ivan@mail.ru");
    Master master(2, "Анна Петрова", "+7-900-444-55-66", "anna@salon.ru",
                  Money(60000.0), "Парикмахер");
    Administrator admin(3, "Ольга Смирнова", "+7-900-777-88-99", "olga@salon.ru",
                        Money(75000.0), 5);

    Warehouse wh;
    wh.addProduct(new Consumable(101, "Шампунь проф.",     Money(450.0),   30, 10));
    wh.addProduct(new Consumable(102, "Краска для волос",  Money(800.0),    8, 15));
    wh.addProduct(new Consumable(103, "Кондиционер",       Money(600.0),   20,  5));
    wh.addProduct(new RetailProduct(201, "Фен Dyson",      Money(35000.0),  2, "Dyson",   1.4));
    wh.addProduct(new RetailProduct(202, "Стайлер Philips",Money(5000.0),   5, "Philips", 1.6));

    Service haircut("Стрижка", Money(1500.0));
    haircut.addConsumable(101, 1);
    haircut.addConsumable(103, 1);

    Service coloring("Окрашивание", Money(4000.0));
    coloring.addConsumable(102, 2);
    coloring.addConsumable(101, 1);

    master.addService(haircut);
    master.addService(coloring);

    Schedule schedule(&master);

    const char* items[] = {
        "Список аккаунтов",
        "Склад — остатки",
        "Аудит склада (friend-класс)",
        "Создать запись (стрижка, 10:00)",
        "Показать расписание",
        "Завершить первую запланированную",
        "Демонстрация операторов",
        "Демонстрация List<T>",
        "Выход"
    };
    const int MENU_SIZE = sizeof(items) / sizeof(items[0]);

    int selected = 0;

    while (true) {
        int choice = runMenu(items, MENU_SIZE, selected);
        clearScreen();

        if (choice == -1 || choice == MENU_SIZE - 1) {
            cout << "Выход.\n";
            break;
        }

        switch (choice) {
            case 0: showAccounts(client, master, admin);          break;
            case 1: showWarehouse(wh);                            break;
            case 2: auditWarehouse(admin, wh);                    break;
            case 3: createAppointment(schedule, client, haircut); break;
            case 4: showSchedule(schedule);                       break;
            case 5: completeAppointment(schedule, wh);            break;
            case 6: demoOperators(client);                        break;
            case 7: demoList();                                   break;
        }

        waitForKey();
    }

    return 0;
}