#include <memory>
#include <sstream>
#include <vector>

// Forward declaration
template <typename NodeDataType, typename WeightType>
struct GraphNode;

template <typename NodeDataType, typename WeightType>
struct Edge
{
    std::shared_ptr<GraphNode<NodeDataType, WeightType>> target;
    WeightType weight;

    Edge(std::shared_ptr<GraphNode<NodeDataType, WeightType>> t, WeightType w) : target{t}, weight{w} {}
};

template <typename NodeDataType, typename WeightType>
struct GraphNode
{
    NodeDataType data{};
    std::vector<Edge<NodeDataType, WeightType>> edges{};  // outgoing edges

    GraphNode(const NodeDataType& d) : data{d}, edges{} {}
};

template <typename NodeDataType, typename WeightType>
struct Graph
{
    using NodePtr = std::shared_ptr<GraphNode<NodeDataType, WeightType>>;

    std::vector<NodePtr> nodes{};

    NodePtr AddNode(const NodeDataType& data)
    {
        auto node = std::make_shared<GraphNode<NodeDataType, WeightType>>(data);
        nodes.push_back(node);
        return node;
    }

    void AddEdge(NodePtr from, NodePtr to, WeightType weight) { from->edges.emplace_back(to, weight); }

    void AddBidirectionalEdge(NodePtr from, NodePtr to, WeightType weight)
    {
        from->edges.emplace_back(to, weight);
        to->edges.emplace_back(from, weight);
    }
};

template <typename T>
struct DoubleLinkedListNode
{
    T value{};

    std::shared_ptr<DoubleLinkedListNode<T>> next{};
    std::shared_ptr<DoubleLinkedListNode<T>> previous{};

    DoubleLinkedListNode(const T& val) : value{val}, next{}, previous{} {}
};

template <typename T>
struct DoubleLinkedList
{
    using NodePtr = std::shared_ptr<DoubleLinkedListNode<T>>;

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
        NodePtr new_node = std::make_shared<DoubleLinkedListNode<T>>(val);
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
        NodePtr new_node = std::make_shared<DoubleLinkedListNode<T>>(val);
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
