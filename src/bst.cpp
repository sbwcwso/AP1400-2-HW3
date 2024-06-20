#include "bst.h"

BST::Node::Node(int value, Node *left, Node *right) : value(value), left{left}, right{right} {}

BST::Node::Node() : value(0), left(nullptr), right(nullptr) {}

BST::Node::Node(const Node &node) : value(node.value), left{node.left}, right{node.right} {}

std::ostream &operator<<(std::ostream &os, const BST::Node &node)
{
    os << std::left;
    os << std::setw(18) << &node << "==> value:" << std::setw(10) << node.value;
    os << "left:" << std::setw(17) << node.left;
    os << "right:" << node.right << std::endl;
    return os;
}

std::ostream &operator<<(std::ostream &os, const BST &bst)
{
    os << "********************************************************************************" << std::endl;
    auto lambda = [&os](BST::Node *&node) -> void
    {
        os << *node;
    };
    bst.bfs(lambda);
    os << "binary search tree size: " << bst.length() << std::endl;
    os << "********************************************************************************" << std::endl;

    return os;
}

bool operator<(const BST::Node &node, const int rhs)
{
    return node.value < rhs;
}

bool operator<=(const BST::Node &node, const int rhs)
{
    return node.value <= rhs;
}

bool operator==(const BST::Node &node, const int rhs)
{
    return node.value == rhs;
}

bool operator>(const BST::Node &node, const int rhs)
{
    return node.value > rhs;
}

bool operator>=(const BST::Node &node, const int rhs)
{
    return node.value >= rhs;
}

bool operator!=(const BST::Node &node, const int rhs)
{
    return node.value != rhs;
}

bool operator<(const int lhs, const BST::Node &node)
{
    return node > lhs;
}

bool operator<=(const int lhs, const BST::Node &node)
{
    return node >= lhs;
}

bool operator==(const int lhs, const BST::Node &node)
{
    return node == lhs;
}

bool operator>(const int lhs, const BST::Node &node)
{
    return node < lhs;
}

bool operator>=(const int lhs, const BST::Node &node)
{
    return node <= lhs;
}

bool operator!=(const int lhs, const BST::Node &node)
{
    return node != lhs;
}

BST::Node *&BST::get_root()
{
    return root;
}

void BST::bfs(std::function<void(BST::Node *&node)> func) const
{
    if (root == nullptr)
        return;
    std::queue<BST::Node *> q;
    q.push(root);
    while (!q.empty())
    {
        BST::Node *&node = q.front();
        q.pop();
        if (node == nullptr)
            continue;
        func(node);
        q.push(node->left);
        q.push(node->right);
    }
}

size_t bst_length_helper(BST::Node *node)
{
    if (node == nullptr)
        return 0;
    return 1 + bst_length_helper(node->left) + bst_length_helper(node->right);
}

size_t BST::length() const
{
    return bst_length_helper(root);
}

bool BST::add_node(int value)
{
    if (root == nullptr)
    {
        root = new BST::Node(value, nullptr, nullptr);
        return true;
    }
    Node *parent = root;
    while (true)
    {
        if (*parent == value)
            return false;
        if (value < *parent)
        {
            if (parent->left == nullptr)
            {
                parent->left = new BST::Node(value, nullptr, nullptr);
                return true;
            }
            parent = parent->left;
        }
        else
        {
            if (parent->right == nullptr)
            {
                parent->right = new BST::Node(value, nullptr, nullptr);
                return true;
            }
            parent = parent->right;
        }
    }
}

BST::Node **BST::find_node(int value)
{
    BST::Node **node_ptr = &root;
    while (*node_ptr != nullptr)
    {
        BST::Node *node = *node_ptr;
        if (node->value == value)
            return node_ptr;
        if (value > node->value)
            node_ptr = &(node->right);
        else
            node_ptr = &(node->left);
    }
    return nullptr;
}

void find_parent_helper(BST::Node *&node, BST::Node **&parent, int value)
{
    if (node == nullptr)
    {
        parent = nullptr;
        return;
    }

    if (node->value == value)
        return;

    parent = &node;
    if (value > node->value)
        return find_parent_helper(node->right, parent, value);
    return find_parent_helper(node->left, parent, value);
}

