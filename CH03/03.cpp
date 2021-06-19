#include <cassert>
#include <cstddef>
#include <iostream>
#include <stack>
#include <vector>

// N-stacks implementation
// now using std::stack for simplicity and still int as data type
// for more complicated data types one would probably want to implement perfect forwarding
// to construct them in place inside the stack

class Stacks
{
public:
  Stacks( size_t size ) : max_stack_size( size ) { assert( size > 0 ); };

  void push( int val )
  {

    if ( buffer.empty() || buffer.back().size() == max_stack_size ) {
      buffer.emplace_back();
    }

    buffer.back().push( val );
  }

  int& top()
  {
    // libstdc++ simply asserts non-empty so let's do the same
    assert( !buffer.empty() && !buffer.back().empty() );
    return buffer.back().top();
  }

  void pop()
  {
    // let's just forward to pop_nth()
    // with n = last stack
    if ( buffer.empty() ) {
      return;
    }
    pop_nth( buffer.size() - 1 );
  }

  void pop_nth( size_t n )
  {
    // n has to be in range [0,size)
    assert( n < buffer.size() );

    // pop from the n-th stack
    buffer[n].pop();

    // if we didn't pop from the last stack we need shuffle everything to the left

    for ( ; n < buffer.size() - 1; n++ ) {
      // for each stack we need the last element of the next stack
      // and put it on top of current one
      // do this until we reach the end and all but last stack will be full again
      //

      // some aliases to make code more readable
      auto& current_stack = buffer[n];
      auto& next_stack    = buffer[n + 1];

      // temporary stack to hold data of next stack while we pop from it until we have the last element
      std::stack<int> tmp{};
      // after the while(), tmp holds all but the last element of next_stack in reverse order
      while ( next_stack.size() > 1 ) {
        tmp.push( next_stack.top() );
        next_stack.pop();
      }
      // move last element to fill current stack
      current_stack.push( next_stack.top() );
      next_stack.pop();

      // refill next_stack from tmp
      // now next_stack is again in correct order but missing it's last element
      // which is now in current stack
      while ( !tmp.empty() ) {
        next_stack.push( tmp.top() );
        tmp.pop();
      }
    }

    // if we removed the last element from the last stack we need to remove that stack
    if ( buffer.back().empty() ) {
      buffer.pop_back();
    }
  }

  void print()
  {

    std::cout << "Set of #" << buffer.size() << " stacks" << std::endl;

    for ( size_t n{ 0 }; n < buffer.size(); n++ ) {
      std::cout << "Stack #" << n << ": ";

      auto tmp = std::stack<int>{};
      while ( !buffer[n].empty() ) {
        tmp.push( buffer[n].top() );
        buffer[n].pop();
        std::cout << tmp.top() << " ";
      }
      std::cout << std::endl;
      while ( !tmp.empty() ) {
        buffer[n].push( tmp.top() );
        tmp.pop();
      }
    }
  }

private:
  std::vector<std::stack<int>> buffer;
  size_t                       max_stack_size{};
};

int main()
{

  Stacks my_stack( 4 );
  for ( int i{ 0 }; i < 18; i++ ) {
    my_stack.push( i );
  }

  my_stack.print();

  std::cout << "top: " << my_stack.top() << std::endl;
  my_stack.pop();
  my_stack.print();
  std::cout << "top: " << my_stack.top() << std::endl;
  my_stack.pop_nth( 1 );
  my_stack.print();
}
