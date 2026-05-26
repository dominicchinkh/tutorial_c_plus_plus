#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <optional>

/*
 *  int main(int argc, char *argv[]) 
 *  {
 *      AvlTree<int> a;
 *  
 *      std::cout << "AVL tree is " << (a.empty()? "empty" : "not empty") << std::endl;
 *  
 *      a.insert(10);
 *      a.insert(11);
 *      a.insert(12);
 *      a.insert(13);
 *      a.insert(14);
 *      a.insert(15);
 *      a.insert(16);
 *      a.insert(8);
 *      a.insert(9);
 *  
 *      std::cout << "AVL tree is " << (a.empty()? "empty" : "not empty") << std::endl;
 *  
 *      a.printTree(); // expect 13, 11, 9, 8, 10, 12, 15, 14, 16
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
 *      a.remove(11);
 *      a.printTree(); // expect 13, 9, 8, 12, 10, 15, 14, 16
 *  
 *      AvlTree<int> b{a};
 *      b.printTree(); // expect 13, 9, 8, 12, 10, 15, 14, 16
 *       a.makeEmpty();
 *      std::cout << "AVL tree is " << (a.empty()? "empty" : "not empty") << std::endl;
 *  
 *      return 0;
 *  }
 */
template <typename Comparable>
class AvlTree
{
    public:
        static constexpr int ALLOWED_IMBALANCE = 1;

    public:
        AvlTree(): root {nullptr} {}

        AvlTree(const AvlTree & rhs): root {nullptr}
        {
            root = clone(rhs.root);
        }

        ~AvlTree()
        {
            makeEmpty();
        }

        std::optional<Comparable> findMin() const
        {
            AvlNode *n = findMin(root);

            if (n == nullptr) {
                return std::nullopt;
            }
            return n->element;
        }

        std::optional<Comparable> findMax() const
        {
            AvlNode *n = findMax(root);

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

        void printTree() const
        {
            printTree(root, std::cout);
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

    private:
        struct AvlNode
        {
            Comparable element;
            AvlNode* left;
            AvlNode* right;
            int height;

            AvlNode(const Comparable & ele, AvlNode *lt, AvlNode* rt, int h = 0):
                element {ele}, left {lt}, right {rt}, height {h} 
            {
            }

            AvlNode(Comparable && ele, AvlNode *lt, AvlNode *rt, int h = 0):
                element {std::move(ele)}, left {lt}, right {rt}, height {h} 
            {
            }
        };

        AvlNode *root;

        int height(AvlNode *t) const
        {
            return (t == nullptr)? -1 : t->height;
        }

        void insert(const Comparable & x, AvlNode * & t)
        {
            if (t == nullptr) {
                t = new AvlNode{x, nullptr, nullptr, 0};
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

            balance(t);
        }

        void insert(Comparable && x, AvlNode * & t)
        {
            if (t == nullptr) {
                t = new AvlNode{std::move(x), nullptr, nullptr, 0};
            }
            else if (x < t->element) {
                insert(std::move(x), t->left);
            }
            else if (x > t->element) {
                insert(std::move(x), t->right);
            }
            else {
                // Duplicate - do nothing
            }

            balance(t);
        }

        void remove(const Comparable & x, AvlNode * & t)
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
                
                AvlNode *oldNode = t;
                t = (t->left != nullptr)? t->left: t->right;
                delete oldNode;
            }

            balance(t);
         }

        void balance(AvlNode * & t)
        {
            if (t == nullptr) {
                return;
            }

            if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE) {
                if (height(t->left->left) >= height(t->left->right)) {
                    rotateWithLeftChild(t);
                }
                else {
                    doubleRotateWithLeftChild(t);
                }
            }
            else if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE) {
                if (height(t->right->right) >= height(t->right->left)) {
                    rotateWithRightChild(t);
                }
                else {
                    doubleRotateWithRightChild(t);
                }
            }

            t->height = std::max(height(t->left), height(t->right)) + 1;
        }

        void rotateWithLeftChild(AvlNode * & k2) 
        {
            AvlNode *k1 = k2->left;
            k2->left = k1->right;
            k1->right = k2;

            k2->height = std::max(height(k2->left), height(k2->right)) + 1;
            k1->height = std::max(height(k1->left), k2->height) + 1;

            k2 = k1;
        }

        void rotateWithRightChild(AvlNode * & k2) 
        {
            AvlNode *k1 = k2->right;
            k2->right = k1->left;
            k1->left = k2;

            k2->height = std::max(height(k2->left), height(k2->right)) + 1;
            k1->height = std::max(k2->height, height(k1->right));

            k2 = k1;
        }

        void doubleRotateWithLeftChild(AvlNode * & k3) 
        {
            rotateWithRightChild(k3->left);
            rotateWithLeftChild(k3);
        }

        void doubleRotateWithRightChild(AvlNode * & k3) 
        {
            rotateWithLeftChild(k3->right);
            rotateWithRightChild(k3);
        }

        AvlNode * findMin(AvlNode *t) const
        {
            if (t == nullptr) {
                return nullptr;
            }
            if (t->left == nullptr) {
                return t;
            }

            return findMin(t->left);
        }

        AvlNode * findMax(AvlNode *t) const
        {
            if (t == nullptr) {
                return nullptr;
            }
            if (t->right == nullptr) {
                return t;
            }

            return findMax(t->right);
        }

        bool contains(const Comparable & x, AvlNode *t) const
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

        void makeEmpty(AvlNode * & t)
        {
            if (t != nullptr) {
                makeEmpty(t->left);
                makeEmpty(t->right);
                delete t;
                t = nullptr;
            }

            t = nullptr;
        }

        void printTree(AvlNode *t, std::ostream & out = std::cout) const
        {
            if (t != nullptr) {
                out << t->element << std::endl;
                printTree(t->left, out);
                printTree(t->right, out);
            }
        }

        AvlNode * clone(AvlNode *t) const
        {
            if (t == nullptr) {
                return nullptr;
            }
            else {
                return new AvlNode{ t->element, clone(t->left), clone(t->right), t->height };
            }
        }
};

#endif
