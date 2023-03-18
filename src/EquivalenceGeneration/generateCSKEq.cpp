#include "./../CBA/colouring_bit_array_canon.hpp"
#include "./../CBA/kempe_closed.hpp"
#include "./../helper.hpp"
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
const int csEqCount = getCsEqCount(len);
const int cEqCount = getCEqCount(len);
const int riaCount = colouring_bit_array_internal::Comparator(len).len; // relevant_indices_absolute
const long long allOnes = (1 << riaCount) - 1;                          // 2^ria-1

std::vector<long long> csEq;
std::vector<long long> cEq;
std::vector<long long> reducedCsEq;
std::set<long long> reducedComplements;

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

    std::string bestStr = numberToBinary(best, riaCount);
    // c -> cs
    std::vector<uint_fast8_t> temp;
    for (int i = 0; i < riaCount; i++)
        temp.push_back(bestStr[i] - '0');
    temp = canonize(from_canon(temp, len), len);

    std::string cs = "";
    for (int i = 0; i < riaCount; i++)
        cs += temp[i] + '0';

    return binaryToNumber(cs);
}

int main()
{
    csEq = getCsEq(len, true);
    // true or false???
    cEq = getCEq(len, false);

    // canons
    std::ofstream canonsFile;
    canonsFile.open("txt/canons" + std::to_string(len) + ".txt");

    std::map<long long, long long> reducedComplementToIndex;

    // finding reducedCsEq
    for (long long i = 0; i < csEq.size(); i++)
    {
        if (i % 1000 == 0)
        {
            std::cout << i << std::endl;
        }
        long long reducedComplement = getReducedComplement(csEq[i]);
        if (!reducedComplements.count(reducedComplement))
        {
            reducedCsEq.push_back(csEq[i]);
            reducedComplements.insert(reducedComplement);
            reducedComplementToIndex[reducedComplement] = i;
        }
        canonsFile << numberToBinary(csEq[i], riaCount) << ", id:"
                   << numberToBinary(csEq[reducedComplementToIndex[reducedComplement]], riaCount) << std::endl;
    }
    canonsFile.close();

    std::ofstream reducedComplementCBAFile;
    std::ofstream reducedComplementCBARawFile;
    std::ofstream foundCSKFile;
    reducedComplementCBAFile.open("txt/reducedComplementCBA/reducedComplementCBA" + std::to_string(len) + ".txt");
    reducedComplementCBARawFile.open("txt/reducedComplementCBA/reducedComplementCBARaw" + std::to_string(len) + ".txt");
    foundCSKFile.open("txt/foundCSK/foundCSK" + std::to_string(len) + ".txt");
    reducedComplementCBAFile << "ReducedComplementCBA:" << std::endl;
    reducedComplementCBAFile << "Count: " << reducedCsEq.size() << std::endl;
    reducedComplementCBAFile << "csEqCBA | complement | reduced complement" << std::endl;
    for (int i = 0; i < reducedCsEq.size(); i++)
    {
        reducedComplementCBARawFile << numberToBinary(reducedCsEq[i], riaCount) << std::endl;
        foundCSKFile << numberToBinary(reducedCsEq[i], riaCount) << " 0" << std::endl;
        reducedComplementCBAFile << numberToBinary(reducedCsEq[i], riaCount);
        reducedComplementCBAFile << " | ";
        reducedComplementCBAFile << numberToBinary(allOnes - reducedCsEq[i], riaCount);
        reducedComplementCBAFile << " | ";
        reducedComplementCBAFile << numberToBinary(getReducedComplement(reducedCsEq[i]), riaCount) << std::endl;
    }
    reducedComplementCBAFile.close();
    reducedComplementCBARawFile.close();
}
