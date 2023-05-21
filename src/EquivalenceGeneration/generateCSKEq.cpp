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

std::vector<long long> csEq;
std::vector<long long> cEq;
std::vector<long long> reducedCsEq;
std::set<long long> reducedComplements;

int main()
{
    csEq = getCsEq(len, true);
    // true or false???
    cEq = getCEq(len, false);

    // canons
    std::ofstream canonsFile;
    canonsFile.open("data/canons" + std::to_string(len) + ".txt");

    std::map<long long, long long> reducedComplementToIndex;

    // finding reducedCsEq
    for (long long i = 0; i < csEq.size(); i++)
    {
        if (i % 1000 == 0)
        {
            std::cout << i << std::endl;
        }
        long long reducedComplement = getReducedComplementFromCEq(csEq[i], cEq);
        if (!reducedComplements.count(reducedComplement))
        {
            reducedCsEq.push_back(csEq[i]);
            reducedComplements.insert(reducedComplement);
            reducedComplementToIndex[reducedComplement] = i;
        }
        canonsFile << longLongCbaToString(csEq[i], RIA_COUNT_6) << ", id:"
                   << longLongCbaToString(csEq[reducedComplementToIndex[reducedComplement]], RIA_COUNT_6) << std::endl;
    }
    canonsFile.close();

    std::ofstream reducedComplementCBAFile;
    std::ofstream reducedComplementCBARawFile;
    std::ofstream foundCSKFile;
    reducedComplementCBAFile.open("data/reducedComplementCBA/reducedComplementCBA" + std::to_string(len) + ".txt");
    reducedComplementCBARawFile.open("data/reducedComplementCBA/reducedComplementCBARaw" + std::to_string(len) + ".txt");
    foundCSKFile.open("data/foundCSK/foundCSK" + std::to_string(len) + ".txt");
    reducedComplementCBAFile << "ReducedComplementCBA:" << std::endl;
    reducedComplementCBAFile << "Count: " << reducedCsEq.size() << std::endl;
    reducedComplementCBAFile << "csEqCBA | complement | reduced complement" << std::endl;
    for (int i = 0; i < reducedCsEq.size(); i++)
    {
        reducedComplementCBARawFile << longLongCbaToString(reducedCsEq[i]) << std::endl;
        foundCSKFile << longLongCbaToString(reducedCsEq[i]) << " 0" << std::endl;
        reducedComplementCBAFile << longLongCbaToString(reducedCsEq[i]);
        reducedComplementCBAFile << " | ";
        reducedComplementCBAFile << longLongCbaToString(FULL_CBA_6_NUM - reducedCsEq[i]);
        reducedComplementCBAFile << " | ";
        reducedComplementCBAFile << longLongCbaToString(getReducedComplementFromCEq(reducedCsEq[i], cEq)) << std::endl;
    }
    reducedComplementCBAFile.close();
    reducedComplementCBARawFile.close();
}
