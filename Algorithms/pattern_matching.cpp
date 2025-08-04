#include<iostream>
#include<vector>
using namespace std;
using str = string;

void naive_matching(const str& s, const str& pattern) {
    for (int i = 0; i + pattern.size() <= s.size(); i++) {
        int j = 0;
        while (j < pattern.size() && s[i+j] == pattern[j]) j++;
        if (j == pattern.size()) {
            cout << "Found Match" << endl;
            return;
        }
    }
    cout << "Did Not Find Match" << endl;
}

void kmp(const str& s, const str& pattern) {
    vector<int> lps((int)pattern.size(), 0);
    int j = 0; // length of the previous longest prefix suffix
    for (int i = 1; i < pattern.size();) {
        if (pattern[i] == pattern[j]) {
            j++;
            lps[i] = j;
            i++;
        }
        else {
            if (j != 0) j = lps[j - 1];
            else {    
                lps[i] = 0;
                i++;
            }
        }
    }

    j = 0;
    for (int i = 0; i < s.size();) {
        if (s[i] == pattern[j]) {
            i++; j++;
        }
        if (j == pattern.size()) {
            cout << "Found pattern at index " << i - j << endl;
            j = lps[j-1];
        }
        else if (i < s.size() && s[i] != pattern[j]) {
            if (j != 0) j = lps[j - 1];
            else i++;
        }
    }
}

void rabin_karp(const str& s, const str& pattern) {

}

void z(const str& s) {

}

int main() {
    str s = "asdfaasdsasdfssaasdsdfd";
    str pattern = "aasds";
    kmp(s, pattern);
    return 0;
}