/*  _______         __
   |_     _|.-----.|  |_.-----.----.
    _|   |_ |     ||   _|  -__|   _|
   |_______||__|__||____|_____|__|
       coded by Questor / Inter      */

#ifndef EASTL_EXTRA_STRINGUTILS_H_
#define EASTL_EXTRA_STRINGUTILS_H_

#include "eastl/string.h"
#include "eastl/vector.h"

namespace eastl {

void replace_all(eastl::string & in, const eastl::string &plain, const eastl::string &tok) {
   eastl::string::size_type n = 0;
   const eastl::string::size_type l = plain.length();
   while(1) {
      n = in.find(plain, n);
      if(n != -1) {
         in.replace(n, l, tok);
      } else {
         break;
      }
   }
}

int wildstrcmp(const char *wild, const char *string) {
  //written by Jack Handy - jakkhandy@hotmail.com
  //http://www.codeproject.com/KB/string/wildcmp.aspx

  const char *cp = NULL, *mp = NULL;
  while ((*string) && (*wild != '*')) {
    if ((*wild != *string) && (*wild != '?')) {
      return 0;
    }
    wild++;
    string++;
  }
  while (*string) {
    if (*wild == '*') {
      if (!*++wild) {
        return 1;
      }
      mp = wild;
      cp = string+1;
    } else if ((*wild == *string) || (*wild == '?')) {
      wild++;
      string++;
    } else {
      wild = mp;
      string = cp++;
    }
  }
  while (*wild == '*') {
    wild++;
  }
  return !*wild;
}

// Levenshtein Distance
// taken from http://www.merriampark.com/ldcpp.htm
int levenshteinDistance(const eastl::string source, const eastl::string target) {

  const int n = source.length();
  const int m = target.length();
  if (n == 0) {
    return m;
  }
  if (m == 0) {
    return n;
  }

  eastl::vector< eastl::vector<int> > matrix(n+1);

  // Size the vectors in the 2.nd dimension. Unfortunately C++ doesn't
  // allow for allocation on declaration of 2.nd dimension of vec of vec

  for (int i = 0; i <= n; i++)
    matrix[i].resize(m+1);

  for (int i = 0; i <= n; i++)
    matrix[i][0]=i;

  for (int j = 0; j <= m; j++)
    matrix[0][j]=j;

  for (int i = 1; i <= n; i++) {
    const char s_i = source[i-1];

    for (int j = 1; j <= m; j++) {
      const char t_j = target[j-1];

      int cost;
      if (s_i == t_j)
        cost = 0;
      else
        cost = 1;

      const int above = matrix[i-1][j];
      const int left = matrix[i][j-1];
      const int diag = matrix[i-1][j-1];
      int cell = min( above + 1, min(left + 1, diag + cost));

      // Cover transposition, in addition to deletion,
      // insertion and substitution. This step is taken from:
      // Berghel, Hal ; Roach, David : "An Extension of Ukkonen's
      // Enhanced Dynamic Programming ASM Algorithm"
      // (http://www.acm.org/~hlb/publications/asm/asm.html)

      if (i>2 && j>2) {
        int trans=matrix[i-2][j-2]+1;
        if (source[i-2]!=t_j)
           trans++;
        if (s_i!=target[j-2])
           trans++;
        if (cell>trans)
           cell=trans;
      }
      matrix[i][j]=cell;
    }
  }
  return matrix[n][m];
}

//number conversion routines
template<typename T> T numberFromString(eastl::string &string) {
   ASSERT(0);
}
template<> float numberFromString(eastl::string &string) {
   return (float)strtod(string.c_str(), NULL);
}
template<> int32_t numberFromString(eastl::string &string) {
   //TODO: check for 0x or 0b in front of string!
   if(string.compare(0, 2, "0x") == 0) {
      XASSERT(0, "DO HEX INPUT");
      return -1;
   } else if(string.compare(0, 2, "0b") == 0) {
      XASSERT(0, "DO BINARY INPUT");
      return -1;
   } else {
     return (int32_t)strtol(string.c_str(), NULL, 10);
   }
}

template<typename T> eastl::string numberToString(T value) {
   ASSERT(0);
}
template<> eastl::string numberToString(float value) {
   //much better implementation in fixedpoint!
   XASSERT(0, "DO BETTER IMPLEMENTATION!");
   char buffer[80];
   char *fstr = buffer;
   int m = (int)log10(value);
   int digit;
   float precision = .0001f;

   while (value > 0 + precision) {
       float weight = pow(10.0f, m);
       digit = (int)floor(value / weight);
       value -= (digit*weight);
       *(fstr++)= '0' + digit;
       if (m == 0)
           *(fstr++) = '.';
       m--;
   }
   *(fstr) = '\0';
   return eastl::string(buffer);
}


}  //namespace eastl

#endif   //#ifndef EASTL_EXTRA_STRINGUTILS_H_
