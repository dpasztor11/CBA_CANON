#include "colouring_bit_array_canon.hpp"
#include "kempe_closed.hpp"
#include "helper.hpp"
#include <iostream>
#include <algorithm>
#include <utility>
#include <execution>
#include <fstream>

using namespace ba_graph;

const int len = 6;
// works for len == 6, 5 or 4
const int riaCount = colouring_bit_array_internal::Comparator(len).len; // relevant_indices_absolute

int main()
{
    std::vector<long long> csEq = getCsEq(len, false);
    std::vector<long long> minimalCsEq;

    std::ofstream minimalCsEqFile;
    std::string minimalCsEqFileString = "data/minimalCsEq/minimalCsEq" + std::to_string(len) + ".txt";
    minimalCsEqFile.open(minimalCsEqFileString);
    minimalCsEqFile << "MinimalCsEq:" << std::endl;

    for (int i = 0; i < csEq.size(); i++)
    {
        for (int j = 0; j < csEq.size(); j++)
        {
            if (j != i && (csEq[i] | csEq[j]) == csEq[i])
                break;
            if (j + 1 == csEq.size())
                minimalCsEq.push_back(csEq[i]);
        }
    }
    minimalCsEqFile << "Count: " << minimalCsEq.size() << std::endl;
    minimalCsEqFile.close();
    storeCbasToFile(minimalCsEqFileString, minimalCsEq, riaCount);
}
