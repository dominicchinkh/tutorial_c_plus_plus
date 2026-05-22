#ifndef LIST_H
#define LIST_H

/*
 *  int main(int argc, char *argv[]) 
 *  {
 *      List<int> a;
 *  
 *      cout << "Current size: " << a.size() << endl;
 *      cout << "Is empty? " << (a.empty() ? 'Y':'N') << endl;
 *     
 *      a.push_back(47);
 *      a.push_front(19);
 *      a.push_back(73);
 *  
 *      cout << "a new size: " << a.size() << endl;
 *      cout << "a first element: " << a.front() << endl;
 *      cout << "a last element: " << a.back() << endl;
 *      cout << "Is empty? " << (a.empty() ? 'Y':'N') << endl;
 *  
 *      a.pop_back();
 *      a.pop_front();
 *  
 *      cout << "a new size: " << a.size() << endl;
 *      cout << "a first element: " << a.front() << endl;
 *      cout << "a last element: " << a.back() << endl;
 *      cout << "Is empty? " << (a.empty() ? 'Y':'N') << endl;
 *      
 *      List<int>::iterator itr = a.begin();
 *  
 *      cout << "a elements: " << endl;
 *      for (auto itr = a.begin(); itr != a.end(); ++itr) {
 *          cout << *(itr) << endl;
 *      }
 *  
 *      return 0;
 *  }
 */
#include <stdexcept>

template <typename Object>
class List
{
    private:
        struct Node
        {
            Object data;
            Node *prev;
            Node *next;

            Node(const Object & d = Object(), Node *p = nullptr, Node *n = nullptr):
                data {d}, prev {p}, next {n}
            {
            }

            Node(Object && d, Node *p = nullptr, Node *n = nullptr):
                data {std::move(d)}, prev {p}, next {n}
            {
            }
        };

    public:
        class const_iterator
        {
            public:
                const_iterator(): theList {nullptr}, current {nullptr} {}

                const Object & operator*() const 
                {
                    return retrieve();
                }

                // Prefix Increment: ++it
                const_iterator & operator++()
                {
                    current = current->next;
                    return *this;
                }

                // Postfix Increment: it++
                //   The int parameter: The dummy int parameter isn't actually used. It is just a syntax 
                //   trick C++ uses to help the compiler distinguish the postfix signature from the prefix 
                //   signature.

                /* 
                 * I cannot safely return a reference (const_iterator &) from the postfix operator
                 *
                 * Because `old` is a local variable created inside the function, it lives on the stack. 
                 * The moment the function hits that closing curly brace, `old` is destroyed.
                 *
                 * If you return a reference (&), you are giving the caller a tracking address to a piece 
                 * of memory that has just been deleted. This is known as a dangling reference.
                 * 
                 */
                const_iterator operator++(int)
                {
                    const_iterator old = *this;
                    ++(*this);
                    return old;
                }

                const_iterator & operator--()
                {
                    current = current->prev;
                    return *this;
                }

                const_iterator operator--(int)
                {
                    const_iterator old = *this;
                    --(*this);
                    return old;
                }

                bool operator==(const const_iterator & rhs) const
                {
                    return current == rhs.current;
                }

                bool operator!=(const const_iterator & rhs) const 
                {
                    return !(*this == rhs);
                }

            protected:
                // In order to determine if an iterator passed to erase or insert is an iterator for the correct 
                // list, the iterator must store an additional data member representing a pointer to the List 
                // from which it was constructed
                const List<Object> *theList;

                Node* current;

                Object & retrieve() const
                {
                    return current->data;
                }

                // const_iterator(Node *p): current {p} {}

                const_iterator(const List<Object> & lst, Node *p): theList{ &lst }, current { p }
                {
                }

                void assertIsValid() const 
                {
                    if (theList == nullptr || current == nullptr || current == theList->head) {
                        throw std::out_of_range{"Iterator out of bound"};
                    }
                }

                // The friend class declaration grants another class complete access to your private and 
                // protected members (variables and functions)
                friend class List<Object>;
        };

        // iterator is type-compatible with const_iterator and can be used wherever const_iterator is 
        // needed

        /*
         * By default, when you use the class keyword in C++, inheritance is private unless you explicitly say 
         * otherwise.
         * 
         * Private inheritance turns all of the public methods in const_iterator into private methods inside 
         * iterator.
         * 
         * Using public inheritance ensures that the standard rules of Object-Oriented Programming apply: 
         * anything a const_iterator can do, a regular iterator can also do.
         * 
         */
        class iterator: public const_iterator
        {
            public:
                /*
                 * In C++, whenever a derived class (child) object is created, the base class (parent) 
                 * constructor must run first to initialize the inherited parts of the object before the 
                 * derived class constructor can execute its own body.
                 * 
                 * Because you didn't explicitly specify which parent constructor to use in your iterator 
                 * constructor, the C++ compiler automatically inserts a call to the parent's default constructor.
                 * 
                 * Behind the scenes, your code behaves exactly as if you had written this:
                 * 
                 *   iterator() : const_iterator() {}
                 * 
                 */
                iterator() {}

                /*
                 * There are two major reasons iterator must redefine these operators:
                 *
                 * 1. Fix the return type so that incrementing an iterator yields an iterator, not a 
                 *     const_iterator.
                 *
                 * 2. Upgrade permissions so that dereferencing (*it) allows write operations, whereas 
                 *    dereferencing a const_iterator is strictly read-only.
                 * 
                 */
                Object & operator*()
                {
                    return const_iterator::retrieve();
                }

