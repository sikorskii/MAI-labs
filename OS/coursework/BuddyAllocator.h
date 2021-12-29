
#include <iostream>
#include <cstring>
#include <cmath>

#define DIV_ROUNDUP(A, B) \
({ \
    typeof(A) _a_ = A; \
    typeof(B) _b_ = B; \
    (_a_ + (_b_ - 1)) / _b_; \
})

#define ALIGN_UP(A, B) \
({ \
    typeof(A) _a__ = A; \
    typeof(B) _b__ = B; \
    DIV_ROUNDUP(_a__, _b__) * _b__; \
})


struct BuddyBlock {
    size_t blockSize;
    bool isFree;
};

class BuddyAllocator {
private:
    BuddyBlock *head = nullptr;
    BuddyBlock *tail = nullptr;
    void *data = nullptr;

    bool expanded = false;

    BuddyBlock *next(BuddyBlock *block) {
        return reinterpret_cast<BuddyBlock*>(reinterpret_cast<uint8_t*>(block) + block->blockSize);
    }

    BuddyBlock *split(BuddyBlock *block, size_t size) {
        if (block != nullptr && size != 0) {
            while (size < block->blockSize) {
                size_t sz = block->blockSize >> 1;
                block->blockSize = sz;
                block = this->next(block);
                block->blockSize = sz;
                block->isFree = true;
            }
            if (size <= block->blockSize) return block;
        }
        return nullptr;
    }

    BuddyBlock *find_best(size_t size) {
        if (size == 0) return nullptr;

        BuddyBlock *best_block = nullptr;
        BuddyBlock *block = this->head;
        BuddyBlock *buddy = this->next(block);

        if (buddy == this->tail && block->isFree) {
            return this->split(block, size);
        }

        while (block < this->tail && buddy < this->tail) {
            if (block->isFree && buddy->isFree && block->blockSize == buddy->blockSize) {
                block->blockSize <<= 1;
                if (size <= block->blockSize && (best_block == nullptr || block->blockSize <= best_block->blockSize)) {
                    best_block = block;
                }

                block = this->next(buddy);
                if (block < this->tail)  {
                    buddy = this->next(block);
                }
                continue;
            }

            if (block->isFree && size <= block->blockSize && (best_block == nullptr || block->blockSize <= best_block->blockSize))  {
                best_block = block;
            }
            if (buddy->isFree && size <= buddy->blockSize && (best_block == nullptr || buddy->blockSize < best_block->blockSize)) {
                best_block = buddy;
            }

            if (block->blockSize <= buddy->blockSize) {
                block = this->next(buddy);
                if (block < this->tail) {
                    buddy = this->next(block);
                }
            }
            else {
                block = buddy;
                buddy = this->next(buddy);
            }
        }

        if (best_block != nullptr) {
            return this->split(best_block, size);
        }

        return nullptr;
    }

    size_t required_size(size_t size) {
        size_t actual_size = sizeof(BuddyBlock);

        size += sizeof(BuddyBlock);
        size = ALIGN_UP(size, sizeof(BuddyBlock));

        while (size > actual_size) {
            actual_size <<= 1;
        }

        return actual_size;
    }

    void coalescence() {
        while (true) {
            BuddyBlock *block = this->head;
            BuddyBlock *buddy = this->next(block);

            bool no_coalescence = true;
            while (block < this->tail && buddy < this->tail) {
                if (block->isFree && buddy->isFree && block->blockSize == buddy->blockSize) {
                    block->blockSize <<= 1;
                    block = this->next(block);
                    if (block < this->tail) {
                        buddy = this->next(block);
                        no_coalescence = false;
                    }
                }
                else if (block->blockSize < buddy->blockSize) {
                    block = buddy;
                    buddy = this->next(buddy);
                }
                else {
                    block = this->next(buddy);
                    if (block < this->tail) {
                        buddy = this->next(block);
                    }
                }
            }

            if (no_coalescence) {
                return;
            }
        }
    }

public:
    bool debug = false;

    BuddyAllocator(size_t size) {
        this->expand(size);
    }

    ~BuddyAllocator() {
        this->head = nullptr;
        this->tail = nullptr;
        std::free(this->data);
    }

    void expand(size_t size) {

        if (this->head) {
            size += this->head->blockSize;
        }
        size = pow(2, ceil(log(size) / log(2)));


        this->data = std::realloc(this->data, size);

        this->head = static_cast<BuddyBlock*>(data);
        this->head->blockSize = size;
        this->head->isFree = true;

        this->tail = next(head);

        if (this->debug) {
            std::cout << "Expanded the heap. Current blockSize: " << size << " bytes" << std::endl;
        }
    }

    void setsize(size_t size) {
        size -= this->head->blockSize;
        this->expand(size);
    }

    void *malloc(size_t size) {
        if (size == 0) return nullptr;

        size_t actual_size = this->required_size(size);

        BuddyBlock *found = this->find_best(actual_size);
        if (found == nullptr) {
            this->coalescence();
            found = this->find_best(actual_size);
        }

        if (found != nullptr) {
            if (this->debug) std::cout << "Allocated " << size << " bytes" << std::endl;
            found->isFree = false;
            this->expanded = false;
            return reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(found) + sizeof(BuddyBlock));
        }

        if (this->expanded) {
            this->expanded = false;
            return nullptr;
        }
        this->expanded = true;
        this->expand(size);
        return this->malloc(size);
    }


    void free(void *ptr) {
        if (ptr == nullptr) {
            return;
        }

        BuddyBlock *block = reinterpret_cast<BuddyBlock*>(reinterpret_cast<uint8_t*>(ptr) - sizeof(BuddyBlock));
        block->isFree = true;

        if (this->debug) {
            std::cout << "Freed " << block->blockSize - sizeof(BuddyBlock) << " bytes" << std::endl;
        }

        this->coalescence();
    }
};