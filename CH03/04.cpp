#include <iostream>
#include <stack>

void solve_hanoi( std::stack<int>& source, std::stack<int>& buffer, std::stack<int>& dest, size_t n )
{

  if ( n == 0 ) {
    return;
  }

  // simplest case where we can do a direct move
  if ( n == 1 ) {
    dest.push( source.top() );
    source.pop();
    return;
  }

  // recursive sub problem where we first move everything but the bottom disk to the buffer
  solve_hanoi( source, dest, buffer, n - 1 );

  // move bottom disk of our source stack to the destitnation one
  dest.push( source.top() );
  source.pop();

  // and now we need to move everything from the buffer to the destination stack
  solve_hanoi( buffer, source, dest, n - 1 );
}

int main()
{
  std::stack<int> rod1{};
  std::stack<int> rod2{};
  std::stack<int> rod3{};

  for ( int i{ 10 }; i > 0; i-- ) {
    rod1.push( i );
  }

  solve_hanoi( rod1, rod2, rod3, rod1.size() );

  if ( !rod1.empty() || !rod2.empty() ) {
    std::cout << "Rod 1 and 2 should be empty!" << std::endl;
  }

  while ( !rod3.empty() ) {
    std::cout << rod3.top() << std::endl;
    rod3.pop();
  }
}
