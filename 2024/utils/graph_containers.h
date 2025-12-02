#include <memory>
#include <sstream>

template <typename T>
struct Node
{
    T value{};

    std::shared_ptr<Node> next{};
    std::shared_ptr<Node> previous{};

    Node(const T& val) : value{val}, next{}, previous{} {}
};

template <typename T>
struct DoubleLinkedList
{
    using NodePtr = std::shared_ptr<Node<T>>;

    NodePtr head{};
    NodePtr tail{};

    bool IsEmpty() { return !head; }

    void InitList(const NodePtr node_ptr)
    {
        head = node_ptr;
        tail = node_ptr;
    }

    void Append(const T& val)
    {
        NodePtr new_node = std::make_shared<Node<T>>(val);
        if (IsEmpty())
        {
            InitList(new_node);
            return;
        }
        tail->next = new_node;
        new_node->previous = tail;
        tail = new_node;
    }

    void Prepend(const T& val)
    {
        NodePtr new_node = std::make_shared<Node<T>>(val);
        if (IsEmpty())
        {
            InitList(new_node);
            return;
        }
        head->prev = new_node;
        new_node->next = head;
        head = new_node;
    }

    bool IsNodeInList(const NodePtr node)
    {
        auto node_it = head;
        while (node_it && node != node_it)
        {
            node_it = node_it->next;
        }
        return node_it != nullptr;
    }

    bool Remove(const NodePtr node)
    {
        if (!node || IsEmpty())
        {
            return false;
        }
        if (node == head)
        {
            head = head->next;
            head->previous = {};
        }
        else if (node == tail)
        {
            tail = tail->previous;
            tail->next = {};
        }
        else if (IsNodeInList(node))
        {
            node->previous->next = node->next;
            node->next->previous = node->previous;
        }
        else
        {
            return false;
        }
        return true;
    }

    std::ostringstream StreamList()
    {
        std::ostringstream stream;
        auto node_it = head;
        while (node_it)
        {
            stream << node_it->value;
            if (node_it->next)
            {
                stream << " ";
            }
            node_it = node_it->next;
        }
        return stream;
    }
};