                const Object & operator*() const
                {
                    return const_iterator::operator*();
                }

                iterator & operator++()
                {
                    /*
                     * In a normal, non-templated class, you can absolutely just drop this->.
                     *
                     * However, because the List and iterator classes are templated, C++ enforces a strict rule 
                     * known as Two-Phase Name Lookup.
                     * 
                     * When the compiler reads your template code for the first time, it has a problem: it 
                     * doesn't know what Object is yet.
                     * 
                     * If you just write current, the compiler assumes it is an independent name, fails to find 
                     * it anywhere, and throws an error immediately: error: use of undeclared identifier 'current'.
                     * 
                     * By using this->, you turn current into a dependent name. The compiler will happily 
                     * postpone the search for current until the second phase (when the template is instantiated 
                     * with a real type), at which point it looks inside const_iterator and finds it perfectly.
                     * 
                     */
                    this->current = this->current->next;
                    return *this;
                }

                iterator operator++(int)
                {
                    iterator old = *this;
                    ++(*this);
                    return old;
                }

                iterator & operator--()
                {
                    this->current = this->current->prev;
                    return *this;
                }

                iterator operator--(int)
                {
                    iterator old = *this;
                    --(*this);
                    return old;
                }

            protected:
                // iterator(Node *p): const_iterator {p} {}

                iterator(const List<Object> & lst, Node *p): const_iterator{ lst, p }
                {
                }

                friend class List<Object>;
        };

    public:
        List()
        {
            init();
        }

        ~List()
        {
            clear();
            delete head;
            delete tail;
        }

        List(const List & rhs)
        {
            init();
            for (auto & x: rhs) {
                push_back(x);
            }
        }

        List & operator=(const List * rhs)
        {
            List copy = rhs;
            std::swap(*this, copy);
            return *this;
        }

        List(List && rhs): theSize {rhs.size}, head {rhs.head}, tail {rhs.tail}
        {
            rhs.theSize = 0;
            rhs.head = nullptr;
            rhs.tail = nullptr;
        }

        List & operator=(List && rhs)
        {
            std::swap(theSize, rhs.theSize);
            std::swap(head, rhs.head);
            std::swap(tail, rhs.tail);

            return *this;
        }

        iterator begin()
        {
            /*
             * These braces are a shorthand way of saying: "Create an iterator object right here on the
             * fly, pass *this and head->next into its constructor, and return that new iterator."
             *
             * When your compiler sees return { *this, head->next };, it checks the return type of the
             * function, which returns an iterator. The compiler looks at the iterator class and asks: 
             * "Does iterator have a constructor that accepts whatever type *this and head->next are?"
             *
             * The braces tell the compiler to implicitly instantiate an iterator object.
             *
             *   iterator(const List<Object> & lst, Node *p): const_iterator {lst, p} {}
             * 
             */

            return { *this, head->next };
        }

        const_iterator begin() const
        {
            return { *this, head->next };
        }

        iterator end()
        {
            return { *this, tail };
        }

        const_iterator end() const
        {
            return { *this, tail };
        }

        int size() const
        {
            return theSize;
        }

        bool empty() const
        {
            return size() == 0;
        }

        void clear()
        {
            while (!empty()) {
                pop_front();
            }
        }

        Object & front()
        {
            return *begin();
        }

        const Object & front() const
        {
            return *begin();
        }

        Object & back()
        {
            return *--end();
        }

        const Object & back() const
        {
            return *--end();
        }

        void push_front(const Object & x)
        {
            insert(begin(), x);
        }

        void push_front(Object && x)
        {
            insert(begin(), std::move(x));
        }

        void push_back(const Object & x) 
        {
            insert(end(), x);
        }

        void push_back(Object && x)
        {
            insert(end(), std::move(x));
        }

        void pop_front()
        {
            erase(begin());
        }

        void pop_back()
        {
            erase(--end());
        }

        iterator insert(iterator itr, const Object & x)
        {
            itr.assertIsValid();
            if (itr.theList != this) {
                throw std::out_of_range{"Iterator out of bound"};
            }

            Node *p = itr.current;
            theSize++;
            return { p->prev = p->prev->next = new Node{ x, p->prev, p } };
        }

        iterator insert(iterator itr, Object && x)
        {
            itr.assertIsValid();
            if (itr.theList != this) {
                throw std::out_of_range{"Iterator out of bound"};
            }

            Node *p = itr.current;
            theSize++;
            return { *this, p->prev = p->prev->next = new Node{ std::move(x), p->prev, p } };
        }

        iterator erase(iterator itr)
        {
            Node *p = itr.current;
            iterator retVal{ *this, p->next };

            p->prev->next = p->next;
            p->next->prev = p->prev;
            delete p;

            theSize--;

            return retVal;
        }

        iterator erase(iterator from, iterator to)
        {
            for (iterator itr = from; itr != to;) {
                itr = erase(itr);
            }

            return to;
        }

    private:
        int theSize;
        Node *head;
        Node *tail;

        void init()
        {
            theSize = 0;
            head = new Node;
            tail = new Node;
            head->next = tail;
            tail->prev = head;
        }
};

#endif
