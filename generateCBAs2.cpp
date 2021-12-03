#include <assert.h>
#include <iostream>
#include "colouring_bit_array_canon.hpp"
#include <set>
#include <queue>
using namespace ba_graph;

const int maxlen=7;
const int maxones=30;

std::set<std::vector<uint_fast8_t>> set[maxlen+1];
std::queue<std::vector<uint_fast8_t>> queue[maxlen+1];


int cannon_ones(std::vector<uint_fast8_t> cannon) {
    int sum=0;
    for(uint_fast8_t b: cannon) {
        if (b&0x01) sum++;
        if (b&0x02) sum++;
        if (b&0x04) sum++;
        if (b&0x08) sum++;
        if (b&0x10) sum++;
        if (b&0x20) sum++;
        if (b&0x40) sum++;
        if (b&0x80) sum++;
    }
    return sum;
}




int main() {
    //add empty colourable graph
    {
        ColouringBitArray emptyCBA(1, true);
        auto emptycanon=compress_canon(canonize(emptyCBA,0));
        queue[0].push(emptycanon);
        set[0].insert(emptycanon);
    }

    unsigned long counter=0;    
    while (1) {
        ColouringBitArray cba;
        int len=0;
        for(;len<=maxlen;len++)
            if (!queue[len].empty()) {
                cba=from_canon(expand_canon(queue[len].front()),len);
                queue[len].pop();
                break;
            }
        if (len==maxlen+1) break;
        
        
//        std::cout<<"cba.size"<<cba.size().first()<<","<<(int)cba.size().second()<<"\n";

        //0,0
        if (len<=maxlen-2) {
            auto newsize=cba.size(); newsize+=newsize; newsize+=newsize; newsize+=newsize; newsize+=cba.size();
            ColouringBitArray res=cba;
            res.reserve(newsize);
            res.concatenate_to_special(ColouringBitArray(cba.size(),false));
            res.concatenate_to_special(ColouringBitArray(cba.size(),false));
            res.concatenate_to_special(ColouringBitArray(cba.size(),false));
            res.concatenate_to_special(cba);
            res.concatenate_to_special(ColouringBitArray(cba.size(),false));
            res.concatenate_to_special(ColouringBitArray(cba.size(),false));
            res.concatenate_to_special(ColouringBitArray(cba.size(),false));
            res.concatenate_to_special(cba);
          
            auto canon=compress_canon(canonize(res, len+2));
//            for(unsigned int i=0; i<canon.size(); i++) std::cout<<(int)canon[i]<<",";
            if (set[len+2].insert(canon).second && canon_ones(canon)<maxones) {
//                std::cout<<"FINE "<<len+2;                
                queue[len+2].push(canon);
            }
//            std::cout<<"\n";
        }

        if (len>=1) for(int i=0;i<len;i++) {
            auto [part0, part1, part2] = cba.split3(i);
            //0,1
            if (len<=maxlen-1) {
                auto newsize=cba.size(); newsize+=cba.size(); newsize+=cba.size();
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

                auto canon=compress_canon(canonize(res, len+1));
//                for(unsigned int i=0; i<canon.size(); i++) std::cout<<(int)canon[i]<<",";
                if (set[len+1].insert(canon).second && canon_ones(canon)<maxones) {
//                    std::cout<<"FINE"<<len+1;
                    queue[len+1].push(canon);
                }
//                std::cout<<"\n";
            }

            if (len>=2) for(int j=0;j<len;j++) if (i!=j) {
                auto [part00, part01, part02] = part0.split3((j>i)?j-1:j);
                auto [part10, part11, part12] = part1.split3((j>i)?j-1:j);
                auto [part20, part21, part22] = part2.split3((j>i)?j-1:j);
                //1,2
                {
                    ColouringBitArray res = part12;
                    res|=part21;
                    res.reserve(part0.size());
                    auto partx=part02;
                    partx|=part20;
                    res.concatenate_to_special(partx);
                    partx=part01;
                    partx|=part10;
                    res.concatenate_to_special(partx);


                    auto canon=compress_canon(canonize(res, len-1));
//                    for(unsigned int i=0; i<canon.size(); i++) std::cout<<(int)canon[i]<<",";
                    if (set[len-1].insert(canon).second && canon_ones(canon)<maxones) {
//                        std::cout<<"FINE"<<len-1;
                        queue[len-1].push(canon);
                    }
//                    std::cout<<"\n";
                }

                if (i<j) {
                    {
                    //2,2
                        ColouringBitArray res = part00;
                        res|=part11;
                        res|=part22;
                        
                        auto canon=compress_canon(canonize(res, len-2));
//                        for(unsigned int i=0; i<canon.size(); i++) std::cout<<(int)canon[i]<<",";
                        if (set[len-2].insert(canon).second && canon_ones(canon)<maxones) {
//                            std::cout<<"FINE"<<len-2;
                            queue[len-2].push(canon);
                        }
//                        std::cout<<"\n";
                    }
                    {
                    //1,1
                        ColouringBitArray res = part11; res |= part22;
                        res.reserve(cba.size());
                        res.concatenate_to_special(part01);
                        res.concatenate_to_special(part02);
                        res.concatenate_to_special(part10);
                        part22|=part00;
                        res.concatenate_to_special(part22);
                        res.concatenate_to_special(part12);
                        res.concatenate_to_special(part20);
                        res.concatenate_to_special(part21);
                        part11|=part00;
                        res.concatenate_to_special(part11);

                        auto canon=compress_canon(canonize(res, len));
//                        for(unsigned int i=0; i<canon.size(); i++) std::cout<<(int)canon[i]<<",";
                        if (set[len].insert(canon).second && canon_ones(canon)<maxones) {
//                            std::cout<<"FINE"<<len;
                            queue[len].push(canon);
                        }
//                        std::cout<<"\n";
                    }
                }
            }
        }

        if (++counter%1000==0) {
            for(int i=0; i<maxlen; i++)
                std::cout<<set[i].size()<<",";
            std::cout<<set[maxlen].size()<<"\n";
        }
    }
    
    for(int i=0; i<maxlen; i++)
        std::cout<<set[i].size()<<",";
    std::cout<<set[maxlen].size()<<"\n";
    
}   
