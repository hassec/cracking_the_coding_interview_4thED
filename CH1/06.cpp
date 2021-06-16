#include <array>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <numeric>

// I read MxN matrix in the question :facepalm:
// But when I noticed my curiosity was already sparked so I went with it
// There isn't much difference, just that one needs to keep track of the axis

// Book solution actually cyclically writes back new array.
// My solution below differs a bit, in that I don't write back a new matrix
// But the core of the solution is the same as it comes down to figuring out
// the right way to play with the indices of the matrix
//
// One could easily implement an additional method to write back the data
// into the array, but that then boils down to what's already in the book.
//
// I'd say which way is better depends what one needs to do.
//
// Large matrix, and only looking up few elements? This solution would be cheaper
//
// Small matrix and doing math on all elements? This solution jumps around instead of continuous reads :/
//
//
//

template <size_t M, size_t N>
struct mat {

  // possible states for 90 deg rotation
  enum class rot { d0 = 0, d90, d180, d270 };

  // let's go row major
  // i == row index, j == column index
  int operator()( size_t const i, size_t const j )
  {

    size_t row = i;
    size_t col = j;

    // if we are rotated we need to adjust our indices
    switch ( _rot ) {
    case rot::d90:
      row = col_max - j;
      col = i;
      break;
    case rot::d180:
      row = row_max - i;
      col = col_max - j;
      break;
    case rot::d270:
      row = j;
      col = row_max - i;
      break;
    case rot::d0:
    default:
      break;
    }

    // sanity check
    assert( row < M && col < N );
    return _mat[row * N + col];
  }

  void rot_by_90_deg()
  {
    // update rotation
    _rot = static_cast<rot>( ( static_cast<int>( _rot ) + 1 ) % 4 );
    // rotation means we swap axis length
    std::swap( row_max, col_max );
  }

  void print()
  {
    std::cout << "MAT, rotated by = " << static_cast<int>( _rot ) * 90 << "deg" << std::endl;
    for ( size_t i{ 0 }; i <= row_max; i++ ) {
      for ( size_t j{ 0 }; j <= col_max; j++ ) {
        std::cout << operator()( i, j ) << " ";
      }
      std::cout << std::endl;
    }
  }

  size_t                 row_max{ M - 1 };
  size_t                 col_max{ N - 1 };
  rot                    _rot{ rot::d0 };
  std::array<int, M * N> _mat{};
};

int main()
{

  auto x = mat<2, 4>{};
  std::iota( begin( x._mat ), end( x._mat ), 1 );

  x.print();
  std::cout << "\n+++++++++++++++++++++++++++" << std::endl;

  x.rot_by_90_deg();
  x.print();
  std::cout << "\n+++++++++++++++++++++++++++" << std::endl;

  x.rot_by_90_deg();
  x.print();
  std::cout << "\n+++++++++++++++++++++++++++" << std::endl;

  x.rot_by_90_deg();
  x.print();
  std::cout << "\n+++++++++++++++++++++++++++" << std::endl;

  x.rot_by_90_deg();
  x.print();
}
