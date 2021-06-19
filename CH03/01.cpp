#include <array>
#include <cstddef>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <vector>

// 3-stacks implementation
// using int as data type for simplicity

class Stacks
{
public:
  Stacks( int size ) : max_stack_size( size ) { buffer.resize( 3 * size ); };

  // let's return false if stack is full and push aborted
  bool push( int stack_num, int val )
  {
    if ( size( stack_num ) == max_stack_size ) {
      return false;
    }
    buffer[stack_num * max_stack_size + ( ++stack_idx[stack_num] )] = val;
    return true;
  }

  // remove top element and return
  // can fail if stack is empty -> return empty optional
  std::optional<int> pop( int stack_num )
  {
    if ( empty( stack_num ) ) {
      return std::nullopt;
    }
    return buffer[max_stack_size * stack_num + stack_idx[stack_num]--];
  }

  // return top element
  // can fail if stack is empty -> return empty optional
  // nodiscard because you don't want to call this function
  // if you don't use the value
  [[nodiscard]] std::optional<int> peek( int stack_num ) const
  {
    if ( empty( stack_num ) ) {
      return std::nullopt;
    }
    return buffer[max_stack_size * stack_num + stack_idx[stack_num]];
  }

  // our stack only holds ints, thus we don't need to call any destructors
  // simply set stack_idx[stack_num] to -1 to clear
  void clear( int stack_num ) { stack_idx[stack_num] = -1; }
  // return size of the stack_num-th stack
  [[nodiscard]] int size( int stack_num ) const { return stack_idx[stack_num] + 1; }
  //  is the stack_num-th stack empty?
  [[nodiscard]] bool empty( int stack_num ) const { return size( stack_num ) == 0; }

private:
  int                max_stack_size{};
  std::vector<int>   buffer{};
  std::array<int, 3> stack_idx{ -1, -1, -1 };
};

int main()
{

  Stacks my_stack( 5 );

  auto const push_helper = [&my_stack]( int stack_num, int val ) {
    auto status = my_stack.push( stack_num, val );
    std::cout << "Push of val: " << val << " to stack #" << stack_num << ( status ? " succeded!" : " failed!" )
              << std::endl;
  };

  auto const pop_helper = [&my_stack]( int stack_num ) {
    auto val = my_stack.pop( stack_num );
    if ( val.has_value() ) {
      std::cout << "pop of stack #" << stack_num << " returned: " << val.value() << std::endl;
    } else {
      std::cout << "pop of stack #" << stack_num << " failed " << std::endl;
    }
  };

  auto const peek_helper = [&my_stack]( int stack_num ) {
    auto val = my_stack.peek( stack_num );
    if ( val.has_value() ) {
      std::cout << "peek of stack #" << stack_num << " returned: " << val.value() << std::endl;
    } else {
      std::cout << "peek of stack #" << stack_num << " failed " << std::endl;
    }
  };

  for ( int i{ 0 }; i < 18; i++ ) {
    push_helper( i % 3, i );
  }

  for ( int i{ 0 }; i < 7; i++ ) {
    pop_helper( 2 );
  }

  for ( int i{ 0 }; i < 7; i++ ) {
    peek_helper( 1 );
    pop_helper( 1 );
  }

  std::cout << "S0 size: " << my_stack.size( 0 ) << std::endl;
  std::cout << "S1 size: " << my_stack.size( 1 ) << std::endl;
  std::cout << "S2 size: " << my_stack.size( 2 ) << std::endl;
}
