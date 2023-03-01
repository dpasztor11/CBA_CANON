#include <vector>
#include <utility>
#include "colouring_bit_array.hpp"

namespace ba_graph
{
    namespace colouring_bit_array_internal
    {

        std::vector<std::vector<std::pair<int, int>>> generate_matchings(int n)
        {
            if (n == 0)
                return {{}};
            auto rec = generate_matchings(n - 2);
            std::vector<std::vector<std::pair<int, int>>> res;
            for (int i = 0; i < n - 1; i++)
            {
                for (auto v : rec)
                {
                    for (auto &p : v)
                    {
                        if (p.first >= i)
                            p.first++;
                        if (p.second >= i)
                            p.second++;
                    }
                    v.push_back({i, n - 1});
                    res.push_back(v);
                }
            }
            return res;
        }

        template <typename T>
        std::vector<std::vector<T>> power_set(std::vector<T> vec)
        {
            if (vec.size() == 0)
                return {{}};
            T back = vec.back();
            vec.pop_back();
            auto rec = power_set(vec);
            std::vector<std::vector<T>> res = rec;
            for (auto &v : rec)
            {
                v.push_back(back);
                res.push_back(v);
            }
            return res;
        }

    } // end namespace colouring_bit_array_internal

    bool is_weak_kempe_closed(const ColouringBitArray &cba, uint_fast8_t len)
    {
        for (auto vec : colouring_bit_array_internal::comparators[len].relevant_indices_absolute)
            if (cba[ColouringBitArray::Index(vec)])
            {
                for (uint_fast8_t j = 0; j < len; j++)
                {
                    for (uint_fast8_t c = 0; c < 3; c++)
                        if (c != vec[j])
                        {
                            bool violation = true;
                            for (uint_fast8_t k = 0; k < len; k++)
                                if (j != k)
                                    if (colouring_bit_array_internal::kempe_switch_exists(cba, vec, {j, k}, c, vec[j]))
                                    {
                                        violation = false;
                                        break;
                                    }
                            if (violation)
                                return false;
                        }
                }
            }
        return true;
    }

    bool check_kempe_violation(const ColouringBitArray &cba, uint_fast8_t len, int index)
    {
        std::vector<uint_fast8_t> vec = colouring_bit_array_internal::comparators[len].relevant_indices_absolute[index];
        for (uint_fast8_t c1 = 0; c1 < 3; c1++)
            for (uint_fast8_t c2 = c1 + 1; c2 < 3; c2++)
            {
                std::vector<int> indices;
                for (unsigned int i = 0; i < vec.size(); i++)
                    if (vec[i] == c1 || vec[i] == c2)
                        indices.push_back(i);
                auto matchings = colouring_bit_array_internal::generate_matchings(indices.size());
                bool violation = true;
                for (const auto &matching : matchings)
                {
                    bool all_ok = true;
                    auto ps = colouring_bit_array_internal::power_set(matching);
                    for (auto small_matching : ps)
                    {
                        auto vec2 = vec;
                        for (std::pair<int, int> p : small_matching)
                        {
                            auto &v1 = vec2[indices[p.first]];
                            auto &v2 = vec2[indices[p.second]];
                            if (v1 == c1)
                                v1 = c2;
                            else
                                v1 = c1;
                            if (v2 == c1)
                                v2 = c2;
                            else
                                v2 = c1;
                        }
                        if (!cba[ColouringBitArray::Index(vec2)])
                        {
                            all_ok = false;
                            break;
                        }
                    }
                    if (all_ok)
                    {
                        violation = false;
                        break;
                    }
                }
                if (violation)
                    return true;
            }
        return false;
    }

    bool is_kempe_closed(const ColouringBitArray &cba, uint_fast8_t len)
    {
        for (int i = 0; i < colouring_bit_array_internal::comparators[len].relevant_indices_absolute.size(); i++)
            if (cba[ColouringBitArray::Index::to_index(i)] && check_kempe_violation(cba, len, i))
                return false;
        return true;
    }

} // end namespace ba_graph
