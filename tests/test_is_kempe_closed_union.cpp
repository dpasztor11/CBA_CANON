#include <assert.h>
#include "./../src/CBA/colouring_bit_array.h"
#include "./../src/helper.hpp"
#include "./helper.cpp"

using namespace ba_graph;
/*
    tests whether for kempe closed A and B
    A union B is kempe closed
*/
int main()
{
    std::cout << "kempe closed union tests\n";

    auto cEq6 = getCEq(6, true);
    auto cEq5 = getCEq(5, true);
    auto cEq4 = getCEq(4, true);

    std::cout << "k = 4:\n";
    // 4
    for (int i = 0; i < cEq4.size(); i++)
    {
        long long cba1 = cEq4[i];
        for (int j = i + 1; j < cEq4.size(); j++)
        {
            long long cba2 = cEq4[j];
            long long cbaUnion = cba1 | cba2;
            assert(is_kempe_closed(from_canon(vectorCbaFromString(longLongCbaToString(cbaUnion)), 4), 4));
        }
    }
    printPassed();

    std::cout << "k = 5:\n";
    // 5
    for (int i = 0; i < cEq5.size(); i++)
    {
        long long cba1 = cEq5[i];
        for (int j = i + 1; j < cEq5.size(); j++)
        {
            long long cba2 = cEq5[j];
            long long cbaUnion = cba1 | cba2;
            assert(is_kempe_closed(from_canon(vectorCbaFromString(longLongCbaToString(cbaUnion)), 5), 5));
        }
    }
    printPassed();

    std::cout << "k = 6:\n";
    // 6
    // too many combinations, we will pick only some of them
    for (int i = 0; i < cEq6.size(); i += 100007)
    {
        long long cba1 = cEq6[i];
        for (int j = i + 1; j < cEq6.size(); j += 10007)
        {
            long long cba2 = cEq6[j];
            long long cbaUnion = cba1 | cba2;
            assert(is_kempe_closed(from_canon(vectorCbaFromString(longLongCbaToString(cbaUnion)), 6), 6));
        }
    }

    printPassed();
    return 0;
}