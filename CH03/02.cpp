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

  struct Node {
    int val{};
    int min{};
  };

  // let's return false if stack is full and push aborted
  bool push( int stack_num, int val )
  {
    if ( size( stack_num ) == max_stack_size ) {
      return false;
    }

    auto const min = empty( stack_num ) ? val : std::min( val, peek( stack_num ).value().min );
    buffer[stack_num * max_stack_size + ( ++stack_idx[stack_num] )] = { val, min };
    return true;
  }

  // remove top element and return
  // can fail if stack is empty -> return empty optional
  std::optional<Node> pop( int stack_num )
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
  [[nodiscard]] std::optional<Node> peek( int stack_num ) const
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

  // return min, if stack isn't empty
  [[nodiscard]] std::optional<int> min( int stack_num ) const
  {
    return empty( stack_num ) ? std::nullopt : std::optional<int>( peek( stack_num ).value().min );
  }

private:
  int                max_stack_size{};
  std::vector<Node>  buffer{};
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
      std::cout << "pop of stack #" << stack_num << " returned: " << val.value().val << std::endl;
    } else {
      std::cout << "pop of stack #" << stack_num << " failed " << std::endl;
    }
  };

  auto const min_helper = [&my_stack]( int stack_num ) {
    auto val = my_stack.min( stack_num );
    if ( val.has_value() ) {
      std::cout << "min of stack #" << stack_num << " returned: " << val.value() << std::endl;
    } else {
      std::cout << "min of stack #" << stack_num << " failed " << std::endl;
    }
  };

  for ( int i{ 17 }; i >= 0; i-- ) {
    push_helper( i % 3, i );
  }

  for ( int i{ 0 }; i < 7; i++ ) {
    min_helper( 1 );
    pop_helper( 1 );
  }
}
