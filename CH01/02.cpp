#include <cstring>
#include <iostream>

// Complexity is N/2 swaps
void reverse( char* str )
{
  // extra -1 since we want to leave the null char at the end
  size_t end   = std::strlen( str ) - 1;
  size_t start = 0;
  while ( end > start ) {
    auto tmp     = str[end];
    str[end--]   = str[start];
    str[start++] = tmp;
  }
}

int main()
{
  char test[] = "abcd";
  reverse( test );
  std::cout << "Test 1: " << std::string( test ) << std::endl;
}
