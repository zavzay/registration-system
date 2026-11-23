#ifndef LIST_HPP
#define LIST_HPP

#include <stdexcept>
#include <utility>

// ============================================================
//  List<T> — двусвязный список с двунаправленными итераторами
// ============================================================
template <typename T>
class List {
    struct Node {
        T     data;
        Node* prev;
        Node* next;
        Node(const T& d);
        Node(T&& d);
    };

    Node* head_;
    Node* tail_;
    int   size_;

public:
    // ---------- const_iterator ----------
    class const_iterator {
        friend class List;
    protected:
        Node* cur_;
        explicit const_iterator(Node* n) : cur_(n) {}
    public:
        const_iterator() : cur_(0) {}
        const T& operator*()  const { return cur_->data; }
        const T* operator->() const { return &cur_->data; }
        const_iterator& operator++();
        const_iterator  operator++(int);
        const_iterator& operator--();
        const_iterator  operator--(int);
        bool operator==(const const_iterator& o) const { return cur_ == o.cur_; }
        bool operator!=(const const_iterator& o) const { return cur_ != o.cur_; }
    };

    // ---------- iterator ----------
    class iterator : public const_iterator {
        friend class List;
        explicit iterator(Node* n) : const_iterator(n) {}
    public:
        iterator() : const_iterator() {}
        T& operator*()  const { return this->cur_->data; }
        T* operator->() const { return &this->cur_->data; }
        iterator& operator++();
        iterator  operator++(int);
        iterator& operator--();
        iterator  operator--(int);
    };

    // ---------- reverse_iterator ----------
    class reverse_iterator {
        friend class List;
        Node* cur_;
        explicit reverse_iterator(Node* n) : cur_(n) {}
    public:
        reverse_iterator() : cur_(0) {}
        T& operator*()  const { return cur_->data; }
        T* operator->() const { return &cur_->data; }
        reverse_iterator& operator++();
        reverse_iterator  operator++(int);
        reverse_iterator& operator--();
        reverse_iterator  operator--(int);
        bool operator==(const reverse_iterator& o) const { return cur_ == o.cur_; }
        bool operator!=(const reverse_iterator& o) const { return cur_ != o.cur_; }
    };

    // ---------- Rule of Five ----------
    List();
    ~List();
    List(const List& other);
    List(List&& other);
    List& operator=(const List& other);
    List& operator=(List&& other);
    void swap(List& other);

    // ---------- Модификаторы ----------
    void push_back(const T& v);
    void push_back(T&& v);
    void push_front(const T& v);
    void push_front(T&& v);
    void pop_back();
    void pop_front();

    iterator insert(iterator pos, const T& value);
    iterator erase(iterator pos);

    bool insertAt(int index, const T& value);
    bool removeAt(int index);
    void clear();

    // ---------- Доступ ----------
    T&       front();
    const T& front() const;
    T&       back();
    const T& back()  const;
    T&       at(int i);
    const T& at(int i) const;
    T&       operator[](int i);
    const T& operator[](int i) const;

    int  size()  const;
    bool empty() const;

    // ---------- Итераторы ----------
    iterator begin();
    iterator end();
    const_iterator begin()  const;
    const_iterator end()    const;
    const_iterator cbegin() const;
    const_iterator cend()   const;
    reverse_iterator rbegin();
    reverse_iterator rend();

private:
    void append(Node* n);
    void prepend(Node* n);
    void removeNode(Node* n);
    Node* nodeAt(int i) const;
};

#endif // LIST_HPP