#include <assert.h>
#include <iostream>
#include <fstream>
#include "colouring_bit_array_canon.hpp"
#include <map>
#include <queue>
#include <set>
using namespace ba_graph;

const int maxlen = 6;
const int cskCount = maxlen == 6 ? 843 : (maxlen == 5 ? 7 : 4);
const int canonCount = maxlen == 6 ? 70986 : (maxlen == 5 ? 16 : 6);

std::map<std::string, std::string> canonsMap;
std::vector<std::pair<std::string, bool>> foundCSK;
std::set<std::string> foundCSKSet;

std::map<const std::vector<uint_fast8_t>, ColouringBitArray> map[maxlen + 1];
std::queue<std::vector<uint_fast8_t>> queue[maxlen + 1];

std::map<const std::vector<uint_fast8_t>, std::vector<uint_fast8_t>> previous[maxlen + 1];
std::map<const std::vector<uint_fast8_t>, std::vector<uint_fast8_t>> edge[maxlen + 1];

std::string cbaToString(std::vector<uint_fast8_t> &cba)
{
    std::string s = "";
    for (unsigned int k = 0; k < cba.size(); k++)
        s += (int)cba[k] + '0';
    return s;
}

int main()
{
    // getting canons from file
    std::ifstream canonFile;
    canonFile.open("txt/canons" + std::to_string(maxlen) + ".txt");
    std::string temp1, temp2;
    for (long long i = 0; i < canonCount; i++)
    {
        canonFile >> temp1 >> temp2;
        temp1 = temp1.substr(0, temp1.length() - 1);
        temp2 = temp2.substr(3, temp2.length() - 3);
        canonsMap[temp1] = temp2;
    }

    canonFile.close();

    // read foundCSK
    std::ifstream foundCSKFile;
    foundCSKFile.open("txt/foundCSK" + std::to_string(maxlen) + ".txt");
    for (long long i = 0; i < cskCount; i++)
    {
        foundCSKFile >> temp1 >> temp2;
        foundCSK.push_back({temp1, temp2 == "1"});
    }

    foundCSKFile.close();

    // add empty colourable graph
    {
        ColouringBitArray emptyCBA(1, true);
        auto emptycanon = compress_canon(canonize(emptyCBA, 0));
        queue[0].push(emptycanon);
        map[0][emptycanon] = emptyCBA;

        previous[0][emptycanon] = {};
        edge[0][emptycanon] = {};
    }

    unsigned long counter = 0;
    while (1)
    {
        ColouringBitArray cba;
        int len = 0;
        for (; len <= maxlen; len++)
            if (!queue[len].empty())
            {
                cba = std::move(map[len][queue[len].front()]);
                queue[len].pop();
                break;
            }
        if (len == maxlen + 1)
            break;

        //        std::cout<<"cba.size"<<cba.size().first()<<","<<(int)cba.size().second()<<"\n";

        // 0,0
        if (len <= maxlen - 2)
        {
            auto newsize = cba.size();
            newsize += newsize;
            newsize += newsize;
            newsize += newsize;
            newsize += cba.size();
            ColouringBitArray res = cba;
            res.reserve(newsize);
            res.concatenate_to_special(ColouringBitArray(cba.size(), false));
            res.concatenate_to_special(ColouringBitArray(cba.size(), false));
            res.concatenate_to_special(ColouringBitArray(cba.size(), false));
            res.concatenate_to_special(cba);
            res.concatenate_to_special(ColouringBitArray(cba.size(), false));
            res.concatenate_to_special(ColouringBitArray(cba.size(), false));
            res.concatenate_to_special(ColouringBitArray(cba.size(), false));
            res.concatenate_to_special(cba);

            auto canon = compress_canon(canonize(res, len + 2));
            if (map[len + 2].try_emplace(canon, std::move(res)).second)
            {
                queue[len + 2].push(canon);

                auto expold = canonize(cba, len);
                auto expnew = expand_canon(canon);
                for (unsigned int k = 0; k < expnew.size(); k++)
                    std::cout << (int)expnew[k] << ",";
                std::cout << "  Created from: ";
                for (unsigned int k = 0; k < expold.size(); k++)
                    std::cout << (int)expold[k] << ",";
                std::cout << "  Operation 0,0 \n";
                std::string s = cbaToString(expnew);
                std::string sCanon = canonsMap[s];
                foundCSKSet.insert(sCanon);
            }
        }

        if (len >= 1)
            for (int i = 0; i < len; i++)
            {
                auto [part0, part1, part2] = cba.split3(i);
                // 0,1
                if (len <= maxlen - 1)
                {
                    //                std::cout<<"Processing 0,1 on "<<i<<"\n";
                    auto newsize = cba.size();
                    newsize += cba.size();
                    newsize += cba.size();
                    ColouringBitArray res(part0.size(), false);
                    res.reserve(newsize);
                    res.concatenate_to_special(part2);
                    res.concatenate_to_special(part1);
                    res.concatenate_to_special(part2);
                    res.concatenate_to_special(ColouringBitArray(part0.size(), false));
                    res.concatenate_to_special(part0);
                    res.concatenate_to_special(part1);
                    res.concatenate_to_special(part0);
                    res.concatenate_to_special(ColouringBitArray(part0.size(), false));

                    auto canon = compress_canon(canonize(res, len + 1));
                    if (map[len + 1].try_emplace(canon, std::move(res)).second)
                    {
                        queue[len + 1].push(canon);

                        auto expold = canonize(cba, len);
                        auto expnew = expand_canon(canon);
                        for (unsigned int k = 0; k < expnew.size(); k++)
                            std::cout << (int)expnew[k] << ",";
                        std::cout << "  Created from: ";
                        for (unsigned int k = 0; k < expold.size(); k++)
                            std::cout << (int)expold[k] << ",";
                        std::cout << "  Operation 0,1:" << i << "\n";
                        std::string s = cbaToString(expnew);
                        std::string sCanon = canonsMap[s];
                        std::cout << "found: " << s << " with canon " << sCanon << "\n";
                        foundCSKSet.insert(sCanon);
                    }
                }

                if (len >= 2)
                    for (int j = 0; j < len; j++)
                        if (i != j)
                        {
                            auto [part00, part01, part02] = part0.split3((j > i) ? j - 1 : j);
                            auto [part10, part11, part12] = part1.split3((j > i) ? j - 1 : j);
                            auto [part20, part21, part22] = part2.split3((j > i) ? j - 1 : j);
                            // 1,2
                            {
                                ColouringBitArray res = part12;
                                res |= part21;
                                res.reserve(part0.size());
                                auto partx = part02;
                                partx |= part20;
                                res.concatenate_to_special(partx);
                                partx = part01;
                                partx |= part10;
                                res.concatenate_to_special(partx);

                                auto canon = compress_canon(canonize(res, len - 1));
                                if (map[len - 1].try_emplace(canon, std::move(res)).second)
                                {
                                    queue[len - 1].push(canon);

                                    auto expold = canonize(cba, len);
                                    auto expnew = expand_canon(canon);
                                    for (unsigned int k = 0; k < expnew.size(); k++)
                                        std::cout << (int)expnew[k] << ",";
                                    std::cout << "  Created from: ";
                                    for (unsigned int k = 0; k < expold.size(); k++)
                                        std::cout << (int)expold[k] << ",";
                                    std::cout << "  Operation 1,2:" << i << "," << j << "\n";
                                    std::string s = cbaToString(expnew);
                                    std::string sCanon = canonsMap[s];
                                    std::cout << "found: " << s << " with canon " << sCanon << "\n";
                                    foundCSKSet.insert(sCanon);
                                }
                            }

                            if (i < j)
                            {
                                {
                                    // 2,2
                                    ColouringBitArray res = part00;
                                    res |= part11;
                                    res |= part22;

                                    auto canon = compress_canon(canonize(res, len - 2));
                                    if (map[len - 2].try_emplace(canon, std::move(res)).second)
                                    {
                                        queue[len - 2].push(canon);

                                        auto expold = canonize(cba, len);
                                        auto expnew = expand_canon(canon);
                                        for (unsigned int k = 0; k < expnew.size(); k++)
                                            std::cout << (int)expnew[k] << ",";
                                        std::cout << "  Created from: ";
                                        for (unsigned int k = 0; k < expold.size(); k++)
                                            std::cout << (int)expold[k] << ",";
                                        std::cout << "  Operation 2,2:" << i << "," << j << "\n";
                                        std::string s = cbaToString(expnew);
                                        std::string sCanon = canonsMap[s];
                                        std::cout << "found: " << s << " with canon " << sCanon << "\n";
                                        foundCSKSet.insert(sCanon);
                                    }
                                }
                                {
                                    // 1,1
                                    ColouringBitArray res = part11;
                                    res |= part22;
                                    res.reserve(cba.size());
                                    res.concatenate_to_special(part01);
                                    res.concatenate_to_special(part02);
                                    res.concatenate_to_special(part10);
                                    part22 |= part00;
                                    res.concatenate_to_special(part22);
                                    res.concatenate_to_special(part12);
                                    res.concatenate_to_special(part20);
                                    res.concatenate_to_special(part21);
                                    part11 |= part00;
                                    res.concatenate_to_special(part11);

                                    auto canon = compress_canon(canonize(res, len));
                                    if (map[len].try_emplace(canon, std::move(res)).second)
                                    {
                                        queue[len].push(canon);

                                        auto expold = canonize(cba, len);
                                        auto expnew = expand_canon(canon);
                                        for (unsigned int k = 0; k < expnew.size(); k++)
                                            std::cout << (int)expnew[k] << ",";
                                        std::cout << "  Created from: ";
                                        for (unsigned int k = 0; k < expold.size(); k++)
                                            std::cout << (int)expold[k] << ",";
                                        std::cout << "  Operation 1,1:" << i << "," << j << "\n";
                                        std::string s = cbaToString(expnew);
                                        std::string sCanon = canonsMap[s];
                                        std::cout << "found: " << s << " with canon " << sCanon << "\n";
                                        foundCSKSet.insert(sCanon);
                                    }
                                }
                            }
                        }
            }

        if (++counter % 1000 == 0)
        {
            for (int i = 0; i < maxlen; i++)
                std::cout << map[i].size() << ",";
            std::cout << map[maxlen].size() << "\n";
        }
    }

    // write foundCSK
    std::ofstream foundCSKFileOutput;
    foundCSKFileOutput.open("txt/foundCSK" + std::to_string(maxlen) + ".txt");
    long totalFound = 0;
    for (long long i = 0; i < cskCount; i++)
    {
        bool found = foundCSK[i].second || foundCSKSet.count(foundCSK[i].first);
        foundCSKFileOutput << foundCSK[i].first << " " << found << "\n";
        totalFound += found;
    }
    foundCSKFileOutput << "found " << totalFound << " / " << cskCount << "\n";
    foundCSKFileOutput.close();

    /*
    for (int i = 0; i < maxlen; i++)
        std::cout << map[i].size() << ",";
    std::cout << map[maxlen].size() << "\n";

    for (auto const &el : map[5])
    {
        auto canon = el.first;
        auto cba = el.second;
        for (unsigned int i = 0; i < canon.size() - 1; i++)
            std::cout << (int)canon[i] << ",";
        std::cout << (int)canon[canon.size() - 1] << "\n";
    }
    */
}
