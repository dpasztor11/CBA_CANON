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
const int cEqCount = len == 6 ? 44601292 : ((len == 5) ? 314 : 12);
const int riaCount = colouring_bit_array_internal::Comparator(len).len; // relevant_indices_absolute
const long long allOnes = (1 << riaCount) - 1;                          // 2^ria-1

std::vector<long long> feasibleCBAs;
std::vector<long long> cEq;
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
    for (int i = 0; i < cEqCount; i++)
    {
        if ((num | cEq[i]) == num && cEq[i] > best)
        {
            best = cEq[i];
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
    std::ifstream csEqFile;
    csEqFile.open("txt/feasibleCBA" + std::to_string(len) + ".txt");
    std::string temp;

    // no first line
    csEqFile >> temp;
    for (long long i = 0; i < feasibleCBAsCount; i++)
    {
        csEqFile >> temp;
        feasibleCBAs.push_back(binaryToNumber(temp));
    }

    csEqFile.close();

    // getting cEq from file
    std::ifstream cEqFile;
    cEqFile.open("txt/cEq" + std::to_string(len) + ".txt");

    // no first line
    cEqFile >> temp >> temp;
    for (long long i = 0; i < cEqCount; i++)
    {
        cEqFile >> temp;
        cEq.push_back(binaryToNumber(temp));
    }

    cEqFile.close();

    // canons
    std::ofstream canonsFile;
    canonsFile.open("txt/canons" + std::to_string(len) + ".txt");

    std::map<long long, long long> reducedComplementToIndex;

    // finding reducedFeasibleCBAs
    for (long long i = 0; i < feasibleCBAs.size(); i++)
    {
        if (i % 1000 == 0)
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
    std::ofstream foundCSKFile;
    reducedComplementCBAFile.open("txt/reducedComplementCBA" + std::to_string(len) + ".txt");
    reducedComplementCBARawFile.open("txt/reducedComplementCBARaw" + std::to_string(len) + ".txt");
    foundCSKFile.open("txt/foundCSK" + std::to_string(len) + ".txt");
    reducedComplementCBAFile << "ReducedComplementCBA:" << std::endl;
    reducedComplementCBAFile << "Count: " << reducedFeasibleCBAs.size() << std::endl;
    reducedComplementCBAFile << "csEqCBA | complement | reduced complement" << std::endl;
    for (int i = 0; i < reducedFeasibleCBAs.size(); i++)
    {
        reducedComplementCBARawFile << numberToBinary(reducedFeasibleCBAs[i]) << std::endl;
        foundCSKFile << numberToBinary(reducedFeasibleCBAs[i]) << " 0" << std::endl;
        reducedComplementCBAFile << numberToBinary(reducedFeasibleCBAs[i]);
        reducedComplementCBAFile << " | ";
        reducedComplementCBAFile << numberToBinary(allOnes - reducedFeasibleCBAs[i]);
        reducedComplementCBAFile << " | ";
        reducedComplementCBAFile << numberToBinary(getReducedComplement(reducedFeasibleCBAs[i])) << std::endl;
    }
    reducedComplementCBAFile.close();
    reducedComplementCBARawFile.close();
}
