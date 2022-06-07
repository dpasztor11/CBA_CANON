#include "colouring_bit_array_canon.hpp"
#include "kempe_closed.hpp"
#include <iostream>
#include <algorithm>
#include <utility>
#include <execution>
#include <fstream>
#include <set>

using namespace ba_graph;

const int len = 6;
// works for len == 6, 5 or 4
const long long reducedComplementCBACount = len == 6 ? 843 : (len == 5 ? 7 : 4);
const int riaCount = colouring_bit_array_internal::Comparator(len).len; // relevant_indices_absolute

std::string numberToBinary(long long num)
{
    std::string sol = "";
    long long toCheck = num;
    for (int j = 0; j < riaCount; j++)
    {
        sol = (char)('0' + toCheck % 2) + sol;
        toCheck /= 2;
    }
    return sol;
}

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
    // getting reducedComplementCBAs from file
    std::ifstream reducedComplementCBAFile;
    reducedComplementCBAFile.open("txt/reducedComplementCBARaw" + std::to_string(len) + ".txt");
    std::string temp;
    std::vector<long long> reducedComplementCBAs;
    for (long long i = 0; i < reducedComplementCBACount; i++)
    {
        reducedComplementCBAFile >> temp;
        reducedComplementCBAs.push_back(binaryToNumber(temp));
    }
    reducedComplementCBAFile.close();

    std::ofstream posetFile;
    posetFile.open("txt/poset" + std::to_string(len) + ".txt");

    std::vector<std::vector<long long>> posetCBAs;
    bool first = true;
    for (long long i = 0; i < reducedComplementCBACount; i++)
    {
        posetCBAs.push_back({});
        first = true;
        for (long long j = 0; j < reducedComplementCBACount; j++)
        {
            if (i == j)
                continue;
            // j-th is subset of i-th
            if ((reducedComplementCBAs[i] | reducedComplementCBAs[j]) == reducedComplementCBAs[i])
            {
                if (first)
                {
                    posetFile << numberToBinary(reducedComplementCBAs[i]) << ":\n";
                    first = false;
                }
                posetCBAs[i].push_back(j);
                posetFile << "  " << numberToBinary(reducedComplementCBAs[j]) << "\n";
            }
        }
    }
    posetFile.close();
}
