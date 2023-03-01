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
#include <chrono>
#include <thread>

using namespace ba_graph;

const std::vector<std::vector<uint8_t>> riaArr7 = colouring_bit_array_internal::Comparator(7).relevant_indices_absolute;
const std::vector<std::vector<uint8_t>> riaArr6 = colouring_bit_array_internal::Comparator(6).relevant_indices_absolute;
const std::vector<std::vector<uint8_t>> riaArr5 = colouring_bit_array_internal::Comparator(5).relevant_indices_absolute;
const std::vector<std::vector<uint8_t>> riaArr4 = colouring_bit_array_internal::Comparator(4).relevant_indices_absolute;

std::map<std::string, bool> foundCsMap;
std::map<std::string, bool> foundCskMap;
std::map<std::string, std::string> canonsMap;

std::vector<long long> cskEq6;
std::vector<long long> cskEq5;
std::vector<long long> cskEq4;

std::map<std::vector<uint8_t>, int> toRia7;

std::vector<std::vector<uint8_t>> perm = {{0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}};

// 3 ways to connect
std::vector<std::vector<uint8_t>> connectWays = {
    // 6x6
    {2, 1, 1, 0, 0, 0},
    {2, 2, 2, 1, 0, 0},
    {2, 2, 2, 2, 2, 0},
    // 2 - connect edges of different colour and add a new vertex + edge
    // 1 - connect edges of same colour
    // 0 - dangling edges
    // 3 - missing edge
};

// assumes cba is in c-equivalence
std::string cToCsEq(std::string c)
{
    // c -> cs
    std::vector<uint_fast8_t> temp;
    for (int i = 0; i < riaArr7.size(); i++)
        temp.push_back(c[i] - '0');
    temp = canonize(from_canon(temp, 7), 7);
    std::string cs = "";
    for (int i = 0; i < riaArr7.size(); i++)
        cs += temp[i] + '0';
    return cs;
}

std::vector<uint8_t> recolour(std::vector<uint8_t> tuple, int permIndex)
{
    std::vector<uint8_t> newTuple;
    for (int i = 0; i < tuple.size(); i++)
        newTuple.push_back(perm[permIndex][tuple[i]]);
    return newTuple;
}

int countOnes(std::string cba7)
{
    int count = 0;
    for (int i = 0; i < cba7.size(); i++)
        count += cba7[i] == '1';
    return count;
}

