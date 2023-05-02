#include <assert.h>
#include <iostream>
#include "./../src/CBA/colouring_bit_array_canon.hpp"
#include "./../src/CBA/kempe_closed.hpp"
#include "./../src/helper.hpp"
#include "./helper.cpp"

using namespace ba_graph;

int main()
{

    /**************************
    generate_matchings, power_set
    **************************/

    std::cout << "generate_matchings, power_set\n";

    auto matchings = colouring_bit_array_internal::generate_matchings(4);
    std::vector<std::vector<std::pair<int, int>>> exp_matchings({{{1, 2}, {0, 3}}, {{0, 2}, {1, 3}}, {{0, 1}, {2, 3}}});
    assert(matchings == exp_matchings);

    auto power = colouring_bit_array_internal::power_set(std::vector<int>{1, 2, 3});
    std::vector<std::vector<int>> exp_power({{}, {1}, {2}, {1, 2}, {3}, {1, 3}, {2, 3}, {1, 2, 3}});
    assert(power == exp_power);

    printPassed();

    /**************************
    is_weak_kempe_closed
    **************************/
    std::cout << "is_weak_kempe_closed\n";
    assert(is_weak_kempe_closed(ColouringBitArray(ColouringBitArray::pow3[5], false), 5));
    assert(is_weak_kempe_closed(ColouringBitArray(ColouringBitArray::pow3[5], true), 5));
    assert(!is_weak_kempe_closed(from_canon(std::vector<uint_fast8_t>({1, 0, 0, 0}), 4), 4));
    assert(!is_weak_kempe_closed(from_canon(std::vector<uint_fast8_t>({0, 1, 0, 0}), 4), 4));
    assert(!is_weak_kempe_closed(from_canon(std::vector<uint_fast8_t>({0, 0, 1, 0}), 4), 4));
    assert(!is_weak_kempe_closed(from_canon(std::vector<uint_fast8_t>({0, 0, 0, 1}), 4), 4));
    assert(is_weak_kempe_closed(from_canon(std::vector<uint_fast8_t>({1, 1, 0, 0}), 4), 4));
    assert(is_weak_kempe_closed(from_canon(std::vector<uint_fast8_t>({1, 0, 1, 0}), 4), 4));
    assert(is_weak_kempe_closed(from_canon(std::vector<uint_fast8_t>({1, 0, 0, 1}), 4), 4));
    assert(is_weak_kempe_closed(from_canon(std::vector<uint_fast8_t>({0, 1, 0, 1}), 4), 4));
    assert(is_weak_kempe_closed(from_canon(std::vector<uint_fast8_t>({0, 0, 1, 1}), 4), 4));
    assert(is_weak_kempe_closed(from_canon(std::vector<uint_fast8_t>({1, 1, 1, 0}), 4), 4));
    assert(is_weak_kempe_closed(from_canon(std::vector<uint_fast8_t>({1, 1, 0, 1}), 4), 4));
    assert(is_weak_kempe_closed(from_canon(std::vector<uint_fast8_t>({1, 0, 1, 1}), 4), 4));
    assert(is_weak_kempe_closed(from_canon(std::vector<uint_fast8_t>({0, 1, 1, 1}), 4), 4));
    printPassed();

    /**************************
    is_kempe_closed
    **************************/
    std::cout << "is_kempe_closed\n";
    assert(is_kempe_closed(ColouringBitArray(ColouringBitArray::pow3[5], false), 5));
    assert(is_kempe_closed(ColouringBitArray(ColouringBitArray::pow3[5], true), 5));
    assert(!is_kempe_closed(from_canon(std::vector<uint_fast8_t>({1, 0, 0, 0}), 4), 4));
    assert(!is_kempe_closed(from_canon(std::vector<uint_fast8_t>({0, 1, 0, 0}), 4), 4));
    assert(!is_kempe_closed(from_canon(std::vector<uint_fast8_t>({0, 0, 1, 0}), 4), 4));
    assert(!is_kempe_closed(from_canon(std::vector<uint_fast8_t>({0, 0, 0, 1}), 4), 4));
    assert(is_kempe_closed(from_canon(std::vector<uint_fast8_t>({1, 1, 0, 0}), 4), 4));
    assert(is_kempe_closed(from_canon(std::vector<uint_fast8_t>({1, 0, 1, 0}), 4), 4));
    assert(is_kempe_closed(from_canon(std::vector<uint_fast8_t>({1, 0, 0, 1}), 4), 4));
    assert(is_kempe_closed(from_canon(std::vector<uint_fast8_t>({0, 1, 0, 1}), 4), 4));
    assert(is_kempe_closed(from_canon(std::vector<uint_fast8_t>({0, 0, 1, 1}), 4), 4));
    assert(is_kempe_closed(from_canon(std::vector<uint_fast8_t>({1, 1, 1, 0}), 4), 4));
    assert(is_kempe_closed(from_canon(std::vector<uint_fast8_t>({1, 1, 0, 1}), 4), 4));
    assert(is_kempe_closed(from_canon(std::vector<uint_fast8_t>({1, 0, 1, 1}), 4), 4));
    assert(is_kempe_closed(from_canon(std::vector<uint_fast8_t>({0, 1, 1, 1}), 4), 4));

    assert(is_kempe_closed(from_canon(std::vector<uint_fast8_t>(
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}),
                                      6),
                           6));
    assert(!is_kempe_closed(from_canon(std::vector<uint_fast8_t>(
                                           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}),
                                       6),
                            6));
    assert(is_kempe_closed(from_canon(std::vector<uint_fast8_t>(
                                          {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
                                      6),
                           6));
    printPassed();
    return 0;
}
