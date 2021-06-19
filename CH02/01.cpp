#include <cstddef>
#include <iostream>
#include <memory>
#include <unordered_set>

// simple implementation of a singly linked list
// the buffered remove_dups function only compiles if std::unordered_set<T> is allowed
// e.g. the STL know how to compute the hash by default
// but as long as that function isn't used we are fine since then the function isn't instantiated
template <typename T>
class SingleList
{

  struct Node {
    Node( T d, std::unique_ptr<Node>&& p ) : m_data( d ), m_next( std::move( p ) ) {}
    T                   m_data{};
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

  void insert( T data )
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

  void remove_dups_buffered()
  {

    std::unordered_set<T> buffer{};

    buffer.insert( m_head->m_data );

    auto pos = m_head.get();
    while ( pos->m_next ) {

      if ( buffer.find( pos->m_next->m_data ) != buffer.end() ) {
        // already seen -> remove node
        pos->m_next = std::move( pos->m_next->m_next );
        m_size--;
      } else {
        buffer.insert( pos->m_next->m_data );
        pos = pos->m_next.get();
      }
    }
  }

  void remove_dups_unbuffered()
  {

    auto pos = m_head.get();

    while ( pos->m_next ) {

      // if the data of pos->_next is already in [_head, pos]
      // we know it's a duplicate
      auto dup = m_head.get();

      while ( dup != pos->m_next.get() ) {
        if ( dup->m_data == pos->m_next->m_data ) {
          pos->m_next = std::move( pos->m_next->m_next );
          m_size--;
          break;
        }
        dup = dup->m_next.get();
      }

      if ( dup == pos->m_next.get() ) {
        pos = pos->m_next.get();
      }
    }
  }
};

int main()
{

  SingleList<int> list1{};
  SingleList<double> list2{};

  for ( int i{ 0 }; i < 2; i++ ) {
    for ( int j{ 0 }; j < 10; j++ ) {
      list1.insert(j);
      list2.insert(j);
    }
  }

  std::cout << "List 1, size: " << list1.size() <<  std::endl;
  list1.print();

  std::cout << "remove dups" << std::endl;
  list1.remove_dups_buffered();
  list2.remove_dups_unbuffered();


  std::cout << "List 1, size: " << list1.size() <<  std::endl;
  list1.print();
  std::cout << "List 2, size: " << list1.size() <<  std::endl;
  list2.print();

}
