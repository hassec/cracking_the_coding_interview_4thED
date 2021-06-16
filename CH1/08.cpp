#include <algorithm>
#include <iostream>
#include <string>


// isSubstring in C++ can be done by checking if you can find string A in string B
// the trick here is to realize that concatenating a rotated sequence with itself
// will result in a larger sequence that contains the original sequence
bool is_rotation( const std::string& s1, const std::string& s2 )
{
  return s1.size() == s2.size() ? ( s1 + s1 ).find( s2 ) != std::string::npos : false;
}

int main()
{

  std::string original{ "teststring" };
  std::string rot{ original };
  std::rotate( begin( rot ), begin( rot ) + 3, end( rot ) );

  auto not_rot = rot;
  not_rot[0]   = 'x';

  std::cout << rot << " is roatation of " << original << " : " << is_rotation( rot, original ) << std::endl;
  std::cout << not_rot << " is roatation of " << original << " : " << is_rotation( not_rot, original ) << std::endl;
}
