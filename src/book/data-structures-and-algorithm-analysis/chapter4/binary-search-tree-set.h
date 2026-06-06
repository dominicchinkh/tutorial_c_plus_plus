#ifndef BINARY_SEARCH_TREE_SET_H
#define BINARY_SEARCH_TREE_SET_H

#include <algorithm>
#include <optional>

/*
 *  int main(int argc, char *argv[]) 
 *  {
 *      BinarySearchTreeSet<int> a;
 *  
 *      std::cout << "Binary search tree is " << (a.empty()? "empty" : "not empty") << std::endl;
 *  
 *      a.insert(4);
 *      a.insert(2);
 *      a.insert(6);
 *      a.insert(7);
 *      a.insert(1);
 *  
 *      std::cout << "After insert, binary search tree is " << (a.empty()? "empty" : "not empty") << std::endl;
 *  
 *      a.printTree(); // expect 1, 2, 4, 6, 7
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
 *  
 *      a.printTree(); // expect 1, 2, 4, 7
 *  
 *      auto itr = a.begin();
 *  
 *      int i = 0;
 *  
 *      cout << "a elements: " << endl;
 *      for (auto itr = a.begin(); itr != a.end(); ++itr) {
 *          cout << *(itr) << endl;
 *  
 *          if(++i > 5) {
 *              break;
 *          }
 *      }
 *  
 *      BinarySearchTreeSet<int> b{a};
 *  
 *      cout << "The value of `b`: ";
 *      b.printTree(); // expect 1, 2, 4, 7
 *      
 *      a.makeEmpty();
 *      std::cout << "After `makeEmpty`, Binary search tree is " << (a.empty()? "empty" : "not empty") << std::endl;
 *  
 *      return 0;
 *  }
 */
template <typename Comparable>
class BinarySearchTreeSet
{
    private:
        struct BinaryNode;

    public:
        // Forward declare the nested iterator classes
        class const_iterator;
        class iterator;

        class const_iterator
        {
            public:
                // Add these lines so std::find works:
                using iterator_category = std::random_access_iterator_tag;
                using value_type        = Comparable;
                using difference_type   = std::ptrdiff_t;
                using pointer           = const Comparable*;
                using reference         = const Comparable&;

                const_iterator(): current {nullptr}, theSet {nullptr} {}

                const Comparable & operator*() const
                {
                    assertIsValid();
                    if (current == theSet->end_ptr()) {
                        throw std::out_of_range("Attempted to dereference an end() iterator");
                    }

                    return *(current->element);
                }

                // The arrow operator -> in C++ has a very specific rule: it must return either 
                // a raw pointer or another object that itself overloads ->
                const Comparable * operator->() const
                {
                    assertIsValid();
                    if (current == theSet->end_ptr()) {
                        throw std::out_of_range("Attempted to dereference an end() iterator");
                    }

                    return &(*(current->element));
                }

                const_iterator & operator++()
                {
                    assertIsValid();
                    if (current == theSet->end_ptr()) {
                        throw std::out_of_range("Attempted to increment pass end() iterator");
                    }

                    current = theSet->next(current);
                    return *this;
                }

                const_iterator operator++(int)
                {
                    const_iterator old = *this;
                    ++(*this);
                    return old;
                }

                const_iterator & operator--()
                {
                    assertIsValid();
                    if (current == theSet->begin_ptr()) {
                        throw std::out_of_range("Attempted to decrement past an begin() iterator");
                    }

                    current = theSet->prev(current);
                    return *this;
                }

                const_iterator operator--(int)
                {
                    const_iterator old = *this;
                    --(*this);
                    return old;
                }

                // The C++ standard algorithm library expects iterator equality operators to work even if the 
                // iterators themselves are marked const. You must append const to your signature.

                bool operator==(const const_iterator & rhs) const
                {
                    return current == rhs.current;
                }

                bool operator!=(const const_iterator & rhs) const
                {
                    return !(*this == rhs);
                }

            private:
                // Can modify current itself (meaning you can change what it points to), but you cannot modify the 
                // data inside the object it points to.
                const BinaryNode * current;
                const BinarySearchTreeSet * theSet;

