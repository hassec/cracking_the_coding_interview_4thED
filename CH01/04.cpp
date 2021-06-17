#include <algorithm>
#include <array>
#include <iostream>
#include <string>

// the easy version to implement is using sort
// followed by string equality
// but it's O( N log(N) ) instead of a possible O(N) solution
// Memory requirements are also not awesome since we copy both strings
// to not inplace alter the originals
bool is_anagram( std::string str1, std::string str2 )
{
  // can only be an anagram if size is the same
  if ( str1.size() != str2.size() ) {
    return false;
  }
  std::sort( begin( str1 ), end( str1 ) );
  std::sort( begin( str2 ), end( str2 ) );

  return str1 == str2;
}

// We can do it in O(N) and without copying our strings
// all we need is a buffer of 128 "counters" -> std::array<int, 128>
// This is more memory efficient compared to the above solution if len(str1)+len(str2) > 256
// This assumes the counts for a char never overflow an int
// otherwise one needs to go to size_t
bool is_anagram2( std::string const& str1, std::string const& str2 )
{
  if ( str1.size() != str2.size() ) {
    return false;
  }

  std::array<int, 128> char_counters{};

  for ( size_t idx{ 0 }; idx < str1.size(); idx++ ) {
    // positive counts for str1 chars
    char_counters[str1[idx]]++;
    // negative counts for str2 chars
    char_counters[str2[idx]]--;
  }
  // if not all entries are zero, it means that one string had characters the other one didn't
  // -> we don't have an anagram
  return std::all_of( begin( char_counters ), end( char_counters ), []( auto const count ) { return count == 0; } );
}

int main()
{
  std::string const s1{ "abcdefg" };
  std::string const s2{ "gfedcba" };
  std::string const s3{ "ffedcba" };
  std::string const s4{ "" };

  std::cout << "Test 1.1: " << is_anagram( s1, s2 ) << std::endl;
  std::cout << "Test 1.2: " << is_anagram( s1, s3 ) << std::endl;
  std::cout << "Test 1.3: " << is_anagram( s1, s4 ) << std::endl;
  std::cout << "Test 2.1: " << is_anagram2( s1, s2 ) << std::endl;
  std::cout << "Test 2.2: " << is_anagram2( s1, s3 ) << std::endl;
  std::cout << "Test 2.3: " << is_anagram2( s1, s4 ) << std::endl;
}
