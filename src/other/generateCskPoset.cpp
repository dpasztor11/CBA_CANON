/*
    generates a poset from csk-equivalence
    the representatives were first renamed to their "maximal" name
    A <= B if and only if edges of A can be permutated
    to A' such that A' is a subset of B
*/

#include <iostream>
#include <chrono>
#include <algorithm>
#include <utility>
#include <execution>
#include <thread>
#include <unistd.h>
#include <set>
#include "./../CBA/colouring_bit_array_canon.hpp"
#include "./../CBA/kempe_closed.hpp"
#include "./../helper.hpp"

using namespace std;

const std::vector<long long> theirCbas = {1112539137, 219, 995328, 1782, 983094, 983277, 1186463953, 259394321, 1186467169, 15275, 986027, 1003880, 1003955, 1005470, 107940770, 108429655, 108429708, 108431226, 108441953, 108442042, 108454116, 108456866, 259391628, 259394506, 259407114, 259415138, 1112539354, 1112542108, 1112554410, 1113522412, 1113525162, 1113543090, 1113545460, 1113545730, 1186479009, 1186490916, 112984273, 127372309};

const std::vector<std::vector<uint8_t>> riaArr6 = colouring_bit_array_internal::Comparator(6).relevant_indices_absolute;

std::vector<std::vector<int>> poset;
std::vector<long long> cEq;
std::vector<std::string> cskEq;
std::map<std::vector<uint_fast8_t>, uint_fast8_t> toRia6;

std::map<std::string, std::string> csToCsk6CanonsMap;
std::map<std::string, bool> isFoundCsk6Map;

const long long allOnes = (1 << (riaArr6.size())) - 1;

long long getMaximalCsk(long long num)
{
    num = allOnes - num;
    long long best = 0;
    for (int i = 0; i < cEq.size(); i++)
    {
        if ((num | cEq[i]) == num && cEq[i] > best)
        {
            best = cEq[i];
        }
    }

    num = allOnes - best;
    best = 0;

    for (int i = 0; i < cEq.size(); i++)
    {
        if ((num | cEq[i]) == num && cEq[i] > best)
        {
            best = cEq[i];
        }
    }

    return best;
}

std::vector<std::string> toStrings(std::vector<long long> cbas)
{
    std::vector<std::string> a;
    for (int i = 0; i < cbas.size(); i++)
    {
        a.push_back(longLongCbaToString(getMaximalCsk(cbas[i]), 31));
    }
    return a;
}

bool isSubset(std::string cba1, std::string cba2)
{
    for (int i = 0; i < cba1.size(); i++)
    {
        if (cba1[i] == '1' && cba2[i] == '0')
            return false;
    }
    return true;
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

std::string permutateCba(std::string cba, std::vector<uint_fast8_t> perm)
{
    std::string sol = EMPTY_CBA_6;

    for (int i = 0; i < 31; i++)
    {
        if (cba[i] == '1')
            sol[toRia6[permutate(riaArr6[i], perm)]] = '1';
    }

    return sol;
}

bool isSmaller(std::string csk1, std::string csk2)
{
    std::vector<uint_fast8_t> perm = {0, 1, 2, 3, 4, 5};
    while (1)
    {
        if (isSubset(permutateCba(csk1, perm), csk2))
            return true;

        next_permutation(perm.begin(), perm.end());
        if (perm == std::vector<uint_fast8_t>{0, 1, 2, 3, 4, 5})
            break;
    }
    return false;
}

int main()
{
    getCanonsFromFile(6, csToCsk6CanonsMap);
    getFoundCskMapFromFile(6, isFoundCsk6Map);
    calculateToRia(toRia6, 6);
    cEq = getCEq(6, true);
    cskEq = toStrings(getCskEq(6, true));

    for (int i = 0; i < cskEq.size(); i++)
    {
        poset.push_back({});
    }

    int id = 0;

    std::vector<std::vector<bool>> isSmallerMemo;
    for (int i = 0; i < cskEq.size(); i++)
    {
        isSmallerMemo.push_back({});
        for (int j = 0; j < cskEq.size(); j++)
        {
            if (i == j)
                isSmallerMemo[i].push_back(false);
            else
                isSmallerMemo[i].push_back(isSmaller(cskEq[i], cskEq[j]));
        }
    }

    ofstream canonFile;
    canonFile.open("data/poset/poset6.txt");

    // A is aboveZero iff 0 <= A and there is no B such that 0 <= B <= A
    int aboveZero = 0;

    for (int i = 0; i < cskEq.size(); i++)
    {
        canonFile << cskEq[i] << ":\n";
        for (int j = 0; j < cskEq.size(); j++)
        {
            if (isSmallerMemo[j][i])
            {
                bool between = false;
                for (int k = 0; k < cskEq.size(); k++)
                {
                    if (k != i && k != j && isSmallerMemo[j][k] && isSmallerMemo[k][i])
                    {
                        between = true;
                    }
                }
                if (!between)
                {
                    canonFile << "  " << cskEq[j] << "\n";
                    if (j == 0)
                    {
                        aboveZero++;
                        std::cout << cskEq[i] << " is aboveZero\n";
                    }
                }
            }
        }
    }
    std::cout << "aboveZero: " << aboveZero << std::endl;

    // cbas found in another work
    std::set<long long> canonicals;

    // generates a table
    for (int i = 0; i < theirCbas.size(); i++)
    {
        std::string theirCba = longLongCbaToString(theirCbas[i]);
        reverse(theirCba.begin(), theirCba.end());
        std::string ourCba = cbaToOurRep(theirCba);
        std::string canonical = cToCsk(ourCba, csToCsk6CanonsMap);
        canonicals.insert(longLongCbaFromString(canonical));
        bool found = isFoundCsk6Map[canonical];
        std::cout << theirCbas[i] << " "
                  << longLongCbaFromString(ourCba) << " " << longLongCbaFromString(canonical) << " "
                  << (found ? "Yes" : "No") << "\n";
    }

    std::cout << canonicals.size() << std::endl;

    return 0;
}