#ifndef HELPER_H
#define HELPER_H

#include "./CBA/colouring_bit_array_canon.hpp"
#include "./CBA/kempe_closed.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>

using namespace ba_graph;
using namespace std;

vector<vector<uint_fast8_t>> PERMS_3 = {{0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}};

const std::string EMPTY_CBA_6 = "0000000000000000000000000000000";
const std::string FULL_CBA_6 = "1111111111111111111111111111111";

string cbaToString(vector<uint_fast8_t> &cba)
{
    string s = "";
    for (unsigned int k = 0; k < cba.size(); k++)
        s += (int)cba[k] + '0';
    return s;
}

long long binaryToNumber(string binary)
{
    long long sol = 0;
    for (int i = 0; i < (int)binary.length(); i++)
        sol = (sol << 1) + (binary[i] == '1');
    return sol;
}

string numberToBinary(long long num, int vectorSize)
{
    string sol = "";
    long long toCheck = num;
    for (int j = 0; j < vectorSize; j++)
    {
        sol = (char)('0' + toCheck % 2) + sol;
        toCheck /= 2;
    }
    return sol;
}

long getCskEqCount(int len)
{
    switch (len)
    {
    case 6:
        return 170;
    case 5:
        return 7;
    case 4:
        return 4;
    default:
        throw;
    }
}

long getCsEqCount(int len)
{
    switch (len)
    {
    case 6:
        return 63049;
    case 5:
        return 16;
    case 4:
        return 6;
    default:
        throw;
    }
}

long getCEqCount(int len)
{
    switch (len)
    {
    case 6:
        return 39962893;
    case 5:
        return 314;
    case 4:
        return 12;
    default:
        throw;
    }
}

vector<long long> getFromFile(int len, int count, int linesToSkip, string filePath)
{
    ifstream file;
    file.open(filePath);

    vector<long long> vectorReadFromFile;
    string temp;

    for (int i = 0; i < linesToSkip; i++)
        file >> temp;

    for (long long i = 0; i < count; i++)
    {
        file >> temp;
        vectorReadFromFile.push_back(binaryToNumber(temp));
    }
    file.close();
    return vectorReadFromFile;
}

vector<long long> getCEq(int len, bool withZeroVector)
{
    return getFromFile(len, getCEqCount(len) - !withZeroVector, !withZeroVector + 1, "txt/cEquivalence/cEq" + to_string(len) + ".txt");
}

vector<long long> getCsEq(int len, bool withZeroVector)
{
    return getFromFile(len, getCsEqCount(len) - !withZeroVector, !withZeroVector + 1, "txt/csEquivalence/csEq" + to_string(len) + ".txt");
}

vector<long long> getCskEq(int len, bool withZeroVector)
{
    return getFromFile(len, getCskEqCount(len) - !withZeroVector, !withZeroVector, "txt/reducedComplementCBA/reducedComplementCBARaw" + to_string(len) + ".txt");
}

// maps cs-equivalent CBA to its canonical csk-equivalent CBA
void getCanonsFromFile(int len, map<string, string> &canonsMap)
{
    ifstream canonFile;
    canonFile.open("txt/canons/canons" + to_string(len) + ".txt");
    string temp1, temp2;
    for (long long i = 0; i < getCsEqCount(len); i++)
    {
        canonFile >> temp1 >> temp2;
        temp1 = temp1.substr(0, temp1.length() - 1);
        temp2 = temp2.substr(3, temp2.length() - 3);
        canonsMap[temp1] = temp2;
    }

    canonFile.close();
}

void getFoundCskVectorFromFile(int len, vector<pair<string, bool>> &foundCsk)
{
    ifstream foundCskFile;
    foundCskFile.open("txt/foundCSK/foundCSK" + to_string(len) + ".txt");
    string temp1, temp2;
    for (long long i = 0; i < getCskEqCount(len); i++)
    {
        foundCskFile >> temp1 >> temp2;
        foundCsk.push_back({temp1, temp2 == "1"});
    }

    foundCskFile.close();
}

void getFoundCskMapFromFile(int len, map<string, bool> &foundCskMap)
{
    ifstream foundCskFile;
    foundCskFile.open("txt/foundCSK/foundCSK" + to_string(len) + ".txt");
    string temp1, temp2;
    for (long long i = 0; i < getCskEqCount(len); i++)
    {
        foundCskFile >> temp1 >> temp2;
        foundCskMap[temp1] = temp2 == "1";
    }

    foundCskFile.close();
}

