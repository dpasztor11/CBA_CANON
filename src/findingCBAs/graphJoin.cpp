#include "./../CBA/colouring_bit_array_canon.hpp"
// #include "./../CBA/kempe_closed.hpp"
#include "./../helper.hpp"
#include <iostream>
#include <algorithm>
#include <utility>
#include <execution>
#include <fstream>
#include <set>
#include <map>
#include <chrono>
#include <thread>
#include <tuple>
#include <vector>

using namespace ba_graph;

const int maxOnesIn7Pole = 25;

const std::vector<std::vector<uint8_t>> riaArr7 = colouring_bit_array_internal::Comparator(7).relevant_indices_absolute;
const std::vector<std::vector<uint8_t>> riaArr6 = colouring_bit_array_internal::Comparator(6).relevant_indices_absolute;
const std::vector<std::vector<uint8_t>> riaArr5 = colouring_bit_array_internal::Comparator(5).relevant_indices_absolute;
const std::vector<std::vector<uint8_t>> riaArr4 = colouring_bit_array_internal::Comparator(4).relevant_indices_absolute;

std::map<std::string, bool> foundCsk6Map;
std::set<std::string> foundCsk7Set;
std::set<std::string> foundC7Set;
std::set<std::string> foundC6Set;
std::map<std::string, std::string> csToCsk6CanonsMap;

std::vector<long long> cskEq6;
std::vector<long long> cskEq5;
std::vector<long long> cskEq4;

std::map<std::vector<uint8_t>, int> toRia6;
std::map<std::vector<uint8_t>, int> toRia7;

std::vector<std::vector<uint8_t>> perm = {{0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}};

// vector of all ways we can connect a-pole and b-pole to get c-pole
std::map<std::tuple<int, int, int>, std::vector<std::vector<uint8_t>>> connectWaysMap;

std::vector<std::string> foundCsk7AsStrings;
std::vector<std::string> foundCsk6AsStrings;
std::vector<std::string> foundCsk5AsStrings;
std::vector<std::string> foundCsk4AsStrings;

int count6 = 0;
int count7 = 0;

// assumes cba is in c-equivalence
std::string cToCskEq6(std::string c)
{
    // c -> cs
    std::vector<uint_fast8_t> temp;
    for (int i = 0; i < riaArr6.size(); i++)
        temp.push_back(c[i] - '0');
    temp = canonize(from_canon(temp, 6), 6);
    std::string cs = "";
    for (int i = 0; i < riaArr6.size(); i++)
        cs += temp[i] + '0';

    // cs -> csk
    return csToCsk6CanonsMap[cs];
}
/*
ColouringBitArray stringToCBA(std::string str)
{
    binaryToNumber(str);
}

// assumes cba is in c-equivalence
std::string cToCskEq7(std::string c)
{
    // c -> cs
    std::vector<uint_fast8_t> temp;
    for (int i = 0; i < riaArr7.size(); i++)
        temp.push_back(c[i] - '0');
    temp = canonize(from_canon(temp, 7), 7);
    std::string cs = "";
    for (int i = 0; i < riaArr6.size(); i++)
        cs += temp[i] + '0';

    // cs -> csk
    return CBAToString(getCBA7ReducedComplement(stringToCBA(cs)));
}
*/

// assumes cba is in c-equivalence
std::string cToCskEq7(std::string c)
{
    // c -> cs
    std::vector<uint_fast8_t> temp;
    for (int i = 0; i < riaArr7.size(); i++)
        temp.push_back(c[i] - '0');
    temp = canonize(from_canon(temp, 7), 7);
    std::string cs = "";
    for (int i = 0; i < riaArr7.size(); i++)
        cs += temp[i] + '0';

    return c;
}

int countOnes(std::string str)
{
    int count = 0;
    for (int i = 0; i < str.size(); i++)
    {
        count += str[i] == '1';
    }
    return count;
}

std::vector<uint8_t> recolour(std::vector<uint8_t> tuple, int permIndex)
{
    std::vector<uint8_t> newTuple;
    for (int i = 0; i < tuple.size(); i++)
        newTuple.push_back(perm[permIndex][tuple[i]]);
    return newTuple;
}

