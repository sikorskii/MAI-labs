#include <iostream>
#include <limits>
#include <iomanip>

#include "TCountingSort.h"

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    const int INDEX_FIELD_SIZE = 6;
    const char OUTPUT_FILL = '0';

    std::vector<TKeyValue> sort;
    int key;
    unsigned long long value;

    while (std::cin >> key >> value) {
        sort.push_back({key, value});
    }

    int lower = std::numeric_limits<int>::max();
    int upper = std::numeric_limits<int>::min();

    for (auto &x: sort) {
        lower = std::min(lower, x.key);
        upper = std::max(upper, x.key);
    }

    std::vector<TKeyValue> result = TCountingSort::CountingSort(sort, lower, upper);

    for (auto &i: result) {
        std::cout << std::setw(INDEX_FIELD_SIZE) << std::setfill(OUTPUT_FILL) << i.key;
        std::cout << '\t';
        std::cout << i.value << '\n';
    }
}