void storeCBAsToFile(string fileName, vector<long long> cbas, int vectorSize)
{
    ofstream file;
    file.open(fileName);
    for (int i = 0; i < (int)cbas.size(); i++)
        file << numberToBinary(cbas[i], vectorSize) << endl;
    file.close();
}

void calculateToRia(map<vector<uint8_t>, int> &mapToFill, int len = 6)
{
    vector<vector<uint_fast8_t>> ria = colouring_bit_array_internal::Comparator(len).relevant_indices_absolute;
    for (int i = 0; i < ria.size(); i++)
    {
        for (int j = 0; j < 6; j++)
        {
            vector<uint_fast8_t> newBoundaryColoring = ria.at(i);
            for (int k = 0; k < len; k++)
            {
                newBoundaryColoring[k] = PERMS_3[j][newBoundaryColoring[k]];
            }
            mapToFill[newBoundaryColoring] = i;
        }
    }
    return;
}

void printVec(const vector<uint8_t> &vec, ostream &file)
{
    file << "[";
    for (const auto &value : vec)
    {
        file << static_cast<int>(value) << ",";
    }
    file << "]" << endl;
}

void addConnectWaysToMap(map<tuple<int, int, int>, vector<vector<uint8_t>>> &map, int i, int j, int k, vector<uint8_t> vec)
{
    if (vec.size() == j)
    {
        int danglingEdges = i - j;
        int danglingMap[3] = {2, 0, 1};
        for (int i = 0; i < vec.size(); i++)
        {
            danglingEdges += danglingMap[vec[i]];
        }
        if (danglingEdges == k)
        {
            map[make_tuple(i, j, k)].push_back(vec);
            if (i != j)
                map[make_tuple(j, i, k)].push_back(vec);
        }
    }
    else
    {
        vec.push_back(0);
        addConnectWaysToMap(map, i, j, k, vec);
        vec[vec.size() - 1] = 1;
        addConnectWaysToMap(map, i, j, k, vec);
        vec[vec.size() - 1] = 2;
        addConnectWaysToMap(map, i, j, k, vec);
        vec.pop_back();
    }
}

// creates a map such that:
// key: (a, b, c)
// value: vector of vectors that decribe ways to connect a-pole and b-pole to get c-pole
// 2 -> connect 2 dangling edges and create a new vertex and edge
// 1 -> connect 2 dangling edges
// 0 -> don't connect 2 dangling edges
// 3 -> one dangling edge (because one side has more dangling edges)
map<tuple<int, int, int>, vector<vector<uint8_t>>> initConnectWaysMap()
{
    map<tuple<int, int, int>, vector<vector<uint8_t>>> map;
    for (int i = 4; i <= 7; i++)
    {
        for (int j = 4; j <= i; j++)
        {
            for (int k = 6; k <= 7; k++)
            {
                map[make_tuple(i, j, k)] = map[make_tuple(j, i, k)] = {};
                addConnectWaysToMap(map, i, j, k, {});
            }
        }
    }
    return map;
}

ColouringBitArray getCBAReducedComplement(ColouringBitArray cba, int len)
{
    bool good = false;
    while (!good)
    {
        good = true;
        for (int i = 0; i < colouring_bit_array_internal::Comparator(len).relevant_indices_absolute.size(); i++)
        {
            if (check_kempe_violation(cba, len, colouring_bit_array_internal::Comparator(7).relevant_indices_absolute[i]))
            {
                good = false;
                cba.set(ColouringBitArray::Index::to_index(i), false);
            }
        }
    }
    return cba;
}

void updateFoundCsk(const std::map<std::string, bool> &foundCsk6Map, const std::vector<std::string> &allCsk6AsStrings)
{
    std::ofstream foundCSKFileOutput;
    int foundCount = 0;
    foundCSKFileOutput.open("txt/foundCSK" + std::to_string(6) + ".txt");
    for (long long i = 0; i < allCsk6AsStrings.size(); i++)
    {
        auto found = foundCsk6Map.at(allCsk6AsStrings[i]);
        foundCSKFileOutput << allCsk6AsStrings[i] << " " << found << "\n";
        foundCount += found;
    }
    foundCSKFileOutput << "found " << foundCount << " / " << allCsk6AsStrings.size() << "\n";
    foundCSKFileOutput.close();
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

#endif