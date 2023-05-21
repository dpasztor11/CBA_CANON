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

const string EMPTY_CBA_6 = "0000000000000000000000000000000";
const string FULL_CBA_6 = "1111111111111111111111111111111";
const string EMPTY_CBA_7 = "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
const string FULL_CBA_7 = "1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111";

const int RIA_COUNT_6 = colouring_bit_array_internal::Comparator(6).relevant_indices_absolute.size();
const int RIA_COUNT_7 = colouring_bit_array_internal::Comparator(7).relevant_indices_absolute.size();
const long long FULL_CBA_6_NUM = (1 << RIA_COUNT_6) - 1;

//
//  CONVERSIONS
//

string vectorCbaToString(vector<uint_fast8_t> &cba)
{
    string s = "";
    for (unsigned int k = 0; k < cba.size(); k++)
        s += (int)cba[k] + '0';
    return s;
}

vector<uint_fast8_t> vectorCbaFromString(string cba)
{
    vector<uint_fast8_t> vecCba;
    for (unsigned int k = 0; k < cba.size(); k++)
        vecCba.push_back(cba[k] - '0');
    return vecCba;
}

long long longLongCbaFromString(string binary)
{
    long long sol = 0;
    for (int i = 0; i < (int)binary.length(); i++)
        sol = (sol << 1) + (binary[i] == '1');
    return sol;
}

string longLongCbaToString(long long num, int vectorSize = RIA_COUNT_6)
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

ColouringBitArray classCbaFromString(string cba, int len)
{
    return from_canon(vectorCbaFromString(cba), len);
}

string classCbaToString(ColouringBitArray &cba, int len)
{
    std::vector<uint_fast8_t> stringCba = canonize(cba, len);
    return vectorCbaToString(stringCba);
}

// in another similar work, cEquivalence is also used,
// but with a different order of representatives
// this maps ours representation to theirs
const int indexMapping[31] =
    {15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
     14, 13, 12, 11, 10, 9, 8, 5, 2, 7, 6, 4, 1, 3, 0, 30};

// this maps theirs -> ours
const int inverseIndexMapping[31] =
    {29, 27, 23, 28, 26, 22, 25, 24, 21, 20, 19, 18, 17, 16, 15,
     0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 30};

string cbaToOurRep(string cba)
{
    string transformed = EMPTY_CBA_6;
    for (int i = 0; i < 31; i++)
    {
        if (cba[i] == '1')
            transformed[inverseIndexMapping[i]] = '1';
    }
    return transformed;
}

string cbaToTheirRep(string cba)
{
    string transformed = EMPTY_CBA_6;
    for (int i = 0; i < 31; i++)
    {
        if (cba[i] == '1')
            transformed[indexMapping[i]] = '1';
    }
    return transformed;
}

// use map f: {0, 1, 2} -> {0, 1, 2} on tuple
vector<uint8_t> mapColours(vector<uint8_t> tuple, int permIndex)
{
    vector<uint8_t> newTuple;
    for (int i = 0; i < tuple.size(); i++)
        newTuple.push_back(PERMS_3[permIndex][tuple[i]]);
    return newTuple;
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
        throw invalid_argument("Invalid length");
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
        throw invalid_argument("Invalid length");
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
        throw invalid_argument("Invalid length");
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
        vectorReadFromFile.push_back(longLongCbaFromString(temp));
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

void storeCbasToFile(string fileName, vector<long long> cbas, int vectorSize)
{
    ofstream file;
    file.open(fileName);
    for (int i = 0; i < (int)cbas.size(); i++)
        file << longLongCbaToString(cbas[i], vectorSize) << endl;
    file.close();
}

void calculateToRia(map<vector<uint_fast8_t>, uint_fast8_t> &mapToFill, int len = 6)
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

void printVec(const vector<uint8_t> &vec, ostream &file = cout)
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
// 0 -> don't connect, 2 dangling edges
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

string getCbaReducedComplement(string cbaString, int len)
{
    auto ria = colouring_bit_array_internal::comparators[len].relevant_indices_absolute;

    for (int i = 0; i < cbaString.size(); i++)
        cbaString[i] = '0' + (cbaString[i] == '0');

    ColouringBitArray cba = classCbaFromString(cbaString, len);

    // reduction
    bool good = false;
    while (!good)
    {
        good = true;
        for (int i = 0; i < ria.size(); i++)
        {
            auto ii = ColouringBitArray::Index(ria[i]);
            if (cba.get(ii) && check_kempe_violation(cba, len, ria[i]))
            {
                good = false;
                cbaString[i] = '0';
                cba = classCbaFromString(cbaString, len);
            }
        }
    }
    return classCbaToString(cba, len);
}

// assumes cba is in c-equivalence
string cToCs(string cba, int len = 6)
{
    int riaCount = colouring_bit_array_internal::Comparator(len).relevant_indices_absolute.size();

    vector<uint_fast8_t> temp;
    for (int i = 0; i < riaCount; i++)
        temp.push_back(cba[i] - '0');
    temp = canonize(from_canon(temp, len), len);

    string cs = "";
    for (int i = 0; i < riaCount; i++)
        cs += temp[i] + '0';
    return cs;
}

// assumes cba is in c-equivalence
string cToCsk(string cba, std::map<std::string, std::string> &csToCsk6CanonsMap)
{
    return csToCsk6CanonsMap[cToCs(cba)];
}

long long getReducedComplementFromCEq(long long num, vector<long long> &cEq)
{
    num = FULL_CBA_6_NUM - num;
    long long best = 0;
    for (int i = 0; i < cEq.size(); i++)
    {
        if ((num | cEq[i]) == num && cEq[i] > best)
        {
            best = cEq[i];
        }
    }

    return longLongCbaFromString(cToCs(longLongCbaToString(best, RIA_COUNT_6)));
}

void updateFoundCsk(const map<string, bool> &foundCsk6Map, const vector<string> &allCsk6AsStrings)
{
    ofstream foundCSKFileOutput;
    int foundCount = 0;
    foundCSKFileOutput.open("txt/foundCSK" + to_string(6) + ".txt");
    for (long long i = 0; i < allCsk6AsStrings.size(); i++)
    {
        auto found = foundCsk6Map.at(allCsk6AsStrings[i]);
        foundCSKFileOutput << allCsk6AsStrings[i] << " " << found << "\n";
        foundCount += found;
    }
    foundCSKFileOutput << "found " << foundCount << " / " << allCsk6AsStrings.size() << "\n";
    foundCSKFileOutput.close();
}

int countOnes(string str)
{
    int count = 0;
    for (int i = 0; i < str.size(); i++)
    {
        count += str[i] == '1';
    }
    return count;
}

#endif