#include <assert.h>
#include "colouring_bit_array_canon.hpp"

using namespace ba_graph;

int main() {
/**************
Comparator test masks
**************/
    ColouringBitArray test60=colouring_bit_array_internal::comparators[6].masks[0];
    ColouringBitArray test6x=colouring_bit_array_internal::comparators[6].masks[30];
    assert(test6x.get(0,0));
    assert(test6x.get(1+3+9,1+3+9));
    assert(test6x.get(2+6+18,2+6+18));
    {
        int cnt1=0;
        int cnt2=0;
        for(ColouringBitArray::Index i(0,0);i<test6x.size();i++, cnt2++)
            if (test6x[i]) cnt1++;
        assert(cnt1==3);
        assert(cnt2==3*3*3*3*3*3);
    }

    assert(test60.get(18+6,0));
    assert(test60.get(9+3,0));
    assert(test60.get(2+3+9,2+6+18));
    assert(test60.get(2+0+0,2+6+18));
    {
        int cnt1=0;
        int cnt2=0;
        for(ColouringBitArray::Index i(0,0);i<test60.size();i++, cnt2++)
            if (test60[i]) cnt1++;
        assert(cnt1==6);
        assert(cnt2==3*3*3*3*3*3);
    }


/**************
calculate_first, compare
**************/

    ColouringBitArray arr1(ColouringBitArray::pow3[5], false);
    arr1.set(3*0+1*0, 9*0+3*1+1*2, true); //21000
    arr1.set(3*0+1*0, 9*0+3*2+1*1, true); //12000

    ColouringBitArray arr2(ColouringBitArray::pow3[5], false);
    arr2.set(3*0+1*1, 9*0+3*0+1*2, true);
    arr2.set(3*0+1*2, 9*0+3*0+1*1, true);

    ColouringBitArray arr3(ColouringBitArray::pow3[5], false);
    arr3.set(3*0+0*1, 9*1+3*0+1*2, true);
    arr3.set(3*0+0*2, 9*2+3*0+1*1, true);

    ColouringBitArray arr4(ColouringBitArray::pow3[5], true);
    arr4.set(3*0+1*0, 9*0+3*1+1*2, false); //21000
    arr4.set(3*0+1*0, 9*0+3*2+1*1, false); //12000

    ColouringBitArray arr5(ColouringBitArray::pow3[5], true);
    arr5.set(3*0+1*1, 9*0+3*0+1*2, false); //20010
    arr5.set(3*0+1*2, 9*0+3*0+1*1, false); //10020

    ColouringBitArray arr6(ColouringBitArray::pow3[5], true);
    arr6.set(3*0+0*1, 9*1+3*0+1*2, false);
    arr6.set(3*0+0*2, 9*2+3*0+1*1, false);

    ColouringBitArray arr61(ColouringBitArray::pow3[6], false);
    arr61.set(9*0+3*0+1*1, 9*1+3*2+1*2, true); //221100
    arr61.set(9*0+3*0+1*2, 9*2+3*1+1*1, true); //112200
    arr61.set(9*1+3*1+1*0, 9*0+3*2+1*2, true); //220011
    arr61.set(9*1+3*1+1*2, 9*2+3*0+1*0, true); //002211
    arr61.set(9*2+3*2+1*0, 9*0+3*1+1*1, true); //110022
    arr61.set(9*2+3*2+1*1, 9*1+3*0+1*0, true); //001122

    ColouringBitArray arr62(ColouringBitArray::pow3[6], true);
    arr62.set(9*0+3*0+1*1, 9*2+3*1+1*2, false); //212100
    arr62.set(9*0+3*0+1*2, 9*1+3*2+1*1, false); //121200
    arr62.set(9*1+3*1+1*0, 9*2+3*0+1*2, false); //202011
    arr62.set(9*1+3*1+1*2, 9*0+3*2+1*0, false); //020211
    arr62.set(9*2+3*2+1*0, 9*1+3*0+1*1, false); //101022
    arr62.set(9*2+3*2+1*1, 9*0+3*1+1*0, false); //010122

    colouring_bit_array_internal::Comparator &cmp = colouring_bit_array_internal::comparators[5];
    colouring_bit_array_internal::Comparator &cmp6 = colouring_bit_array_internal::comparators[6];

    auto [res1a, res1b] = cmp.calculateFirst(arr1);
    auto [res2a, res2b] = cmp.calculateFirst(arr2);
    auto [res3a, res3b] = cmp.calculateFirst(arr3);
    auto [res4a, res4b] = cmp.calculateFirst(arr4);
    auto [res5a, res5b] = cmp.calculateFirst(arr5);
    auto [res6a, res6b] = cmp.calculateFirst(arr6);
    auto [res61a, res61b] = cmp6.calculateFirst(arr61);
    auto [res62a, res62b] = cmp6.calculateFirst(arr62);

    assert(res1a==1 && res2a==1 && res3a==1);
    assert(res4a==0 && res5a==0 && res6a==0);

    assert(res61a==1 && res62a==0);

    assert(res1b==std::vector<uint_fast8_t>({0,0,0,0,0,0,0,0,0,1}));
    assert(res2b==std::vector<uint_fast8_t>({0,0,0,0,0,0,0,1,0,0}));
    assert(res3b==std::vector<uint_fast8_t>({0,0,0,0,0,0,0,0,1,0}));
    assert(res4b==std::vector<uint_fast8_t>({1,1,1,1,1,1,1,1,1,0}));
    assert(res5b==std::vector<uint_fast8_t>({1,1,1,1,1,1,1,0,1,1}));
    assert(res6b==std::vector<uint_fast8_t>({1,1,1,1,1,1,1,1,0,1}));
    assert(res61b==std::vector<uint_fast8_t>({0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}));
    assert(res62b==std::vector<uint_fast8_t>({1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1}));

    assert(cmp.compare(arr1, std::vector<uint_fast8_t>({0, 1, 2, 3, 4}), res1b, 1)==0);
    assert(cmp.compare(arr1, std::vector<uint_fast8_t>({1, 0, 2, 3, 4}), res1b, 1)==0);
    assert(cmp.compare(arr1, std::vector<uint_fast8_t>({1, 0, 3, 4, 2}), res1b, 1)==0);
    assert(cmp.compare(arr1, std::vector<uint_fast8_t>({2, 1, 0, 3, 4}), res1b, 1)==1);
    assert(res1b==std::vector<uint_fast8_t>({0,0,0,0,0,1,0,0,0,0}));
    assert(cmp.compare(arr1, std::vector<uint_fast8_t>({0, 1, 2, 3, 4}), res1b, 1)==-1);

    assert(cmp.compare(arr5, std::vector<uint_fast8_t>({0, 1, 2, 3, 4}), res5b, 0)==0);
    assert(cmp.compare(arr5, std::vector<uint_fast8_t>({3, 1, 2, 0, 4}), res5b, 0)==0);
    assert(cmp.compare(arr5, std::vector<uint_fast8_t>({1, 0, 2, 3, 4}), res5b, 0)==1);
    assert(res5b==std::vector<uint_fast8_t>({1,1,1,1,0,1,1,1,1,1}));
    assert(cmp.compare(arr5, std::vector<uint_fast8_t>({1, 0, 3, 4, 2}), res5b, 0)==-1);
    assert(cmp.compare(arr5, std::vector<uint_fast8_t>({1, 2, 4, 0, 3}), res5b, 0)==1);
    assert(res5b==std::vector<uint_fast8_t>({0,1,1,1,1,1,1,1,1,1}));
    
    
    

/**************************
Canonical form testing
**************************/
    auto can1=canonize(arr1, 5);
    auto can2=canonize(arr2, 5);
    auto can3=canonize(arr3, 5);
    auto can4=canonize(arr4, 5);
    auto can5=canonize(arr5, 5);
    auto can6=canonize(arr6, 5);
    auto can61=canonize(arr61, 6);
    auto can62=canonize(arr62, 6);
    assert(can1==std::vector<uint_fast8_t>({1,0,0,0,0,0,0,0,0,0}));
    assert(can2==std::vector<uint_fast8_t>({1,0,0,0,0,0,0,0,0,0}));
    assert(can3==std::vector<uint_fast8_t>({1,0,0,0,0,0,0,0,0,0}));
    assert(can4==std::vector<uint_fast8_t>({0,1,1,1,1,1,1,1,1,1}));
    assert(can5==std::vector<uint_fast8_t>({0,1,1,1,1,1,1,1,1,1}));
    assert(can6==std::vector<uint_fast8_t>({0,1,1,1,1,1,1,1,1,1}));
    assert(can61==std::vector<uint_fast8_t>({0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}));
    assert(can62==std::vector<uint_fast8_t>({1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}));

    arr1.set(3*0+1*1, 9*2+3*0+1*0, true);
    arr1.set(3*0+1*2, 9*1+3*0+1*0, true);
    //arr1 ma 12000, 21000, 00120, 00210
    can1=canonize(arr1, 5);
    assert(can1==std::vector<uint_fast8_t>({1,0,0,0,0,1,0,0,0,0}));
    
    auto can1c=compress_canon(std::move(can1));
    auto can2c=compress_canon(std::move(can2));
    auto can4c=compress_canon(std::move(can4));
    auto canx=compress_canon(std::move(std::vector<uint_fast8_t>({1,0,0,0,0,1,0,0})));
    assert(can2c==std::vector<uint_fast8_t>({6, 1,0}));
    assert(can1c==std::vector<uint_fast8_t>({6, 33,0}));
    assert(can4c==std::vector<uint_fast8_t>({6, 254,3}));
    assert(canx==std::vector<uint_fast8_t>({0, 33}));
    assert(expand_canon(canx)==std::vector<uint_fast8_t>({1,0,0,0,0,1,0,0}));
    assert(expand_canon(can1c)==std::vector<uint_fast8_t>({1,0,0,0,0,1,0,0,0,0}));

/**************************
from_canon test`
**************************/
    ColouringBitArray constructed_cba=from_canon(std::vector<uint_fast8_t>({1,0,0,0,0,1,0,0}), 5); //{0,0,0,1,2}, {0,1,2,0,0}
    assert(constructed_cba.get(1+6,0+0+0));
    assert(constructed_cba.get(2+3,0+0+0));
    assert(constructed_cba.get(2+0,1+3+9));
    assert(constructed_cba.get(0+0,0+3+18));
    assert(constructed_cba.get(2+6,2+0+9));
    {
        int cnt1=0;
        int cnt2=0;
        for(ColouringBitArray::Index i(0,0);i<constructed_cba.size();i++, cnt2++)
            if (constructed_cba[i]) cnt1++;
        assert(cnt1==12);
        assert(cnt2==3*3*3*3*3);
    }    
    

}   
