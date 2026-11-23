#include "system.hpp"

// ============================================================
//  DateTime
// ============================================================
DateTime::DateTime(int y, int m, int d, int h, int mi)
    : y_(y), m_(m), d_(d), h_(h), mi_(mi) {}

int DateTime::getYear()  const { return y_; }
int DateTime::getMonth() const { return m_; }
int DateTime::getDay()   const { return d_; }

bool DateTime::operator<(const DateTime& o) const {
    if (y_  != o.y_)  return y_  < o.y_;
    if (m_  != o.m_)  return m_  < o.m_;
    if (d_  != o.d_)  return d_  < o.d_;
    if (h_  != o.h_)  return h_  < o.h_;
    return mi_ < o.mi_;
}
bool DateTime::operator==(const DateTime& o) const {
    return y_ == o.y_ && m_ == o.m_ && d_ == o.d_
        && h_ == o.h_ && mi_ == o.mi_;
}
bool DateTime::operator<=(const DateTime& o) const { return *this < o || *this == o; }
bool DateTime::operator>(const DateTime& o)  const { return o < *this; }
bool DateTime::operator>=(const DateTime& o) const { return o <= *this; }
bool DateTime::operator!=(const DateTime& o) const { return !(*this == o); }

DateTime& DateTime::operator++() {
    ++h_;
    if (h_ >= 24) { h_ = 0; ++d_; }
    return *this;
}
DateTime DateTime::operator++(int) {
    DateTime tmp = *this;
    ++(*this);
    return tmp;
}
DateTime DateTime::operator+(int minutes) const {
    DateTime r = *this;
    r.mi_ += minutes;
    while (r.mi_ >= 60) { r.mi_ -= 60; ++r.h_; }
    while (r.h_  >= 24) { r.h_  -= 24; ++r.d_; }
    return r;
}

ostream& operator<<(ostream& os, const DateTime& dt) {
    os << setfill('0')
       << setw(4) << dt.y_ << '-'
       << setw(2) << dt.m_ << '-'
       << setw(2) << dt.d_ << ' '
       << setw(2) << dt.h_ << ':'
       << setw(2) << dt.mi_
       << setfill(' ');
    return os;
}
istream& operator>>(istream& is, DateTime& dt) {
    is >> dt.y_ >> dt.m_ >> dt.d_ >> dt.h_ >> dt.mi_;
    return is;
}

// ============================================================
//  Money
// ============================================================
Money::Money(double rub) : kopecks_(static_cast<long long>(rub * 100 + 0.5)) {}
Money::Money(long long kop, bool) : kopecks_(kop) {}

long long Money::getKopecks() const { return kopecks_; }

Money& Money::operator=(const Money& o) {
    if (this != &o) kopecks_ = o.kopecks_;
    return *this;
}
Money Money::operator-() const { return Money(-kopecks_, true); }
Money& Money::operator++() { kopecks_ += 100; return *this; }
Money Money::operator++(int) { Money t = *this; kopecks_ += 100; return t; }

Money Money::operator+(const Money& o) const { return Money(kopecks_ + o.kopecks_, true); }
Money Money::operator-(const Money& o) const { return Money(kopecks_ - o.kopecks_, true); }
Money Money::operator*(double k)         const {
    return Money(static_cast<long long>(kopecks_ * k + 0.5), true);
}
Money& Money::operator+=(const Money& o) { kopecks_ += o.kopecks_; return *this; }
Money& Money::operator-=(const Money& o) { kopecks_ -= o.kopecks_; return *this; }

bool Money::operator==(const Money& o) const { return kopecks_ == o.kopecks_; }
bool Money::operator!=(const Money& o) const { return kopecks_ != o.kopecks_; }
bool Money::operator<(const Money& o)  const { return kopecks_ < o.kopecks_; }
bool Money::operator>(const Money& o)  const { return kopecks_ > o.kopecks_; }
bool Money::operator<=(const Money& o) const { return kopecks_ <= o.kopecks_; }
bool Money::operator>=(const Money& o) const { return kopecks_ >= o.kopecks_; }

