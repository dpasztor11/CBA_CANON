#include <iostream>
#include <chrono>
#include <algorithm>
#include <utility>
#include <execution>
#include <thread>
#include <unistd.h>
#include "./../src/CBA/colouring_bit_array_canon.hpp"
#include "./../src/CBA/kempe_closed.hpp"
#include "./../src/helper.hpp"

using namespace std;

const std::vector<std::vector<uint8_t>> riaArr6 = colouring_bit_array_internal::Comparator(6).relevant_indices_absolute;

std::vector<std::vector<int>> poset;
std::vector<long long> cEq;
std::vector<std::string> cskEq;
std::map<std::vector<uint8_t>, int> toRia6;

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

int countOnes(std::string str)
{
    int count = 0;
    for (int i = 0; i < str.size(); i++)
    {
        count += str[i] == '1';
    }
    return count;
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
    calculateToRia(toRia6, 6);
    cEq = getCEq(6, true);
    cskEq = toStrings(getCskEq(6, true));

    for (int i = 0; i < cskEq.size(); i++)
    {
        poset.push_back({});
    }

    int id = 0;

    for (int i = 0; i < cskEq.size(); i++)
    {
        for (int j = 0; j < cskEq.size(); j++)
        {
            if (i != j && isSmaller(cskEq[i], cskEq[j]))
            {
                std::cout << j << " -> " << i << "\n";
                poset[i].push_back(j);
            }
        }
    }
}