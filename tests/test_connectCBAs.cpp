#define TEST_MODE

#include <assert.h>
#include "src/CBA/colouring_bit_array.h"
#include "src/findingCBAs/graphJoin.cpp"
#include "helper.hpp"

using namespace ba_graph;

/*
    tests connectCBAs
*/
int main()
{
    std::cout << "connectCBA tests\n";

    auto connectWay = connectWaysMap[std::make_tuple(6, 6, 6)][0];
    auto connectOrder = std::vector<uint_fast8_t>{0, 3, 2, 1, 5, 6};

    assert(connectCBAs(EMPTY_CBA_6, 6,
                       FULL_CBA_6, 6, 6,
                       connectOrder, connectWay) == EMPTY_CBA_6);

    assert(connectCBAs(FULL_CBA_6, 6,
                       FULL_CBA_6, 6, 6,
                       connectOrder, connectWay) == FULL_CBA_6);

    std::cout << "                  PASSED\n";
}