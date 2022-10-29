#ifndef BA_GRAPH_SNARKS_COLOURING_BIT_ARRAY_CANNON_HPP
#define BA_GRAPH_SNARKS_COLOURING_BIT_ARRAY_CANNON_HPP

#ifdef BA_GRAPH_DEBUG
#include <iostream>
#endif

#include "colouring_bit_array.hpp"
#include <algorithm>
namespace ba_graph {
namespace colouring_bit_array_internal {

class Comparator {
public:
    std::vector<std::vector<int_fast8_t>> relevant_indices_relative;
    std::vector<std::vector<uint_fast8_t>> relevant_indices_absolute;
    std::vector<ColouringBitArray> masks;
    uint_fast8_t len2;
    uint_fast8_t len;
    Comparator(uint_fast8_t i) {
        switch(i) {
            case 0:
                relevant_indices_absolute =  {{}}; 
                break;
            case 1:
                relevant_indices_absolute =  {{0}}; 
                break;
            case 2:
                relevant_indices_absolute =  {{0,0}}; 
                break;
            case 3:
                relevant_indices_absolute =  {{0,1,2}}; 
                break;
            case 4:
                relevant_indices_absolute =  {{0,0,1,1}, {0,1,0,1}, {0,1,1,0}, {0,0,0,0}}; 
                break;
            case 5:
                relevant_indices_absolute =  {{0,0,0,1,2}, {0,0,1,0,2}, {0,0,1,2,0}, {0,1,0,0,2}, {0,1,0,2,0}, 
                                     {0,1,2,0,0}, {1,0,0,0,2}, {1,0,0,2,0}, {1,0,2,0,0}, {1,2,0,0,0}}; 
                break;
            case 6:
                relevant_indices_absolute =  {{0,0,0,0,1,1}, {0,0,0,1,0,1}, {0,0,0,1,1,0}, {0,0,1,0,0,1},
                                     {0,0,1,0,1,0}, {0,0,1,1,0,0}, {0,1,0,0,0,1}, {0,1,0,0,1,0},
                                     {0,1,0,1,0,0}, {0,1,1,0,0,0}, {1,0,0,0,0,1}, {1,0,0,0,1,0},
                                     {1,0,0,1,0,0}, {1,0,1,0,0,0}, {1,1,0,0,0,0}, {0,0,1,1,2,2},
                                     {0,0,1,2,1,2}, {0,0,1,2,2,1}, {0,1,0,1,2,2}, {0,1,0,2,1,2}, 
                                     {0,1,0,2,2,1}, {0,1,1,0,2,2}, {0,1,1,2,0,2}, {0,1,1,2,2,0},
                                     {0,1,2,0,1,2}, {0,1,2,0,2,1}, {0,1,2,1,0,2}, {0,1,2,1,2,0},
                                     {0,1,2,2,0,1}, {0,1,2,2,1,0}, {0,0,0,0,0,0}}; 
                break;
            case 7:
                relevant_indices_absolute =  {{0,0,0,0,0,1,2}, {0,0,0,0,1,0,2}, {0,0,0,0,1,2,0}, {0,0,0,1,0,0,2},
                                     {0,0,0,1,0,2,0}, {0,0,0,1,2,0,0}, {0,0,1,0,0,0,2}, {0,0,1,0,0,2,0},
                                     {0,0,1,0,2,0,0}, {0,0,1,2,0,0,0}, {0,1,0,0,0,0,2}, {0,1,0,0,0,2,0},
                                     {0,1,0,0,2,0,0}, {0,1,0,2,0,0,0}, {0,1,2,0,0,0,0}, {1,0,0,0,0,0,2},
                                     {1,0,0,0,0,2,0}, {1,0,0,0,2,0,0}, {1,0,0,2,0,0,0}, {1,0,2,0,0,0,0},
                                     {1,2,0,0,0,0,0}, 
                                     {2,0,0,0,1,1,1}, {2,0,0,1,0,1,1}, {2,0,0,1,1,0,1}, {2,0,0,1,1,1,0}, {2,0,1,0,0,1,1}, 
                                     {2,0,1,0,1,0,1}, {2,0,1,0,1,1,0}, {2,0,1,1,0,0,1}, {2,0,1,1,0,1,0}, {2,0,1,1,1,0,0}, 
                                     {0,2,0,0,1,1,1}, {0,2,0,1,0,1,1}, {0,2,0,1,1,0,1}, {0,2,0,1,1,1,0}, {0,2,1,0,0,1,1}, 
                                     {0,2,1,0,1,0,1}, {0,2,1,0,1,1,0}, {0,2,1,1,0,0,1}, {0,2,1,1,0,1,0}, {0,2,1,1,1,0,0}, 
                                     {0,0,2,0,1,1,1}, {0,0,2,1,0,1,1}, {0,0,2,1,1,0,1}, {0,0,2,1,1,1,0}, {0,1,2,0,0,1,1}, 
                                     {0,1,2,0,1,0,1}, {0,1,2,0,1,1,0}, {0,1,2,1,0,0,1}, {0,1,2,1,0,1,0}, {0,1,2,1,1,0,0}, 
                                     {0,0,0,2,1,1,1}, {0,0,1,2,0,1,1}, {0,0,1,2,1,0,1}, {0,0,1,2,1,1,0}, {0,1,0,2,0,1,1}, 
                                     {0,1,0,2,1,0,1}, {0,1,0,2,1,1,0}, {0,1,1,2,0,0,1}, {0,1,1,2,0,1,0}, {0,1,1,2,1,0,0}, 
                                     {0,0,0,1,2,1,1}, {0,0,1,0,2,1,1}, {0,0,1,1,2,0,1}, {0,0,1,1,2,1,0}, {0,1,0,0,2,1,1}, 
                                     {0,1,0,1,2,0,1}, {0,1,0,1,2,1,0}, {0,1,1,0,2,0,1}, {0,1,1,0,2,1,0}, {0,1,1,1,2,0,0}, 
                                     {0,0,0,1,1,2,1}, {0,0,1,0,1,2,1}, {0,0,1,1,0,2,1}, {0,0,1,1,1,2,0}, {0,1,0,0,1,2,1}, 
                                     {0,1,0,1,0,2,1}, {0,1,0,1,1,2,0}, {0,1,1,0,0,2,1}, {0,1,1,0,1,2,0}, {0,1,1,1,0,2,0}, 
                                     {0,0,0,1,1,1,2}, {0,0,1,0,1,1,2}, {0,0,1,1,0,1,2}, {0,0,1,1,1,0,2}, {0,1,0,0,1,1,2}, 
                                     {0,1,0,1,0,1,2}, {0,1,0,1,1,0,2}, {0,1,1,0,0,1,2}, {0,1,1,0,1,0,2}, {0,1,1,1,0,0,2}}; 
                break;
        }            
        len = relevant_indices_absolute.size();
        len2 = i;
        //make relevant indices relative 
        relevant_indices_relative=std::vector<std::vector<int_fast8_t>>(len, std::vector<int_fast8_t>(len2));
        for(int i=len-1; i>0; i--) {
            for(int j=0; j<len2;j++)
                relevant_indices_relative[i][j] = relevant_indices_absolute[i][j] - relevant_indices_absolute[i-1][j];    
        }
        for(int j=0; j<len2;j++) relevant_indices_relative[0][j]= relevant_indices_absolute[0][j];
        
        //prepare masks and relevant indices
        masks=std::vector<ColouringBitArray>(len, ColouringBitArray(ColouringBitArray::pow3[len2], false));
        for(unsigned int i=0;i<len;i++) {                
            int a[6][3]={{0,1,2}, {0,2,1}, {1,0,2}, {1,2,0}, {2,0,1}, {2,1,0}};
            for(int j=0;j<6;j++) {
                ColouringBitArray::Index sum(0,0);
                for(int k=0; k<len2;k++) {
                    int val=relevant_indices_absolute[i][k];
                    if (a[j][val]>1)
                        sum+=ColouringBitArray::pow3[k];
                    if (a[j][val]>0)
                        sum+=ColouringBitArray::pow3[k];
                }
                masks[i].set(sum,true);
            }
        }        
    }

