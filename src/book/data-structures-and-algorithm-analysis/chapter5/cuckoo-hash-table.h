#pragma once

#include <iostream>
#include <vector>

#include "prime.h"
#include "uniform-random.h"

/*
 *  int main(int argc, char *argv[]) 
 *  {
 *      CuckooHashTable<string, StringHashFamily<7>> t;
 *  
 *      t.insert("window");
 *      t.insert("linux");
 *      t.insert("macos");
 *  
 *      std::cout << "Contain 'window' (before remove)? " << (t.contains("window")? "YES" : "NO") << std::endl;
 *      t.remove("window");
 *      std::cout << "Contain 'window' (after remove)? " << (t.contains("window")? "YES" : "NO") << std::endl;
 *  
 *      return 0;
 *  }
 */
template <typename AnyType, typename HashFamily>
class CuckooHashTable
{
    public:
        explicit CuckooHashTable(int size=101): array(nextPrime(size))
        {
            numHashFunctions = hashFunctions.getNumberOfFunctions();
            rehashes = 0;
            makeEmpty();
        }

        void makeEmpty()
        {
            currentSize = 0;
            for (auto & entry: array) {
                entry.isActive = false;
            }
        }

        bool contains(const AnyType & x) const
        {
            return findPos(x) != -1;
        }

        bool remove(const AnyType & x)
        {
            int currentPos = findPos(x);
            if (!isActive(currentPos)) {
                return false;
            }

            array[currentPos].isActive = false;
            --currentSize;

            return true;
        }

        bool insert(const AnyType & x)
        {
            if (contains(x)) {
                return false;
            }

            if (currentSize >= array.size() * MAX_LOAD) {
                expand();
            }

            return insertHelper(x);
        }

        // bool insert(AnyType && x);

    private:
        struct HashEntry
        {
            AnyType element;
            bool isActive;

            HashEntry(const AnyType & e = AnyType(), bool a = false): 
                element {e}, isActive {a} {}
            
            HashEntry(AnyType && e, bool a = false):
                element {std::move(e)}, isActive {a} {}
        };

        static constexpr double MAX_LOAD = 0.40;
        static constexpr int ALLOWED_REHASHES = 5;

        std::vector<HashEntry> array;
        int currentSize;
        int numHashFunctions;
        int rehashes;
        UniformRandom r;
        HashFamily hashFunctions;

        bool insertHelper(const AnyType & xx)
        {
            const int COUNT_LIMIT = 100;

            AnyType x = xx;

            while (true) {
                int lastPos = -1;
                int pos;

                for (int count = 0; count < COUNT_LIMIT; ++count) {

                    for (int i = 0; i < numHashFunctions; ++i) {
                        pos = myhash(x, i);

                        if (!isActive(pos)) {
                            array[pos] = std::move(HashEntry{std::move(x), true} );
                            array[pos].isActive = true;

                            ++currentSize;
                            return true;
                        }
                    }

                    // None of the spots are available. Evict a random one
                    int i = 0;
                    do {
                        pos = myhash(x, r.nextInt(numHashFunctions));

                        // Attempting NOT to re-evict the last item.
                    } while (pos == lastPos && i++ < 5);

                    lastPos = pos;
                    std::swap(x, array[pos].element);
                }

                if (++rehashes > ALLOWED_REHASHES) {
                    expand();
                    rehashes = 0;
                }
                else {
                    rehash();
                }
            }
        }

        // bool insertHelper(AnyType && x);

        bool isActive(int currentPos) const
        {
            if (currentPos < 0 || currentPos >= array.size()) {
                return false;
            }

            return array[currentPos].isActive;
        }

        size_t myhash(const AnyType & x, int which) const
        {
            return hashFunctions.hash(x, which) % array.size();
        }

        int findPos(const AnyType & x) const
        {
            for (int i = 0; i < numHashFunctions; ++i) {
                int pos = myhash(x, i);

                if (isActive(pos) && array[pos].element == x) {
                    return pos;
                }
            }

            return -1;
        }

        void expand()
        {
            rehash(static_cast<int>(array.size() / MAX_LOAD));
        }

        void rehash()
        {
            hashFunctions.generateNewFunctions();
            rehash(array.size());

            ++rehashes;
        }

        void rehash(int newSize)
        {
            std::vector<HashEntry> oldArray = array;

            array.resize(nextPrime(newSize));

            for (auto & entry: array) {
                entry.isActive = false;
            }

            currentSize = 0;
            for (auto & entry: oldArray) {
                if (entry.isActive) {
                    insert(std::move(entry.element));
                }
            }

            ++rehashes;
        }
};
