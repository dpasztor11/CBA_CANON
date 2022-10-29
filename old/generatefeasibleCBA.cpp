#include "colouring_bit_array_canon.hpp"
#include "kempe_closed.hpp"
#include<iostream>

using namespace ba_graph;

const int len=6;

int main(){
    const auto &cmp=colouring_bit_array_internal::comparators[len];
    
    std::vector<uint_fast8_t> canon(cmp.relevant_indices_absolute.size(),1);
    int cnt=0;
    int cnt2=0;
    
    while(1) {
        ColouringBitArray cba=from_canon(canon, len);
        if (canonize(cba,len)==canon) {
            cnt2++;
            if (is_kempe_closed(cba,len)) cnt++;
        }
        
        unsigned int i=0;
        for(;i<canon.size();i++) {
            if (canon[i]==0) canon[i]=1;
            else {canon[i]=0; break;}
        }
        if(i>15) std::cout<<"Working: "<<(int)canon[16]<<(int)canon[17]<<(int)canon[18]<<(int)canon[19]<<(int)canon[20]<<(int)canon[21]<<(int)canon[22]<<(int)canon[23]<<(int)canon[24]<<(int)canon[25]<<(int)canon[26]<<(int)canon[27]<<(int)canon[28]<<(int)canon[29]<<(int)canon[30]<<" cnt:"<<cnt<<"/"<<cnt2<<"\n";
        if (i==canon.size()) break;
        
    }
    
    std::cout<<cnt<<"\n";
    

    
}

