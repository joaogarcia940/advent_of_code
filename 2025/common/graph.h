#include <memory>
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

    NodePtr FindNode(const NodeDataType& data)
    {
        for (const auto& node : nodes)
        {
            if (node->data == data)
            {
                return node;
            }
        }
        return nullptr;
    }
};