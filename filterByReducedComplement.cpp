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
const long long allOnes = 2147483647; // 2^31-1

std::vector<long long> feasibleCBAs;
std::vector<long long> reducedFeasibleCBAs;
std::set<long long> reducedComplements;

std::string numberToBinary(long long num)
{
    std::string sol = "";
    long long toCheck = num;
    for (int j = 0; j < 31; j++)
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
    feasibleCBAFile.open("feasibleCBA.txt");
    std::string temp;
    std::vector<long long> minimalFeasibleCBAs;
    for (long long i = 0; i < 70986; i++)
    {
        feasibleCBAFile >> temp;
        if (i != 0)
        {
            feasibleCBAs.push_back(binaryToNumber(temp));
        }
    }
    // removing zero vector
    feasibleCBAs.erase(feasibleCBAs.begin());

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
        }
    }

    std::ofstream reducedComplementCBAFile;
    reducedComplementCBAFile.open("reducedComplementCBA.txt");
    reducedComplementCBAFile << "ReducedComplementCBA:" << std::endl;
    reducedComplementCBAFile << "Count: " << reducedFeasibleCBAs.size() << std::endl;
    reducedComplementCBAFile << "feasibleCBA | complement | reduced complement" << std::endl;
    for (int i = 0; i < reducedFeasibleCBAs.size(); i++)
    {
        reducedComplementCBAFile << numberToBinary(reducedFeasibleCBAs[i]);
        reducedComplementCBAFile << " | ";
        reducedComplementCBAFile << numberToBinary(allOnes - reducedFeasibleCBAs[i]);
        reducedComplementCBAFile << " | ";
        reducedComplementCBAFile << numberToBinary(getReducedComplement(reducedFeasibleCBAs[i])) << std::endl;
    }
    reducedComplementCBAFile.close();
}
