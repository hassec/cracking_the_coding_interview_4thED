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
  // let's keep track of size;
  size_t m_size{ 0 };

public:
  // explicit destructor to avoid stack overflow which can
  // be caused by long recursive destructor chain for large lists
  ~SingleList()
  {
    while ( m_head ) m_head = std::move( m_head->m_next );
  }

  void insert( int data )
  {
    // create a new node with data
    // this node now takes ownership of what was previouly pointed at by _head
    // new node is then assigned to _head, completing the insertion
    m_head = std::make_unique<Node>( data, std::move( m_head ) );
    m_size++;
  }

  size_t size() { return m_size; }

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

  // Acces of Nth-last element is access of M=(size-1-N), Mth element
  // from the front. Thus we need to walk M elements.
  // -> O(N) access as usualy with linked list
  Node& nth_last_elem( size_t const n )
  {
    if ( n >= m_size ) {
      throw std::runtime_error( "nth_last_elem with n >= size!" );
    }

    // we need the element at position cnt from the beginning
    auto cnt = m_size - 1 - n;

    auto pos = m_head.get();

    while ( cnt-- > 0 ) {
      pos = pos->m_next.get();
    }
    return *pos;
  }

  // Also O(N) but doesn't need a size variable
  Node& nth_last_elem2( size_t n )
  {
    if ( n >= m_size ) {
      throw std::runtime_error( "nth_last_elem with n >= size!" );
    }

    auto p1 = m_head.get();
    auto p2 = m_head.get();

    // move p1 to the Nth position

    while ( n-- > 0 ) {
      // we know m_next != null since we checked before that n < m_size
      p1 = p1->m_next.get();
    }

    // the trick is that p1 is now size - n steps from the end.
    // so if we move p1 and p2 at the same time until p1->next is nullptr
    // we will perform exactly (size-n-1) steps, which is where the nth-last
    // element is.
    while ( p1->m_next ) {
      p1 = p1->m_next.get();
      p2 = p2->m_next.get();
    }

    return *p2;
  }

  // No access to previous node, thus we can't do the normal removal procedure
  // But a node is only a pointer and its data, thus just overwrite the current one
  // with the next one, then delete the next one.
  // This works because the constraint is that the given node is in the middle of the list.
  void remove_node( Node& node )
  {
    if ( !node.m_next ) {
      throw std::runtime_error( "Can't remove last node with remove_node()" );
    }

    node.m_data = node.m_next->m_data;
    node.m_next = std::move( node.m_next->m_next );
    m_size--;
  }

  void remove_nth_last_node( size_t const n )
  {
    auto& node = nth_last_elem( n );
    remove_node( node );
  }
};

int main()
{

  SingleList list1{};

  for ( int j{ 0 }; j < 10; j++ ) {
    list1.insert( j );
  }

  std::cout << "List 1, size: " << list1.size() << std::endl;
  list1.print();

  std::cout << "remove 5th last elem" << std::endl;
  list1.remove_nth_last_node( 5 );
  list1.print();

  std::cout << "remove 3rd last elem" << std::endl;
  list1.remove_nth_last_node( 3 );
  list1.print();

  std::cout << "should throw" << std::endl;
  list1.remove_nth_last_node( list1.size() - 1 );
}
