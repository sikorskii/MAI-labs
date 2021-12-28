//
// Created by aldes on 25.12.2021.
//

#ifndef COURSEWORK_BLOCKALLOCATOR_H
#define COURSEWORK_BLOCKALLOCATOR_H
#include "Allocator.h"

class BlockAllocator : public Allocator{
public:
    explicit BlockAllocator(size_type size) {
        if ((startPointer = malloc(size)) == nullptr) {
            std::cout << "cant allocate such memory" << std::endl;
            return;
        }
        totalSize = size;
        endPointer = static_cast<void*>(static_cast<char*>(startPointer) + totalSize);
        auto* header = (Header*) startPointer;
        header->isAvailable = true;
        header->size = (totalSize - headerSize);
        header->previousSize = 0;
        usedSize = headerSize;
    }

    pointer allocate(size_type size) override {
        if (size <= 0) {
            std::cout << "size must be > 0" << std::endl;
            return nullptr;
        }
        if (size > totalSize - usedSize) {
            return nullptr;
        }
        auto* header = find(size);
        if (header == nullptr) {
            return nullptr;
        }
        splitBlock(header, size);
        return header + 1;
    }

    void deallocate(pointer ptr) override {
        if (!validateAddress(ptr)) {
            return;
        }
        auto* header = static_cast<Header*>(ptr) - 1;
        header->isAvailable = true;
        usedSize -= header->size;
    }

private:
    bool isPrevious(Header* header) {
        auto* previous = header->previous();
        return header != startPointer && previous->isAvailable;
    }

    bool isNextFree(Header* header) {
        auto* next = header->next();
        return header != endPointer && next->isAvailable;
    }
};


#endif //COURSEWORK_BLOCKALLOCATOR_H
