#pragma once
#include <iostream>
#include <string>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;

using std::size_t;

typedef std::string String;
typedef std::vector<String> ArrayOfStrings;

inline ArrayOfStrings toArrayOfStrings(const String &str, const char delim) {
    ArrayOfStrings newArrayOfStrings;
    // cout << "str: " << str << endl << "delim: " << delim << endl;
    size_t found = str.find_first_of(delim);
    size_t startPos = 0;
    size_t i = 0;
    while (found != String::npos) {
        newArrayOfStrings.push_back(str.substr(startPos, found - startPos));
        cout << "newArrayOfStrings[" << i << "]: " << newArrayOfStrings[i] << endl;
        startPos = found + 1;
        found = str.find_first_of(delim, startPos);
        i++;
    }

    // cout << "newArrayOfStrings.size(): " << newArrayOfStrings.size() << endl
    // << "i: " << i << endl;
    return newArrayOfStrings;
}

inline String eraseAllMatching(const String &str, const String &remStr) {
    // cout << "str: " << str << endl << "remStr: " << remStr << endl;
    String newString = str;
    size_t found = newString.find_first_of(remStr);
    size_t startPos = 0;
    // cout << "found: " << found << endl;
    while (found != String::npos) {
        newString.erase(found, 1);
        found = newString.find_first_of(remStr);
        // cout << "found: " << found << endl;
    }

    return newString;
}
/*
Example Code to convert to lowercase

#include <algorithm>
#include <iostream>
using namespace std;

int main() {
   string my_str = "Hello WORLD";

   cout << "Main string: " << my_str << endl;
   transform(my_str.begin(), my_str.end(), my_str.begin(), ::tolower);

   cout << "Converted String: " << my_str;
}
Output
Main string: Hello WORLD
Converted String: hello world
*/