// CBA1 is a (len1)-pole, CBA2 is a (len2)-pole
// we want to try all ways to connect them to (len3)-pole
void connectCBAs(std::string cba1, int len1, std::string cba2, int len2, int len3)
{
    std::vector<uint8_t> tuple1;
    std::vector<uint8_t> tuple2;
    std::vector<uint8_t> createdTuple;
    auto riaArr = colouring_bit_array_internal::Comparator(len1).relevant_indices_absolute;
    auto riaArr2 = colouring_bit_array_internal::Comparator(len2).relevant_indices_absolute;
    auto riaArr3 = colouring_bit_array_internal::Comparator(len3).relevant_indices_absolute;
    for (auto connectWay : connectWaysMap[std::make_tuple(len1, len2, len3)])
    {
        std::vector<uint8_t> connectOrder;
        std::vector<uint8_t> start;
        for (int i = 0; i < len1; i++)
            connectOrder.push_back(i);
        start = connectOrder;
        while (1)
        {
            // std::cout << (int)connectOrder[0] << (int)connectOrder[1] << (int)connectOrder[2] << (int)connectOrder[3] << (int)connectOrder[4] << (int)connectOrder[5] << std::endl;
            auto toRia = (len3 == 6 ? toRia6 : toRia7);

            std::string sol = "";
            for (int i = 0; i < riaArr3.size(); i++)
                sol += "0";

            // we will check all boundary colourings
            for (long long ria1 = 0; ria1 < riaArr.size(); ria1++)
            {
                // sleep(1);
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

                        // std::cout << int(riaArr[ria2][0]) << int(riaArr[ria2][1]) << int(riaArr[ria2][2]) << int(riaArr[ria2][3]) << std::endl;

                        tuple2 = recolour(riaArr2[ria2], ria2Recolor);

                        // std::cout << int(tuple1[0]) << int(tuple1[1]) << int(tuple1[2]) << int(tuple1[3]) << int(tuple1[4]) << int(tuple1[5]) << std::endl;
                        // std::cout << int(tuple2[0]) << int(tuple2[1]) << int(tuple2[2]) << int(tuple2[3]) << std::endl;

                        createdTuple = {};
                        for (int l = 0; joinValid && l < len1; l++)
                        {
                            if (connectWay[l] == 0)
                            {
                                // std::cout << 2 << std::endl;
                                createdTuple.push_back(tuple1[l]);
                                createdTuple.push_back(tuple2[connectOrder[l]]);
                            }
                            else if (connectWay[l] == 1)
                            {
                                // std::cout << 0 << std::endl;
                                joinValid = tuple1[l] == tuple2[connectOrder[l]];
                            }
                            else if (connectWay[l] == 2)
                            {
                                // std::cout << 1 << std::endl;
                                joinValid = tuple1[l] != tuple2[connectOrder[l]];
                                createdTuple.push_back(3 - tuple1[l] - tuple2[connectOrder[l]]);
                            }
                            else
                            {
                                // std::cout << 1000 << std::endl;
                                createdTuple.push_back(tuple1[l]);
                            }
                        }
                        // std::cout << (joinValid ? "true" : "false") << std::endl;
                        if (!joinValid)
                            continue;

                        // std::cout << int(createdTuple[0]) << int(createdTuple[1]) << int(createdTuple[2]) << int(createdTuple[3]) << int(createdTuple[4]) << int(createdTuple[5]) << std::endl;
                        sol[toRia[createdTuple]] = '1';
                    }
                }
            }

            if (len3 == 6)
            {
                if (foundC6Set.count(sol) == 0)
                {
                    foundC6Set.insert(sol);
                    std::string csk = cToCskEq6(sol);
                    if (!foundCsk6Map[csk])
                    {
                        foundCsk6Map[csk] = true;
                        count6++;
                        std::cout << count6 << ". 6 sol found!!! " << csk << std::endl;
                        foundCsk6AsStrings.push_back(csk);
                    }
                }
            }
            else if (len3 == 7)
            {
                // right now only does c -> cs
                if (foundC7Set.count(sol) == 0)
                {
                    foundC7Set.insert(sol);
                    std::string csk = cToCskEq7(sol);
                    if (foundCsk7Set.count(csk) == 0 && countOnes(csk) <= maxOnesIn7Pole)
                    {
                        foundCsk7Set.insert(csk);
                        count7++;
                        // std::cout << count7 << ". 7 sol found!!! " << csk << std::endl;
                        foundCsk7AsStrings.push_back(csk);
                    }
                }
            }
            else
            {
                std::cerr << "not implemented\n";
            }

            next_permutation(connectOrder.begin(), connectOrder.begin() + len2);
            /*
            if (connectOrder[0] == 0 && connectOrder[1] == 1 && connectOrder[2] == 2 && connectOrder[3] == 3)
            {
                for (const auto &num : connectOrder)
                {
                    std::cout << static_cast<int>(num) << " ";
                }
                std::cout << std::endl;
            }
            */
            if (connectOrder == start)
                break;
        }
    }
}

