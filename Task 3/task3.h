#include <iostream>
#include <memory>
#include <vector>

#ifndef LIST_WITH_TAIL
#define LIST_WITHOUT_TAIL
template<typename value_type>
class SinglyLinkedList
{
private:
    struct Node
    {
        explicit Node(const value_type& value) : value_(value), next_(nullptr){}
        Node(const value_type& value, Node* next) : value_(value), next_(next){}

        value_type value_;
        std::shared_ptr<Node> next_; 
    };

    std::shared_ptr<Node> head_;

public:
    SinglyLinkedList() : head_(nullptr) {} 
    SinglyLinkedList(std::initializer_list<value_type> values)
    {
        if (values.size())
        {
            for (auto &value : values)
            {
                push_back(value);
            }
        }
        else
        {
            head_ = nullptr;
        }
    }

    /**
     * Finds the first node with the given value.
     * @param value The value to search for.
     * @return A shared_ptr to the found node or nullptr if not found.
    */ 
    std::shared_ptr<Node> find(const value_type& value)
    {
        auto cur = head_;
        while (cur != nullptr && cur->value_ != value) 
        {
            cur = cur->next_;
        }
        return cur;
    }

    /**
     * Finds all nodes with the given value.
     * @param value The value to search for.
     * @return A vector of shared_ptr to the founded nodes.
    */ 
    std::vector<std::shared_ptr<Node>> find_all(const value_type& value)
    {
        std::vector<std::shared_ptr<Node>> ret;
        auto cur = head_;
        while (cur != nullptr) 
        {
            if (cur->value_ == value)
            {
                ret.push_back(cur);
            }
            cur = cur->next_;
        }
        return ret;
    }
    
    /**
     * Inserts a new value after a specified node.
     * @param node The node after which the new value will be inserted.
     * @param value The value to be inserted.
     * @return true if the operation is successful, false otherwise.
    */ 
    bool push_after(std::shared_ptr<Node> node, const value_type& value) 
    {
        if (node == nullptr)
        {
            return false;
        }

        auto newNode = std::make_shared<Node>(value);
        auto tmpNode = node->next_;
        node->next_ = newNode;
        newNode->next_ = tmpNode;

        return true;
    }

    /**
     * Inserts a new value after the first occurrence of another value.
     * @param after_value The value after which the new value should be inserted.
     * @param new_value The new value to insert.
     * @return true if the operation is successful, false otherwise.
    */ 
    bool push_after(const value_type& after_value, const value_type& new_value) {
        if (head_ == nullptr) 
        {
            return false;
        }
        return push_after(find(after_value), new_value);
    }

    /**
     * Adds a new value at the end of the list.
     * @param value The value to be inserted.
    */ 
    void push_back(const value_type& value) 
    {
        auto newNode = std::make_shared<Node>(value);
        if (head_ == nullptr)
        {
            head_ = newNode;
            return;
        }
        auto last = head_;
        while (last->next_ != nullptr)
        {
            last = last->next_;
        }
        last->next_ = newNode;
    }

    /**
     * Inserts a new value at the beginning of the list.
     * @param value The value to be inserted.
    */ 
    void push_front(const value_type& value) 
    {
        auto newNode = std::make_shared<Node>(value);
        if (head_ == nullptr)
        {
            head_ = newNode;
        }
        else
        {
            newNode->next_ = head_;
            head_ = newNode;
        }
    }

    /**
     * Prints the entire list to standard output.
    */ 
    void print()
    {
        auto cur = head_;
        while (cur != nullptr)
        {
            std::cout << cur->value_ << "->";
            cur = cur->next_;
        }
        std::cout << "nullptr" << std::endl;
    }

    /**
     * Removes the specified node from the list.
     * @param node The node to be removed.
     * @return true if the operation is successful, false otherwise.
    */ 
    bool remove(std::shared_ptr<Node> node) 
    {
        if (node == nullptr || head_ == nullptr) 
        {
            return false;
        }

        if (head_ == node) 
        {
            head_ = head_->next_;
            return true;
        }

        auto cur = head_;
        while (cur->next_ != nullptr && cur->next_ != node) 
        {
            cur = cur->next_;
        }

        if (cur->next_ == node) 
        {
            cur->next_ = node->next_;
            return true;
        }

        return false; 
    }

    /**
     * Removes the first occurrence of a value from the list.
     * @param value The value to be removed.
     * @return true if the operation is successful, false otherwise.
    */ 
    bool remove(const value_type& value)
    {
        return remove(find(value));
    }
    
    /**
     * Removes all occurrences of a value from the list.
     * @param value The value to be removed.
    */ 
    void remove_all(const value_type& value)
    {
        while(remove(value) != false) {}
    }

    /**
     * Removes the last node from the list.
     * @return true if the operation is successful, false otherwise.
    */ 
    bool remove_back()
    {
        if (head_ == nullptr) 
        {
            return false;
        }

        if (head_->next_ == nullptr) 
        {
            head_ = nullptr;
            return true;
        }

        auto cur = head_;
        while (cur->next_ && cur->next_->next_ != nullptr) 
        {
            cur = cur->next_;
        }

        cur->next_ = nullptr;
        return true;
    }

    /**
     * Removes the head node from the list.
     * @return true if the operation is successful, false otherwise.
    */ 
    bool remove_front()
    {
        return remove(head_);
    }
};
#endif //LIST_WITHOUT_TAIL

#ifndef LIST_WITHOUT_TAIL
#define LIST_WITH_TAIL
template<typename value_type>
class SinglyLinkedList
{
private:
    struct Node
    {
        explicit Node(const value_type& value) : value_(value), next_(nullptr){}
        Node(const value_type& value, Node* next) : value_(value), next_(next){}

