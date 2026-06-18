#pragma once

#include <algorithm>
#include <list>
#include <vector>

#include "hash.h"
#include "prime.h"

/*
 *  int main(int argc, char *argv[]) 
 *  {
 *      SeparateChainingHashTable<int> t;
 *  
 *      t.insert(47);
 *      t.insert(13);
 *      t.insert(81);
 *      t.insert(95);
 *      t.insert(42);
 *  
 *      std::cout << "Contain 81? " << (t.contains(81)? "YES": "NO") << std::endl;
 *      std::cout << "Current size: " << t.size() << std::endl;
 *  
 *      t.remove(81);
 *  
 *      std::cout << "Contain 81? " << (t.contains(81)? "YES": "NO") << std::endl;
 *      std::cout << "Current size: " << t.size() << std::endl;
 *  }
 */
template <typename HashedObj>
class SeparateChainingHashTable
{
    public:
        /*
         * Note the parentheses `(size)`
         *
         * if you use curly braces, the compiler will almost always assume you are providing a 
         * list of initial values (an initializer_list).
         *
         * Writing theLists{size} will create a vector containing exactly 1 list, and that 
         * single list will have a single element inside it initialized to 101.
         *
         */

        explicit SeparateChainingHashTable(int size=101): theLists(nextPrime(size))
        {
            makeEmpty();
        }

        bool contains(const HashedObj & x) const
        {
            auto & whichList = theLists[myhash(x)];
            return std::find(begin(whichList), end(whichList), x) != end(whichList);
        }

        void makeEmpty()
        {
            for (auto & theList: theLists) {
                theList.clear();
            }
        }

        int size() const
        {
            return currentSize;
        }

        bool insert(const HashedObj & x)
        {
            auto & whichList = theLists[myhash(x)];

            if (std::find(begin(whichList), end(whichList), x) != end(whichList)) {
                return false;
            }

            whichList.push_back(x);

            if (++currentSize > theLists.size()) {
                rehash();
            }

            return true;
        }

        bool insert(HashedObj && x)
        {
            auto & whichList = theLists[myhash(x)];

            if (std::find(begin(whichList), end(whichList), x) != end(whichList)) {
                return false;
            }

            whichList.push_back(std::move(x));

            if (++currentSize > theLists.size()) {
                rehash();
            }

            return true;
        }

        bool remove(const HashedObj & x)
        {
            auto & whichList = theLists[myhash(x)];
            auto itr = std::find(begin(whichList), end(whichList), x);

            if (itr == end(whichList)) {
                return false;
            }

            whichList.erase(itr);
            --currentSize;

            return true;
        }

    private:
        std::vector<std::list<HashedObj>> theLists;
        int currentSize;

        void rehash()
        {
            std::vector<std::list<HashedObj>> oldLists = theLists;

            theLists.resize(nextPrime(2 * oldLists.size()));
            for (auto & thisList: theLists) {
                thisList.clear();
            }

            currentSize = 0;
            for (auto & thisList: oldLists) {
                for (auto & x: thisList) {
                    insert(std::move(x));
                }
            }
        }

        size_t myhash(const HashedObj & x) const
        {
            static Hash<HashedObj> hf;
            return hf(x) % theLists.size();
        }
};
