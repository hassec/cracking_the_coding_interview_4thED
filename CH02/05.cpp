#include <cstddef>
#include <iostream>
#include <memory>
#include <unordered_set>
// simple singly linked list
// let's just use int for simplicity
class SingleList
{

public:
  struct Node {
    Node( int d, std::unique_ptr<Node>&& p ) : m_data( d ), m_next( std::move( p ) ) {}
    int                   m_data{};
    std::unique_ptr<Node> m_next{ nullptr };
  };

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

  Node* find( int d )
  {
    auto pos = m_head.get();
    while ( pos && pos->m_data != d ) {
      pos = pos->m_next.get();
    }
    return pos;
  }

  // similar to the remove duplicates solution
  // O(N)
  Node* find_loop()
  {
    // A -> B -> C -> D -> E -> C [the same C as earlier]
    // output C

    if ( !m_head ) {
      return nullptr;
    }

    auto                            pos = m_head->m_next.get();
    std::unordered_set<const Node*> seen;

    while ( pos ) {
      if ( seen.find( pos ) != seen.end() ) {
        return pos;
      }
      seen.insert( pos );
      pos = pos->m_next.get();
    }
    return nullptr;
  }

private:
  std::unique_ptr<Node> m_head{ nullptr };
  // let's keep track of size;
  size_t m_size{ 0 };
};

int main()
{

  SingleList l1{};

  l1.insert( 4 ).insert( 3 ).insert( 2 ).insert( 1 ).insert( 0 );

  std::cout << "List 1" << std::endl;
  l1.print();

  auto* n1 = l1.find( 2 );
  auto* n2 = l1.find( 4 );
  if ( !n1 || !n2 ) {
    std::cout << "Something is wrong" << std::endl;
    return -1;
  }
  // set loop
  n2->m_next = std::unique_ptr<SingleList::Node>( n1 );

  auto loop = l1.find_loop();
  std::cout << "Found Begin: " << ( loop == n1 ) << std::endl;
}
