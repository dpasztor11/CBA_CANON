#include "colouring_bit_array_canon.hpp"
#include "kempe_closed.hpp"
#include <iostream>
#include <algorithm>
#include <utility>
#include <execution>
#include <fstream>
#include <set>
#include <map>

using namespace ba_graph;

const int len = 6;
// works for len == 6, 5 or 4
const int feasibleCBAsCount = len == 6 ? 70986 : (len == 5 ? 16 : 6);
const int riaCount = colouring_bit_array_internal::Comparator(len).len; // relevant_indices_absolute
const long long allOnes = (1 << riaCount) - 1;                          // 2^ria-1

std::vector<long long> feasibleCBAs;
std::vector<long long> reducedFeasibleCBAs;
std::set<long long> reducedComplements;

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

long long getReducedComplement(long long num)
{
    num = allOnes - num;
    long long best = 0;
    for (int i = 0; i < feasibleCBAs.size(); i++)
    {
        if ((num | feasibleCBAs[i]) == num && feasibleCBAs[i] > best)
        {
            best = feasibleCBAs[i];
        }
    }
    return best;
}

bool compareByReducedComplement(long long a, long long b)
{
    return getReducedComplement(a) == getReducedComplement(b);
}

int main()
{
    // getting feasibleCBAs from file
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

    // canons
    std::ofstream canonsFile;
    canonsFile.open("txt/canons" + std::to_string(len) + ".txt");

    std::map<long long, long long> reducedComplementToIndex;

    // finding reducedFeasibleCBAs
    for (long long i = 0; i < feasibleCBAs.size(); i++)
    {
        if (i % 10000 == 0)
        {
            std::cout << i << std::endl;
        }
        long long reducedComplement = getReducedComplement(feasibleCBAs[i]);
        if (!reducedComplements.count(reducedComplement))
        {
            reducedFeasibleCBAs.push_back(feasibleCBAs[i]);
            reducedComplements.insert(reducedComplement);
            reducedComplementToIndex[reducedComplement] = i;
        }
        canonsFile << numberToBinary(feasibleCBAs[i]) << ", id:"
                   << numberToBinary(feasibleCBAs[reducedComplementToIndex[reducedComplement]]) << std::endl;
    }
    canonsFile.close();

    std::ofstream reducedComplementCBAFile;
    std::ofstream reducedComplementCBARawFile;
    reducedComplementCBAFile.open("txt/reducedComplementCBA" + std::to_string(len) + ".txt");
    reducedComplementCBARawFile.open("txt/reducedComplementCBARaw" + std::to_string(len) + ".txt");
    reducedComplementCBAFile << "ReducedComplementCBA:" << std::endl;
    reducedComplementCBAFile << "Count: " << reducedFeasibleCBAs.size() << std::endl;
    reducedComplementCBAFile << "feasibleCBA | complement | reduced complement" << std::endl;
    for (int i = 0; i < reducedFeasibleCBAs.size(); i++)
    {
        reducedComplementCBARawFile << numberToBinary(reducedFeasibleCBAs[i]) << std::endl;
        reducedComplementCBAFile << numberToBinary(reducedFeasibleCBAs[i]);
        reducedComplementCBAFile << " | ";
        reducedComplementCBAFile << numberToBinary(allOnes - reducedFeasibleCBAs[i]);
        reducedComplementCBAFile << " | ";
        reducedComplementCBAFile << numberToBinary(getReducedComplement(reducedFeasibleCBAs[i])) << std::endl;
    }
    reducedComplementCBAFile.close();
    reducedComplementCBARawFile.close();
}
