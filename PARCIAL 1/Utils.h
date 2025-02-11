#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>
#include <cstring>
#include <cmath>

class Utils {
public:
    static std::vector<int> stringToC(const char* str);
    static char* cToString(const std::vector<int>& bytes);
    static void freeCString(char* str);
    static int powerModulus(int base, int expo, int m);
    static int modInverse(int e, int phi);
};

#endif
