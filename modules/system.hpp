#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <iostream>
#include <iomanip>
#include <cstring>
#include "list.hpp"

using namespace std;

// ---------- константы ----------
const int NAME_LEN         = 64;
const int MAX_SERVICES     = 10;

enum Status { PLANNED, COMPLETED, CANCELLED };

// ---------- forward-объявления ----------
class DateTime;
class Money;
class Service;
class Account;
class Client;
class Staff;
class Master;
class Administrator;
class Product;
class Consumable;
class RetailProduct;
class Warehouse;
class Appointment;
class Schedule;

// ---------- forward-объявления friend-функций ----------
ostream& operator<<(ostream&, const DateTime&);
istream& operator>>(istream&, DateTime&);
ostream& operator<<(ostream&, const Money&);
istream& operator>>(istream&, Money&);
ostream& operator<<(ostream&, const Account&);
ostream& operator<<(ostream&, const Product&);
ostream& operator<<(ostream&, const Appointment&);
Money calculateDiscount(const Client&, const Appointment&);

// ============================================================
//  DateTime
// ============================================================
class DateTime {
    int y_, m_, d_, h_, mi_;
public:
    DateTime(int y = 2024, int m = 1, int d = 1, int h = 0, int mi = 0);

    int getYear()  const;
    int getMonth() const;
    int getDay()   const;

    bool operator<(const DateTime&)  const;
    bool operator==(const DateTime&) const;
    bool operator<=(const DateTime&) const;
    bool operator>(const DateTime&)  const;
    bool operator>=(const DateTime&) const;
    bool operator!=(const DateTime&) const;

    DateTime& operator++();
    DateTime  operator++(int);
    DateTime  operator+(int minutes) const;

    friend ostream& operator<<(ostream&, const DateTime&);
    friend istream& operator>>(istream&, DateTime&);
};

// ============================================================
//  Money
// ============================================================
class Money {
    long long kopecks_;
public:
    Money(double rub = 0.0);
    Money(long long kop, bool);

    long long getKopecks() const;

    Money& operator=(const Money&);
    Money  operator-() const;
    Money& operator++();
    Money  operator++(int);

    Money operator+(const Money&) const;
    Money operator-(const Money&) const;
    Money operator*(double)       const;
    Money& operator+=(const Money&);
    Money& operator-=(const Money&);

    bool operator==(const Money&) const;
    bool operator!=(const Money&) const;
    bool operator<(const Money&)  const;
    bool operator>(const Money&)  const;
    bool operator<=(const Money&) const;
    bool operator>=(const Money&) const;

    friend ostream& operator<<(ostream&, const Money&);
    friend istream& operator>>(istream&, Money&);
};

// ============================================================
//  Service + ConsumableUsage
// ============================================================
struct ConsumableUsage {
    int productId;
    int qty;
};

class Service {
    char name_[NAME_LEN];
    Money price_;
    ConsumableUsage items_[5];
    int itemCount_;
public:
    Service(const char* name = "", Money price = 0.0);

    const char* getName()  const;
    Money       getPrice() const;

    bool addConsumable(int productId, int qty);
    int  getItemCount() const;
    const ConsumableUsage* getItem(int i) const;
};

// ============================================================
//  Account (уровень 1, abstract)
// ============================================================
class Account {
protected:
    int id_;
    char fullName_[NAME_LEN];
    char phone_[32];
    char email_[NAME_LEN];

    virtual void print(ostream& os) const;
    friend ostream& operator<<(ostream&, const Account&);
public:
    Account(int id = 0, const char* name = "",
            const char* phone = "", const char* email = "");
    virtual ~Account() = default;

    virtual const char* getRole() const = 0;
    int getId()               const;
    const char* getFullName() const;
    const char* getPhone()    const;
    const char* getEmail()    const;
};

// ============================================================
//  Client (уровень 2)
// ============================================================
class Client : public Account {
    Money bonus_;
    int   visitCount_;
public:
    Client(int id = 0, const char* name = "",
           const char* phone = "", const char* email = "");

    const char* getRole() const override;
    Money getBonus()      const;
    int   getVisitCount() const;

    void addBonus(const Money&);
    void registerVisit();

    Client& operator++();
    Client  operator++(int);

protected:
    void print(ostream& os) const override;
    friend Money calculateDiscount(const Client&, const Appointment&);
};

// ============================================================
//  Staff (уровень 2, abstract)
// ============================================================
class Staff : public Account {
protected:
    Money salary_;
public:
    Staff(int id = 0, const char* name = "", const char* phone = "",
          const char* email = "", Money salary = 0.0);

