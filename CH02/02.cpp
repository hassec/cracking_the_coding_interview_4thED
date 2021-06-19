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
};

int main()
{

  SingleList list1{};

  for ( int j{ 0 }; j < 10; j++ ) {
    list1.insert( j );
  }

  std::cout << "List 1, size: " << list1.size() << std::endl;
  list1.print();

  std::cout << "get 5th last elem" << std::endl;
  std::cout << list1.nth_last_elem( 5 ).m_data << std::endl;

  std::cout << "get 3th last elem" << std::endl;
  std::cout << list1.nth_last_elem( 3 ).m_data << std::endl;

  std::cout << "get 0th last elem" << std::endl;
  std::cout << list1.nth_last_elem( 0 ).m_data << std::endl;

  std::cout << "get Nth last elem" << std::endl;
  std::cout << list1.nth_last_elem( list1.size() - 1 ).m_data << std::endl;

  std::cout << "get 5th last elem2" << std::endl;
  std::cout << list1.nth_last_elem2( 5 ).m_data << std::endl;

  std::cout << "get 3th last elem2" << std::endl;
  std::cout << list1.nth_last_elem2( 3 ).m_data << std::endl;

  std::cout << "get 0th last elem2" << std::endl;
  std::cout << list1.nth_last_elem2( 0 ).m_data << std::endl;

  std::cout << "get Nth last elem2" << std::endl;
  std::cout << list1.nth_last_elem2( list1.size() - 1 ).m_data << std::endl;

  std::cout << "should throw" << std::endl;
  std::cout << list1.nth_last_elem2( list1.size() ).m_data << std::endl;
}
