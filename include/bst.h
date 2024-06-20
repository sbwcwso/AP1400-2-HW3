#ifndef BST_H
#define BST_H

#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <queue>
#include <stack>

class BST
{
public:
    class Node
    {
    public:
        Node(int value, Node *left, Node *right);
        Node();
        Node(const Node &node);

        friend std::ostream &operator<<(std::ostream &os, const Node &node);
        friend bool operator<(const Node &node, const int rhs);
        friend bool operator<=(const Node &node, const int rhs);
        friend bool operator==(const Node &node, const int rhs);
        friend bool operator>(const Node &node, const int rhs);
        friend bool operator>=(const Node &node, const int rhs);
        friend bool operator!=(const Node &node, const int rhs);

        friend bool operator<(const int lhs, const Node &node);
        friend bool operator<=(const int lhs, const Node &node);
        friend bool operator==(const int lhs, const Node &node);
        friend bool operator>(const int lhs, const Node &node);
        friend bool operator>=(const int lhs, const Node &node);
        friend bool operator!=(const int lhs, const Node &node);

        int value;
        Node *left;
        Node *right;
    };

    Node *&get_root();
    void bfs(std::function<void(Node *&node)> func) const;
    size_t length() const;
    bool add_node(int value);
    Node **find_node(int value);
    Node **find_parent(int value);
    Node **find_predecessor(int value);
    bool delete_node(int value);

    friend std::ostream &operator<<(std::ostream &os, const BST &bst);
    BST &operator++();   // ++bst
    BST operator++(int); // bst++
    BST();
    BST(std::initializer_list<int> values);
    BST(BST &other);                      // copy
    BST(BST &&other) noexcept;            // move
    BST &operator=(BST &other);           // copy
    BST &operator=(BST &&other) noexcept; // move
    ~BST() noexcept;

private:
    Node *root;
};

#endif // BST_H