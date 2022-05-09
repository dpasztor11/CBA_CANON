#include "colouring_bit_array_canon.hpp"
#include "kempe_closed.hpp"
#include <iostream>
#include <algorithm>
#include <utility>
#include <execution>
#include <fstream>

using namespace ba_graph;

const int len = 6;
// works for len == 6, 5 or 4
const int feasibleCBAsCount = len == 6 ? 70986 : (len == 5 ? 16 : 6);
const int riaCount = colouring_bit_array_internal::Comparator(len).len; // relevant_indices_absolute

long long binaryToNumber(std::string binary)
{
    long long sol = 0;
    for (int i = 0; i < binary.length(); i++)
    {
        sol = (sol << 1) + (binary[i] == '1');
    }
    return sol;
}

int main()
{
    // getting feasibleCBAs from file
    std::vector<long long> feasibleCBAs;
    std::ifstream feasibleCBAFile;
    feasibleCBAFile.open("txt/feasibleCBA" + std::to_string(len) + ".txt");
    std::string temp;
    std::vector<long long> minimalFeasibleCBAs;
    for (long long i = 0; i < feasibleCBAsCount; i++)
    {
        feasibleCBAFile >> temp;
        // no first line or zero vector
        if (i >= 2)
        {
            feasibleCBAs.push_back(binaryToNumber(temp));
        }
    }

    feasibleCBAFile.close();

    std::ofstream minimalFeasibleCBAFile;
    minimalFeasibleCBAFile.open("txt/minimalfeasibleCBA" + std::to_string(len) + ".txt");
    minimalFeasibleCBAFile << "MinimalfeasibleCBA:" << std::endl;

    for (int i = 0; i < feasibleCBAs.size(); i++)
    {
        for (int j = 0; j < feasibleCBAs.size(); j++)
        {
            if (j != i && (feasibleCBAs[i] | feasibleCBAs[j]) == feasibleCBAs[i])
                break;
            if (j + 1 == feasibleCBAs.size())
                minimalFeasibleCBAs.push_back(feasibleCBAs[i]);
        }
    }
    minimalFeasibleCBAFile << "Count: " << minimalFeasibleCBAs.size() << std::endl;
    for (int i = 0; i < minimalFeasibleCBAs.size(); i++)
    {
        std::string sol = "";
        long long toCheck = minimalFeasibleCBAs[i];
        for (int j = 0; j < riaCount; j++)
        {
            sol = (char)('0' + toCheck % 2) + sol;
            toCheck /= 2;
        }
        minimalFeasibleCBAFile << sol << std::endl;
    }
    minimalFeasibleCBAFile.close();
}