BST::Node **BST::find_parent(int value)
{

    BST::Node **parent_node_ptr = nullptr;
    BST::Node **node_ptr = &root;
    while (*node_ptr != nullptr)
    {
        BST::Node *node = *node_ptr;
        if (node->value == value)
            return parent_node_ptr;
        parent_node_ptr = node_ptr;
        if (value > node->value)
            node_ptr = &(node->right);
        else
            node_ptr = &(node->left);
    }
    return nullptr;
}

BST::Node **BST::find_predecessor(int value)
{

    BST::Node **pred_parent_node_ptr = nullptr;
    BST::Node **node_ptr = &root;

    while (*node_ptr != nullptr)
    {
        BST::Node *node = *node_ptr;
        if (node->value == value)
            break;
        if (value > node->value)
        {
            pred_parent_node_ptr = node_ptr;
            node_ptr = &(node->right);
        }
        else
            node_ptr = &(node->left);
    }

    if (*node_ptr == nullptr)
        return nullptr;

    BST::Node *node = *node_ptr;
    if (node->left != nullptr)
    {
        BST::Node **predecessor = &(node->left);
        while ((*predecessor)->right != nullptr)
            predecessor = &((*predecessor)->right);
        return predecessor;
    }

    return pred_parent_node_ptr;
}

bool BST::delete_node(int value)
{
    // find node and parent
    BST::Node *parent = nullptr;
    BST::Node *node = root;

    while (node != nullptr)
    {
        if (node->value == value)
            break;
        parent = node;
        if (value > node->value)
            node = node->right;
        else
            node = node->left;
    }

    if (node == nullptr)
        return false;

    // update parent with the given node;
    auto update_parent = [parent, node, this](BST::Node *new_node) mutable
    {
        if (parent == nullptr)
            root = new_node;
        else if (parent->left == node)
            parent->left = new_node;
        else
            parent->right = new_node;
    };

    // node has no child
    if (node->left == nullptr && node->right == nullptr)
        update_parent(nullptr);
    // node has only one child
    else if (node->left == nullptr)
        update_parent(node->right);
    else if (node->right == nullptr)
        update_parent(node->left);
    // node has two child
    else
    {
        BST::Node **predcessor_ptr = find_predecessor(value);
        BST::Node *predcessor = *predcessor_ptr;
        BST::Node **predcessor_parent_ptr = find_parent(predcessor->value);
        BST::Node *predcessor_parent = *predcessor_parent_ptr;
        if (predcessor_parent->left == predcessor)
            predcessor_parent->left = predcessor->left;
        else
            predcessor_parent->right = predcessor->left;
        predcessor->left = node->left;
        predcessor->right = node->right;

        update_parent(predcessor);
    }

    delete node;
    return true;
}

void copy_tree_helper(BST::Node &src, BST::Node &dest)
{
    dest.value = src.value;

    if (src.left != nullptr)
    {
        dest.left = new BST::Node();
        copy_tree_helper(*(src.left), *(dest.left));
    }

    if (src.right != nullptr)
    {
        dest.right = new BST::Node();
        copy_tree_helper(*(src.right), *(dest.right));
    }
}

BST::BST()
{
    root = nullptr;
}

BST::BST(BST &other)
{
    if (other.root == nullptr)
    {
        this->root = nullptr;
        return;
    }

    root = new BST::Node();
    copy_tree_helper(*(other.root), *root);
}

BST::BST(BST &&other) noexcept
{
    root = other.root;
    other.root = nullptr;
}

BST::BST(std::initializer_list<int> values)
{
    root = nullptr;
    for (int value : values)
        add_node(value);
}

void delete_all_nodes(BST &bst)
{
    std::vector<BST::Node *> nodes;
    bst.bfs([&nodes](BST::Node *&node)
            { nodes.push_back(node); });
    for (auto &node : nodes)
        delete node;
}

BST &BST::operator=(BST &other)
{
    if (this == &other)
        return *this;
    delete_all_nodes(*this);
    root = new BST::Node();
    copy_tree_helper(*(other.root), *root);
    return *this;
}

BST &BST::operator=(BST &&other) noexcept
{
    if (this == &other)
        return *this;
    delete_all_nodes(*this);
    root = other.root;
    other.root = nullptr;
    return *this;
}

BST &BST::operator++()
{
    // ++bst
    bfs([](BST::Node *&node)
        { node->value += 1; });
    return *this;
}

BST BST::operator++(int value)
{
    // bst++
    BST ret = BST{*this};
    bfs([](BST::Node *&node)
        { node->value += 1; });
    return ret;
}

BST::~BST() noexcept
{
    delete_all_nodes(*this);
}
