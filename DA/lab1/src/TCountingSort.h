//
// Created by djent on 26.02.2022.
//

#ifndef LAB1_TCOUNTINGSORT_H
#define LAB1_TCOUNTINGSORT_H

#include <vector>
#include "TKeyValue.h"

class TCountingSort {
public:
    static std::vector<TKeyValue> CountingSort(const std::vector<TKeyValue> &source, const int lower, const int upper) {
        std::vector<int> vectorCount = CountEntries(source, lower, upper);
        CountPrefixSum(vectorCount);

        std::vector<TKeyValue> result(source.size());

        for (int i = source.size() - 1; i >= 0; i--) {
            int index = vectorCount[source[i].key - lower] - 1;
            vectorCount[source[i].key - lower]--;
            result[index] = source[i];
        }

        return result;
    }

private:
    static std::vector<int> CountEntries(const std::vector<TKeyValue> &source, const int lower, const int upper) {
        std::vector<int> result(upper - lower + 1, 0);

        for (TKeyValue entry: source) {
            result[entry.key - lower]++;
        }
        return result;
    }

private:
    static void CountPrefixSum(std::vector<int> &source) {
        for (int i = 1; i < source.size(); i++) {
            source[i] = source[i] + source[i - 1];
        }
    }
};

#endif //LAB1_TCOUNTINGSORT_H
