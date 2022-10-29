#include <basic_impl.hpp>
#include <cassert>
#include <snarks/colouring_pd.hpp>
#include <algorithm>
#include <map>

using namespace ba_graph;

ColouringBitArray calculate(const Graph &g, std::vector<int> perm) {
    PDColorizer c;

    PathDecomposition pd;
    for(auto i:g.list(RP::all(), IP::primary())) 
        if ((i->r1().degree()==3) && (i->r2().degree()==3))
            pd.ordered_edges.push_back(i->e());
    pd.ordered_edges.push_back(g[perm[0]][0].e());
    pd.ordered_edges.push_back(g[perm[1]][0].e());
    pd.ordered_edges.push_back(g[perm[2]][0].e());
    pd.ordered_edges.push_back(g[perm[3]][0].e());
    pd.ordered_edges.push_back(g[perm[4]][0].e());
       
    c.initialize(g);
    c.isColourable(g, pd);
    
    
    ColouringBitArray res(ColouringBitArray::Index(9,0), false);
    assert(c.state.size()==243);
    for(int i=0;i<243;i++) {
        if (c.state[i][ColouringBitArray::Index(0,0)]==1) {
            res.set(ColouringBitArray::Index(i/27,i%27), true);
        }
    }
    return res;   
}

int main() {
    std::vector<Graph> Gs;
    for(int i=0;i<16;i++)
        Gs.push_back(createG());
    
    addMultipleV(Gs[0], {0, 1, 2, 3, 4, 9, 7, 8}); //7,8 unnecessary
    addMultipleE(Gs[0], Locs({{0,7}, {1,8}, {7,8}, {7,8}, {2,9}, {3,9}, {4,9}}));

    addMultipleV(Gs[1], {0, 1, 2, 3, 4, 7, 8, 9, 10, 11, 12, 13}); //10--13 unnecessary
    addMultipleE(Gs[1], Locs({{0,10}, {1,11}, {10,11}, {10,11}, {2,12}, {3,13}, {12,13}, {12,13}, 
                              {4,7}, {7,8}, {7,9}, {8,9}, {8,9}}));

    addMultipleV(Gs[2], {0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
    addMultipleE(Gs[2], Locs({{5,6}, {6,7}, {7,8}, {8,5}, {5,9}, {0,9}, {1,9}, {2,6}, {3,7}, {4,8}}));

    addMultipleV(Gs[3], {0, 1, 2, 3, 4, 7, 8, 9});
    addMultipleE(Gs[3], Locs({{0,7}, {1,7}, {7,8}, {2,8}, {8,9}, {3,9}, {4,9}}));

    addMultipleV(Gs[4], {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13});
    addMultipleE(Gs[4], Locs({{0,5}, {1,6}, {5,6}, {5,7}, {6,8}, {7,9}, {8,9}, {11,9}, {7,10}, 
                              {8,10}, {12,10}, {11,12}, {11,2}, {12,13}, {13,3}, {13,4}}));

    addMultipleV(Gs[5], {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    addMultipleE(Gs[5], Locs({{0,5}, {1,6}, {5,6}, {5,7}, {6,8}, {7,9}, {8,9}, {2,9}, {7,10}, 
                              {8,10}, {11,10}, {3,11}, {4,11}}));
    
    addMultipleV(Gs[6], {0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
    addMultipleE(Gs[6], Locs({{5,6}, {6,7}, {7,8}, {8,9}, {9,5}, {0,5}, {1,6}, {2,7}, {3,8}, {4,9}}));
    
    addMultipleV(Gs[7], {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    addMultipleE(Gs[7], Locs({{0,5}, {1,6}, {5,6}, {5,7}, {6,8}, {7,8}, {7,9}, {9,10}, {10,11}, 
                              {11,8}, {9,2}, {10,3}, {4,11}}));

    addMultipleV(Gs[8], {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    addMultipleE(Gs[8], Locs({{0,5}, {1,6}, {5,6}, {5,7}, {6,8}, {7,2}, {7,9}, {9,10}, {9,11}, 
                              {11,8}, {10,8}, {10,3}, {4,11}}));
    
    addMultipleV(Gs[9], {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13});
    addMultipleE(Gs[9], Locs({{0,5}, {1,6}, {5,6}, {5,7}, {6,8}, {7,9}, {7,10}, {8,9}, {8,10},
                              {9,11}, {11,12}, {12,13}, {13,10}, {11,2}, {12,3}, {13,4}}));

    addMultipleV(Gs[10], {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15});
    addMultipleE(Gs[10], Locs({{0,5}, {1,6}, {5,6}, {5,7}, {6,8}, {7,9}, {8,9}, {11,9}, {7,10}, 
                               {8,10}, {12,10}, {11,12}, {11,13}, {13,14}, {14,15}, {15,12}, {13,2}, {14,3}, {15,4}}));

    addMultipleV(Gs[11], {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    addMultipleE(Gs[11], Locs({{0,5}, {1,6}, {5,6}, {5,7}, {6,8}, {7,9}, {8,9}, {9,2}, {7,10}, 
                               {10,11}, {11,8}, {10,3}, {4,11}}));

    addMultipleV(Gs[12], {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17});
    addMultipleE(Gs[12], Locs({{0,5}, {1,6}, {5,6}, {5,7}, {6,8}, {7,9}, {8,9}, {10,9}, {7,11},
                              {11,2}, {11,12}, {12,3}, {12,13}, {13,14}, {14,4}, {8,15},
                              {10,16}, {15,16}, {14,16}, {10,17}, {15,17}, {13,17}}));

    addMultipleV(Gs[13], {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13});
    addMultipleE(Gs[13], Locs({{0,5}, {1,6}, {5,6}, {5,7}, {6,8}, {7,9}, {8,9}, {10,9}, {7,11},
                              {8,12}, {11,13}, {13,10}, {10,12}, {11,2}, {12,3}, {13,4}}));

    addMultipleV(Gs[14], {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19});
    addMultipleE(Gs[14], Locs({{0,5}, {1,6}, {5,6}, {5,7}, {6,8}, {7,9}, {8,9}, {10,9}, {7,11},
                              {11,2}, {11,12}, {12,18}, {12,13}, {13,14}, {14,19}, {8,15},
                              {10,16}, {15,16}, {14,16}, {10,17}, {15,17}, {13,17}, {18,19}, {18,3}, {19,4}}));

    addMultipleV(Gs[15], {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15});
    addMultipleE(Gs[15], Locs({{0,5}, {1,6}, {5,6}, {2,7}, {3,8}, {7,8}, {5,9}, {9,7}, {9,10}, 
                               {11,10}, {12,10}, {11,13}, {12,13}, {13,6}, {11,14}, {12,15}, {14,15}, {14,4}, {15,8}}));


    for(Graph &G:Gs) 
        for(Rotation &r: G) {
            if (r.n()<5) assert(r.degree()==1);
            else assert(r.degree()==3);
        }
        
        
    int cnt=0;
    
    std::map<ColouringBitArray, int> m;
    
    
    for(int i=0; i<16; i++) {
        std::vector<int> perm = {0,1,2,3,4};
        do {
            auto cba=calculate(Gs[i], perm);
            assert(cba.size()==ColouringBitArray::Index(9,0));
            
            auto it=m.find(cba);
            if (it==m.end()) m[cba]=i;
            
            for(ColouringBitArray::Index j(0,0); j<cba.size(); j++) std::cout<<(int)cba[j];
            std::cout<<"\n";
            std::cout<<cnt<<"\n";
            
            if (m[cba]!=i) {
                std::cout<<"Pruser: "<<i<<" vs "<<m[cba]<<"    odmakane "<<cnt<<"\n";
                return 0;
            }
            cnt++;
        } while(std::next_permutation(perm.begin(), perm.end()));
    }
    
    assert(cnt==16*120);

}