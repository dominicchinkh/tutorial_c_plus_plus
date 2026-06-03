#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <iostream>
#include <optional>

/*
 *  int main(int argc, char *argv[]) 
 *  {
 *      BinarySearchTree<int> a;
 *  
 *      std::cout << "Binary search tree is " << (a.empty()? "empty" : "not empty") << std::endl;
 *  
 *      a.insert(4);
 *      a.insert(2);
 *      a.insert(6);
 *      a.insert(7);
 *      a.insert(1);
 *  
 *      std::cout << "Binary search tree is " << (a.empty()? "empty" : "not empty") << std::endl;
 *  
 *      a.printTree(); // expect 4, 2, 1, 6, 7
 *  
 *      if (auto minVal = a.findMin()) {
 *          std::cout << "The minimum value is: " << *minVal << std::endl;
 *      } else {
 *          std::cout << "The structure is empty, no minimum found." << std::endl;
 *      }
 *  
 *      if (auto maxVal = a.findMax()) {
 *          std::cout << "The maximum value is: " << *maxVal << std::endl;
 *      } else {
 *          std::cout << "The structure is empty, no maximum found." << std::endl;
 *      }
 *  
 *      a.remove(6);
 *      a.printTree(); // expect 4, 2, 1, 7
 *  
 *      BinarySearchTree<int> b{a};
 *      b.printTree(); // expect 4, 2, 1, 7
 * 
 *      a.makeEmpty();
 *      std::cout << "Binary search tree is " << (a.empty()? "empty" : "not empty") << std::endl;
 *  
 *      return 0;
 *  }
*/
template <typename Comparable>
class BinarySearchTree
{
    public:
        BinarySearchTree(): root {nullptr} {}
        
        BinarySearchTree(const BinarySearchTree & rhs): root {nullptr}
        {
            root = clone(rhs.root);
        }

        // BinarySearchTree(BinarySearchTree && rhs);

        ~BinarySearchTree()
        {
            makeEmpty();
        }

        std::optional<Comparable> findMin() const
        {
            BinaryNode * n = findMin(root);

            if (n == nullptr) {
                return std::nullopt;
            }  
            return n->element;
        }

        std::optional<Comparable> findMax() const
        {
            BinaryNode * n = findMax(root);

            if (n == nullptr) {
                return std::nullopt;
            }  
            return n->element;
        }

        bool contains(const Comparable & x) const
        {
            return contains(x, root);
        }

        bool empty() const
        {
            return root == nullptr;
        }

        void makeEmpty() 
        {
            makeEmpty(root);
        }

        void insert(const Comparable & x)
        {
            insert(x, root);
        }

        void insert(Comparable && x)
        {
            insert(std::move(x), root);
        }

        void remove(const Comparable & x)
        {
            remove(x, root);
        }

        void printTree(std::ostream & out = std::cout) const
        {
            printTree(root, std::cout);
        }

    private:
        struct BinaryNode
        {
            Comparable element;
            BinaryNode *left;
            BinaryNode *right;

            BinaryNode(const Comparable & theElement, BinaryNode* lt, BinaryNode* rt):
                element {theElement}, left {lt}, right {rt} {}

            BinaryNode(Comparable && theElement, BinaryNode* lt, BinaryNode* rt):
                element {std::move(theElement)}, left {lt}, right {rt} {}
        };

        BinaryNode *root;

        void insert(const Comparable & x, BinaryNode * & t)
        {
            if (t == nullptr) {
                t = new BinaryNode{x, nullptr, nullptr};
            }
            else if (x < t->element) {
                insert(x, t->left);
            }
            else if (x > t->element) {
                insert(x, t->right);
            }
            else {
                // Duplicate - do nothing
            }
        }

        void insert(Comparable && x, BinaryNode * & t)
        {
            if (t == nullptr) {
                t = new BinaryNode{std::move(x), nullptr, nullptr};
            }
            else if (x < t->element) {
                insert(std::move(x), t->left);
            }
            else if (x > t->element) {
                insert(std::move(x), t->right);
            }
            else {
                // Duplicate - Do nothing
            }
        }

        void remove(const Comparable & x, BinaryNode * & t)
        {
            if (t == nullptr) {
                return;
            }
            else if (x < t->element) {
                remove(x, t->left);
            }
            else if (x > t->element) {
                remove(x, t->right);
            }
            else if (t->left != nullptr && t->right != nullptr) {

                // Replace the data of this node with the smallest data of the right subtree
                t->element = findMin(t->right)->element;
                remove(t->element, t->right);
            }
            else {
                // 1. If the node is a leaf, it can be deleted immediately
                // 2. If the node has one child, the node can be deleted after its parent adjusts a link to bypass the node
                
                BinaryNode *oldNode = t;
                t = (t->left != nullptr)? t->left: t->right;
                delete oldNode;
            }
        }
        
        BinaryNode * findMin(BinaryNode *t) const
        {
            if (t == nullptr) {
                return nullptr;
            }
            if (t->left == nullptr) {
                return t;
            }

            return findMin(t->left);
        }

        BinaryNode * findMax(BinaryNode *t) const
        {
            if (t == nullptr) {
                return nullptr;
            }
            if (t->right == nullptr) {
                return t;
            }

            return findMax(t->right);
        }

        /*
        BinaryNode * findMin(BinaryNode *t) const
        {
            if (t != nullptr) {
                while (t != nullptr) {
                    t = t->left;
                }
            }

            return t;
        }

        BinaryNode * findMax(BinaryNode *t) const
        {
            if (t != nullptr) {
                while (t != nullptr) {
                    t = t->right;
                }
            }

            return t;
        }
        */

        bool contains(const Comparable & x, BinaryNode * t) const
        {
            if (t == nullptr) {
                return false;
            }

            if (x < t->element) {
                return contains(x, t->left);
            }
            else if (x > t->element) {
                return contains(x, t->right);
            }
            else {
                return true;
            }
        }

        void makeEmpty(BinaryNode * & t)
        {
            if (t != nullptr) {
                makeEmpty(t->left);
                makeEmpty(t->right);
                delete t;
                t = nullptr;
            }
        }

        void printTree(BinaryNode *t, std::ostream & out = std::cout) const
        {
            if (t != nullptr) {
                out << t->element << std::endl;
                printTree(t->left);
                printTree(t->right);
            }
        }

        BinaryNode * clone(BinaryNode *t) const
        {
            if (t == nullptr) {
                return nullptr;
            }
            else {
                return new BinaryNode{t->element, clone(t->left), clone(t->right)};
            }
        }
};

#endif