ostream& operator<<(ostream& os, const Money& m) {
    long long r = m.kopecks_ >= 0 ? m.kopecks_ : -m.kopecks_;
    if (m.kopecks_ < 0) os << '-';
    os << (r / 100) << '.' << setw(2) << setfill('0')
       << (r % 100) << setfill(' ');
    return os;
}
istream& operator>>(istream& is, Money& m) {
    double v; is >> v; m = Money(v); return is;
}

// ============================================================
//  Service
// ============================================================
Service::Service(const char* name, Money price) : price_(price), itemCount_(0) {
    strncpy(name_, name, NAME_LEN - 1); name_[NAME_LEN - 1] = 0;
}
const char* Service::getName()  const { return name_; }
Money       Service::getPrice() const { return price_; }

bool Service::addConsumable(int productId, int qty) {
    if (itemCount_ >= 5) return false;
    items_[itemCount_].productId = productId;
    items_[itemCount_].qty       = qty;
    ++itemCount_;
    return true;
}
int Service::getItemCount() const { return itemCount_; }
const ConsumableUsage* Service::getItem(int i) const {
    if (i < 0 || i >= itemCount_) return 0;
    return &items_[i];
}

// ============================================================
//  Account
// ============================================================
Account::Account(int id, const char* name, const char* phone, const char* email)
    : id_(id) {
    strncpy(fullName_, name,  NAME_LEN - 1); fullName_[NAME_LEN - 1] = 0;
    strncpy(phone_,    phone, 31);           phone_[31] = 0;
    strncpy(email_,    email, NAME_LEN - 1); email_[NAME_LEN - 1] = 0;
}
void Account::print(ostream& os) const {
    os << getRole() << " | " << fullName_ << " | " << phone_;
}
int Account::getId()               const { return id_; }
const char* Account::getFullName() const { return fullName_; }
const char* Account::getPhone()    const { return phone_; }
const char* Account::getEmail()    const { return email_; }

ostream& operator<<(ostream& os, const Account& a) {
    a.print(os);
    return os;
}

// ============================================================
//  Client
// ============================================================
Client::Client(int id, const char* name, const char* phone, const char* email)
    : Account(id, name, phone, email), bonus_(0.0), visitCount_(0) {}

const char* Client::getRole()    const { return "Клиент"; }
Money Client::getBonus()         const { return bonus_; }
int   Client::getVisitCount()    const { return visitCount_; }

void Client::addBonus(const Money& m) { bonus_ += m; }
void Client::registerVisit()          { ++visitCount_; }

Client& Client::operator++()    { ++visitCount_; return *this; }
Client  Client::operator++(int) { Client t = *this; ++visitCount_; return t; }

void Client::print(ostream& os) const {
    Account::print(os);
    os << " | бонусы: " << bonus_ << " | визитов: " << visitCount_;
}

// ============================================================
//  Staff
// ============================================================
Staff::Staff(int id, const char* name, const char* phone,
             const char* email, Money salary)
    : Account(id, name, phone, email), salary_(salary) {}

Money Staff::getSalary()       const { return salary_; }
void  Staff::setSalary(Money s)      { salary_ = s; }

void Staff::print(ostream& os) const {
    Account::print(os);
    os << " | оклад: " << salary_;
}

// ============================================================
//  Master
// ============================================================
Master::Master(int id, const char* name, const char* phone,
               const char* email, Money salary, const char* spec)
    : Staff(id, name, phone, email, salary), rating_(0.0) {
    strncpy(specialization_, spec, 31); specialization_[31] = 0;
}
const char* Master::getRole()           const { return "Мастер"; }
const char* Master::getSpecialization() const { return specialization_; }
double      Master::getRating()         const { return rating_; }
void        Master::setRating(double r)       { rating_ = r; }

bool Master::addService(const Service& s) {
    if (services_.size() >= MAX_SERVICES) return false;
    services_.push_back(s);
    return true;
}
bool Master::hasService(const char* name) const {
    for (int i = 0; i < services_.size(); ++i)
        if (strcmp(services_.at(i).getName(), name) == 0) return true;
    return false;
}
int Master::getServiceCount() const { return services_.size(); }
const Service* Master::getService(int i) const {
    if (i < 0 || i >= services_.size()) return 0;
    return &services_.at(i);
}
void Master::print(ostream& os) const {
    Staff::print(os);
    os << " | спец: " << specialization_ << " | рейтинг: " << rating_;
}