        value_type value_;
        std::shared_ptr<Node> next_; 
    };

    std::shared_ptr<Node> head_;
    std::shared_ptr<Node> tail_;

public:
    SinglyLinkedList() : head_(nullptr), tail_(nullptr) {} 
    SinglyLinkedList(std::initializer_list<value_type> values)
    {
        if (values.size())
        {
            for (auto &value : values)
            {
                push_back(value);
            }
        }
        else
        {
            head_ = nullptr;
        }
    }

    /**
     * Finds the first node with the given value.
     * @param value The value to search for.
     * @return A shared_ptr to the found node or nullptr if not found.
    */ 
    std::shared_ptr<Node> find(const value_type& value)
    {
        auto cur = head_;
        while (cur != nullptr && cur->value_ != value) 
        {
            cur = cur->next_;
        }
        return cur;
    }

    /**
     * Finds all nodes with the given value.
     * @param value The value to search for.
     * @return A vector of shared_ptr to the founded nodes.
    */ 
    std::vector<std::shared_ptr<Node>> find_all(const value_type& value)
    {
        std::vector<std::shared_ptr<Node>> ret;
        auto cur = head_;
        while (cur != nullptr) 
        {
            if (cur->value_ == value)
            {
                ret.push_back(cur);
            }
            cur = cur->next_;
        }
        return ret;
    }
    
    /**
     * Inserts a new value after a specified node.
     * @param node The node after which the new value will be inserted.
     * @param value The value to be inserted.
     * @return true if the operation is successful, false otherwise.
    */ 
    bool push_after(std::shared_ptr<Node> node, const value_type& value) 
    {
        if (node == nullptr)
        {
            return false;
        }

        auto newNode = std::make_shared<Node>(value);
        auto tmpNode = node->next_;
        node->next_ = newNode;
        newNode->next_ = tmpNode;

        return true;
    }

    /**
     * Inserts a new value after the first occurrence of another value.
     * @param after_value The value after which the new value should be inserted.
     * @param new_value The new value to insert.
     * @return true if the operation is successful, false otherwise.
    */ 
    bool push_after(const value_type& after_value, const value_type& new_value) {
        if (head_ == nullptr) 
        {
            return false;
        }
        return push_after(find(after_value), new_value);
    }

    /**
     * Adds a new value at the end of the list.
     * @param value The value to be inserted.
    */ 
    void push_back(const value_type& value) 
    {
        auto newNode = std::make_shared<Node>(value);
        if (head_ == nullptr)
        {
            head_ = tail_ = newNode;
        }
        else
        {
            tail_->next_ = newNode;
            tail_ = newNode;
        }
    }

    /**
     * Inserts a new value at the beginning of the list.
     * @param value The value to be inserted.
    */ 
    void push_front(const value_type& value) 
    {
        auto newNode = std::make_shared<Node>(value);
        if (head_ == nullptr)
        {
            head_ = tail_ = newNode;
        }
        else
        {
            newNode->next_ = head_;
            head_ = newNode;
        }
    }

    /**
     * Prints the entire list to standard output.
    */ 
    void print()
    {
        auto cur = head_;
        while (cur != nullptr)
        {
            std::cout << cur->value_ << "->";
            cur = cur->next_;
        }
        std::cout << "nullptr" << std::endl;
    }

    /**
     * Removes the specified node from the list.
     * @param node The node to be removed.
     * @return true if the operation is successful, false otherwise.
    */ 
    bool remove(std::shared_ptr<Node> node) 
    {
        if (node == nullptr || head_ == nullptr) 
        {
            return false;
        }

        if (head_ == node)
        {
            if (head_ == tail_)
            {
                head_ = tail_ = nullptr;
            }
            else
            {
                head_ = head_->next_;
            }
            return true;
        }

        auto cur = head_;
        while (cur->next_ != nullptr && cur->next_ != node) 
        {
            cur = cur->next_;
        }

        if (cur->next_ == node) 
        {
            cur->next_ = node->next_;
            if (tail_ == node)
            {
                tail_ = cur; 
            }
            return true;
        }

        return false; 
    }

    /**
     * Removes the first occurrence of a value from the list.
     * @param value The value to be removed.
     * @return true if the operation is successful, false otherwise.
    */ 
    bool remove(const value_type& value)
    {
        return remove(find(value));
    }
    
    /**
     * Removes all occurrences of a value from the list.
     * @param value The value to be removed.
    */ 
    void remove_all(const value_type& value)
    {
        while(remove(value) != false) {}
    }

    /**
     * Removes the last node from the list.
     * @return true if the operation is successful, false otherwise.
    */ 
    bool remove_back()
    {
        if (head_ == nullptr) 
        {
            return false;
        }

        if (head_ == tail_) 
        {
            head_ = tail_ = nullptr;
            return true;
        }

        auto cur = head_;
        while (cur->next_ && cur->next_->next_ != nullptr) 
        {
            cur = cur->next_;
        }

        cur->next_ = nullptr;
        tail_ = cur; 
        return true;
    }

    /**
     * Removes the head node from the list.
     * @return true if the operation is successful, false otherwise.
    */ 
    bool remove_front()
    {
        if(head_ == nullptr)
        {
            return false;
        }
        if(head_ == tail_)
        {
            head_ = tail_ = nullptr;
        }
        else
        {
            head_ = head_->next_;
        }
        return true;
    }
};
#endif //LIST_WITH_TAIL
