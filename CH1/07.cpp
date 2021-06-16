#include <iomanip>
#include <iostream>
#include <numeric>
#include <unordered_set>
#include <vector>

// simple matrix on the heap
struct Mat {

  Mat( size_t rows, size_t cols ) : _rows( rows ), _cols( cols ) { _mat.resize( rows * cols ); };

  // using at() for added safety
  int& operator()( size_t const i, size_t const j ) { return _mat.at( _cols * i + j ); }

  void print()
  {
    for ( size_t i{ 0 }; i < _rows; i++ ) {
      for ( size_t j{ 0 }; j < _cols; j++ ) {
        std::cout << std::setw( 5 ) << operator()( i, j );
      }
      std::cout << std::endl;
    }
  }

  size_t           _rows{};
  size_t           _cols{};
  std::vector<int> _mat{};
};

// need to look at every element twice
// once for checking and once to later potentially set it to 0
// -> O(M*N), Space is proportional to number of zeros
// thus worst case O(M + N), if all entries are 0
void setZero( Mat& mat )
{
  std::unordered_set<int> zero_rows{};
  std::unordered_set<int> zero_cols{};
  for ( size_t i{ 0 }; i < mat._rows; i++ ) {
    for ( size_t j{ 0 }; j < mat._cols; j++ ) {
      if ( mat( i, j ) == 0 ) {
        zero_rows.insert( i );
        zero_cols.insert( j );
      }
    }
  }

  for ( size_t i{ 0 }; i < mat._rows; i++ ) {
    for ( size_t j{ 0 }; j < mat._cols; j++ ) {
      if ( zero_rows.find( i ) != zero_rows.end() || zero_cols.find( j ) != zero_cols.end() ) {
        mat( i, j ) = 0;
      }
    }
  }
}

int main()
{

  Mat M{ 5, 5 };
  std::iota( begin( M._mat ), end( M._mat ), 1 );

  M( 1, 1 ) = 0;
  M( 2, 3 ) = 0;
  M( 1, 3 ) = 0;

  M.print();

  setZero( M );
  std::cout << "++++++++++++++++++++++++++++++++" << std::endl;

  M.print();
}
