/*
    Connects 3 cbas from csk-equivalence, such that there are two edges between each pair
    and two dangling edges from each cba

    == CBA1 == CBA2 ==
         \\    //
           CBA3
            ||

*/
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
std::map<std::vector<uint_fast8_t>, uint_fast8_t> toRia6;
std::vector<std::string> foundCsk6AsStrings;

int count6 = 0;

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
        throw invalid_argument("Invalid number of cbas");

    std::string sol = EMPTY_CBA_6;

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
                tuple2 = mapColours(riaArr6[j], perm1);
                if (tuple2[0] != tuple1[0] || tuple2[1] != tuple1[1])
                    continue;
                for (int k = 0; k < 31; k++)
                {
                    if (cbas[2][k] != '1')
                        continue;
                    for (int perm2 = 0; perm2 < 6; perm2++)
                    {
                        tuple3 = mapColours(riaArr6[k], perm2);
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
        std::string csk = cToCsk(sol, csToCsk6CanonsMap);
        if (!foundCsk6Map[csk])
        {
            foundCsk6Map[csk] = true;
            count6++;
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
        std::string cskString = longLongCbaToString(cskEq6[i], riaArr6.size());
        allCsk6AsStrings.push_back(cskString);
        if (foundCsk6Map[cskString])
            foundCsk6AsStrings.push_back(cskString);
    }

    std::cout << "initial size: " << foundCsk6AsStrings.size() << std::endl;

    // 3 cycle
    for (long long i = 1; i < foundCsk6AsStrings.size(); i++)
    {
        printProgress(i);
        checkCycle3({foundCsk6AsStrings[i], foundCsk6AsStrings[i], foundCsk6AsStrings[i]}, {0, 1, 2, 3, 4, 5});

        for (long long j = 1; j <= i; j++)
        {
            for (long long k = 1; k <= j; k++)
            {
                // we only permutate edges of one of them
                // technically we should permutate edges of two CBAs, but that would take too long
                std::vector<uint8_t> perm6 = {0, 1, 2, 3, 4, 5};
                while (1)
                {
                    checkCycle3({foundCsk6AsStrings[i], foundCsk6AsStrings[j], foundCsk6AsStrings[k]}, perm6);

                    next_permutation(perm6.begin(), perm6.end());
                    if (perm6 == std::vector<uint8_t>{0, 1, 2, 3, 4, 5})
                        break;
                }
            }
        }
    }

    updateFoundCsk(foundCsk6Map, allCsk6AsStrings);
    return 0;
}
