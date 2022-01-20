#include "colouring_bit_array_canon.hpp"
#include "kempe_closed.hpp"
#include <iostream>
#include <algorithm>
#include <utility>
#include <execution>
#include <fstream>

using namespace ba_graph;

const int len = 6;

int main()
{
    std::ofstream feasibleCBAFile;
    feasibleCBAFile.open("feasibleCBA.txt");
    feasibleCBAFile << "FeasibleCBA:" << std::endl;

    const auto &cmp = colouring_bit_array_internal::comparators[len];

    std::vector<uint_fast8_t> canon(cmp.relevant_indices_absolute.size(), 0);

    std::vector<std::pair<std::vector<uint_fast8_t>, int>> tocompute;
    tocompute.push_back({canon, 0});

    int cnt = 0;

    std::vector<long long> feasibleCBAs;
    std::vector<long long> minimalFeasibleCBAs;

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
                          if (canonize(cba, len) == canon)
                              if (is_weak_kempe_closed(cba, len))
                              {
                                  p.second = 1;
                              }
                      });

        for (const auto &p : tocompute)
        {
            if (p.second)
            {
                long long feasibleCBA = 0;
                for (int i = 0; i < 31; i++)
                {
                    feasibleCBA = (feasibleCBA << 1) + p.first[i];
                    feasibleCBAFile << (int)p.first[i];
                }
                feasibleCBAs.push_back(feasibleCBA);
                feasibleCBAFile << std::endl;
            }
        }
        tocompute = {};
    }

    feasibleCBAFile << "Count: " << feasibleCBAs.size() << std::endl;
    feasibleCBAFile.close();
    std::ofstream minimalFeasibleCBAFile;
    minimalFeasibleCBAFile.open("minimalfeasibleCBA.txt");
    minimalFeasibleCBAFile << "MinimalfeasibleCBA:" << std::endl;

    for (int i = 0; i < feasibleCBAs.size(); i++)
    {
        bool isMinimal = true;
        for (int j = 0; j < feasibleCBAs.size(); j++)
        {
            if (j == i)
                break;
            isMinimal &= feasibleCBAs[i] | feasibleCBAs[j] != feasibleCBAs[i];
        }
        if (isMinimal)
        {
            minimalFeasibleCBAs.push_back(feasibleCBAs[i]);
        }
    }
    minimalFeasibleCBAFile << "Count: " << minimalFeasibleCBAs.size() << std::endl;
    for (int i = 0; i < minimalFeasibleCBAs.size(); i++)
    {
        std::string sol = "";
        long long toCheck = minimalFeasibleCBAs[i];
        for (int j = 0; j < 31; j++)
        {
            sol = (char)('0' + toCheck % 2) + sol;
            toCheck /= 2;
        }
        minimalFeasibleCBAFile << sol << std::endl;
    }
    minimalFeasibleCBAFile.close();
}
