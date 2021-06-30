#include <cstddef>
#include <iostream>
#include <stack>

// for simplicity let's just focus on only int here
// and only implement the parts we need, e.g. no size() or empty etc.
// enqueue is O(N) since we have to rebuild the entire stack
struct MyQueue {

  int front() const { return s1.top(); }

  void dequeue() { s1.pop(); }

  void enqueue( int val )
  {

    if ( s1.empty() ) {
      s1.push( val );
      return;
    }

    // if not empty we need to do some work to make sure the order is correct
    // first store reversed s1 in s2
    while ( !s1.empty() ) {
      s2.push( s1.top() );
      s1.pop();
    }

    // push back the new value which is now the only value in s1
    s1.push( val );

    // now push all values from s2 back into s1
    // now s1 is in previous order and val is at bottom.
    while ( !s2.empty() ) {
      s1.push( s2.top() );
      s2.pop();
    }
  }

  // stack which represents the current state of the queue
  std::stack<int> s1{};
  // simply a helper stack, but keeping it around means we don't have
  // to allocate a new helper stack on every enqueue call
  std::stack<int> s2{};
};

int main()
{

  MyQueue q{};

  for ( int i{ 0 }; i < 10; ++i ) {
    q.enqueue( i );
  }

  for ( int i{ 0 }; i < 10; ++i ) {
    std::cout << q.front() << std::endl;
    q.dequeue();
  }
}
