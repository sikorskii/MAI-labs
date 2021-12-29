#include <iostream>
#include <unistd.h>
#include "BlockAllocator.h"
#include "BuddyAllocator.h"
#include <vector>

int main() {
    BlockAllocator all = BlockAllocator(1024);
    void* mem = all.allocate(1024);
    all.deallocate(mem);
    BuddyAllocator allocator = BuddyAllocator(1024);
    std::vector<void*> v(100);
    for (int i = 0; i < v.size(); i++) {
        v[i] = allocator.malloc(i + (i / 2 % 5 + 1));
    }
    for (auto & i : v) {
        allocator.free(i);
    }
    for (int i = 0; i < v.size(); i++) {
        v[i] = all.allocate(i + (i / 2 % 5 + 1));
    }
    for (auto & i : v) {
        all.deallocate(i);
    }
}
