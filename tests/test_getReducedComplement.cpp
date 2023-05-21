#include <assert.h>
#include "./../src/CBA/colouring_bit_array.h"
#include "./../src/helper.hpp"
#include "./helper.cpp"

using namespace ba_graph;

/*
    we have a fast way of getting reduced complement for 6-poles
    using a map that has stored c-equivalence

    we also have a more general way of getting reduced complement
    that doesn't need stored c-equivalence, but it is much slower
    (we use it mainly for 7-poles)

    this test compares both ways of getting reducedComplement
    of each 6-pole CBA from c-equivalence
*/

int main()
{
    std::cout << "getReducedComplement tests\n";
    auto ria = colouring_bit_array_internal::comparators[6].relevant_indices_absolute;
    auto cEq = getCEq(6, true);
    for (int i = 0; i < cEq.size(); i += 10007)
    {
        if (i % 1000 == 0)
            std::cout << i << "/" << cEq.size() << " checked\n";

        assert(getReducedComplementFromCEq(cEq[i], cEq) ==
               longLongCbaFromString(getCbaReducedComplement(longLongCbaToString(cEq[i]), 6)));
    }
    printPassed();
    return 0;
}