// ============================================================
//  Product
// ============================================================
Product::Product(int id, const char* name, Money price, int qty)
    : id_(id), price_(price), quantity_(qty) {
    strncpy(name_, name, NAME_LEN - 1); name_[NAME_LEN - 1] = 0;
}
Money Product::getTotalValue() const { return price_ * quantity_; }
int Product::getId()           const { return id_; }
const char* Product::getName() const { return name_; }
Money Product::getPrice()      const { return price_; }
int   Product::getQuantity()   const { return quantity_; }
void Product::addStock(int q) { quantity_ += q; }
bool Product::writeOff(int q) {
    if (q <= 0 || q > quantity_) return false;
    quantity_ -= q;
    return true;
}
void Product::print(ostream& os) const {
    os << "[" << id_ << "] " << name_ << " | " << getCategory()
       << " | " << quantity_ << " шт | " << price_;
}
ostream& operator<<(ostream& os, const Product& p) {
    p.print(os);
    return os;
}

// ============================================================
//  Consumable
// ============================================================
Consumable::Consumable(int id, const char* name, Money price,
                       int qty, int minStock)
    : Product(id, name, price, qty), minStock_(minStock) {}
const char* Consumable::getCategory() const { return "Расходник"; }
bool Consumable::isLowStock() const { return quantity_ < minStock_; }
int  Consumable::getMinStock() const { return minStock_; }

// ============================================================
//  RetailProduct
// ============================================================
RetailProduct::RetailProduct(int id, const char* name, Money price,
                             int qty, const char* brand, double markup)
    : Product(id, name, price, qty), markup_(markup) {
    strncpy(brand_, brand, 31); brand_[31] = 0;
}
const char* RetailProduct::getCategory() const { return "Товар на продажу"; }
const char* RetailProduct::getBrand()    const { return brand_; }
Money RetailProduct::getSalePrice()      const { return price_ * markup_; }

// ============================================================
//  Administrator
// ============================================================
Administrator::Administrator(int id, const char* name, const char* phone,
                             const char* email, Money salary, int access)
    : Staff(id, name, phone, email, salary), accessLevel_(access) {}

const char* Administrator::getRole()        const { return "Администратор"; }
int         Administrator::getAccessLevel() const { return accessLevel_; }

void Administrator::print(ostream& os) const {
    Staff::print(os);
    os << " | доступ: " << accessLevel_;
}

void Administrator::auditWarehouse(const Warehouse& wh) {
    cout << "[Admin " << getFullName() << "] аудит (friend-класс):\n";
    cout << "  позиций: " << wh.products_.size() << "\n";
    for (int i = 0; i < wh.products_.size(); ++i) {
        Product* p = wh.products_.at(i);
        cout << "    id=" << p->getId()
             << " "     << p->getName()
             << " qty=" << p->getQuantity() << "\n";
    }
}

// ============================================================
//  Warehouse
// ============================================================
Warehouse::Warehouse() {}
Warehouse::~Warehouse() {
    for (int i = 0; i < products_.size(); ++i)
        delete products_.at(i);
}

bool Warehouse::addProduct(Product* p) {
    if (!p) return false;
    if (findById(p->getId())) return false;
    products_.push_back(p);
    return true;
}
Product* Warehouse::findById(int id) const {
    for (int i = 0; i < products_.size(); ++i)
        if (products_.at(i)->getId() == id) return products_.at(i);
    return 0;
}
bool Warehouse::writeOff(int id, int qty) {
    Product* p = findById(id);
    return p ? p->writeOff(qty) : false;
}
bool Warehouse::sell(int id, int qty) {
    Product* p = findById(id);
    if (!p) return false;
    if (!dynamic_cast<RetailProduct*>(p)) return false;
    return p->writeOff(qty);
}
Money Warehouse::getTotalValue() const {
    Money s(0.0);
    for (int i = 0; i < products_.size(); ++i) s += products_.at(i)->getTotalValue();
    return s;
}
int Warehouse::getCount() const { return products_.size(); }
Product* Warehouse::getProduct(int i) const {
    if (i < 0 || i >= products_.size()) return 0;
    return products_.at(i);
}
void Warehouse::printAll(ostream& os) const {
    os << "=== СКЛАД (" << products_.size() << " позиций) ===\n";
    for (int i = 0; i < products_.size(); ++i)
        os << "  " << *products_.at(i) << "\n";
    os << "  Итого: " << getTotalValue() << "\n";
}

