#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <ctype.h>
#include <stdio.h>
#include <string>

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
      std::string copy = k;
      HASH_INDEX_T hash = 0;

      // initialize w[i] array to 0's
      unsigned long long w[5] = {0, 0, 0, 0, 0};

      // keep zeros in leading positions of w if not enough chars
      for(size_t i = 0; i < 5; i++) {
        if(k.size() > (6 * (4-i))) {
          // subset last 6 characters of copy
          int substr_len = 6;
          if((copy.size() % 6 != 0) && (copy.size() > 6)) {
            substr_len = copy.size() % 6;
          }
          std::string substr = copy.substr(0, substr_len);
          copy.erase(0, substr_len);

          // convert each letter in substr to base-36
          for(int j = substr.size()-1; j >= 0; j--) {
            int base36 = letterDigitToNumber(substr[j]);
            w[i] += pow(36, substr.size()-1-j) * base36;
          }
        }

        //std::cout << "w[" << i << "] = " << w[i] << "\n";
        hash += rValues[i] * w[i];
      }

      return hash;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
      HASH_INDEX_T value;

      // convert a-z to integer value 0-25
      if(isalpha(letter)) {
        value = tolower(letter) - 97;
      }

      // convert 0-9 to integer value 26-35
      else if(isdigit(letter)) {
        value = atoi(&letter) + 26;
      }

      return value;
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
