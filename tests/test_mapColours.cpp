#include <assert.h>
#include "./../src/CBA/colouring_bit_array.h"
#include "./../src/helper.hpp"

using namespace ba_graph;
/*
    tests mapColours function according to PERMS_3 defined in helper.hpp

    PERMS_3 = {{0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}}
*/
int main()
{
    std::cout << "mapColours tests\n";
    assert((mapColours({1, 2, 0, 0, 2, 1}, 0) == std::vector<uint_fast8_t>{1, 2, 0, 0, 2, 1}));
    assert((mapColours({1, 0, 0, 1, 2, 0}, 0) == std::vector<uint_fast8_t>{1, 0, 0, 1, 2, 0}));
    assert((mapColours({1, 1, 1, 0, 0, 2, 2, 2}, 4) == std::vector<uint_fast8_t>{0, 0, 0, 2, 2, 1, 1, 1}));

    assert((mapColours(mapColours({1, 0, 0, 1, 2, 0}, 1), 1) == std::vector<uint_fast8_t>{1, 0, 0, 1, 2, 0}));
    assert((mapColours(mapColours(mapColours({1, 0, 0, 1, 2, 0}, 3), 3), 3) == std::vector<uint_fast8_t>{1, 0, 0, 1, 2, 0}));
    std::cout << "                  PASSED\n";
    return 0;
}