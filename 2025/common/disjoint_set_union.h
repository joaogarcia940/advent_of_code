#include <numeric>
#include <vector>

template <typename T>
class DSU
{
  public:
    DSU(const std::size_t N) : parent_(N), size_(N, 1) { std::iota(parent_.begin(), parent_.end(), 0); }

    // preconditions:
    // you can't have loops in the parent tree
    // parent[0] = 1;
    // parent[1] = 0;    // <- creates a cycle
    // -> will recurse infinitely
    T FindParentAndCompress(T x)
    {
        if (parent_[x] != x)  // find the leader (leader is its own parent)
        {
            // two things happening here:
            // 1 - enable recursive loop to find the leader
            // 2 - overwriting the current parent to the leader (compression)
            parent_[x] = FindParentAndCompress(parent_[x]);
        }
        return parent_[x];  // return compressed parent
    }

    bool Unite(T a, T b)
    {
        a = FindParentAndCompress(a);
        b = FindParentAndCompress(b);
        if (a == b)  // already united
        {
            return false;
        }

        // chosen of the two parent will be the biggest
        if (size_[a] < size_[b])
        {
            std::swap(a, b);
        }
        parent_[b] = a;
        size_[a] += size_[b];
        // no need to update size[b] since sizes are only important for leaders
        return true;
    }

    const std::vector<T>& ViewParents() { return parent_; }

    const std::vector<T>& ViewSizes() { return size_; }

  private:
    std::vector<T> parent_;
    std::vector<T> size_;
};