#ifndef LINKED_LIST_STACK_H
#define LINKED_LIST_STACK_H

#include <stdexcept>

/*
 *  int main(int argc, char *argv[]) 
 *  {
 *      LinkedListStack<int> a;
 *  
 *      cout << "Current size: " << a.size() << endl;
 *      cout << "Is empty? " << (a.empty() ? 'Y':'N') << endl;
 *     
 *      a.push(47);
 *      a.push(19);
 *      a.push(73);
 *  
 *      cout << "`a` new size: " << a.size() << endl;
 *      cout << "Is empty? " << (a.empty() ? 'Y':'N') << endl;
 *      
 *      for (; !a.empty();) {
 *          cout << "`a` element: " << a.pop() << endl;
 *      }
 *  
 *      return 0;
 *  }
*/

template <typename Object>
class LinkedListStack
{
    private:
        struct Node
        {
            Object data;
            Node* prev;

            Node(const Object & d = Object(), Node *p = nullptr):
                data { d }, prev { p }
            {}

            Node(Object && d, Node *p = nullptr):
                data { std::move(d) }, prev { p }
            {}
        };

        class iterator
        {
            public:
                iterator(): current {nullptr} {}

                const Object & operator*() const
                {
                    return retrieve();
                }

                iterator & operator--()
                {
                    current = current->prev;
                    return *this;
                }

                iterator operator--(int)
                {
                    iterator old = *this;
                    --(*this);
                    return old;
                }

            private:
                Node * current;

                Object & retrieve() const
                {
                    return current->data;
                }

                iterator(Node *p): current {p}
                {
                }

                friend class LinkedListStack<Object>;
        };

    public:
        LinkedListStack()
        {
            init();
        }

        ~LinkedListStack()
        {
            clear();
            delete tail;
            delete head;
        }

        iterator end() const
        {
            return { tail };
        }

        int size() const
        {
            return theSize;
        }

        bool empty() const
        {
            return theSize == 0;
        }

        void clear() 
        {
            while (!empty()) {
                pop();
            }
        }

        void push(const Object & x)
        {
            insert(end(), x);
        }

        void push(Object && x)
        {
            insert(end(), std::move(x));
        }

        Object pop()
        {
            if (empty()) {
                throw std::underflow_error("Cannot pop from an empty stack");
            }
            return erase(end());
        }

        iterator insert(iterator itr, const Object & x)
        {
            Node *p = itr.current;
            theSize++;
            return { p->prev = new Node{x, p->prev} };
        }

        iterator insert(iterator itr, Object && x)
        {
            Node *p = itr.current;
            theSize++;
            return { p->prev = new Node{std::move(x), p->prev} };
        }

        Object erase(iterator itr)
        {
            Node *p = itr.current;
            
            // 1. Identify the actual node you want to delete (the one right before p)
            Node *toDelete = p->prev;
            
            // 2. Bypass the toDelete node
            p->prev = toDelete->prev;
            
            // 3. Extract the data safely from the target node
            // Using std::move makes this highly efficient for heavy objects!
            Object retVal = std::move(toDelete->data);
            
            // 4. Free up the memory
            delete toDelete;
            theSize--;

            return retVal;
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
            tail->prev = head;
        }
};

#endif
