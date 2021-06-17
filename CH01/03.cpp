#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// easy solution assuming the str does not need to stay in order
// complexity is dominated by sort -> O( N log(N) )
void dedup( std::string& str )
{
  std::sort( begin( str ), end( str ) );
  auto new_end = std::unique( begin( str ), end( str ) );
  str.erase( new_end, end( str ) );
}

// assume we need to keep chars in original order
// given that we can't use a buffer we will have to do with O(N^2)
// we will essentially rewrite the string into itself
// but jump over chars we already saw
void dedup2( std::string& str )
{
  // can't have duplicates for <= 1 char
  if ( str.size() <= 1 ) return;

  // keep track of where to insert next unseen char
  // starting at 1, as the first char can't be a duplicate
  size_t insert_pos = 1;

  for ( size_t current_pos = 1; current_pos < str.size(); current_pos++ ) {
    // check all chars that we already rewrote
    // to make sure the current char isn't already in there
    auto is_duplicate = std::any_of( begin( str ), begin( str ) + insert_pos,
                                     [ch = str[current_pos]]( auto const elem ) { return ch == elem; } );

    if ( not is_duplicate ) {
      str[insert_pos++] = str[current_pos];
    }
  }
  // if we had duplicates, we should cleanup the string
  str.resize( insert_pos );
  str.shrink_to_fit();
}

int main()
{
  std::string test{ "cbbccbaaacbbaaaabbaaaa" };
  dedup( test );
  std::cout << "Test 1: " << test << std::endl;

  std::string test2{ "aaaccbcbaacbacbabccccccab" };
  dedup2( test2 );
  std::cout << "Test 2: " << test2 << std::endl;
}
