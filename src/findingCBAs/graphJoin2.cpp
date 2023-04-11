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
#include <random>
#include <unistd.h>

using namespace ba_graph;

const std::vector<std::vector<uint8_t>> riaArr6 = colouring_bit_array_internal::Comparator(6).relevant_indices_absolute;

std::map<std::string, bool> foundCsk6Map;
std::set<std::string> foundC6Set;
std::map<std::string, std::string> csToCsk6CanonsMap;

std::vector<long long> cskEq6;
std::map<std::vector<uint8_t>, int> toRia6;

std::vector<std::string> foundCsk6AsStrings;

int count6 = 0;
int cskFound = 115;

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
        newTuple.push_back(PERMS_3[permIndex][tuple[i]]);
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

/*
void logFoundCsk(std::vector<std::string> old, std::string neww, std::string newwCsk, std::vector<std::vector<uint8_t>> perm6s)
{
    ofstream file;
    file.open("txt/foundCSK/CSK6FindingsLogger.txt", std::ios_base::app);
    cskFound++;
    file << cskFound << ". " << newwCsk << "\n";
    file << " c: " << neww << " from:\n";
    for (int i = 0; i < old.size(); i++)
    {
        file << " " << old[i] << std::endl;
    }
    file << " permutations: ";
    for (int i = 0; i < perm6s.size(); i++)
    {
        printVec(perm6s[i], file);
    }
}
*/

std::string createMessage(const std::vector<std::string> &cbas,
                          const std::vector<std::vector<uint_fast8_t>> &perms,
                          const std::vector<std::vector<std::tuple<int, int, int>>> &supergraph,
                          int vertexCount)
{
    std::string message = "Function arguments:\n";
    message += "cbas: [";
    for (const auto &cba : cbas)
    {
        message += cba + ",";
    }
    message.pop_back(); // Remove the last comma
    message += "]\nperms: [\n";
    for (const auto &perm : perms)
    {
        message += "  [";
        for (const auto &p : perm)
        {
            message += std::to_string(p) + ",";
        }
        message.pop_back(); // Remove the last comma
        message += "],\n";
    }
    message.pop_back(); // Remove the last comma and newline
    message.pop_back();
    message += "\nsupergraph: [\n";
    for (const auto &sg : supergraph)
    {
        message += "  [";
        for (const auto &t : sg)
        {
            message += "(" + std::to_string(std::get<0>(t)) + "," + std::to_string(std::get<1>(t)) + "," + std::to_string(std::get<2>(t)) + "),";
        }
        message.pop_back(); // Remove the last comma
        message += "],\n";
    }
    message.pop_back(); // Remove the last comma and newline
    message.pop_back();
    message += "\n]vertexCount: " + std::to_string(vertexCount) + "\n";
    return message;
}

struct Result
{
    std::string logMessage;
    std::string newFoundCsk;
};

