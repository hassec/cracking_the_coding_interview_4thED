#include <algorithm>
#include <iostream>
#include <random>
#include <stack>

void sort_stack( std::stack<int>& stack )
{
  std::stack<int> buffer{};

  while ( !stack.empty() ) {

    int val = stack.top();
    stack.pop();

    while ( !buffer.empty() && buffer.top() < val ) {
      stack.push( buffer.top() );
      buffer.pop();
    }

    buffer.push( val );
  }
  stack = std::move( buffer );
}

int main()
{
  std::stack<int> to_sort{};
  // no need to seed with random_device for this
  std::mt19937 g( 1234 );

  // a bit more complicated but this way I know I should get 0..9 as result
  // instead of random numbers in sorted order
  std::vector<int> vec( 10 );
  std::iota( begin( vec ), end( vec ), 0 );
  std::shuffle( begin( vec ), end( vec ), g );

  for ( auto const val : vec ) {
    to_sort.push( val );
  }

  sort_stack( to_sort );

  while ( !to_sort.empty() ) {
    std::cout << to_sort.top() << std::endl;
    to_sort.pop();
  }
}
