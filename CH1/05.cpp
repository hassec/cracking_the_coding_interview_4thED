#include <algorithm>
#include <array>
#include <iostream>
#include <string>

// in place substitution of spaces (ASCII 32) with "%20"
// O(N) complexity
// Alternative would be find + replace member functions
// But that could result in multiple reallocations of the string
void replace( std::string& str )
{

  if ( str.empty() ) return;

  auto const num_spaces = std::count( begin( str ), end( str ), 32 );

  // need to remember size of current string for below
  auto const old_size = str.size();
  // %20 takes 3 chars compared to one for the ASCII char
  // -> replaced string needs two more chars for each space
  auto const new_size = old_size + 2 * num_spaces;

  // make our string just large enough for us to fit everything
  str.resize( new_size );

  // pointer to last char in new string
  auto* end = &str.back();
  // pointer to last char of old string
  auto const* old_end = &str[old_size - 1];

  // if the pointers are the same we have substituted all spaces
  // thus we keep going until that is the case
  while ( old_end != end ) {
    if ( *old_end == 32 ) {
      *end-- = '0';
      *end-- = '2';
      *end-- = '%';
      old_end--;
    } else {
      *end-- = *old_end--;
    }
  }
}

int main()
{

  std::string s1{ "this is a test" };
  std::string s2{ "  a  b " };

  replace( s1 );
  replace( s2 );

  std::cout << s1 << std::endl;
  std::cout << s2 << std::endl;
}
