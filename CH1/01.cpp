#include <algorithm>
#include <iostream>
#include <string>

// Complexity is dominated by the sort -> O(N log(N))
// adjacent_find is at worst N
// Take string by value since we modify it inplace
// Would not want to screw up the users string!
// Note:  One could make this O(N) by keeping track of each seen char value
//        But without using std::array this would mean bitshifting which ends
//        up with code that is hard to read and one would probably impose
//        restrictions to get away with an int or maximally a long int as bitfield
bool all_unique_chars( std::string str )
{
  std::sort( begin( str ), end( str ) );
  return std::adjacent_find( begin( str ), end( str ) ) == end( str );
}

int main()
{
  std::cout << "Test 1: " << all_unique_chars( "abcdefgh" ) << std::endl;
  std::cout << "Test 2: " << all_unique_chars( "abcdefgha" ) << std::endl;
}
