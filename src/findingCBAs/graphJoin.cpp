#include "./../CBA/colouring_bit_array_canon.hpp"
#include "./../CBA/kempe_closed.hpp"
#include "./../helper.hpp"
#include <iostream>
#include <algorithm>
#include <utility>
#include <fstream>
#include <set>
#include <map>
#include <chrono>
#include <thread>
#include <tuple>
#include <vector>

#ifndef TEST_MODE
#include <execution>
#endif

using namespace ba_graph;

const int MAX_ONES_IN_7_POLE = 20;
// joining two 7poles to create a 7pole takes a long time
const bool ALLOW_7x7_TO_7 = false;

const std::vector<std::vector<uint8_t>> riaArr7 = colouring_bit_array_internal::Comparator(7).relevant_indices_absolute;
const std::vector<std::vector<uint8_t>> riaArr6 = colouring_bit_array_internal::Comparator(6).relevant_indices_absolute;
const std::vector<std::vector<uint8_t>> riaArr5 = colouring_bit_array_internal::Comparator(5).relevant_indices_absolute;
const std::vector<std::vector<uint8_t>> riaArr4 = colouring_bit_array_internal::Comparator(4).relevant_indices_absolute;

std::map<std::string, bool> isFoundCsk6Map;
std::set<std::string> foundCsk7Set;
std::set<std::string> foundC7Set;
std::set<std::string> foundC6Set;
std::map<std::string, std::string> csToCsk6CanonsMap;

std::vector<long long> cskEq6;
std::vector<long long> cskEq5;
std::vector<long long> cskEq4;

std::map<std::vector<uint_fast8_t>, uint_fast8_t> toRia6;
std::map<std::vector<uint_fast8_t>, uint_fast8_t> toRia7;

// vector of all ways we can connect a-pole and b-pole to get c-pole
std::map<std::tuple<int, int, int>, std::vector<std::vector<uint8_t>>> connectWaysMap;

std::vector<std::string> foundCsk7AsStrings;
std::vector<std::string> foundCsk6AsStrings;
std::vector<std::string> foundCsk5AsStrings;
std::vector<std::string> foundCsk4AsStrings;

int count6 = 0;
int count7 = 0;

int cskCount = 115;

// assumes cba is in c-equivalence
std::string cToCskEq7(std::string c)
{
    std::string cs = cToCs(c);
    std::string csk = getCbaReducedComplement(cs, 7);
    return csk;
}

void log(std::string csk, std::string c, std::string cba1, std::string cba2,
         std::vector<uint_fast8_t> connectWay, std::vector<uint_fast8_t> connectOrder)
{
    ofstream file;
    file.open("txt/foundCSK/CSK6FindingsLogger.txt", std::ios_base::app);
    file << cskCount << ". " << csk << std::endl;
    file << cskCount << " c: " << c << std::endl;
    file << " from: " << cba1 << " and " << cba2 << std::endl;
    file << " connectWay: ";
    printVec(connectWay, file);
    file << " connectOrder: ";
    printVec(connectOrder, file);
}

std::string connectCBAs(std::string cba1, int len1,
                        std::string cba2, int len2, int len3,
                        std::vector<uint_fast8_t> &connectOrder, std::vector<uint_fast8_t> &connectWay)
{
    if (len3 != 6 && len3 != 7)
        throw invalid_argument("Invalid len3 argument");

    auto riaArr = colouring_bit_array_internal::Comparator(len1).relevant_indices_absolute;
    auto riaArr2 = colouring_bit_array_internal::Comparator(len2).relevant_indices_absolute;
    auto riaArr3 = colouring_bit_array_internal::Comparator(len3).relevant_indices_absolute;
    std::vector<uint8_t> tuple1;
    std::vector<uint8_t> tuple2;
    std::vector<uint8_t> createdTuple;
    auto toRia = (len3 == 6 ? toRia6 : toRia7);

    std::string sol = len3 == 6 ? EMPTY_CBA_6 : EMPTY_CBA_7;

    // we will check all boundary colourings
    for (long long ria1 = 0; ria1 < riaArr.size(); ria1++)
    {
        if (cba1[ria1] == '0')
            continue;

        tuple1 = riaArr[ria1];

        // how to permutate colours in ria2
        for (long long ria2Recolor = 0; ria2Recolor < 6; ria2Recolor++)
        {

            // check all ria2 colours
            for (long long ria2 = 0; ria2 < riaArr2.size(); ria2++)
            {
                if (cba2[ria2] == '0')
                    continue;

                bool joinValid = true;

                tuple2 = mapColours(riaArr2[ria2], ria2Recolor);

                createdTuple = {};
                int next = 0;
                for (int l = 0; joinValid && l < len1; l++)
                {
                    // that means it is a dangling edge
                    if (connectOrder[l] >= len2)
                    {
                        createdTuple.push_back(tuple1[l]);
                    }
                    else if (connectWay[next] == 0)
                    {
                        next++;
                        createdTuple.push_back(tuple1[l]);
                        createdTuple.push_back(tuple2[connectOrder[l]]);
                    }
                    else if (connectWay[next] == 1)
                    {
                        next++;
                        joinValid = tuple1[l] == tuple2[connectOrder[l]];
                    }
                    else if (connectWay[next] == 2)
                    {
                        next++;
                        joinValid = tuple1[l] != tuple2[connectOrder[l]];
                        createdTuple.push_back(3 - tuple1[l] - tuple2[connectOrder[l]]);
                    }
                }
                if (!joinValid)
                    continue;

                sol[toRia[createdTuple]] = '1';
            }
        }
    }
    return sol;
}

