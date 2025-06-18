#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// ===== Класи =====

class Dish {
protected:
    string name;
    double price;

public:
    Dish(string name, double price) : name(name), price(price) {}
    virtual ~Dish() {}

    string getName() const { return name; }
    double getPrice() const { return price; }

    virtual void print() const = 0;
};

class MainCourse : public Dish {
public:
    MainCourse(string name, double price) : Dish(name, price) {}

    void print() const override {
        cout << "Основна страва: " << name << " - " << fixed << setprecision(2) << price << " грн" << endl;
    }
};

class Drink : public Dish {
public:
    Drink(string name, double price) : Dish(name, price) {}

    void print() const override {
        cout << "Напій: " << name << " - " << fixed << setprecision(2) << price << " грн" << endl;
    }
};

class Menu {
private:
    vector<Dish*> items;

public:
    ~Menu() {
        for (auto item : items)
            delete item;
    }

    void addItem(Dish* dish) {
        items.push_back(dish);
    }

    void printMenu() const {
        cout << "----- Меню -----" << endl;
        for (size_t i = 0; i < items.size(); ++i) {
            cout << i + 1 << ". ";
            items[i]->print();
        }
    }

    Dish* getDish(int index) const {
        if (index < 0  index >= static_cast<int>(items.size()))
            return nullptr;
        return items[index];
    }

    size_t size() const {
        return items.size();
    }
};

class Visitor {
private:
    string name;
    double balance;

public:
    Visitor(string name, double balance) : name(name), balance(balance) {}

    string getName() const { return name; }
    double getBalance() const { return balance; }

    void pay(double amount) {
        balance -= amount;
    }

    bool canAfford(double amount) const {
        return balance >= amount;
    }
};

class Seller {
private:
    double money = 0;
    double stolenMoney = 0;
    bool wasEverDishonest = false;

public:
    void serveCustomer(const Menu& menu, Visitor& visitor) {
        int choice, quantity;
        double total = 0;

        cout << "\nКасир: Вітаю, " << visitor.getName() << "! Оберіть номер страви з меню (0 - завершити):\n";
        menu.printMenu();

        while (true) {
            cout << "\nВаш вибір (0 - завершити): ";
            cin >> choice;
            if (choice == 0) break;

            if (choice < 1  choice > static_cast<int>(menu.size())) {
                cout << "Неправильний номер, спробуйте знову." << endl;
                continue;
            }

            cout << "Кількість: ";
            cin >> quantity;

            Dish* selected = menu.getDish(choice - 1);
            double cost = selected->getPrice() * quantity;

            if (!visitor.canAfford(total + cost)) {
                cout << "Недостатньо коштів. Спробуйте іншу страву або меншу кількість." << endl;
                continue;
            }

            total += cost;
            cout << "Додано: " << selected->getName() << " x" << quantity << " = " << cost << " грн" << endl;
        }

        if (total > 0) {
            cout << "\nСума до сплати: " << fixed << setprecision(2) << total << " грн\n";

            int cheatChoice;
            cout << "Чи буде касир красти гроші?\n0 - Ні, 1 - Так: ";
            cin >> cheatChoice;

            if (cheatChoice == 1) {
                double stealAmount = total * 0.1;
                money += total - stealAmount;
                stolenMoney += stealAmount;
                wasEverDishonest = true;
                cout << "Касир вкрав " << fixed << setprecision(2) << stealAmount << " грн " << endl;
            } else {
                money += total;
                cout << "Касир провів платіж чесно." << endl;
            }

            visitor.pay(total);
        }
    }

    double getMoney() const { return money; }
    double getStolenMoney() const { return stolenMoney; }
    bool wasDishonest() const { return wasEverDishonest; }
};
class Inspector {
public:
    void audit(const Seller& seller) const {
        cout << "\n--- Інспекція каси ---" << endl;
        cout << "Каса містить: " << fixed << setprecision(2) << seller.getMoney() << " грн" << endl;
        if (seller.wasDishonest()) {
            cout << "Виявлено махінації! Вкрадено: " << seller.getStolenMoney() << " грн!" << endl;
        } else {
            cout << "Все чесно. Порушень не виявлено." << endl;
        }
    }
};

// ===== main =====

int main() {
    Menu menu;
    menu.addItem(new MainCourse("Борщ", 60));
    menu.addItem(new MainCourse("Котлета з пюре", 75));
    menu.addItem(new Drink("Компот", 25));
    menu.addItem(new Drink("Чай з лимоном", 20));

    Seller seller;
    Inspector inspector;

    while (true) {
        //cin.ignore();
        string name;
        double balance;

        cout << "Введіть ім’я клієнта: ";
        getline(cin, name);
        cout << "Введіть суму грошей клієнта: ";
        cin >> balance;

        Visitor visitor(name, balance);
        seller.serveCustomer(menu, visitor);

        int next;
        cout << "\nЧи прийде ще один клієнт? (1 - так, 0 - ні): ";
        cin >> next;
        if (next != 1) break;
        
        cin.ignore();
    }

    // Інспекція наприкінці дня
    inspector.audit(seller);

    return 0;
}