                const_iterator(const BinaryNode * x, const BinarySearchTreeSet * s): current {x}, theSet {s} {}

                bool assertIsValid() const
                {
                    if (current == nullptr || theSet == nullptr) {
                        throw std::runtime_error("Attempted to use an uninitialized iterator");
                    }

                    return true;
                }

                friend class BinarySearchTreeSet;
        };

        class iterator: public const_iterator
        {
            public:
                iterator() {}

                /*
                 * When a C++ compiler processes a template class, it does so in two distinct phases:
                 *
                 * Phase 1 (Syntax Check): 
                 *   The compiler looks at the template before it knows what Comparable is (e.g., when it 
                 *   compiles the general BinarySearchTreeSet code). It looks for variables and functions 
                 *   that don't depend on the template parameters.
                 * 
                 * Phase 2 (Instantiation): 
                 *   The compiler looks at the code again when you actually create an object (like 
                 *   BinarySearchTreeSet<int>), substituting Comparable with int.
                 * 
                 * During Phase 1, the compiler looks at the iterator class and sees assertIsValid() 
                 * and current.
                 * 
                 * By default, C++ does not look inside dependent base classes during Phase 1. Because 
                 * the compiler doesn't know what const_iterator looks like yet, it assumes assertIsValid
                 * and current do not exist in the current scope. It will throw a compilation error: "use 
                 * of undeclared identifier" or "identifier not found".
                 * 
                 * Using this-> turns those variables and functions into dependent names, forcing the 
                 * compiler to delay its search until it fully resolves the base class const_iterator.
                 * 
                 */

                 Comparable & operator*()
                 {
                    this->assertIsValid();
                    if (this->current == this->theSet->end_ptr()) {
                        throw std::out_of_range("Attempted to dereference an end() iterator.");
                    }

                    // `current` is `const Comparable*` in the base class, iterator::operator->() will fail
                    // to compile here because it tries to cast away const-ness implicitly to return a
                    // mutable Comparable*. To fix this, you must use a const_cast

                    return const_cast<Comparable &>(*(this->current->element));
                 }

                /*
                 * C++ non-virtual function hiding.
                 *
                 * When the mutable iterator class overrides Comparable & operator*(), it creates a function 
                 * with the name operator*. In C++, if a derived class defines a function with the same 
                 * name as a function in the base class, it completely hides all versions of that function
                 * in the base class - even if their signatures (like const qualifiers) are completely 
                 * different.
                 * 
                 * If you try to use a const iterator in your code like this:
                 * 
                 *     void printFirst(const Vector<int>::iterator & itr) {
                 *         std::cout << *itr << std::endl; // ERROR! Compiler fails here.
                 *     }
                 * 
                 * The compiler looks at itr, sees it is a const iterator, and looks for a const version 
                 * of operator* inside the iterator class. It cannot find one there, and because the 
                 * non-const Comparable & operator*() hid the base class, it won't check const_iterator either.
                 * 
                 * Your compilation will fail with an error like:
                 *   error: passing 'const Vector<int>::iterator' as 'this' argument discards qualifiers
                 * 
                 * You can achieve the exact same result using a using declaration.
                 * 
                 *     using const_iterator::operator*;
                 * 
                 */

                 const Comparable & operator*() const
                 {
                    return const_iterator::operator*();
                 }

                 Comparable* operator->()
                 {
                    this->assertIsValid();
                    if (this->current == this->theSet->end_ptr()) {
                        throw std::out_of_range("Attempted to access member of an end() iterator.");
                    }

                    // `current` is `const Object*` in the base class, iterator::operator->() will fail
                    // to compile here because it tries to cast away const-ness implicitly to return a
                    // mutable Object*. To fix this, you must use a const_cast

                    return const_cast<Comparable *>(&(*(this->current->element)));
                 }

                 iterator operator++()
                 {
                    const_iterator::operator++();
                    return *this;
                 }

                 iterator operator++(int)
                 {
                    iterator old = *this;
                    const_iterator::operator++();
                    return old;
                 }

                 iterator operator--()
                 {
                    const_iterator::operator--();
                    return *this;
                 }

