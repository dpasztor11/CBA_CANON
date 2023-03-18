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
#include <tuple>
#include <vector>

using namespace ba_graph;

const std::vector<std::vector<uint8_t>> riaArr6 = colouring_bit_array_internal::Comparator(6).relevant_indices_absolute;

std::map<std::string, bool> foundCsk6Map;
std::set<std::string> foundC6Set;
std::map<std::string, std::string> csToCsk6CanonsMap;

std::vector<long long> cskEq6;
std::map<std::vector<uint8_t>, int> toRia6;

std::vector<std::vector<uint8_t>> perm = {{0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}};
std::vector<std::string> foundCsk6AsStrings;

int count6 = 0;

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

std::vector<uint8_t> recolour(std::vector<uint8_t> tuple, int permIndex)
{
    std::vector<uint8_t> newTuple;
    for (int i = 0; i < tuple.size(); i++)
        newTuple.push_back(perm[permIndex][tuple[i]]);
    return newTuple;
}

std::vector<uint8_t> permutate(std::vector<uint8_t> vec, std::vector<uint8_t> perm)
{
    std::vector<uint8_t> sol;
    for (int i = 0; i < vec.size(); i++)
    {
        sol.push_back(vec[perm[i]]);
    }
    return sol;
}

void checkCycle3(std::vector<std::string> cbas, std::vector<uint8_t> perm6)
{
    if (cbas.size() != 3)
        return;

    std::string sol = "0000000000000000000000000000000";

    std::vector<uint8_t> tuple1;
    std::vector<uint8_t> tuple2;
    std::vector<uint8_t> tuple3;
    std::vector<uint8_t> newTuple;

    for (int i = 0; i < 31; i++)
    {
        if (cbas[0][i] != '1')
            continue;
        tuple1 = permutate(riaArr6[i], perm6);
        for (int j = 0; j < 31; j++)
        {
            if (cbas[1][j] != '1')
                continue;
            for (int perm1 = 0; perm1 < 6; perm1++)
            {
                tuple2 = recolour(riaArr6[j], perm1);
                if (tuple2[0] != tuple1[0] || tuple2[1] != tuple1[1])
                    continue;
                for (int k = 0; k < 31; k++)
                {
                    if (cbas[2][k] != '1')
                        continue;
                    for (int perm2 = 0; perm2 < 6; perm2++)
                    {
                        tuple3 = recolour(riaArr6[k], perm2);
                        if (tuple3[0] != tuple2[5] ||
                            tuple3[1] != tuple2[4] ||
                            tuple3[4] != tuple1[5] ||
                            tuple3[5] != tuple1[4])
                            continue;

                        newTuple = {tuple1[2], tuple1[3],
                                    tuple2[2], tuple2[3],
                                    tuple3[2], tuple3[3]};

                        sol[toRia6[newTuple]] = '1';
                    }
                }
            }
        }
    }

    if (foundC6Set.count(sol) == 0)
    {
        foundC6Set.insert(sol);
        std::string csk = cToCskEq6(sol);
        if (!foundCsk6Map[csk])
        {
            foundCsk6Map[csk] = true;
            count6++;
            std::cout << count6 << ". 6 sol found!!! " << csk << std::endl;
            std::cout << "from " << sol << std::endl;
            foundCsk6AsStrings.push_back(csk);
        }
    }
}

bool twoBeforeThree(std::vector<uint8_t> perm6)
{
    for (int i = 0; i < perm6.size(); i++)
    {
        if (perm6[i] == 2)
        {
            return true;
        }
        else if (perm6[i] == 3)
        {
            return false;
        }
    }
    std::cout << "error\n";
    return true;
}

void checkCycle4(std::vector<std::string> cbas)
{
    if (cbas.size() != 4)
        return;

    std::string sol = "0000000000000000000000000000000";

    std::vector<uint8_t> tuple1;
    std::vector<uint8_t> tuple2;
    std::vector<uint8_t> tuple3;
    std::vector<uint8_t> newTuple;

    for (int i = 0; i < 31; i++)
    {
        if (cbas[0][i] != '1')
            continue;
        tuple1 = riaArr6[i];
        for (int j = 0; j < 31; j++)
        {
            if (cbas[1][j] != '1')
                continue;
            for (int perm1 = 0; perm1 < 6; perm1++)
            {
                tuple2 = recolour(riaArr6[j], perm1);
                if (tuple2[0] != tuple1[0] || tuple2[1] != tuple1[1])
                    continue;
                for (int k = 0; k < 31; k++)
                {
                    if (cbas[2][k] != '1')
                        continue;
                    for (int perm2 = 0; perm2 < 6; perm2++)
                    {
                        tuple3 = recolour(riaArr6[k], perm2);
                        if (tuple3[0] != tuple2[5] ||
                            tuple3[1] != tuple2[4] ||
                            tuple3[4] != tuple1[5] ||
                            tuple3[5] != tuple1[4])
                            continue;

                        newTuple = {tuple1[2], tuple1[3],
                                    tuple2[2], tuple2[3],
                                    tuple3[2], tuple3[3]};

                        sol[toRia6[newTuple]] = '1';
                    }
                }
            }
        }
    }

    if (foundC6Set.count(sol) == 0)
    {
        foundC6Set.insert(sol);
        std::string csk = cToCskEq6(sol);
        if (!foundCsk6Map[csk])
        {
            foundCsk6Map[csk] = true;
            count6++;
            std::cout << count6 << ". 6 sol found!!! " << csk << std::endl;
            std::cout << "from " << sol << std::endl;
            foundCsk6AsStrings.push_back(csk);
        }
    }
}

void printProgress(int i)
{
    std::cout << i << "/" << foundCsk6AsStrings.size() << std::endl;
}

int main()
{
    getCanonsFromFile(6, csToCsk6CanonsMap);
    getFoundCskMapFromFile(6, foundCsk6Map);
    cskEq6 = getCskEq(6, true);
    calculateToRia(toRia6, 6);

    std::vector<std::string> allCsk6AsStrings;
    for (long long i = 0; i < (long long)cskEq6.size(); i++)
    {
        std::string cskString = numberToBinary(cskEq6[i], riaArr6.size());
        allCsk6AsStrings.push_back(cskString);
        if (foundCsk6Map[cskString])
            foundCsk6AsStrings.push_back(cskString);
    }

    std::cout << "initial size: " << foundCsk6AsStrings.size() << std::endl;

    // 3 cycle
    for (long long i = 1; i < foundCsk6AsStrings.size(); i++)
    {
        printProgress(i);
        for (long long j = 1; j <= i; j++)
        {
            for (long long k = 1; k <= j; k++)
            {
                std::vector<uint8_t> perm6 = {0, 1, 2, 4, 5, 3};
                while (1)
                {
                    // if (twoBeforeThree(perm6))
                    checkCycle3({foundCsk6AsStrings[i], foundCsk6AsStrings[j], foundCsk6AsStrings[k]}, perm6);

                    next_permutation(perm6.begin(), perm6.begin() + 4);
                    if (perm6 == std::vector<uint8_t>{0, 1, 2, 4, 5, 3})
                        break;
                }
                for (long long l = 1; l <= k; l++)
                {
                    // checkCycle4({foundCsk6AsStrings[i], foundCsk6AsStrings[j], foundCsk6AsStrings[k], foundCsk6AsStrings[l]});
                }
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
