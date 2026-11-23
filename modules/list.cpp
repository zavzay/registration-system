#include "list.hpp"
#include "system.hpp" // ОБЯЗАТЕЛЬНО: дает полные типы Money, Service и т.д.

// ============================================================
//  Node
// ============================================================
template <typename T>
List<T>::Node::Node(const T& d) : data(d), prev(0), next(0) {}

template <typename T>
List<T>::Node::Node(T&& d) : data(std::move(d)), prev(0), next(0) {}

// ============================================================
//  const_iterator
// ============================================================
template <typename T>
typename List<T>::const_iterator& List<T>::const_iterator::operator++() {
    cur_ = cur_->next; return *this;
}
template <typename T>
typename List<T>::const_iterator List<T>::const_iterator::operator++(int) {
    const_iterator t = *this; cur_ = cur_->next; return t;
}
template <typename T>
typename List<T>::const_iterator& List<T>::const_iterator::operator--() {
    cur_ = cur_->prev; return *this;
}
template <typename T>
typename List<T>::const_iterator List<T>::const_iterator::operator--(int) {
    const_iterator t = *this; cur_ = cur_->prev; return t;
}

// ============================================================
//  iterator
// ============================================================
template <typename T>
typename List<T>::iterator& List<T>::iterator::operator++() {
    this->cur_ = this->cur_->next; return *this;
}
template <typename T>
typename List<T>::iterator List<T>::iterator::operator++(int) {
    iterator t = *this; this->cur_ = this->cur_->next; return t;
}
template <typename T>
typename List<T>::iterator& List<T>::iterator::operator--() {
    this->cur_ = this->cur_->prev; return *this;
}
template <typename T>
typename List<T>::iterator List<T>::iterator::operator--(int) {
    iterator t = *this; this->cur_ = this->cur_->prev; return t;
}

// ============================================================
//  reverse_iterator
// ============================================================
template <typename T>
typename List<T>::reverse_iterator& List<T>::reverse_iterator::operator++() {
    cur_ = cur_->prev; return *this;
}
template <typename T>
typename List<T>::reverse_iterator List<T>::reverse_iterator::operator++(int) {
    reverse_iterator t = *this; cur_ = cur_->prev; return t;
}
template <typename T>
typename List<T>::reverse_iterator& List<T>::reverse_iterator::operator--() {
    cur_ = cur_->next; return *this;
}
template <typename T>
typename List<T>::reverse_iterator List<T>::reverse_iterator::operator--(int) {
    reverse_iterator t = *this; cur_ = cur_->next; return t;
}

// ============================================================
//  Rule of Five
// ============================================================
template <typename T>
List<T>::List() : head_(0), tail_(0), size_(0) {}

template <typename T>
List<T>::~List() { clear(); }

template <typename T>
List<T>::List(const List& other) : head_(0), tail_(0), size_(0) {
    try {
        for (const_iterator it = other.begin(); it != other.end(); ++it)
            push_back(*it);
    } catch (...) { clear(); throw; }
}

template <typename T>
List<T>::List(List&& other)
    : head_(other.head_), tail_(other.tail_), size_(other.size_) {
    other.head_ = other.tail_ = 0;
    other.size_ = 0;
}

template <typename T>
List<T>& List<T>::operator=(const List& other) {
    if (this != &other) { List tmp(other); swap(tmp); }
    return *this;
}

template <typename T>
List<T>& List<T>::operator=(List&& other) {
    if (this != &other) {
        clear();
        head_ = other.head_; tail_ = other.tail_; size_ = other.size_;
        other.head_ = other.tail_ = 0; other.size_ = 0;
    }
    return *this;
}

template <typename T>
void List<T>::swap(List& other) {
    Node* th = head_; head_ = other.head_; other.head_ = th;
    Node* tt = tail_; tail_ = other.tail_; other.tail_ = tt;
    int   ts = size_; size_ = other.size_; other.size_ = ts;
}

// ============================================================
//  Модификаторы
// ============================================================
template <typename T>
void List<T>::push_back(const T& v)  { append(new Node(v)); }
template <typename T>
void List<T>::push_back(T&& v)       { append(new Node(std::move(v))); }
template <typename T>
void List<T>::push_front(const T& v) { prepend(new Node(v)); }
template <typename T>
void List<T>::push_front(T&& v)      { prepend(new Node(std::move(v))); }

template <typename T>
void List<T>::pop_back() {
    if (!tail_) throw std::out_of_range("List::pop_back: empty");
    removeNode(tail_);
}
template <typename T>
void List<T>::pop_front() {
    if (!head_) throw std::out_of_range("List::pop_front: empty");
    removeNode(head_);
}

