//
// Created by aldes on 25.12.2021.
//

#ifndef COURSEWORK_ALLOCATOR_H
#define COURSEWORK_ALLOCATOR_H


class Allocator {
public:
    typedef void value_type;
    typedef value_type* pointer;
    typedef size_t size_type;

    Allocator() = default;

    ~Allocator() {
        ::free(startPointer);
    }

    virtual pointer allocate(size_type size) = 0;

    virtual void deallocate(pointer ptr) = 0;

    void free() {
        auto* header = static_cast<Header*>(startPointer);

    }

protected:

    struct Header {
    public:
        size_type size;
        size_type previousSize;
        bool isAvailable;

        inline Header* next() {
            return (Header*)((char*) (this + 1) + size);
        }

        inline Header* previous() {
            return (Header*)((char*) this - previousSize) - 1;
        }
    };

    const size_type headerSize = sizeof(Header);
    const size_type blockAlignment = 4;
    pointer startPointer = nullptr;
    pointer endPointer = nullptr;
    size_type totalSize = 0;
    size_type usedSize = 0;

    Header* find(size_type size) {
        auto* header = static_cast<Header*>(startPointer);
        while (!header->isAvailable || header->size < size) {
            header = header->next();
            if (header >= endPointer) {
                return nullptr;
            }
        }
        return header;
    }

    void splitBlock(Header* header, size_type chunk) {
        size_type blockSize = header->size;
        header->size = chunk;
        header->isAvailable = false;
        if (blockSize - chunk >= headerSize) {
            auto *next = header->next();
            next->previousSize = chunk;
            next->size = blockSize - chunk - headerSize;
            next->isAvailable = true;
            usedSize += chunk + headerSize;
            auto *followed = next->next();
            if (followed < endPointer) {
                followed->previousSize = next->size;
            }
        } else {
            header->size = blockSize;
            usedSize += blockSize;
        }
    }

    bool validateAddress(void* ptr) {
        auto* header = static_cast<Header*>(startPointer);
        while (header < endPointer) {
            if (header + 1 == ptr) {
                return true;
            }
            header = header->next();
        }
        return false;
    }
};


#endif //COURSEWORK_ALLOCATOR_H
