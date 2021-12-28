//
// Created by aldes on 27.12.2021.
//

#ifndef COURSEWORK_BUDDYALLOCATOR_H
#define COURSEWORK_BUDDYALLOCATOR_H

#include <map>

class BuddyAllocator {
public:


    BuddyAllocator(size_t _minBlockSize, size_t _maxSize) :
        minBlockSize(_minBlockSize), maxSize(_maxSize) {
        allocatedBlocks = (unsigned char*)(::operator new(maxSize));
        allocMap.insert({maxSize, allocatedBlocks});
    }

    ~BuddyAllocator() {
        ::operator delete(allocatedBlocks);
        allocMap.clear();
    }

    void* alloc(size_t size) {
        if (size == 0) {
            return nullptr;
        }
        size_t powerOfTwo = getNearestSize(size);

        for (auto it = allocMap.begin(), end = allocMap.end(); it != end; it++) {
            if (it->first == size) {
                void* result = it->second;
                allocMap.erase(it);
                return result;
            } else if (it->first > size) {
                int allocSize = it->first;
                unsigned char* blockStart = it->second;

                while(allocSize > size) {
                    allocSize /= 2;
                    if (allocSize < size) {
                        break;
                    }
                    allocMap.insert(std::pair<size_t, unsigned char*> (allocSize, blockStart + allocSize));
                }
                allocMap.erase(it);
                deallocMap.insert(std::pair<unsigned char*, size_t> (blockStart, allocSize));
                return blockStart;
            }
        }
        throw std::bad_alloc();
    }

    void deallocate(void* ptr) {
        auto pos = deallocMap.find(ptr);
        int bufferedSize = 0;
        if (pos == deallocMap.end()) {
            throw std::bad_exception();
        } else {
            bufferedSize = pos->second;
        }
        while(true) {
            std::pair<std::multimap<size_t, unsigned char*>::iterator,
                std::multimap<size_t, unsigned char*>::iterator> range (allocMap.equal_range(bufferedSize));
            if (range.first != allocMap.end() && range.first->first == bufferedSize) {
                auto it = range.first;
                int i = ((unsigned char*)ptr - allocatedBlocks)  / bufferedSize % 2;

                while(it != range.second) {
                    if (i == 1) {
                        if (((unsigned char*)ptr - it->second) == bufferedSize) {
                            bufferedSize *= 2;
                            ptr = it->second;
                            allocMap.erase(it);
                            break;
                        }
                    } else {
                        if ((it->second - (unsigned char*)ptr) == bufferedSize) {
                            bufferedSize *= 2;
                            allocMap.erase(it);
                            break;
                        }
                    }
                    it++;
                }
                if (it == range.second) {
                    allocMap.insert(std::pair<size_t, unsigned char*> (bufferedSize, (unsigned char*)ptr));
                    break;
                } else {
                    continue;
                }
            } else {
                allocMap.insert(std::pair<size_t, unsigned char*> (bufferedSize, (unsigned char*)ptr));
                break;
            }
        }
    }

    void showMemory() {
        for (auto& p: allocMap) {
            std::cout << p.first << " ";
        }
    }


    size_t getFreeSize() {
        return maxSize;
    }

    unsigned char* getAllocatedBlocks() {
        return allocatedBlocks;
    }

    std::multimap<size_t, unsigned char*> getFreeBlocks() {
        return allocMap;
    }

private:
    size_t getNearestSize(const size_t size) {
        if (size <= minBlockSize) {
            return minBlockSize;
        }
        size_t powerOfTwo = 1;
        while (powerOfTwo < size) {
            powerOfTwo <<= 1;
        }
        return powerOfTwo;
    }

    size_t minBlockSize;
    size_t maxSize;
    std::multimap<size_t, unsigned char*> allocMap;
    std::map<void*, size_t> deallocMap;
    unsigned char* allocatedBlocks;
};


#endif //COURSEWORK_BUDDYALLOCATOR_H