    Money getSalary()       const;
    void  setSalary(Money);

    void print(ostream& os) const override;
};

// ============================================================
//  Master (уровень 3)
// ============================================================
class Master : public Staff {
    char specialization_[32];
    double rating_;
    List<Service> services_;
public:
    Master(int id = 0, const char* name = "", const char* phone = "",
           const char* email = "", Money salary = 0.0, const char* spec = "");

    const char* getRole()           const override;
    const char* getSpecialization() const;
    double      getRating()         const;
    void        setRating(double);

    bool addService(const Service&);
    bool hasService(const char* name) const;
    int  getServiceCount() const;
    const Service* getService(int i) const;

protected:
    void print(ostream& os) const override;
};

// ============================================================
//  Product (уровень 1, abstract)
// ============================================================
class Product {
protected:
    int id_;
    char name_[NAME_LEN];
    Money price_;
    int quantity_;

    virtual void print(ostream& os) const;
    friend ostream& operator<<(ostream&, const Product&);
public:
    Product(int id = 0, const char* name = "",
            Money price = 0.0, int qty = 0);
    virtual ~Product() = default;

    virtual const char* getCategory() const = 0;
    virtual Money getTotalValue()     const;

    int getId()           const;
    const char* getName() const;
    Money getPrice()      const;
    int   getQuantity()   const;

    void addStock(int q);
    bool writeOff(int q);
};

// ============================================================
//  Consumable (уровень 2)
// ============================================================
class Consumable : public Product {
    int minStock_;
public:
    Consumable(int id = 0, const char* name = "", Money price = 0.0,
               int qty = 0, int minStock = 0);

    const char* getCategory() const override;
    bool isLowStock() const;
    int  getMinStock() const;
};

// ============================================================
//  RetailProduct (уровень 2)
// ============================================================
class RetailProduct : public Product {
    char brand_[32];
    double markup_;
public:
    RetailProduct(int id = 0, const char* name = "", Money price = 0.0,
                  int qty = 0, const char* brand = "", double markup = 1.5);

    const char* getCategory() const override;
    const char* getBrand()    const;
    Money getSalePrice()      const;
};

// ============================================================
//  Administrator (уровень 3)
// ============================================================
class Administrator : public Staff {
    int accessLevel_;
public:
    Administrator(int id = 0, const char* name = "", const char* phone = "",
                  const char* email = "", Money salary = 0.0, int access = 0);

    const char* getRole()        const override;
    int         getAccessLevel() const;

    void auditWarehouse(const Warehouse& wh);

protected:
    void print(ostream& os) const override;
};

// ============================================================
//  Warehouse
// ============================================================
class Warehouse {
    List<Product*> products_;
    Warehouse(const Warehouse&) = delete;
    Warehouse& operator=(const Warehouse&) = delete;
public:
    Warehouse();
    ~Warehouse();

    bool addProduct(Product* p);
    Product* findById(int id) const;
    bool writeOff(int id, int qty);
    bool sell(int id, int qty);
    Money getTotalValue() const;
    int getCount() const;
    Product* getProduct(int i) const;
    void printAll(ostream& os) const;

    friend class Administrator;
};

// ============================================================
//  Appointment
// ============================================================
class Appointment {
    int id_;
    Client* client_;
    Master* master_;
    Service service_;
    DateTime start_, end_;
    Status  status_;
public:
    Appointment(int id = 0, Client* c = 0, Master* m = 0,
                Service s = Service(),
                DateTime st = DateTime(), DateTime en = DateTime());

    bool overlaps(const DateTime& s, const DateTime& e) const;
    bool complete(Warehouse& wh);
    void cancel();

    int getId()                 const;
    Client* getClient()         const;
    Master* getMaster()         const;
    const Service& getService() const;
    const DateTime& getStart()  const;
    const DateTime& getEnd()    const;
    Status getStatus()          const;

    friend Money calculateDiscount(const Client&, const Appointment&);
    friend ostream& operator<<(ostream&, const Appointment&);
};

// ============================================================
//  Schedule
// ============================================================
class Schedule {
    Master* master_;
    List<Appointment> appts_;
    int nextId_;
public:
    Schedule(Master* m = 0);

    Appointment* book(Client* c, const Service& srv,
                      DateTime start, DateTime end);
    int getCount() const;
    Appointment* getAppointment(int i);
    Appointment& operator[](int i);
    void printAll(ostream& os) const;
};

#endif // SYSTEM_HPP