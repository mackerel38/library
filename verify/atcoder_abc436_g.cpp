#include <bits/stdc++.h>
#include "dp/linearinequality.hpp"
using namespace std;using namespace poe;
int main(){ios::sync_with_stdio(false);cin.tie(nullptr);int n;unsigned long long m;cin>>n>>m;vector<int>a(n);for(int&x:a)cin>>x;cout<<count_nonnegative_weighted_sum_at_most(a,m).val()<<'\n';}
