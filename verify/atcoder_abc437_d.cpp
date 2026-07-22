#include <bits/stdc++.h>
#include "algorithm/absolutedifference.hpp"
using namespace std;using namespace poe;
int main(){ios::sync_with_stdio(false);cin.tie(nullptr);int n,m;cin>>n>>m;vector<__int128>a(n),b(m);for(auto&x:a){long long v;cin>>v;x=v;}for(auto&x:b){long long v;cin>>v;x=v;}cout<<(long long)(sum_cross_absolute(a,b)%998244353)<<'\n';}
