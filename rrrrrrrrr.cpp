#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// ===== ����� =====

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
        cout << "������� ������: " << name << " - " << fixed << setprecision(2) << price << " ���" << endl;
    }
};

class Drink : public Dish {
public:
    Drink(string name, double price) : Dish(name, price) {}

    void print() const override {
        cout << "����: " << name << " - " << fixed << setprecision(2) << price << " ���" << endl;
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
        cout << "----- ���� -----" << endl;
        for (size_t i = 0; i < items.size(); ++i) {
            cout << i + 1 << ". ";
            items[i]->print();
        }
    }

    Dish* getDish(int index) const {
        if (index < 0 || index >= static_cast<int>(items.size()))
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

        cout << "\n�����: ³���, " << visitor.getName() << "! ������ ����� ������ � ���� (0 - ���������):\n";
        menu.printMenu();

        while (true) {
            cout << "\n��� ���� (0 - ���������): ";
            cin >> choice;
            if (choice == 0) break;

            if (choice < 1 || choice > static_cast<int>(menu.size())) {
                cout << "������������ �����, ��������� �����." << endl;
                continue;
            }

            cout << "ʳ������: ";
            cin >> quantity;

            Dish* selected = menu.getDish(choice - 1);
            double cost = selected->getPrice() * quantity;

            if (!visitor.canAfford(total + cost)) {
                cout << "����������� �����. ��������� ���� ������ ��� ����� �������." << endl;
                continue;
            }

            total += cost;
            cout << "������: " << selected->getName() << " x" << quantity << " = " << cost << " ���" << endl;
        }

        if (total > 0) {
            cout << "\n���� �� ������: " << fixed << setprecision(2) << total << " ���\n";

            int cheatChoice;
            cout << "�� ���� ����� ������ �����?\n0 - ͳ, 1 - ���: ";
            cin >> cheatChoice;

            if (cheatChoice == 1) {
                double stealAmount = total * 0.1;
                money += total - stealAmount;
                stolenMoney += stealAmount;
                wasEverDishonest = true;
                cout << "����� ����� " << fixed << setprecision(2) << stealAmount << " ���" << endl;
            }
            else {
                money += total;
                cout << "����� ����� ����� �����." << endl;
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
        cout << "\n--- ��������� ���� ---" << endl;
        cout << "���� ������: " << fixed << setprecision(2) << seller.getMoney() << " ���" << endl;
        if (seller.wasDishonest()) {
            cout << " �������� ���������! ��������: " << seller.getStolenMoney() << " ���!" << endl;
        }
        else {
            cout << " ��� �����. �������� �� ��������." << endl;
        }
    }
};

// ===== main =====

int main() {
    Menu menu;
    menu.addItem(new MainCourse("����", 60));
    menu.addItem(new MainCourse("������� � ����", 75));
    menu.addItem(new Drink("������", 25));
    menu.addItem(new Drink("��� � �������", 20));

    Seller seller;
    Inspector inspector;

    while (true) {
        //cin.ignore();
        string name;
        double balance;

        cout << "������ ��� �볺���: ";
        getline(cin, name);
        cout << "������ ���� ������ �볺���: ";
        cin >> balance;

        Visitor visitor(name, balance);
        seller.serveCustomer(menu, visitor);

        int next;
        cout << "\n�� ������ �� ���� �볺��? (1 - ���, 0 - �): ";
        cin >> next;
        if (next != 1) break;

        cin.ignore();
    }

    // ��������� ��������� ���
    inspector.audit(seller);

    return 0;
}