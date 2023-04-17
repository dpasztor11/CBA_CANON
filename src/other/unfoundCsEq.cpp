#include <iostream>
#include "./../CBA/colouring_bit_array_canon.hpp"
#include "./../CBA/kempe_closed.hpp"
#include "./../helper.hpp"

const std::vector<std::vector<uint8_t>> riaArr6 = colouring_bit_array_internal::Comparator(6).relevant_indices_absolute;
std::map<std::string, std::string> csToCsk6CanonsMap;
std::map<std::string, bool> foundCsk6Map;

int main()
{
    getCanonsFromFile(6, csToCsk6CanonsMap);
    getFoundCskMapFromFile(6, foundCsk6Map);
    auto csEq = getCsEq(6, true);
    int count = 0;
    std::vector<int> csNotFound;
    for (int i = 0; i < csEq.size(); i++)
    {
        std::string cs = numberToBinary(csEq[i], 31);
        if (!foundCsk6Map[csToCsk6CanonsMap[cToCs(cs)]])
        {
            count++;
            csNotFound.push_back(countOnes(cs));
        }
    }
    sort(csNotFound.begin(), csNotFound.end());
    for (int i = 0; i < csNotFound.size(); i++)
    {
        std::cout << csNotFound[i] << std::endl;
    }
}