Result joinWithSupergraph(std::vector<std::string> cbas,
                          std::vector<std::vector<uint_fast8_t>> perms,
                          std::vector<std::vector<std::tuple<int, int, int>>> supergraph,
                          int vertexCount)
{
    Result result;
    result.logMessage = "";
    result.newFoundCsk = "";
    std::string sol = EMPTY_CBA_6;
    constexpr int RIA_LEN = 31;

    std::vector<std::vector<uint_fast8_t>> tuples;
    std::vector<uint8_t> newTuple;

    // describes how will we colour each cba
    std::vector<uint_fast8_t> boundaryColouringIndeces;

    // describes how will we permutate colours of each cba
    // index of perm={{0, 1, 2},{0, 2, 1},{1, 0, 2},{1, 2, 0},{2, 0, 1},{2, 1, 0}}
    std::vector<uint8_t> recolours;

    // init
    for (int i = 0; i < vertexCount; i++)
    {
        tuples.push_back({});
        boundaryColouringIndeces.push_back(0);
        while (cbas[i][boundaryColouringIndeces[i]] == '0')
            boundaryColouringIndeces[i]++;
        recolours.push_back(0);
    }

    bool stop = false;
    while (!stop)
    {
        newTuple = {};
        bool skip = false;
        for (int i = 0; i < vertexCount && !skip; i++)
        {
            tuples[i] = recolour(permutate(riaArr6[boundaryColouringIndeces[i]], perms[i]), recolours[i]);
            // edge A connected to vertex B, edge C
            // (A, B, C)
            for (int j = 0; j < supergraph[i].size() && !skip; j++)
            {
                if (get<1>(supergraph[i][j]) == -1)
                    newTuple.push_back(tuples[i][get<0>(supergraph[i][j])]);
                else if (tuples[i][get<0>(supergraph[i][j])] != tuples[get<1>(supergraph[i][j])][get<2>(supergraph[i][j])])
                    skip = true;
            }

            if (!skip && i + 1 == vertexCount)
                sol[toRia6[newTuple]] = '1';
        }
        if (countOnes(sol) > 21)
            return result;
        for (int i = 0; i < vertexCount; i++)
        {
            do
            {
                boundaryColouringIndeces[i]++;
            } while (boundaryColouringIndeces[i] < RIA_LEN && cbas[i][boundaryColouringIndeces[i]] == '0');

            if (boundaryColouringIndeces[i] == RIA_LEN)
            {
                boundaryColouringIndeces[i] = 0;
                while (cbas[i][boundaryColouringIndeces[i]] == '0')
                    boundaryColouringIndeces[i]++;
            }
            else
                break;
            if (i + 1 == vertexCount)
            {
                // no need to permutate the first one
                for (int j = 1; j < vertexCount; j++)
                {
                    recolours[j]++;
                    if (recolours[j] == PERMS_3.size())
                        recolours[j] = 0;
                    else
                        break;
                    if (j + 1 == vertexCount)
                    {
                        stop = true;
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
            result.logMessage = createMessage(cbas, perms, supergraph, vertexCount);
            result.newFoundCsk = csk;
        }
    }
    return result;
}

std::vector<std::vector<uint8_t>> getRandomPermutations(int len, std::mt19937 &generator, int count)
{
    std::vector<std::vector<uint8_t>> perms;
    std::vector<uint8_t> perm;

    for (int i = 0; i < len; i++)
        perm.push_back(i);

    for (int i = 0; i < count; i++)
    {
        shuffle(perm.begin(), perm.end(), generator);
        perms.push_back(perm);
    }
    return perms;
}

std::vector<std::string> getRandomCsks(std::mt19937 &generator, int count)
{
    int cskCount = foundCsk6AsStrings.size();
    std::uniform_int_distribution<std::uint32_t> distr(1, cskCount - 1);

    std::vector<std::string> cbas = {};
    for (int i = 0; i < count; i++)
        cbas.push_back(foundCsk6AsStrings[distr(generator)]);
    return cbas;
}

/*
const std::vector<std::pair<int, std::vector<std::pair<int, int>>>> supergraphs = {
    // vertexCount, edges
    {3, {{0, 1}, {0, 1}, {1, 2}, {1, 2}, {0, 2}, {0, 2}}},                         // 222
    {4, {{0, 1}, {0, 1}, {1, 3}, {1, 3}, {0, 2}, {0, 2}, {2, 3}, {2, 3}, {0, 3}}}, // 22221
    {4, {{0, 1}, {1, 2}, {1, 3}, {1, 3}, {0, 2}, {0, 2}, {2, 3}, {2, 3}, {0, 3}}}, // 222111a
    {4, {{0, 1}, {1, 2}, {1, 2}, {1, 3}, {0, 2}, {0, 2}, {2, 3}, {2, 3}, {0, 3}}}, // 222111b
};
*/

// [node] = {(edge, node2, edge2), ...}
// danglingEdge = (edge, -1, -1)
// edges are one-directional, from node A to B iff A > B
const std::vector<std::vector<std::vector<std::tuple<int, int, int>>>> supergraphs = {
    {{{2, -1, -1}, {3, -1, -1}},
     {{2, -1, -1}, {3, -1, -1}, {0, 0, 0}, {1, 0, 1}},
     {{2, -1, -1}, {3, -1, -1}, {4, 1, 4}, {5, 1, 5}, {1, 0, 4}, {0, 0, 5}}}, // 222
    {{{2, -1, -1}},
     {{2, -1, -1}, {3, -1, -1}, {0, 0, 0}, {1, 0, 1}},
     {{2, -1, -1}, {3, -1, -1}, {0, 0, 5}, {1, 0, 4}},
     {{3, -1, -1}, {0, 2, 5}, {1, 2, 4}, {2, 0, 3}, {4, 1, 4}, {5, 1, 5}}}, // 22221
    {{{1, -1, -1}, {2, -1, -1}},
     {{1, -1, -1}, {2, -1, -1}, {0, 0, 0}},
     {{2, -1, -1}, {3, 1, 3}, {0, 0, 5}, {1, 0, 4}},
     {{3, -1, -1}, {0, 2, 5}, {1, 2, 4}, {2, 0, 3}, {4, 1, 4}, {5, 1, 5}}}, // 222111a
    {{{1, -1, -1}, {2, -1, -1}},
     {{1, -1, -1}, {4, -1, -1}, {0, 0, 0}},
     {{2, 1, 2}, {3, 1, 3}, {0, 0, 5}, {1, 0, 4}},
     {{3, -1, -1}, {0, 2, 5}, {1, 2, 4}, {2, 0, 3}, {4, -1, -1}, {5, 1, 5}}}, // 222111b
};

void logMessage(std::string message)
{
    ofstream file;
    file.open("txt/foundCSK/CSK6FindingsLogger.txt", std::ios_base::app);
    file << message;
    file.close();
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

    std::random_device rd;        // obtain a random number from hardware
    std::mt19937 generator(rd()); // seed the generator

    int done = 0;
    int toCheck = 100000;

    while (1)
    {
        std::cout << done << " done\n";
        std::vector<std::pair<int, Result>> toCompute;

        for (long long i = 0; i < toCheck; i++)
            toCompute.push_back({i, {}});

        std::for_each(
            std::execution::par,
            toCompute.begin(),
            toCompute.end(),
            [&generator](std::pair<int, Result> &p)
            {
                auto supergraph = supergraphs[0 /*p.first % supergraphs.size()*/];
                int vertexCount = supergraph.size();
                auto cbas = getRandomCsks(generator, vertexCount);
                auto perms = getRandomPermutations(6, generator, vertexCount);
                p.second = joinWithSupergraph(cbas, perms, supergraph, vertexCount);
            });

        done += toCheck;

        for (const auto &p : toCompute)
        {
            if (p.second.newFoundCsk != "" && !foundCsk6Map[p.second.newFoundCsk])
            {
                foundCsk6Map[p.second.newFoundCsk] = true;
                logMessage(p.second.logMessage);
            }
        }
    }
    updateFoundCsk(foundCsk6Map, allCsk6AsStrings);

    return 0;
}
