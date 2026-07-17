#include <bits/stdc++.h>
#include "utility/presetsegtree.hpp"
using namespace std;using namespace poe;
int main(){ios::sync_with_stdio(false);cin.tie(nullptr);int n,q;cin>>n>>q;vector<pair<long long,long long>>p(n);for(auto&[x,y]:p)cin>>x>>y;point_set_range_manhattan_max d(p);while(q--){int t;cin>>t;if(t==1){int i;long long x,y;cin>>i>>x>>y;d.set(i-1,x,y);}else{int l,r;long long x,y;cin>>l>>r>>x>>y;cout<<d.max(l-1,r,x,y)<<'\n';}}}