std::string processFoundCBA(std::string cba, int len3)
{
    if (len3 == 6)
    {
        if (foundC6Set.count(cba) == 0)
        {
            foundC6Set.insert(cba);
            std::string csk = cToCsk(cba, csToCsk6CanonsMap);
            if (!isFoundCsk6Map[csk])
            {
                isFoundCsk6Map[csk] = true;
                count6++;
                foundCsk6AsStrings.push_back(csk);
                return csk;
            }
        }
    }
    else if (len3 == 7)
    {
        if (foundC7Set.count(cba) == 0)
        {
            foundC7Set.insert(cba);
            std::string cs = cToCs(cba, 7);
            if (foundCsk7Set.count(cs) == 0 && countOnes(cs) <= MAX_ONES_IN_7_POLE)
            {
                foundCsk7Set.insert(cs);
                count7++;
                foundCsk7AsStrings.push_back(cs);
            }
        }
    }
    return "";
}

// CBA1 is a (len1)-pole, CBA2 is a (len2)-pole
// we want to try all ways to connect them to (len3)-pole
void connectCBAsAllWays(std::string cba1, int len1, std::string cba2, int len2, int len3)
{
    for (auto connectWay : connectWaysMap[std::make_tuple(len1, len2, len3)])
    {
        std::vector<uint8_t> connectOrder;
        std::vector<uint8_t> start;
        for (int i = 0; i < len1; i++)
            connectOrder.push_back(i);

        start = connectOrder;
        while (1)
        {
            std::string sol = connectCBAs(cba1, len1, cba2, len2, len3, connectOrder, connectWay);
            std::string foundCsk = processFoundCBA(sol, len3);
            if (foundCsk != "")
                log(foundCsk, sol, cba1, cba2, connectWay, connectOrder);

            next_permutation(connectOrder.begin(), connectOrder.end());
            if (connectOrder == start)
                break;
        }
    }
}

void connectWithAll4And5Poles(std::string cba, int len)
{
    for (int i = 0; i < foundCsk4AsStrings.size(); i++)
    {
        connectCBAsAllWays(cba, len, foundCsk4AsStrings[i], 4, 6);
        connectCBAsAllWays(cba, len, foundCsk4AsStrings[i], 4, 7);
    }
    for (int i = 0; i < foundCsk5AsStrings.size(); i++)
    {
        connectCBAsAllWays(cba, len, foundCsk5AsStrings[i], 5, 6);
        connectCBAsAllWays(cba, len, foundCsk5AsStrings[i], 5, 7);
    }
}

void connectWithPrevious6And7Poles(std::string cba, int len, int last6poleIndex, int last7poleIndex)
{
    for (int k = 0; k <= last6poleIndex; k++)
    {
        connectCBAsAllWays(cba, len, foundCsk6AsStrings[k], 6, 6);
        connectCBAsAllWays(cba, len, foundCsk6AsStrings[k], 6, 7);
    }
    for (int k = 0; k <= last7poleIndex; k++)
    {
        connectCBAsAllWays(foundCsk7AsStrings[k], 7, cba, len, 6);
        if (len != 7 || ALLOW_7x7_TO_7)
            connectCBAsAllWays(foundCsk7AsStrings[k], 7, cba, len, 7);
    }
}

void printProgress(int i, int j)
{
    std::cout << "6-poles: " << i << "/" << foundCsk6AsStrings.size() << std::endl;
    std::cout << "7-poles: " << j << "/" << foundCsk7AsStrings.size() << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
}

#ifndef TEST_MODE

int main()
{
    getCanonsFromFile(6, csToCsk6CanonsMap);
    getFoundCskMapFromFile(6, isFoundCsk6Map);
    cskEq6 = getCskEq(6, true);
    cskEq5 = getCskEq(5, true);
    cskEq4 = getCskEq(4, true);
    calculateToRia(toRia6, 6);
    calculateToRia(toRia7, 7);

    connectWaysMap = initConnectWaysMap();

    std::vector<std::string> allCsk6AsStrings;
    for (long long i = 0; i < (long long)cskEq6.size(); i++)
    {
        std::string cskString = longLongCbaToString(cskEq6[i], riaArr6.size());
        allCsk6AsStrings.push_back(cskString);
        if (isFoundCsk6Map[cskString])
            foundCsk6AsStrings.push_back(cskString);
    }

    for (long long i = 0; i < (long long)cskEq5.size(); i++)
        foundCsk5AsStrings.push_back(longLongCbaToString(cskEq5[i], riaArr5.size()));

    for (long long i = 0; i < (long long)cskEq4.size(); i++)
        foundCsk4AsStrings.push_back(longLongCbaToString(cskEq4[i], riaArr4.size()));

    foundCsk7AsStrings = {};

    std::cout << "initial size: " << foundCsk6AsStrings.size() << std::endl;
    int j = 0;
    for (long long i = 1; i < foundCsk6AsStrings.size(); i++)
    {
        printProgress(i, j);
        connectWithAll4And5Poles(foundCsk6AsStrings[i], 6);
        connectWithPrevious6And7Poles(foundCsk6AsStrings[i], 6, i, j - 1);
        if (i + 1 == foundCsk6AsStrings.size())
        {
            std::cout << "!!!switching to 7-poles!!!" << std::endl;
            for (; j < foundCsk7AsStrings.size(); j++)
            {
                printProgress(i, j);
                connectWithAll4And5Poles(foundCsk7AsStrings[j], 7);
                connectWithPrevious6And7Poles(foundCsk7AsStrings[j], 7, i, j);
                if (i + 1 != foundCsk6AsStrings.size())
                {
                    j++;
                    break;
                }
            }
        }
    }

    updateFoundCsk(isFoundCsk6Map, allCsk6AsStrings);
    return 0;
}

#endif
