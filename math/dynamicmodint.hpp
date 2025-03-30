#pragma once
#include<bits/stdc++.h>
using namespace std;
vector<long long> dynamicmodint_MOD;
// mod の値を追加する
void addmod(long long x) { dynamicmodint_MOD.push_back(x); }
struct dmint {
    vector<long long> val;
    dmint(long long x=0) : val(dynamicmodint_MOD.size()) {
        assert(0 < dynamicmodint_MOD.size());
        for (size_t i = 0; i < dynamicmodint_MOD.size(); ++i) val[i] = (x % dynamicmodint_MOD[i] + dynamicmodint_MOD[i]) % dynamicmodint_MOD[i];
    }
    dmint& operator+=(const dmint& a) { for (int i=0; i<dynamicmodint_MOD.size(); i++) val[i] = (val[i] + a.val[i]) % dynamicmodint_MOD[i]; return *this; }
    dmint& operator-=(const dmint& a) { for (int i=0; i<dynamicmodint_MOD.size(); i++) val[i] = (val[i] - a.val[i]) % dynamicmodint_MOD[i]; return *this; }
    dmint& operator*=(const dmint& a) { for (int i=0; i<dynamicmodint_MOD.size(); i++) val[i] = (val[i] * a.val[i]) % dynamicmodint_MOD[i]; return *this; }
    dmint& operator/=(const dmint& a) { return *this *= a.inv(); }
    dmint operator+(const dmint& a) const { return dmint(*this) += a; }
    dmint operator-(const dmint& a) const { return dmint(*this) -= a; }
    dmint operator*(const dmint& a) const { return dmint(*this) *= a; }
    dmint operator/(const dmint& a) const { return dmint(*this) /= a; }
    dmint& operator+=(int a) { return *this += dmint(a); }
    dmint& operator-=(int a) { return *this -= dmint(a); }
    dmint& operator*=(int a) { return *this *= dmint(a); }
    dmint& operator/=(int a) { return *this /= dmint(a); }
    dmint operator+(int a) const { return dmint(*this) += a; }
    dmint operator-(int a) const { return dmint(*this) -= a; }
    dmint operator*(int a) const { return dmint(*this) *= a; }
    dmint operator/(int a) const { return dmint(*this) /= a; }
    friend dmint operator+(int a,const dmint& b) { return dmint(a) + b; }
    friend dmint operator-(int a,const dmint& b) { return dmint(a) - b; }
    friend dmint operator*(int a,const dmint& b) { return dmint(a) * b; }
    friend dmint operator/(int a,const dmint& b) { return dmint(a) / b; }
    dmint& operator++() { return *this += 1; }
    dmint operator++(int) { dmint r = *this; *this += 1; return r; }
    dmint& operator--() { return *this -= 1; }
    dmint operator--(int) { dmint r = *this; *this -= 1; return r; }
    // modpow を計算する。計算量O(log mod)
    dmint pow(long long n) const {
        dmint re = dmint(0);
        for (int i=0; i < dynamicmodint_MOD.size(); i++) {
            int m = dynamicmodint_MOD[i];
            if (n != 0) n = ((n-2) % (m-1) + m) % (m-1) + 1;
            long long r = 1, a = val[i];
           while (n) {
                if (n & 1) r = r * a % m;
                a = a * a % m;
                n >>= 1;
            }
            re.val[i] = r;
        }
        return re;
    }
    dmint inv() const { return pow(-1); }
};