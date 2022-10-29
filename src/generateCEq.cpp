#include "colouring_bit_array_canon.hpp"
#include "kempe_closed.hpp"
#include <iostream>
#include <algorithm>
#include <utility>
#include <execution>
#include <fstream>

using namespace ba_graph;

const int len = 6;
const int riaCount = colouring_bit_array_internal::Comparator(len).len; // relevant_indices_absolute

int main()
{
    std::ofstream cEqFile;
    cEqFile.open("txt/cEq" + std::to_string(len) + ".txt");
    cEqFile << "C-equivalence:" << std::endl;

    const auto &cmp = colouring_bit_array_internal::comparators[len];

    std::vector<uint_fast8_t> canon(cmp.relevant_indices_absolute.size(), 0);

    std::vector<std::pair<std::vector<uint_fast8_t>, int>> tocompute;
    tocompute.push_back({canon, 0});

    int cnt = 0;

    std::vector<long long> cEqCBAs;

    int stay = 1;

    while (stay)
    {
        for (int j = 0; j < 1000; j++)
        {
            unsigned int i = 0;
            for (; i < canon.size(); i++)
            {
                if (canon[i] == 1)
                    canon[i] = 0;
                else
                {
                    canon[i] = 1;
                    break;
                }
            }
            if (i == canon.size())
            {
                stay = 0;
                break;
            }
            if (i > 15)
                std::cout << "Working: " << (int)canon[16] << (int)canon[17] << (int)canon[18]
                          << (int)canon[19] << (int)canon[20] << (int)canon[21] << (int)canon[22]
                          << (int)canon[23] << (int)canon[24] << (int)canon[25] << (int)canon[26] << (int)canon[27]
                          << (int)canon[28] << (int)canon[29] << (int)canon[30] << " cnt:" << cnt << "\n";

            tocompute.push_back({canon, 0});
        }

        std::for_each(std::execution::par, tocompute.begin(), tocompute.end(), [](std::pair<std::vector<uint_fast8_t>, int> &p)
                      {
                          auto canon = p.first;
                          ColouringBitArray cba = from_canon(canon, len);
                          if (is_weak_kempe_closed(cba, len))
                            {
                                p.second = 1;
                            } });

        for (const auto &p : tocompute)
        {
            if (p.second)
            {
                long long CBA = 0;
                for (int i = 0; i < riaCount; i++)
                {
                    CBA = (CBA << 1) + p.first[i];
                    cEqFile << (int)p.first[i];
                }
                cEqCBAs.push_back(CBA);
                cEqFile << std::endl;
            }
        }
        tocompute = {};
    }

    cEqFile << "Count: " << cEqCBAs.size() << std::endl;
    cEqFile.close();
}
