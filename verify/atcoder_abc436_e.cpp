#include <bits/stdc++.h>
#include "graph/functionalgraph.hpp"
using namespace std;using namespace poe;
int main(){ios::sync_with_stdio(false);cin.tie(nullptr);int n;cin>>n;vector<int>p(n);for(int&x:p){cin>>x;--x;}functionalgraph graph(p);long long answer=0;for(int id=0;id<graph.cycle_count();++id){long long size=graph.cycle(id).size();answer+=size*(size-1)/2;}cout<<answer<<'\n';}