    int_fast8_t compare(const ColouringBitArray &cba, const std::vector<uint_fast8_t> &s, std::vector<uint_fast8_t>& best, uint_fast8_t order) const {
        ColouringBitArray::Index idx=ColouringBitArray::Index(0,0);
        int_fast8_t copymode=0;
        for (uint_fast8_t i=0; i<len; i++) {
            for(uint_fast8_t j=0; j<len2;j++)
                idx.manipulate_special(s[j], relevant_indices_relative[i][j]);
//            std::cout<<idx.first()<<","<<(int)idx.second()<<"\n";
            uint_fast8_t res;
            if (cba[idx]) res=1;
            else res=0;
            uint_fast8_t best2=best[i];
//            std::cout<<"res:"<<(int)res<<"best"<<(int)best2<<"Cmode"<<(int)copymode<<" O"<<(int)order<<"\n";
            if (copymode) {best[i]=res; continue;}
            if (res!=best2) { 
                uint_fast8_t res2=res;
                if (order) {res2=1-res; best2=1-best2;}
                if (res2<best2) { //better ... or just copyingcopy
                    best[i]=res;
                    copymode=1; //we just have to will new best
                }
                else return -1; //worse
            }
        }
        return copymode;
    }

    std::pair<uint_fast8_t, std::vector<uint_fast8_t>> calculateFirst(const ColouringBitArray &cba) const {
        ColouringBitArray::Index idx=ColouringBitArray::Index(0,0);
        std::vector<uint_fast8_t> ret(len);
        int cnt = 0;
        for (uint_fast8_t i=0; i<len; i++) {
            for(uint_fast8_t j=0; j<len2;j++) {
                idx.manipulate_special(j, relevant_indices_relative[i][j]);
            }
            if (cba[idx]) {ret[i] = 1; cnt++;}
            else ret[i] = 0;                 
        }
        if (2*cnt<len) return {1, ret};
        return {0, ret};
    }

};

inline static Comparator comparators[] = {Comparator(0), Comparator(1), Comparator(2), Comparator(3), Comparator(4), Comparator(5), Comparator(6), Comparator(7)}; //as of now


inline bool kempe_switch_exists(const ColouringBitArray cba, std::vector<uint_fast8_t> vec, 
                                const std::vector<uint_fast8_t> &pos, uint_fast8_t c1, uint_fast8_t c2) {
    for(auto i: pos) {
        if (vec[i]==c1) vec[i]=c2;
        else vec[i]=c1;
    }
    return cba[ColouringBitArray::Index(vec)];
}


} //internal namespace end


std::vector<uint_fast8_t> canonize(const ColouringBitArray &cba, uint_fast8_t len) {
    if (len<2) {
        uint_fast8_t t=0;
        if (cba[ColouringBitArray::Index(0,0)]) t=1;
        return std::vector<uint_fast8_t>({t});
    }
    const auto& cmp=colouring_bit_array_internal::comparators[len];
    auto [order, best] = cmp.calculateFirst(cba);
    std::vector<uint_fast8_t> sge(len); for(int i=0;i<len;i++) sge[i]=i;
    
    while(std::next_permutation(sge.begin(), sge.end())) {
//        std::cout<<"SGE"<<(int)sge[0]<<(int)sge[1]<<(int)sge[2]<<(int)sge[3]<<(int)sge[4]<<"\n";
        cmp.compare(cba, sge, best, order);  
    } 
    return best;
}

ColouringBitArray from_canon(const std::vector<uint_fast8_t> &canon, uint_fast8_t len) {
    ColouringBitArray res(ColouringBitArray::pow3[len], false);
    for(unsigned int i=0; i<canon.size();i++)
        if (canon[i])
            res|=colouring_bit_array_internal::comparators[len].masks[i];
    return res;
}

int canon_ones(const std::vector<uint_fast8_t> &canon) {
    int cnt=0;
    for(auto &i:canon) if (i==1) cnt++;
    return cnt;
}

std::vector<uint_fast8_t> compress_canon(std::vector<uint_fast8_t> &&canon) {
    std::vector<uint_fast8_t> res(((int)canon.size()-1)/8+1+1, 0); //+1 as rounding up, +1 res[0]
    res[0] = 7-(canon.size()-1)%8; //how many unused bits do we have
    canon.insert(canon.end(), res[0], 0);
    for(unsigned int i=1,j=0;j<canon.size();i++,j+=8) {
        for(int k1=0, k2=1; k1<8; k1++, k2*=2)
            if (canon[j+k1]) res[i]+=k2;
    }
    return res;
}

std::vector<uint_fast8_t> expand_canon(const std::vector<uint_fast8_t> &canon) {
    std::vector<uint_fast8_t> res(8*(canon.size()-1)-canon[0], 0); 
    for(unsigned int i=1,j=0;i<canon.size();i++,j+=8) {
        uint_fast8_t n=canon[i];
        for(int k1=0; k1<8 && j+k1<res.size(); k1++) {
            res[j+k1]=(n%2);
            n/=2;
        }
    }
    return res;
}







} //ba_graph end
#endif

