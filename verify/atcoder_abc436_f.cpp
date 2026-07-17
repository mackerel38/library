#include <bits/stdc++.h>
#include "structure/waveletmatrix.hpp"
using namespace std;using namespace poe;
int main(){ios::sync_with_stdio(false);cin.tie(nullptr);int n;cin>>n;vector<int>b(n);for(int&x:b)cin>>x;waveletmatrix<int,long long> matrix(b);long long answer=0;for(int i=0;i<n;++i){long long left=matrix.count_less(0,i,b[i]);long long right=matrix.count_less(i+1,n,b[i]);answer+=(left+1)*(right+1);}cout<<answer<<'\n';}
