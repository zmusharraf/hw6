#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
        
        //stores integer values of each 6 char block
        //w[4] is last block of string
        unsigned long long w[5] = {0, 0, 0, 0, 0};

        int groupSize = 6;
        int len = (int)k.size();
        if(len == 0){
          return 0;
        }

        //how many 6 character groups
        int numGroups = (len + groupSize)/groupSize;
        if(numGroups > 5){
          numGroups = 5; 
        }

        //build groups from end of string
        for(int g = 0; g < numGroups; g++){
          int end = len - g*groupSize;
          int start = end - groupSize;
          if(start < 0){
            start = 0;
          }

          unsigned long long groupVal = 0;

          for(int i = start; i < end; i++){
            char c = k[i];
            unsigned long long digit = (unsigned long long)letterDigitToNumber(c);
            groupVal = groupVal*36 + digit;
          }

          int wIndex = 4-g;
          if(wIndex < 0){
            wIndex = 0;
          }
          w[wIndex] = groupVal;
        }
        //compute hash: sum r[i]*w[i]
        unsigned long long total = 0;
        for(int i = 0; i < 5; i++){
          total += (unsigned long long)rValues[i] * w[i];
        }
        return (HASH_INDEX_T)total;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it

        //convert uppercase to lowercase
        if(letter >= 'A' && letter <= 'Z'){
          letter = letter - 'A' + 'a';
        }

        if(letter >= 'a' && letter <= 'z'){
          return (HASH_INDEX_T)(letter - 'a');
        } else if(letter >= '0' && letter <= '9'){
          return (HASH_INDEX_T)(26 + (letter - '0'));
        } else {
          //outside letters or digits
          return 0;
        }

    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