void connectWithAll4And5Poles(std::string cba, int len)
{
    // std::cout << "connecting " << cba << " with all 4,5poles\n";
    for (int i = 0; i < foundCsk4AsStrings.size(); i++)
    {
        connectCBAs(cba, len, foundCsk4AsStrings[i], 4, 6);
        connectCBAs(cba, len, foundCsk4AsStrings[i], 4, 7);
    }
    for (int i = 0; i < foundCsk5AsStrings.size(); i++)
    {
        connectCBAs(cba, len, foundCsk5AsStrings[i], 5, 6);
        connectCBAs(cba, len, foundCsk5AsStrings[i], 5, 7);
    }
}

// i -> index of last 6pole
// j -> index of last 7pole
void connectWithPrevious6And7Poles(std::string cba, int len, int i, int j)
{
    for (int k = 0; k <= i; k++)
    {
        // std::cout << "aaa\n";
        connectCBAs(cba, len, foundCsk6AsStrings[k], 6, 6);
        // std::cout << "aaa\n";
        connectCBAs(cba, len, foundCsk6AsStrings[k], 6, 7);
    }
    for (int k = 0; k <= j; k++)
    {
        // std::cout << "aaa\n";
        connectCBAs(foundCsk7AsStrings[k], 7, cba, len, 6);
        // std::cout << "aaa\n";
        connectCBAs(foundCsk7AsStrings[k], 7, cba, len, 7);
    }
}

void printProgress(int i, int j)
{
    std::cout << "6-poles: " << i << "/" << foundCsk6AsStrings.size() << std::endl;
    std::cout << "7-poles: " << j << "/" << foundCsk7AsStrings.size() << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
}

int main()
{
    getCanonsFromFile(6, csToCsk6CanonsMap);
    getFoundCskMapFromFile(6, foundCsk6Map);
    cskEq6 = getCskEq(6, true);
    cskEq5 = getCskEq(5, true);
    cskEq4 = getCskEq(4, true);
    calculateToRia(toRia6, 6);
    calculateToRia(toRia7, 7);

    connectWaysMap = initConnectWaysMap();
    /*
        // Iterate over the map and print the contents
        for (const auto &[key, value] : connectWaysMap)
        {
            std::cout << "Key: (" << std::get<0>(key) << ", " << std::get<1>(key) << ", " << std::get<2>(key) << ")" << std::endl;
            std::cout << "Value: " << std::endl;
            for (const auto &vec : value)
            {
                for (const auto &num : vec)
                {
                    std::cout << static_cast<int>(num) << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
        sleep(100);
    */

    std::vector<std::string> allCsk6AsStrings;
    for (long long i = 0; i < (long long)cskEq6.size(); i++)
    {
        std::string cskString = numberToBinary(cskEq6[i], riaArr6.size());
        allCsk6AsStrings.push_back(cskString);
        if (foundCsk6Map[cskString])
            foundCsk6AsStrings.push_back(cskString);
    }

    for (long long i = 0; i < (long long)cskEq5.size(); i++)
        foundCsk5AsStrings.push_back(numberToBinary(cskEq5[i], riaArr5.size()));

    for (long long i = 0; i < (long long)cskEq4.size(); i++)
        foundCsk4AsStrings.push_back(numberToBinary(cskEq4[i], riaArr4.size()));

    foundCsk7AsStrings = {};

    std::cout << "initial size: " << foundCsk6AsStrings.size() << std::endl;
    int j = 0;
    for (long long i = 1; i < foundCsk6AsStrings.size(); i++)
    {
        printProgress(i, j);
        connectWithAll4And5Poles(foundCsk6AsStrings[i], 6);
        connectWithPrevious6And7Poles(foundCsk6AsStrings[i], 6, i, j);
        if (i + 1 == foundCsk6AsStrings.size())
        {
            // mali by sme koncit, tak vyskusame povytvarat 7polovy uzaver
            std::cout << "!!!switching to 7-poles!!!" << std::endl;
            printProgress(i, j);
            for (; j < foundCsk7AsStrings.size(); j++)
            {
                connectWithAll4And5Poles(foundCsk7AsStrings[j], 7);
                connectWithPrevious6And7Poles(foundCsk7AsStrings[j], 7, i, j);
            }
        }
    }

    // write foundCSK
    std::ofstream foundCSKFileOutput;
    foundCSKFileOutput.open("txt/foundCSK" + std::to_string(6) + ".txt");
    for (long long i = 0; i < allCsk6AsStrings.size(); i++)
    {
        foundCSKFileOutput << allCsk6AsStrings[i] << " " << foundCsk6Map[allCsk6AsStrings[i]] << "\n";
    }
    foundCSKFileOutput << "found " << foundCsk6AsStrings.size() << " / " << allCsk6AsStrings.size() << "\n";
    foundCSKFileOutput.close();
}