                 iterator operator--(int)
                 {
                    iterator old = *this;
                    const_iterator::operator--();
                    return old;
                 }

            private:
                iterator(BinaryNode * p, const BinarySearchTreeSet * s): const_iterator{p, s} {}

                friend class BinarySearchTreeSet<Comparable>;
        };

        BinarySearchTreeSet(): root {nullptr} 
        {
            root = new BinaryNode{nullptr, nullptr, nullptr};
        }

        // In C++, a copy constructor has a strict, language-defined signature. It must take its argument 
        // by reference
        BinarySearchTreeSet(const BinarySearchTreeSet & rhs): root {nullptr}
        {
            root = clone(rhs.root, nullptr);

            BinaryNode * maxNode = findMax(root);
            endNode = new BinaryNode{nullptr, nullptr, maxNode};
            maxNode->right = endNode;
        }

        ~BinarySearchTreeSet()
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
            insert(x, root, nullptr);
        }

        void insert(Comparable && x)
        {
            insert(std::move(x), root, nullptr);
        }

        void remove(const Comparable & x)
        {
            remove(x, root);
        }

        iterator begin()
        {
            return iterator{findMin(root), this};
        }

        const_iterator begin() const
        {
            return const_iterator{findMin(root), this};
        }

        iterator end()
        {
            return iterator{endNode, this};
        }

        const_iterator end() const
        {
            return const_iterator{endNode, this};
        }

        void printTree(std::ostream & out = std::cout) const
        {
            printTree(root, out);
            out << std::endl;
        }

    private:
        struct BinaryNode
        {
            std::optional<Comparable> element;
            BinaryNode *left;
            BinaryNode *right;
            BinaryNode *parent;

            BinaryNode(const Comparable & el, BinaryNode * lt, BinaryNode * rt, BinaryNode * pt):
                element {el}, left {lt}, right {rt}, parent {pt} 
            {
            }

            BinaryNode(Comparable && el, BinaryNode * lt, BinaryNode * rt, BinaryNode * pt):
                element {std::move(el)}, left {lt}, right {rt}, parent {pt}
            {
            }

            BinaryNode(BinaryNode * lt, BinaryNode * rt, BinaryNode* pt):
                element {std::nullopt}, left {lt}, right {rt}, parent {pt}
            {
            }
        };

        BinaryNode * root;
        BinaryNode * endNode;

        void insert(const Comparable & x, BinaryNode * & t, BinaryNode * pt)
        {
            if (t == nullptr) {
                t = new BinaryNode{x, nullptr, nullptr, pt};
            }
            else {
                if (t->element) {
                    if (x < *(t->element)) {
                        insert(std::move(x), t->left, t);
                    }
                    else if (x > *(t->element)) {
                        insert(std::move(x), t->right, t);
                    }
                    else {
                        // Duplicate - do nothing
                    }
                }
                else {
                    // t is currently the endNode sentinel (its element is std::nullopt)
                    
                    // Transform the current endNode into a normal data node
                    t->element = std::move(x);
                    
                    // Allocate a new empty endNode sentinel as its right child
                    t->right = new BinaryNode{nullptr, nullptr, t}; 
                    
                    // Update the class-wide endNode tracker
                    endNode = t->right;
                }
            }
        }

        void insert(Comparable && x, BinaryNode * & t, BinaryNode * pt)
        {
            if (t == nullptr) {
                t = new BinaryNode{std::move(x), nullptr, nullptr, pt};
            }
            else {
                if (t->element) {
                    if (x < *(t->element)) {
                        insert(std::move(x), t->left, t);
                    }
                    else if (x > *(t->element)) {
                        insert(std::move(x), t->right, t);
                    }
                    else {
                        // Duplicate - do nothing
                    }
                }
                else {
                    // t is currently the endNode sentinel (its element is std::nullopt)
                    
                    // Transform the current endNode into a normal data node
                    t->element = std::move(x);
                    
                    // Allocate a new empty endNode sentinel as its right child
                    t->right = new BinaryNode{nullptr, nullptr, t}; 
                    
                    // Update the class-wide endNode tracker
                    endNode = t->right;
                }
            }
        }

        void remove(const Comparable & x, BinaryNode * & t)
        {
            if (t == nullptr) {
                return;
            }
            
            if (t->element) {
                if (x < *(t->element)) {
                    remove(x, t->left);
                }
                else if (x > *(t->element)) {
                    remove(x, t->right);
                }
                else if (t->left != nullptr && t->right != nullptr && t->right != end_ptr()) {

                    // Replace the data of this node with the smallest data of the right subtree
                    t->element = findMin(t->right)->element;
                    remove(*(t->element), t->right);
                }
                else {
                    // 1. If the node is a leaf, it can be deleted immediately
                    // 2. If the node has one child, the node can be deleted after its parent adjusts a link to bypass the node

                    BinaryNode * oldNode = t;
                    t = (t->left != nullptr)? t->left: t->right;
                    delete oldNode;
                }
            }
        }

        std::optional<BinaryNode> find(const Comparable * x, BinaryNode * t) const
        {
            if (t == nullptr || t == end_ptr()) {
                return std::nullopt;
            }

            if (x < *(t->element)) {
                return find(x, t->left);
            }
            else if(x > *(t->element)) {
                return find(x, t->right);
            }
            else {
                return t;
            }
        }

        BinaryNode * findMin(BinaryNode * t) const
        {
            if (t == nullptr) {
                return nullptr;
            }

            if (t->left == nullptr) {
                return t;
            }

            return findMin(t->left);
        }

        BinaryNode * findMax(BinaryNode * t) const
        {
            if (t == nullptr) {
                return nullptr;
            }

            if (t->right == nullptr || t->right == end_ptr()) {
                return t;
            }

            return findMax(t->right);
        }

        bool contains(const Comparable & x, BinaryNode * t) const
        {
            if (t == nullptr) {
                return false;
            }

            if (t->element) {
                if (x < *(t->element)) {
                    return contains(t->left);
                }
                else if (x > *(t->element)) {
                    return contains(t->right);
                }
                else {
                    return true;
                }
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

        void printTree(BinaryNode * t, std::ostream & out) const
        {
            if (t != nullptr && t->element) {
                printTree(t->left, out);
                out << *(t->element) << " ";
                printTree(t->right, out);
            }
        }

        BinaryNode* clone(BinaryNode *t, BinaryNode *pt) const
        {
            if (t == nullptr) {
                return nullptr;
            }
            else {
                BinaryNode* n = (t->element)? 
                    new BinaryNode(*(t->element), nullptr, nullptr, pt):
                    new BinaryNode(nullptr, nullptr, pt);

                n->left  = clone(t->left, n);
                n->right = clone(t->right, n);
                return n;
            }
        }

        BinaryNode * begin_ptr() const
        {
            return findMin(root);
        }

        BinaryNode * end_ptr() const
        {
            return endNode;
        }

        BinaryNode * next(const BinaryNode * cur) const
        {
            if (cur->right != nullptr) {
                if (cur->right != end_ptr()) {
                    return findMin(cur->right);
                }
                else {
                    return end_ptr();
                }
            }

            /*
             * If you are larger than your parent (at the right side), you drag the pointer up 
             * until you find an ancestor that finally larger than you (at the left side)
             *
             *      20 (Root)
             *     /
             *    15  <-- We stop here (the final p)
             *   /
             *  10
             *   \
             *    12
             *      \
             *       14  <-- We start here (cur)
             */

            const BinaryNode* p = cur->parent;
            while (p != nullptr && cur == p->right) {
                cur = p;
                p = p->parent;
            }

            return const_cast<BinaryNode*>(p);
        }

        BinaryNode * prev(const BinaryNode * cur) const
        {
            if (cur->left != nullptr) {
                return findMax(cur->left);
            }

            /*
             * If you are smaller than your parent (at the left side), you drag the pointer up 
             * until you find an ancestor that finally smaller than you (at the right side)
             */

            const BinaryNode* p = cur->parent;
            while (p != nullptr && cur == p->left) {
                cur = p;
                p = p->parent;
            }

            return const_cast<BinaryNode*>(p);
        }
};

#endif
