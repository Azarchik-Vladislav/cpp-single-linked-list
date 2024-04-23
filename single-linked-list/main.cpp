#pragma once

#include <cassert>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

public:
    SingleLinkedList() {}
    
    ~SingleLinkedList() {
         Clear();   
    }
    
    template <typename Iter>
    explicit SingleLinkedList(Iter begin, Iter end);
    
    SingleLinkedList(std::initializer_list<Type> values) 
        : SingleLinkedList(values.begin(), values.end())    {}
    
    SingleLinkedList(const SingleLinkedList& other) 
        : SingleLinkedList(other.begin(), other.end())   {}
    
    SingleLinkedList& operator=(const SingleLinkedList& rhs);
    
    void PushFront(const Type& value);
    void Clear() noexcept;
    void swap(SingleLinkedList& other) noexcept;
    
    template <typename ValueType>
    class BasicIterator;

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    // Итератор, допускающий изменение элементов списка
    using Iterator = BasicIterator<Type>;
    // Константный итератор, предоставляющий доступ для чтения к элементам списка
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator before_begin() noexcept;

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept;
    [[nodiscard]] ConstIterator before_begin() const noexcept;

    [[nodiscard]] Iterator begin() noexcept;    
    [[nodiscard]] Iterator end() noexcept;

    [[nodiscard]] ConstIterator begin() const noexcept;
    [[nodiscard]] ConstIterator end() const noexcept;

    [[nodiscard]] ConstIterator cbegin() const noexcept;
    [[nodiscard]] ConstIterator cend() const noexcept;


    [[nodiscard]] size_t GetSize() const noexcept;
    
    [[nodiscard]] bool IsEmpty() const noexcept;
    
    void PopFront() noexcept;

    Iterator InsertAfter(ConstIterator pos, const Type& value);

    Iterator EraseAfter(ConstIterator pos) noexcept;

private:
    Node head_;
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (lhs < rhs) || (lhs == rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return  !(lhs < rhs);
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs) || (lhs == rhs);
}

template <typename Type>
template <typename Iter>
SingleLinkedList<Type>::SingleLinkedList(Iter begin, Iter end) {
    assert(size_ == 0 && head_.next_node == nullptr);
        
    SingleLinkedList tmp;
   
    try{
        SingleLinkedList::Iterator iter_list;
        iter_list.node_ = &tmp.head_; 
        for(auto it = begin; it != end; ++it) {       
            iter_list.node_->next_node = new Node(*it, iter_list.node_->next_node);  
            ++iter_list;
            ++tmp.size_;  
        }
            
        swap(tmp);
    } catch(...) {
        Clear();
        throw;
    }
}

template <typename Type>
SingleLinkedList<Type> &SingleLinkedList<Type>::operator=(const SingleLinkedList<Type> &rhs) {
    if(this != &rhs){
        auto tmp_object(rhs);
        swap(tmp_object);
    }
    return *this;
}

template <typename Type>
void SingleLinkedList<Type>::PushFront(const Type &value) {
    head_.next_node = new Node(value, head_.next_node);
    ++size_;
}

template <typename Type>
void SingleLinkedList<Type>::Clear() noexcept {
    while(head_.next_node != nullptr){
        Node* tmp_node = head_.next_node;
        head_.next_node = head_.next_node->next_node;
            
        delete tmp_node;  
    }

    size_ = 0;
}

template <typename Type>
void SingleLinkedList<Type>::swap(SingleLinkedList &other) noexcept{
    std::swap(other.head_.next_node, head_.next_node);
    std::swap(other.size_, size_);
}

template <typename Type>
template <typename ValueType>
class SingleLinkedList<Type>::BasicIterator{

    friend class SingleLinkedList;

    explicit BasicIterator(Node* node) : node_(node) {}

    public:
        // Объявленные ниже типы сообщают стандартной библиотеке о свойствах этого итератора
        // Категория итератора — forward iterator
        // (итератор, который поддерживает операции инкремента и многократное разыменование)
        using iterator_category = std::forward_iterator_tag;
        // Тип элементов, по которым перемещается итератор
        using value_type = Type;
        // Тип, используемый для хранения смещения между итераторами
        using difference_type = std::ptrdiff_t;
        // Тип указателя на итерируемое значение
        using pointer = ValueType*;
        // Тип ссылки на итерируемое значение
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept : node_(other.node_) {}

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }
        
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        BasicIterator& operator++() noexcept {
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            assert(node_->next_node != nullptr);

            auto last_value(*this);
            node_ = node_->next_node;
            return last_value;
        }

        [[nodiscard]] reference operator*() const noexcept {
            assert(node_ != nullptr);

            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_ != nullptr);

            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };

template <typename Type>
typename SingleLinkedList<Type>::Iterator SingleLinkedList<Type>::before_begin() noexcept {
    return SingleLinkedList::Iterator{&head_};
}

template <typename Type>
typename SingleLinkedList<Type>::ConstIterator SingleLinkedList<Type>::cbefore_begin() const noexcept {
    return before_begin();  
}

template <typename Type>
typename SingleLinkedList<Type>::ConstIterator SingleLinkedList<Type>::before_begin() const noexcept {
    return ConstIterator {const_cast<Node*>(&head_)};
}

template <typename Type>
typename SingleLinkedList<Type>::Iterator SingleLinkedList<Type>::begin() noexcept {
    return Iterator{head_.next_node};
}
template <typename Type>
typename SingleLinkedList<Type>::Iterator SingleLinkedList<Type>::end() noexcept {
    return Iterator{nullptr};
}

template <typename Type>
typename SingleLinkedList<Type>::ConstIterator SingleLinkedList<Type>::begin() const noexcept {
    return Iterator{head_.next_node};
}

template <typename Type>
typename SingleLinkedList<Type>::ConstIterator SingleLinkedList<Type>::end() const noexcept {
    return Iterator{nullptr};
}

template <typename Type>
typename SingleLinkedList<Type>::ConstIterator SingleLinkedList<Type>::cbegin() const noexcept {
    return ConstIterator{head_.next_node};
}

template <typename Type>
typename SingleLinkedList<Type>::ConstIterator SingleLinkedList<Type>::cend() const noexcept {
    return ConstIterator{nullptr};
}

template <typename Type>
size_t SingleLinkedList<Type>::GetSize() const noexcept {
    return size_;
}

template <typename Type>
bool SingleLinkedList<Type>::IsEmpty() const noexcept {
    return size_ == 0;
}

template <typename Type>
void SingleLinkedList<Type>::PopFront() noexcept{
    if(begin() == end()){
        return;
    }
    Iterator tmp_iter;

    tmp_iter.node_ = head_.next_node;
    head_.next_node = head_.next_node->next_node;
    --size_;

    delete tmp_iter.node_;
}

template <typename Type>
typename SingleLinkedList<Type>::Iterator SingleLinkedList<Type>::InsertAfter(ConstIterator pos, const Type &value) {
    assert(pos.node_ != nullptr);

    Node* new_node = new Node(value, pos.node_->next_node);
    pos.node_->next_node = new_node;
    ++size_;

    return Iterator{new_node};
}

template <typename Type>
typename SingleLinkedList<Type>::Iterator SingleLinkedList<Type>::EraseAfter(ConstIterator pos) noexcept {
    assert(pos.node_ != nullptr);

    if(begin() == end()){
        return {};
    }

    Iterator deleted_elem(pos.node_->next_node);
    Iterator new_position(pos.node_->next_node->next_node);
    pos.node_->next_node = new_position.node_;
        
    delete deleted_elem.node_;
    --size_;

    return new_position;
}
