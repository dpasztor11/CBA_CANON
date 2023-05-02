#define TEST_MODE

#include <assert.h>
#include "./../src/CBA/colouring_bit_array.h"
#include "./../src/findingCBAs/graphJoin.cpp"
#include "./../src/helper.hpp"
#include "./helper.cpp"

using namespace ba_graph;

/*
    tests connectCBAs
*/
int main()
{
    std::cout << "connectCBA tests\n";

    connectWaysMap = initConnectWaysMap();
    calculateToRia(toRia6, 6);
    calculateToRia(toRia7, 7);

    auto connectWay = connectWaysMap[std::make_tuple(6, 6, 6)][0];
    auto connectWay2 = connectWaysMap[std::make_tuple(7, 7, 6)][0];

    auto connectOrder6 = std::vector<uint_fast8_t>{0, 1, 2, 3, 4, 5};
    auto connectOrder7 = std::vector<uint_fast8_t>{0, 1, 2, 3, 4, 5};

    assert(connectCBAs(EMPTY_CBA_6, 6, FULL_CBA_6, 6, 6, connectOrder6, connectWay) == EMPTY_CBA_6);
    assert(connectCBAs(EMPTY_CBA_7, 7, FULL_CBA_7, 7, 6, connectOrder7, connectWay2) == EMPTY_CBA_6);
    assert(connectCBAs(FULL_CBA_6, 6, FULL_CBA_6, 6, 6, connectOrder6, connectWay) == FULL_CBA_6);

    printPassed();
    return 0;
}