template <typename T>
typename List<T>::iterator List<T>::insert(iterator pos, const T& value) {
    if (!pos.cur_) { push_back(value); return iterator(tail_); }
    Node* n = new Node(value);
    n->prev = pos.cur_->prev;
    n->next = pos.cur_;
    if (pos.cur_->prev) pos.cur_->prev->next = n;
    else                head_ = n;
    pos.cur_->prev = n;
    ++size_;
    return iterator(n);
}

template <typename T>
typename List<T>::iterator List<T>::erase(iterator pos) {
    if (!pos.cur_) return iterator(0);
    Node* nx = pos.cur_->next;
    removeNode(pos.cur_);
    return iterator(nx);
}

template <typename T>
bool List<T>::insertAt(int index, const T& value) {
    if (index < 0 || index > size_) return false;
    if (index == size_) { push_back(value); return true; }
    insert(iterator(nodeAt(index)), value);
    return true;
}

template <typename T>
bool List<T>::removeAt(int index) {
    Node* n = nodeAt(index);
    if (!n) return false;
    removeNode(n);
    return true;
}

template <typename T>
void List<T>::clear() {
    Node* cur = head_;
    while (cur) { Node* nx = cur->next; delete cur; cur = nx; }
    head_ = tail_ = 0;
    size_ = 0;
}

// ============================================================
//  Доступ
// ============================================================
template <typename T>
T& List<T>::front() {
    if (!head_) throw std::out_of_range("List::front: empty");
    return head_->data;
}
template <typename T>
const T& List<T>::front() const {
    if (!head_) throw std::out_of_range("List::front: empty");
    return head_->data;
}
template <typename T>
T& List<T>::back() {
    if (!tail_) throw std::out_of_range("List::back: empty");
    return tail_->data;
}
template <typename T>
const T& List<T>::back() const {
    if (!tail_) throw std::out_of_range("List::back: empty");
    return tail_->data;
}

template <typename T>
T& List<T>::at(int i) {
    Node* n = nodeAt(i);
    if (!n) throw std::out_of_range("List::at: index out of range");
    return n->data;
}
template <typename T>
const T& List<T>::at(int i) const {
    Node* n = nodeAt(i);
    if (!n) throw std::out_of_range("List::at: index out of range");
    return n->data;
}

template <typename T>
T& List<T>::operator[](int i)       { return at(i); }
template <typename T>
const T& List<T>::operator[](int i) const { return at(i); }

template <typename T>
int  List<T>::size()  const { return size_; }
template <typename T>
bool List<T>::empty() const { return size_ == 0; }

// ============================================================
//  Итераторы
// ============================================================
template <typename T>
typename List<T>::iterator List<T>::begin()              { return iterator(head_); }
template <typename T>
typename List<T>::iterator List<T>::end()                { return iterator(0); }
template <typename T>
typename List<T>::const_iterator List<T>::begin()  const { return const_iterator(head_); }
template <typename T>
typename List<T>::const_iterator List<T>::end()    const { return const_iterator(0); }
template <typename T>
typename List<T>::const_iterator List<T>::cbegin() const { return const_iterator(head_); }
template <typename T>
typename List<T>::const_iterator List<T>::cend()   const { return const_iterator(0); }
template <typename T>
typename List<T>::reverse_iterator List<T>::rbegin()     { return reverse_iterator(tail_); }
template <typename T>
typename List<T>::reverse_iterator List<T>::rend()       { return reverse_iterator(0); }

// ============================================================
//  Приватные помощники
// ============================================================
template <typename T>
void List<T>::append(Node* n) {
    n->prev = tail_;
    if (tail_) tail_->next = n;
    else       head_ = n;
    tail_ = n;
    ++size_;
}

template <typename T>
void List<T>::prepend(Node* n) {
    n->next = head_;
    if (head_) head_->prev = n;
    else       tail_ = n;
    head_ = n;
    ++size_;
}

template <typename T>
void List<T>::removeNode(Node* n) {
    if (n->prev) n->prev->next = n->next;
    else         head_ = n->next;
    if (n->next) n->next->prev = n->prev;
    else         tail_ = n->prev;
    delete n;
    --size_;
}

template <typename T>
typename List<T>::Node* List<T>::nodeAt(int i) const {
    if (i < 0 || i >= size_) return 0;
    if (i < size_ / 2) {
        Node* cur = head_;
        for (int k = 0; k < i; ++k) cur = cur->next;
        return cur;
    } else {
        Node* cur = tail_;
        for (int k = size_ - 1; k > i; --k) cur = cur->prev;
        return cur;
    }
}

// ============================================================
//  Явные инстанциации (обязательно совпадают с extern в .hpp)
// ============================================================
template class List<int>;
template class List<Money>;
template class List<Service>;
template class List<Appointment>;
template class List<Account*>;
template class List<Product*>;