int main()
{
    getCanonsFromFile(6, canonsMap);
    getFoundCskMapFromFile(6, foundCskMap);
    cskEq6 = getCskEq(6, true);
    cskEq5 = getCskEq(5, true);
    cskEq4 = getCskEq(4, true);

    calculateToRia(toRia7, 7);

    std::vector<std::string> foundCs7AsStrings;
    std::vector<std::string> foundCsk6AsStrings;
    std::vector<std::string> foundCsk5AsStrings;
    std::vector<std::string> foundCsk4AsStrings;

    std::vector<std::string> allCsk6AsStrings;
    for (long long i = 0; i < (long long)cskEq6.size(); i++)
    {
        std::string cskString = numberToBinary(cskEq6[i], riaArr6.size());
        allCsk6AsStrings.push_back(cskString);
        if (foundCskMap[cskString])
            foundCsk6AsStrings.push_back(cskString);
    }

    for (long long i = 0; i < (long long)cskEq5.size(); i++)
        foundCsk5AsStrings.push_back(numberToBinary(cskEq5[i], riaArr5.size()));

    for (long long i = 0; i < (long long)cskEq4.size(); i++)
        foundCsk4AsStrings.push_back(numberToBinary(cskEq4[i], riaArr4.size()));

    int count = 0;
    std::vector<uint8_t> tuple1;
    std::vector<uint8_t> tuple2;
    std::vector<uint8_t> createdTuple;
    std::cout << "initial size: " << foundCsk6AsStrings.size() << std::endl;
    for (long long i = 1; i < foundCsk6AsStrings.size(); i++)
    {
        std::cout << i << "/" << foundCsk6AsStrings.size() << std::endl;

        // whether we connect with 4-pole, 5-pole or 6-pole
        for (int danglingEdges = 6; danglingEdges <= 6; danglingEdges++)
        {
            int cbaCount = (danglingEdges == 6 ? (i + 1) : (danglingEdges == 5 ? cskEq5.size() : cskEq4.size()));
            int connectWaysBegin = (danglingEdges == 6 ? 0 : (danglingEdges == 5 ? 0 : -1));
            int connectWaysEnd = (danglingEdges == 6 ? 2 : (danglingEdges == 5 ? 2 : -1));

            for (long long j = 1; j < cbaCount; j++)
            {
                std::string cba2 = (danglingEdges == 6 ? foundCsk6AsStrings[j] : (danglingEdges == 5 ? foundCsk5AsStrings[j] : foundCsk4AsStrings[j]));

                for (int connectWay = connectWaysBegin; connectWay <= connectWaysEnd; connectWay++)
                {
                    std::vector<uint8_t> connectOrder = {0, 1, 2, 3, 4, 5};
                    while (1)
                    {
                        // std::cout << (int)connectOrder[0] << (int)connectOrder[1] << (int)connectOrder[2] << (int)connectOrder[3] << (int)connectOrder[4] << (int)connectOrder[5] << std::endl;
                        std::string sol = "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"; // 91 zeroes

                        // we will check all boundary colourings
                        for (long long ria1 = 0; ria1 < riaArr6.size(); ria1++)
                        {
                            // sleep(1);
                            if (foundCsk6AsStrings[i][ria1] == '0')
                                continue;

                            tuple1 = riaArr6[ria1];

                            // how to permutate colours in ria2
                            for (long long ria2Recolor = 0; ria2Recolor < 6; ria2Recolor++)
                            {
                                std::vector<std::vector<uint8_t>> riaArr = (danglingEdges == 6 ? riaArr6 : (danglingEdges == 5 ? riaArr5 : riaArr4));
                                // check all ria2 colours
                                for (long long ria2 = 0; ria2 < riaArr.size(); ria2++)
                                {
                                    if (cba2[ria2] == '0')
                                        continue;

                                    bool joinValid = true;

                                    // std::cout << int(riaArr[ria2][0]) << int(riaArr[ria2][1]) << int(riaArr[ria2][2]) << int(riaArr[ria2][3]) << std::endl;

                                    tuple2 = recolour(riaArr[ria2], ria2Recolor);

                                    // std::cout << int(tuple1[0]) << int(tuple1[1]) << int(tuple1[2]) << int(tuple1[3]) << int(tuple1[4]) << int(tuple1[5]) << std::endl;
                                    // std::cout << int(tuple2[0]) << int(tuple2[1]) << int(tuple2[2]) << int(tuple2[3]) << std::endl;

                                    createdTuple = {};
                                    for (int l = 0; joinValid && l < 6; l++)
                                    {
                                        if (connectWays[connectWay][l] == 0)
                                        {
                                            // std::cout << 2 << std::endl;
                                            createdTuple.push_back(tuple1[l]);
                                            createdTuple.push_back(tuple2[connectOrder[l]]);
                                        }
                                        else if (connectWays[connectWay][l] == 1)
                                        {
                                            // std::cout << 0 << std::endl;
                                            joinValid = tuple1[l] == tuple2[connectOrder[l]];
                                        }
                                        else if (connectWays[connectWay][l] == 2)
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
                                    sol[toRia7[createdTuple]] = '1';
                                }
                            }
                        }

                        std::string cs = cToCsEq(sol);

                        if (!foundCsMap[cs])
                        {
                            if (countOnes(cs) <= 30)
                            {
                                foundCsMap[cs] = true;
                                count++;
                                std::cout << count << ". sol found!!! " << cs << std::endl;
                                std::cout << i << " " << j << std::endl;
                                foundCs7AsStrings.push_back(cs);
                            }
                        }

                        next_permutation(connectOrder.begin(), connectOrder.end() - 6 + danglingEdges);
                        if (connectOrder == std::vector<uint8_t>{0, 1, 2, 3, 4, 5})
                            break;
                    }
                }
            }
        }
    }

    // write foundCS
    std::ofstream foundCSFileOutput;
    foundCSFileOutput.open("txt/foundCS" + std::to_string(7) + ".txt");
    for (long long i = 0; i < foundCs7AsStrings.size(); i++)
    {
        foundCSFileOutput << foundCs7AsStrings[i] << "\n";
    }
    foundCSFileOutput << "found " << foundCs7AsStrings.size() << "\n";
    foundCSFileOutput.close();
}
