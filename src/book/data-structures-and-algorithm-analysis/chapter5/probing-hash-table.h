#ifndef PROBING_HASH_TABLE_H
#define PROBING_HASH_TABLE_H

#include <vector>

#include "hash.h"
#include "prime.h"

/*
 *  int main(int argc, char *argv[]) 
 *  {
 *      ProbingHashTable<int> t;
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
class ProbingHashTable
{
    public:
        explicit ProbingHashTable(int size=101): array(nextPrime(size))
        {
            makeEmpty();
        }

        bool contains(const HashedObj & x) const
        {
            return isActive(findPos(x));
        }

        void makeEmpty()
        {
            currentSize = 0;
            for (auto & entry: array) {
                entry.info = EMPTY;
            }
        }

        int size() const
        {
            return currentSize;
        }

        bool insert(const HashedObj & x)
        {
            int currentPos = findPos(x);
            if (isActive(currentPos)) {
                return false;
            }

            array[currentPos].element = x;
            array[currentPos].info = ACTIVE;

            if (++currentSize > array.size() / 2) {
                rehash();
            }

            return true;
        }

        bool insert(HashedObj && x)
        {
            int currentPos = findPos(x);
            if (isActive(currentPos)) {
                return false;
            }

            array[currentPos].element = std::move(x);
            array[currentPos].info = ACTIVE;

            if (++currentSize > array.size() / 2) {
                rehash();
            }

            return true;
        }

        bool remove(const HashedObj & x)
        {
            int currentPos = findPos(x);
            if (!isActive(currentPos)) {
                return false;
            }

            array[currentPos].info = DELETED;

            return true;
        }

        enum EntryType {ACTIVE, EMPTY, DELETED};

    private:
        struct HashEntry
        {
            HashedObj element;
            EntryType info;

            HashEntry(const HashedObj & e = HashedObj{}, EntryType i = EMPTY):
                element {e}, info {i}
            {
            }

            HashEntry(HashedObj && e, EntryType i = EMPTY):
                element {std::move(e)}, info {i}
            {
            }
        };

        std::vector<HashEntry> array;
        int currentSize;

        bool isActive(int currentPos) const
        {
            if (currentPos < 0 || currentPos >= array.size()) {
                return false;
            }

            return array[currentPos].info == ACTIVE;
        }

        int findPos(const HashedObj & x) const
        {
            int offset = 1;
            int currentPos = myhash(x);

            while (
                array[currentPos].info != EMPTY &&
                array[currentPos].element != x
            ) {
                currentPos += offset;
                offset += 2;

                if (currentPos >= array.size()) {
                    currentPos -= array.size();
                }
            }

            return currentPos;
        }

        void rehash()
        {
            std::vector<HashEntry> oldArray = array;

            array.resize(nextPrime(2 * oldArray.size()));
            for (auto & entry: array) {
                entry.info = EMPTY;
            }

            currentSize = 0;
            for (auto & entry: oldArray) {
                if (entry.info == ACTIVE) {
                    insert(std::move(entry.element));
                }
            }
        }

        size_t myhash(const HashedObj & x) const
        {
            static Hash<HashedObj> hf;
            return hf(x) % array.size();
        }
};

#endif