// ============================================================
//  Appointment
// ============================================================
Appointment::Appointment(int id, Client* c, Master* m,
                         Service s, DateTime st, DateTime en)
    : id_(id), client_(c), master_(m), service_(s),
      start_(st), end_(en), status_(PLANNED) {}

bool Appointment::overlaps(const DateTime& s, const DateTime& e) const {
    return start_ < e && s < end_;
}
bool Appointment::complete(Warehouse& wh) {
    if (status_ != PLANNED) return false;
    for (int i = 0; i < service_.getItemCount(); ++i) {
        const ConsumableUsage* c = service_.getItem(i);
        Product* p = wh.findById(c->productId);
        if (!p || p->getQuantity() < c->qty) return false;
    }
    for (int i = 0; i < service_.getItemCount(); ++i) {
        const ConsumableUsage* c = service_.getItem(i);
        wh.writeOff(c->productId, c->qty);
    }
    client_->registerVisit();
    client_->addBonus(service_.getPrice() * 0.05);
    status_ = COMPLETED;
    return true;
}
void Appointment::cancel() { if (status_ == PLANNED) status_ = CANCELLED; }

int Appointment::getId()                 const { return id_; }
Client* Appointment::getClient()         const { return client_; }
Master* Appointment::getMaster()         const { return master_; }
const Service& Appointment::getService() const { return service_; }
const DateTime& Appointment::getStart()  const { return start_; }
const DateTime& Appointment::getEnd()    const { return end_; }
Status Appointment::getStatus()          const { return status_; }

ostream& operator<<(ostream& os, const Appointment& a) {
    const char* s = "?";
    switch (a.status_) {
        case PLANNED:   s = "запланирована"; break;
        case COMPLETED: s = "выполнена";     break;
        case CANCELLED: s = "отменена";      break;
    }
    os << "#" << a.id_ << " | " << a.start_ << " — " << a.end_
       << " | " << a.client_->getFullName()
       << " → " << a.master_->getFullName()
       << " | " << a.service_.getName()
       << " | " << s;
    return os;
}

// friend-функция
Money calculateDiscount(const Client& c, const Appointment& a) {
    if (c.bonus_ >= Money(50.0))
        return a.service_.getPrice() * 0.10;
    return Money(0.0);
}

// ============================================================
//  Schedule
// ============================================================
Schedule::Schedule(Master* m) : master_(m), nextId_(1) {}

Appointment* Schedule::book(Client* c, const Service& srv,
                            DateTime start, DateTime end) {
    if (!c || !master_) return 0;
    if (!master_->hasService(srv.getName())) {
        cout << "  [!] мастер не оказывает: " << srv.getName() << "\n";
        return 0;
    }
    for (int i = 0; i < appts_.size(); ++i) {
        if (appts_.at(i).getStatus() == CANCELLED) continue;
        if (appts_.at(i).overlaps(start, end)) {
            cout << "  [!] слот " << start << " — " << end << " занят\n";
            return 0;
        }
    }
    appts_.push_back(Appointment(nextId_++, c, master_, srv, start, end));
    return &appts_.at(appts_.size() - 1);
}

int Schedule::getCount() const { return appts_.size(); }
Appointment* Schedule::getAppointment(int i) {
    if (i < 0 || i >= appts_.size()) return 0;
    return &appts_.at(i);
}
Appointment& Schedule::operator[](int i) {
    if (i < 0 || i >= appts_.size()) {
        cerr << "Schedule: индекс " << i << " вне диапазона\n";
        i = 0;
    }
    return appts_.at(i);
}
void Schedule::printAll(ostream& os) const {
    os << "=== РАСПИСАНИЕ (" << appts_.size() << " записей) ===\n";
    for (int i = 0; i < appts_.size(); ++i)
        os << "  " << appts_.at(i) << "\n";
}