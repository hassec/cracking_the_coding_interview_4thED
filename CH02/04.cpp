#include <cstddef>
#include <iostream>
#include <memory>

// simple singly linked list
// let's just use int for simplicity
class SingleList
{

  struct Node {
    Node( int d, std::unique_ptr<Node>&& p ) : m_data( d ), m_next( std::move( p ) ) {}
    int                   m_data{};
    std::unique_ptr<Node> m_next{ nullptr };
  };

  std::unique_ptr<Node> m_head{ nullptr };

public:
  // explicit destructor to avoid stack overflow which can
  // be caused by long recursive destructor chain for large lists
  ~SingleList()
  {
    while ( m_head ) m_head = std::move( m_head->m_next );
  }

  // explicit move constructor needed due to above destructor
  SingleList( SingleList&& l ) = default;
  // but we want our default constructor too
  SingleList() = default;

  SingleList& insert( int data )
  {
    // create a new node with data
    // this node now takes ownership of what was previouly pointed at by _head
    // new node is then assigned to _head, completing the insertion
    m_head = std::make_unique<Node>( data, std::move( m_head ) );
    return *this;
  }

  template <typename F>
  void apply( F func )
  {
    auto pos = m_head.get();
    while ( pos ) {
      func( pos );
      pos = pos->m_next.get();
    }
  }

  void print()
  {
    apply( []( auto* const node ) { std::cout << "Node (" << node->m_data << ")" << std::endl; } );
  }

  // O(N) walks both lists from head -> tail
  // and recurisvely reconstructs the summed list
  friend SingleList operator+( SingleList const& l1, SingleList const& l2 )
  {

    SingleList sum{};

    auto p1 = l1.m_head.get();
    auto p2 = l2.m_head.get();
    if ( !p1 and !p2 ) {
      return sum;
    }
    int carry = 0;

    // one could just define the below as a separate function, but for practice
    // lets do a recursive lambda instead
    auto summer = []( Node const* p1, Node const* p2, int carry ) {
      // nicer to encapsullate the self-calling part to not make that part of the interface of the summer
      // need the trailing return type, since the compiler doesn't manage otherwise.
      auto summer_impl = []( Node const* p1, Node const* p2, int carry, auto& summer_ref ) -> std::unique_ptr<Node> {
        int sum{ 0 };
        if ( p1 ) {
          sum += p1->m_data;
          p1 = p1->m_next.get();
        }
        if ( p2 ) {
          sum += p2->m_data;
          p2 = p2->m_next.get();
        }

        // add carry  and carry again if sum_node > 10
        sum += carry;
        carry = sum >= 10;
        // reduce sum_node to digit
        sum = sum % 10;

        if ( p1 || p2 || carry ) {
          // still have some work to do
          return std::make_unique<Node>( sum, summer_ref( p1, p2, carry, summer_ref ) );
        } else {
          return std::make_unique<Node>( sum, nullptr );
        }
      };
      return summer_impl( p1, p2, carry, summer_impl );
    };

    sum.m_head = summer( p1, p2, carry );
    return sum;
  };
};

int main()
{

  SingleList l1{};
  SingleList l2{};
  SingleList l3{};
  SingleList l4{};

  l1.insert( 5 ).insert( 1 ).insert( 3 );
  l2.insert( 2 ).insert( 9 ).insert( 5 );
  l3.insert( 2 ).insert( 9 ).insert( 8 );
  l4.insert( 2 ).insert( 9 ).insert( 5 ).insert( 8 );

  std::cout << "List 1" << std::endl;
  l1.print();

  std::cout << "List 2" << std::endl;
  l2.print();

  std::cout << "List 3" << std::endl;
  l3.print();

  std::cout << "List 4" << std::endl;
  l4.print();

  std::cout << "l1 + l2" << std::endl;
  ( l1 + l2 ).print();

  std::cout << "l1 + l3" << std::endl;
  ( l1 + l3 ).print();

  std::cout << "l1 + l4" << std::endl;
  ( l1 + l4 ).print();
}
