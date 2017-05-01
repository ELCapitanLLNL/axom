/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and further
 * review from Lawrence Livermore National Laboratory.
 */

#include "axom/Types.hpp" // for AXOM_NULLPTR

#include "axom_utils/Utilities.hpp" // for utilities::swap()

// C/C++ includes
#include <cassert> // for assert()

#ifndef AXOM_MATRIX_HPP_
#define AXOM_MATRIX_HPP_

namespace axom {
namespace numerics {

// Forward Declaration
template < typename T >
class Matrix;

/// \name Overloaded Matrix Operators
/// @{
/*!
 *******************************************************************************
 * \brief Computes \f$ \mathcal{C} = \mathcal{A} + \mathcal{B} \f$
 *
 * \param [in] A \f$ M \times N \f$ matrix on the left-hand side.
 * \param [in] B \f$ M \times N \f$ matrix on the right-hand side.
 * \return \f$ \mathcal{C} \f$ result matrix
 *
 * \pre \f$ \mathcal{A} \f$ and \f$ \mathcal{B} \f$ have the same dimensions.
 * \post \f$ \mathcal{C} \f$ is an \f$ M \times N \f$ matrix.
 * \post \f$ c_{ij} = a_{ij} + b_{ij} \f$, \f$\forall c_{ij} \in \mathcal{C}\f$
 *
 * \note Matrix addition is undefined for matrices that have different
 *  dimensions. If the input matrices, \f$ \mathcal{A} \f$, \f$ \mathcal{B} \f$
 *  have different dimensions, a \f$ 1 \times 1 \f$ null matrix is returned in
 *  \f$ \mathcal{C} \f$
 *
 * \warning Use this method with caution. Chaining overloaded operators may
 *  lead to the creation of an undesirable number of temporaries and slow the
 *  performance considerably. Consider using a variant for this method.
 *
 * \see add() variant
 *******************************************************************************
 */
template < typename T >
Matrix< T > operator+( const Matrix< T >& A, const Matrix< T >& B );

/*!
 *******************************************************************************
 * \brief Computes \f$ \mathcal{C} = \mathcal{A} - \mathcal{B} \f$
 *
 * \param [in] A \f$ M \times N \f$ matrix on the left-hand side.
 * \param [in] B \f$ M \times N \f$ matrix on the right-hand side.
 * \return \f$ \mathcal{C} \f$ result matrix
 *
 * \pre \f$ \mathcal{A} \f$ and \f$ \mathcal{B} \f$ have the same dimensions.
 * \post \f$ \mathcal{C} \f$ is an \f$ M \times N \f$ matrix.
 * \post \f$ c_{ij} = a_{ij} - b_{ij} \f$, \f$\forall c_{ij} \in \mathcal{C}\f$
 *
 * \note Matrix subtraction is undefined for matrices that have different
 *  dimensions. If the input matrices, \f$ \mathcal{A} \f$, \f$ \mathcal{B} \f$
 *  have different dimensions, a \f$ 1 \times 1 \f$ null matrix is returned in
 *  \f$ \mathcal{C} \f$
 *
 * \warning Use this method with caution. Chaining overloaded operators may
 *  lead to the creation of an undesirable number of temporaries and slow the
 *  performance considerably. Consider using a variant for this method.
 *
 * \see subtract() variant
 *******************************************************************************
 */
template < typename T >
Matrix< T > operator-( const Matrix< T >& A, const Matrix< T >& B );

/*!
 *******************************************************************************
 * \brief Computes a scalar-matrix product of a matrix \f$ \mathcal{A} \f$ and
 *  a scalar \f$ c \f$
 *
 * \param [in] A \f$ M \times N \f$ matrix
 * \param [in] c scalar value to multiply the matrix entries.
 * \return \f$ \mathcal{C} \f$ the resulting matrix.
 *
 * \post \f$ \mathcal{C} \f$ has the same dimensions as \f$ \mathcal{A} \f$
 * \post \f$ c_{ij} = c \cdot a_{ij} \f$, \f$\forall c_{ij} \in \mathcal{C}\f$
 *
 * \note Matrix-scalar multiplication is a commutative operation
 *  \f$ c \cdot \mathcal{A} = \mathcal{A} \cdot c \f$
 *
 * \warning Use this method with caution. Chaining overloaded operators may
 *  lead to the creation of an undesirable number of temporaries and slow the
 *  performance considerably. Consider using a variant for this method.
 *
 * \see scalar_multiply() variant
 *******************************************************************************
 */
template < typename T >
Matrix< T > operator*( const Matrix< T >& A, const T& c );
template < typename T >
Matrix< T > operator*( const T& c,const Matrix< T >& A) { return A*c; };

/*!
 *******************************************************************************
 * \brief Computes the matrix-vector product of a matrix \f$\mathcal{A}\f$
 *  and a vector \f$\mathbf{x}\f$
 *
 * \param [in] A \f$ M \times N \f$ matrix on the left hand-side.
 * \param [in] x right-hand side vector of dimension \f$ N \times 1 \f$
 * \return \f$ \mathbf{b} \f$ the result vector of dimension \f$ M \times 1 \f$
 *
 * \pre \f$ \mathbf{x} \f$ != AXOM_NULLPTR
 * \pre \f$ \mathbf{x} \f$ must be of dimension \f$ N \f$
 * \post \f$ \mathbf{b} \f$ solution vector of dimension \f$ M \times 1 \f$
 * \post \f$ b_{i} = \sum\limits_{j=0}^N a_{ij} \cdot x_{j} \f$,
 *  \f$ \forall i \in [0,M-1] \f$
 *
 * \warning Use this method with caution. Chaining overloaded operators may
 *  lead to the creation of an undesirable number of temporaries and slow the
 *  performance considerably. Consider using a variant for this method.
 *
 * \see vector_multiply() variant
 *******************************************************************************
 */
template < typename T >
Matrix< T > operator*( const Matrix< T >& A, const T* x );

/*!
 *******************************************************************************
 * \brief Computes the matrix-matrix product of \f$ \mathcal{A} \f$ and
 *  \f$ \mathcal{B} \f$
 *
 * \param [in] A \f$ M \times N \f$ matrix on the left hand-side.
 * \param [in] B \f$ N \times K \f$ matrix on the right hand-side.
 * \return \f$ \mathcal{C} \f$ result matrix of dimensions \f$ M \times K \f$
 *
 * \pre The inner dimensions of the two matrices must match
 *
 * \note Matrix multiplication is undefined for matrices with different inner
 *  dimension. If the inner dimensions are not matching, the code returns a
 *  \f$ 1 \times 1 \f$ null matrix in \f$ \mathcal{C} \f$
 *
 * \warning Use this method with caution. Chaining overloaded operators may
 *  lead to the creation of an undesirable number of temporaries and slow the
 *  performance considerably. Consider using a variant for this method.
 *
 * \see matrix_multiply() variant
 *******************************************************************************
 */
template < typename T >
Matrix< T > operator*( const Matrix< T >& A, const Matrix< T >& B );

/*!
 *******************************************************************************
 * \brief Overloaded output stream operator. Outputs the matrix coefficients
 *  in to the given output stream.
 *
 * \param [in,out] os output stream object.
 * \param [in] A user-supplied matrix instance.
 * \return os the updated output stream object.
 *******************************************************************************
 */
template < typename T >
std::ostream& operator<<( std::ostream& os, const Matrix< T >& A );

/// @}

/// \name Matrix Operators
/// @{

/*!
 *******************************************************************************
 * \brief Computes \f$ \mathcal{C} = \mathcal{A} + \mathcal{B} \f$
 *
 * \param [in]  A \f$ M \times N \f$ matrix on the left-hand side.
 * \param [in]  B \f$ M \times N \f$ matrix on the right-hand side.
 * \param [out] C \f$ M \times N \f$ output matrix.
 *
 * \post \f$ c_{ij} = a_{ij} + b{ij} \f$, \f$\forall c_{ij} \in \mathcal{C}\f$
 *
 * \note Matrix addition is undefined for matrices that have different
 *  dimensions. If the input matrices, \f$ \mathcal{A} \f$, \f$ \mathcal{B} \f$
 *  or \f$ \mathcal{C} \f$ have different dimensions, a \f$ 1 \times 1 \f$ null
 *  matrix is returned in \f$ \mathcal{C} \f$
 *******************************************************************************
 */
template < typename T >
void add( const Matrix< T >& A, const Matrix< T >&B, Matrix< T >& C );

/*!
 *******************************************************************************
 * \brief Computes \f$ \mathcal{C} = \mathcal{A} - \mathcal{B} \f$
 *
 * \param [in]  A \f$ M \times N \f$ matrix on the left-hand side.
 * \param [in]  B \f$ M \times N \f$ matrix on the right-hand side.
 * \param [out] C \f$ M \times N \f$ output matrix.
 *
 * \post \f$ c_{ij} = a_{ij} + b{ij} \f$, \f$\forall c_{ij} \in \mathcal{C}\f$
 *
 * \note Matrix addition is undefined for matrices that have different
 *  dimensions. If the input matrices, \f$ \mathcal{A} \f$, \f$ \mathcal{B} \f$
 *  or \f$ \mathcal{C} \f$ have different dimensions, a \f$ 1 \times 1 \f$ null
 *  matrix is returned in \f$ \mathcal{C} \f$
 *******************************************************************************
 */
template < typename T >
void subtract( const Matrix< T >& A, const Matrix< T >& B, Matrix< T >& C );

/*!
 *******************************************************************************
 * \brief Computes a scalar-matrix produect of a matrix \f$ \mathcal{A} \f$ and
 *  a scalar \f$ c \f$ and stores the result in \f$ \mathcal{A} \f$
 *
 * \param [in,out] A an \f$ M \times N \f$ matrix
 * \param [in] c scalar value to multiply the matrix coefficients
 *
 * \post \f$ a_{ij} = c \cdot a_{ij} \f$, \f$ \forall a_{ij} \in mathcal{A} \f$
 *******************************************************************************
 */
template < typename T >
void scalar_multiply( Matrix< T >& A, const T& c );

/*!
 *******************************************************************************
 * \brief Computes the matrix-vector product of a matrix \f$\mathcal{A}\f$ and
 *  a vector \f$\mathbf{x}\f$ and store the result in the user-supplied output
 *  vector.
 *
 * \param [in] A an \f$ M \times N \f$ matrix
 * \param [in] x pointer to user-supplied vector storing the vector
 * \param [out] c pointer to the user-supplied output vector
 *
 * \pre vec != AXOM_NULLPTR
 * \pre vec must be of dimension \f$ N \f$
 * \pre output != AXOM_NULLPTR
 * \pre output must be of dimension \f$ M \f$
 *
 * \post \f$ b_i = \sum\limits_{j=0}^N a_{ij} \cdot x_j \f$,
 *  \f$\forall i \in [0,M-1] \f$
 *******************************************************************************
 */
template < typename T >
void vector_multiply( const Matrix< T >& A, const T* vec, T* output );

/*!
 *******************************************************************************
 * \brief Computes the matrix-matrix product of \f$ \mathcal{A} \f$ and
 *  \f$ \mathcal{B} \f$ and stores the result in \f$ \mathcal{C} \f$
 *
 * \param [in] A  \f$ M \times N \f$ matrix on the left hand-side.
 * \param [in] B  \f$ N \times K \f$ matrix on the right hand-side.
 * \param [out] C \f$ M \times K \f$ output matrix
 *
 * \pre The inner dimensions of matrices A, B must match
 * \pre Output matrix should be an \f$ M \times K \f$ matrix
 *
 * \note Matrix multiplication is undefined for matrices with different inner
 *  dimension. If the inner dimensions are not matching, the code returns a
 *  \f$ 1 \times 1 \f$ null matrix in \f$ \mathcal{C} \f$
 *******************************************************************************
 */
template < typename T >
void matrix_multiply( const Matrix< T >&A,
                      const Matrix< T >& B,
                      Matrix< T >& C );

/*!
 *****************************************************************************
 * \brief Computes the matrix transpose of a given matrix \f$ \mathcal{A} \f$
 * \param [in] A an \f$ M \times N \f$ matrix
 * \return \f$ A^\top \f$ the matrix transpose.
 *****************************************************************************
 */
template < typename T >
Matrix< T > transpose( const Matrix< T >& A );

/*!
 *******************************************************************************
 * \brief Extracts the lower triangular part of a square matrix.
 *
 * \param [in] A a square matrix
 * \param [in] unit_diagonal indicates if the diagonal entries are implicitly
 *  set to ones (1s) or copied from the original matrix, A. Default is false.
 *
 * \return L a new matrix instance of the same dimensions as A consisting of
 *  the lower triangular part of A and all the rest of its entries set to
 *  zero.
 *
 * \pre A.isSquare() == true
 * \post upper-triangular of matrix L is set to zeros (0s)
 *******************************************************************************
 */
template < typename T >
Matrix< T > lower_triangular( const Matrix< T >& A, bool unit_diagonal=false );

/*!
 *******************************************************************************
 * \brief Extract the upper triangular part of a square matrix.
 *
 * \param [in] A a square matrix
 * \param [in] unit_diagonal indicates if the diagonal entries are implicitly
 *  set to ones (1s) or copied from the original matrix, A. Default is true.
 *
 * \return U a new matrix instance of the same dimensions as A consisting of
 *  the upper triangulart part of A and all the rest of its entries set to
 *  zero.
 *
 * \pre A.isSquare() == true
 * \post lower-triangular of matrix U is set to zeros (0s)
 *******************************************************************************
 */
template < typename T >
Matrix< T > upper_triangular( const Matrix< T >& A, bool unit_diagonal=true );

/// @}

/*!
 *******************************************************************************
 * \class Matrix
 *
 * \brief The Matrix class is used to represent \f$ M \times N \f$ matrices. It
 *  provides common matrix operations and allows accessing matrix elements in
 *  a more natural way, using row and column indices, regardless of the
 *  underlying flat array storage layout.
 *
 * \note The underlying storage layout is column-major.
 *
 * Basic usage example:
 * \code
 * int main( int argc, char**argv )
 * {
 *   // Allocate a 5,5 matrix
 *   Matrix< double > A(5,5);
 *
 *   const int nrows = A.getNumRows(); // nrows=5
 *   const int ncols = A.getNumColumnds(); // ncols=5
 *
 *   // loop over the elements of the matrix, row by row
 *   typedef typename Matrix< double >::IndexType IndexType;
 *
 *   for ( IndexType i=0; i < nrows; ++i ) {
 *      for ( IndexType j=0; j < ncols; ++j ) {
 *
 *        A( i,j ) = getValue( );
 *
 *      } // END for all columns
 *   } // END for all rows
 *   return 0;
 * }
 * \endcode
 *
 * \tparam T the underlying matrix data type, e.g., float, double, etc.
 *******************************************************************************
 */
template < typename T >
class Matrix
{
public:
  typedef long IndexType;

public:

  /*!
   *****************************************************************************
   * \brief Constructor, creates a Matrix with the given rows and columns.
   *
   * \param [in] rows the number of rows in the matrix.
   * \param [in] cols the number of columns in the matrix.
   * \param [in] val optional argument to initialize the entries of the matrix.
   *  If not supplied, the default is zero.
   *
   * \pre rows >= 1
   * \pre cols >= 1
   *****************************************************************************
   */
  Matrix( int rows, int cols, T val=static_cast< T >( 0 ) );

  /*!
   *****************************************************************************
   * \brief Array constructor, creates a Matrix with the given rows and columns
   *  and initializes its entries with the data from the supplied array.
   *
   * \param [in] rows the number of rows in the matrix
   * \param [in] cols the number of columns in the matrix
   * \param [in] data pointer to user-supplied buffer to initialize the matrix.
   *
   * \pre rows >= 1
   * \pre cols >= 1
   * \pre data != AXOM_NULLPTR
   *****************************************************************************
   */
  Matrix( int rows, int cols, T* data );

  /*!
   *****************************************************************************
   * \brief Copy constructor.
   * \param [in] m the matrix instance that is being passed.
   *****************************************************************************
   */
  Matrix( const Matrix< T >& m );

  /*!
   *****************************************************************************
   * \brief Destructor.
   *****************************************************************************
   */
  ~Matrix();

  /*!
   *****************************************************************************
   * \brief Check to see if the matrix is square.
   * \return status true iff this instance is a square matrix, else, false.
   *****************************************************************************
   */
  bool isSquare() const { return (m_rows==m_cols); };

  /*!
   *****************************************************************************
   * \brief Checks if the matrix is empty
   * \return status true iff the matrix is empty, else, false.
   *****************************************************************************
   */
  bool empty() const { return (m_rows*m_cols==0); };

  /*!
   *****************************************************************************
   * \brief Returns the number of rows in the matrix..
   * \return numRows the number of rows in the matrix.
   *****************************************************************************
   */
  int getNumRows() const { return m_rows; };

  /*!
   *****************************************************************************
   * \brief Returns the number of columns in the matrix.
   * \return numCols the number of columns in the matrix.
   *****************************************************************************
   */
  int getNumColumns() const { return m_cols; };

  /*!
   *****************************************************************************
   * \brief Returns the size of the diagonal.
   * \return N the size of the diagonal.
   * \note For non-square matrices, \f$ N=min(num\_rows, num\_cols) \f$
   *****************************************************************************
   */
  int getDiagonalSize() const { return (m_rows < m_cols)? m_rows : m_cols; };

  /*!
   *****************************************************************************
   * \brief Returns the diagonal entries of this Matrix instance.
   *
   * \param [in] diagonal user-supplied buffer to store the diagonal entries.
   *
   * \pre diagonal != AXOM_NULLPTR
   * \pre diagonal must have sufficient storage for all the diagonal entries.
   *****************************************************************************
   */
  void getDiagonal( T* diagonal ) const;

  /*!
   *****************************************************************************
   * \brief Assigns <em>val</em> to all entries in the diagonal.
   * \param [in] val value to assign to all diagonal entries.
   *****************************************************************************
   */
  void fillDiagonal( const T& val );

  /*!
   *****************************************************************************
   * \brief Assigns <em>val</em> to all elements in the given matrix row.
   * \param [in] i the row index.
   * \param [in] val value to assign to all elements in the given row.
   * \pre i >= 0 && i < m_rows
   *****************************************************************************
   */
  void fillRow( IndexType i, const T& val );

  /*!
   *****************************************************************************
   * \brief Assigns <em>val</em> to all elements in the given matrix column.
   * \param [in] j the column index.
   * \param [in] val value to assign to all elements in the given column.
   * \pre j >= 0 && j < m_cols
   *****************************************************************************
   */
  void fillColumn( IndexType j, const T& val );

  /*!
   *****************************************************************************
   * \brief Assigns <em>val</em> to all elements of the matrix.
   * \param [in] val value to assign to all elements of the matrix.
   *****************************************************************************
   */
  void fill( const T& val );

  /*!
   ****************************************************************************
   * \brief Swaps the rows of this matrix instance.
   * \param [in] i index of the first row to swap
   * \param [in] j index of the second row to swap
   * \pre i >= 0 && i < m_rows
   * \pre j >= 0 && j < m_rows
   ****************************************************************************
   */
  void swapRows( IndexType i, IndexType j );

  /*!
   *****************************************************************************
   * \brief Swaps the columns of this matrix instance.
   * \param [in] i index of the first column to swap
   * \param [in] j index of the second column to swap
   * \pre i >= 0 && i < m_cols
   * \pre j >= 0 && j < m_cols
   *****************************************************************************
   */
  void swapColumns( IndexType i, IndexType j );

  /// \name Random Access Operators
  /// @{

  /*!
   *****************************************************************************
   * \brief Given an \f$ M \times N \f$ matrix, \f$ \mathcal{A} \f$, return
   *  a const reference to matrix element \f$ \alpha_{ij} \f$
   *
   * \param [in] i the row index of the matrix element,
   * \param [in] j the column index of the matrix element.
   * \return \f$ \alpha_{ij} \f$ const reference to matrix element
   *
   * \pre i >= 0 && i < m_rows
   * \pre j >= 0 && j < m_cols
   *****************************************************************************
   */
  const T& operator()(IndexType i, IndexType j) const;

  /*!
   *****************************************************************************
   * \brief Given an \f$ M \times N \f$ matrix, \f$ \mathcal{A} \f$, return a
   *  reference to matrix element \f$ \alpha_{ij} \f$
   *
   * \param [in] i the row index of the matrix element.
   * \param [in] j the column index of the matrix element.
   * \return \f$ \alpha_{ij} \f$ const reference to matrix element
   *
   * \pre i >= 0 && i < m_rows
   * \pre j >= 0 && j < m_cols
   *****************************************************************************
   */
  T& operator()(IndexType i, IndexType j);

  /*!
   *****************************************************************************
   * \brief Returns a const pointer to the  \f$ jth \f$ column of an
   *  \f$ M \times N \f$ matrix, \f$ \mathcal{A} \f$
   *
   * \param [in] j the jth column of the matrix.
   * \return \f$ \mathcal{A}_{*j} \f$ pointer to the \f$ jth \f$ column.
   * \pre j >= 0 && j < m_cols
   *****************************************************************************
   */
  const T* getColumn( IndexType j ) const;

  /*!
   *****************************************************************************
   * \brief Returns pointer to the \f$ jth \f$ column of an \f$ M \times N \f$
   *  matrix, \f$ \mathcal{A} \f$
   *
   * \param [in] j the jth column of the matrix.
   * \return \f$ \mathcal{A}_{*j} \f$ pointer to the \f$ jth \f$ column.
   * \pre j >= 0 && j < m_cols
   *****************************************************************************
   */
  T* getColumn( IndexType j );

  /*!
   *****************************************************************************
   * \brief Returns a const pointer for strided access along the main diagonal.
   *
   * \param [out] p stride used to access elements along the main diagonal.
   * \param [out] N upper-bound used to loop over the main diagonal entries.
   * \return diag pointer along the main diagonal.
   *
   * \post p = m_rows+1
   * \post N = this->getDiaonalSize()*m_rows
   *
   * \note Example Usage:
   * \code
   *  ...
   *  typedef typename Matrix< double >::IndexType IndexType;
   *
   *  Matrix< double > A (MROWS,NCOLS);
   *
   *  IndexType  p = 0;
   *  IndexThype N = 0;
   *  const double* diag  = A.getDiagonal( p, N );
   *
   *  for ( IndexType i=0; i < N; i+=p ) {
   *     std::cout << diag[ i ];
   *  }
   *  ...
   * \endcode
   *****************************************************************************
   */
  const T* getDiagonal( IndexType& p, IndexType& N ) const;

  /*!
   *****************************************************************************
   * \brief Returns a pointer for strided access along the main diagonal.
   *
   * \param [out] p stride used to access elements along the main diagonal.
   * \param [out] N upper-bound used to loop over the main diagonal entris.
   * \return diag pointer along the main diagonal.
   *
   * \post p = m_rows+1
   * \post N = this->getDiagonalSize()*m_rows
   *
   * \note Example Usage:
   * \code
   *  ...
   *  typedef typename Matrix< double >::IndexType IndexType;
   *
   *  Matrix< double > A (MROWS,NCOLS);
   *
   *  IndexType p = 0;
   *  IndexType N = 0;
   *  const double* diag  = A.getDiagonal( p,N );
   *
   *  for ( IndexType i=0; i < N; i+=p ) {
   *     diag[ i ] = newval;
   *  }
   *  ...
   * \endcode
   *****************************************************************************
   */
  T* getDiagonal( IndexType& p, IndexType& N );

  /*!
   *****************************************************************************
   * \brief Returns a const pointer to the \f$ ith \f$ row of an
   *  \f$ M \times N \f$ matrix, \f$ \mathcal{A} \f$
   *
   * \param [in]  i index to the \f$ ith \f$ row of the matrix
   * \param [out] p stride used to access row elements
   * \param [out] N upper-bound to loop over
   * \return \f$ \mathcal{A}_{i*} \f$ pointer to the \f$ ith \f$ row.
   *
   * \pre i >= 0 && i < m_rows
   * \post p == m_rows
   *
   * \note Example Usage:
   * \code
   *   ...
   *   typedef typename Matrix< double >::IndexType IndexType;
   *   Matrix< double > A( MROWS,NCOLS );
   *
   *   IndexType p = 0;
   *   IndexType N = 0;
   *   const double* row = A.getRow( irow, p, N );
   *
   *   for ( IndexType j=0; j < N; j+=p ) {
   *      std::cout << row[ j ]
   *   }
   *   ...
   * \endcode
   *****************************************************************************
   */
  const T* getRow( IndexType i, IndexType& p, IndexType& N ) const;

  /*!
   *****************************************************************************
   * \brief Returns a pointer to the \f$ ith \f$ row of an  \f$ M \times N \f$
   *  matrix, \f$ \mathcal{A} \f$
   *
   * \param [in]  i index to the \f$ ith \f$ row of the matrix
   * \param [out] p stride used to access row elements
   * \param [out] N upper-bound to loop over
   * \return \f$ \mathcal{A}_{i*} \f$ pointer to the \f$ ith \f$ row.
   *
   * \pre i >= 0 && i < m_rows
   * \post p == m_rows
   *
   * \note Example Usage:
   * \code
   *   ...
   *   typedef typename Matrix< double >::IndexType IndexType;
   *   Matrix< double > A( MROWS,NCOLS );
   *
   *   IndexType p = 0;
   *   IndexType N = 0;
   *   double* row = A.getRow( irow, p, N );
   *
   *   for ( IndexType j=0; j < N; j+=p ) {
   *      row[ j ] = newval;
   *   }
   *   ...
   * \endcode
   *
   *****************************************************************************
   */
  T* getRow( IndexType i, IndexType& p, IndexType& N );

  /*!
   *****************************************************************************
   * \brief Returns a const pointer to the raw data.
   *
   * \return ptr pointer to the raw data.
   * \note The raw data are stored in column-major layout.
   * \post ptr != AXOM_NULLPTR
   *****************************************************************************
   */
  const T* data() const;

  /*!
   *****************************************************************************
   * \brief Returns pointer to the raw data.
   *
   * \return ptr pointer to the raw data.
   * \note The raw data are stored in column-major layout.
   * \post ptr != AXOM_NULLPTR
   *****************************************************************************
   */
  T* data();

  /// @}

  /// \name Overloaded Operators
  /// @{

  /*!
   *****************************************************************************
   * \brief Overloaded assignment operator.
   *
   * \param [in] rhs matrix instance on the right-hand side.
   * \return M a copy of the matrix instance in rhs.
   *****************************************************************************
   */
  Matrix< T >& operator=(const Matrix< T >& rhs);

  /// @}

  /// \name Static Methods
  /// @{

  /*!
   *****************************************************************************
   * \brief Returns an <em> identity matrix </em> \f$ \mathcal{I}_n \f$
   *
   * \param [in] n the size of the identity matrix.
   * \return M the identity matrix of size \f$ \mathcal{I}_n \f$
   *
   * \pre n >= 1
   * \post M.isSquare()==true
   * \post M.getNumRows() == M.getNumCols() == n
   *****************************************************************************
   */
  static Matrix< T > identity( int n );

  /*!
   *****************************************************************************
   * \brief Returns a <em>zero</em> matrix, \f$ \mathcal{A} \f$
   *
   * \param [in] nrows the number of rows in the matrix.
   * \param [in] ncols the number of columns in the matrix.
   * \return \f$ \mathcal{A} \f$ a zero matrix.
   *
   * \pre nrows >= 1
   * \pre ncols >= 1
   * \post \f$ \alpha_{ij}=0 \forall \alpha_{ij} \in \mathcal{A} \f$
   *****************************************************************************
   */
  static Matrix< T > zeros( int nrows, int ncols );

  /*!
   *****************************************************************************
   * \brief Returns a <em>unity</em> matrix, \f$ \mathcal{A} \f$
   *
   * \param [in] nrows the number of rows in the matrix.
   * \param [in] ncols the number of columns in the matrix.
   * \return \f$ \mathcal{A} \f$ a zero matrix.
   *
   * \pre nrows >= 1
   * \pre ncols >= 1
   * \post \f$ \alpha_{ij}=1 \forall \alpha_{ij} \in \mathcal{A} \f$
   *****************************************************************************
   */
  static Matrix< T > ones( int nrows, int ncols );

  /// @}

private:

  /*!
   *****************************************************************************
   * \brief Default constructor. Does nothing.
   * \note Made private to prevent host-code from calling this.
   *****************************************************************************
   */
  Matrix() : m_rows(0), m_cols(0), m_data(AXOM_NULLPTR) { };

  /// \name Private Helper Methods
  /// @{

  /*!
   *****************************************************************************
   * \brief Copies the matrix into this matrix instance.
   * \param [in] rhs matrix on the right-hand side.
   *****************************************************************************
   */
  void copy( const Matrix< T >& rhs );

  /*!
   ****************************************************************************
   * \brief Deallocates all matrix data.
   ****************************************************************************
   */
  void clear();

  /// @}

  /// \name Private Data Members
  /// @{

  int m_rows; /*!< the number of rows in the matrix */
  int m_cols; /*!< the number of columns in the matrix */
  T* m_data;  /*!< raw storage buffer for the matrix data */

  /// @}

};

} /* end namespace numerics */
} /* end namespace axom */

//------------------------------------------------------------------------------
// Matrix class implementation
//------------------------------------------------------------------------------
namespace axom {
namespace numerics {

template < typename T >
Matrix< T >::Matrix( int rows, int cols, T val ) : m_rows( rows ), m_cols( cols )
{
  // sanity checks
  assert( m_rows > 0 );
  assert( m_cols > 0 );

  m_data = new T [ m_rows*m_cols ];
  this->fill( val );
}

//-----------------------------------------------------------------------------
template < typename T >
Matrix< T >::Matrix( int rows, int cols, T* data ) :
 m_rows( rows ),
 m_cols( cols )
{
  assert( data != AXOM_NULLPTR );

  const int nitems = m_rows*m_cols;
  m_data = new T[ nitems ];
  memcpy( m_data, data, nitems*sizeof(T) );
}

//-----------------------------------------------------------------------------
template < typename T >
Matrix< T >::Matrix( const Matrix< T >& rhs )
{
  m_rows = m_cols = 0;
  m_data = AXOM_NULLPTR;
  this->copy( rhs );
}

//-----------------------------------------------------------------------------
template < typename T >
Matrix< T >::~Matrix( )
{
  this->clear();
}

//-----------------------------------------------------------------------------
template < typename T >
void Matrix< T >::getDiagonal( T* diagonal ) const
{
  assert( diagonal != AXOM_NULLPTR );

  const int N = this->getDiagonalSize();
  const int p = m_rows+1;
  for ( IndexType i=0, j=0; i < N; ++i, j+=p ) {
     diagonal[ i ] = m_data[ j ];
  }
}

//-----------------------------------------------------------------------------
template < typename T >
void Matrix< T >::fillDiagonal( const T& val )
{
  const int N = this->getDiagonalSize()*m_rows;
  const int p = m_rows+1;
  for ( IndexType i=0; i < N; i+=p ) {
     m_data[ i ] = val;
  }

}

//-----------------------------------------------------------------------------
template < typename T >
void Matrix< T >::fillRow( IndexType i, const T& val )
{
  assert( (i>=0) && (i < m_rows) );

  const int N = (m_cols-1)*m_rows + i + 1;
  const int p = m_rows;
  for ( IndexType j=i; j < N; j+=p ) {
     m_data[ j ] = val;
  }

}

//-----------------------------------------------------------------------------
template < typename T >
void Matrix< T >::fillColumn( IndexType j, const T& val )
{
  assert( (j>=0) && (j < m_cols) );

  const IndexType offset = j*m_rows;
  for ( IndexType i=0; i < m_rows; ++i ) {
     m_data[ offset+i ] = val;
  }

}

//-----------------------------------------------------------------------------
template < typename T >
void Matrix< T >::fill( const T& val )
{
  const int nitems = m_rows*m_cols;
  for ( IndexType i=0; i < nitems; ++i ) {
     m_data[ i ] = val;
  }

}

//-----------------------------------------------------------------------------
template < typename T >
void Matrix< T >::swapRows( IndexType irow, IndexType jrow )
{
  assert( (irow>=0) && (irow < m_rows) );
  assert( (jrow>=0) && (jrow < m_rows) );

  if ( irow == jrow ) {
    /* short-circuit */
    return;
  }

  // convenience reference to *this
  Matrix< T >& A = *this;

  for ( IndexType k=0; k < m_cols; ++k ) {
     utilities::swap( A( irow,k ), A( jrow,k ) );
  }

}

//-----------------------------------------------------------------------------
template < typename T >
void Matrix< T >::swapColumns( IndexType icol, IndexType jcol )
{
  assert( (icol >= 0) && (icol < m_cols) );
  assert( (jcol >= 0) && (jcol < m_cols) );

  if ( icol == jcol ) {
    /* short-circuit */
    return;
  }

  T* icol_data = this->getColumn( icol );
  T* jcol_data = this->getColumn( jcol );
  for ( IndexType i=0; i < m_rows; ++i ) {
     utilities::swap( icol_data[ i ], jcol_data[ i ] );
  }

}

//-----------------------------------------------------------------------------
template < typename T >
const T& Matrix< T >::operator()(IndexType i, IndexType j ) const
{
  assert( (i>=0) && (i < m_rows) );
  assert( (j>=0) && (j < m_cols) );
  return m_data[ j*m_rows+i ];
}

//-----------------------------------------------------------------------------
template < typename T >
T& Matrix< T >::operator()(IndexType i, IndexType j)
{
  assert( (i>=0) && (i < m_rows) );
  assert( (j>=0) && (j < m_cols) );
  return m_data[ j*m_rows+i ];
}

//-----------------------------------------------------------------------------
template < typename T >
const T* Matrix< T >::getColumn( IndexType j ) const
{
  assert( (j>=0) && (j < m_cols) );
  return &m_data[ j*m_rows ];
}

//-----------------------------------------------------------------------------
template < typename T >
T* Matrix< T >::getColumn( IndexType j )
{
  assert( (j>=0) && (j < m_cols) );
  return &m_data[ j*m_rows ];
}

//-----------------------------------------------------------------------------
template < typename T >
const T* Matrix< T >::getRow( IndexType i, IndexType& p, IndexType& N ) const
{
  assert( (i >= 0) && (i < m_rows) );
  p = m_rows;
  N = (m_cols-1)*m_rows + i + 1;
  return &m_data[ i ];
}

//-----------------------------------------------------------------------------
template < typename T >
T* Matrix< T>::getRow( IndexType i, IndexType& p, IndexType& N )
{
  assert( (i >= 0) && (i < m_rows) );
  p = m_rows;
  N = (m_cols-1)*m_rows + i + 1;
  return &m_data[ i ];
}

//-----------------------------------------------------------------------------
template < typename T >
const T* Matrix< T >::getDiagonal( IndexType& p, IndexType& N ) const
{
  p = m_rows + 1;
  N = this->getDiagonalSize()*m_rows;
  return &m_data[ 0 ] ;
}

//-----------------------------------------------------------------------------
template < typename T >
T* Matrix< T >::getDiagonal( IndexType& p, IndexType& N )
{
  p = m_rows + 1;
  N = this->getDiagonalSize()*m_rows;
  return &m_data[ 0 ] ;
}

//-----------------------------------------------------------------------------
template < typename T >
const T* Matrix< T >::data() const
{
  return m_data;
}

//-----------------------------------------------------------------------------
template < typename T >
T* Matrix< T  >::data()
{
  return m_data;
}

//-----------------------------------------------------------------------------
template < typename T >
Matrix< T >& Matrix< T >::operator=( const Matrix< T >& rhs )
{

  if ( this != &rhs ) {

    if ( (m_rows != rhs.m_rows) || (m_cols != rhs.m_cols) ) {
      this->clear();
    }

    this->copy( rhs );
  }

  return *this;
}

//-----------------------------------------------------------------------------
// STATIC METHODS
//-----------------------------------------------------------------------------
template < typename T >
Matrix< T > Matrix< T >::identity( int n )
{
  Matrix< T > In( n, n );

  for ( IndexType i=0; i < n; ++i ) {
     for ( IndexType j=0; j < n; ++j ) {
        In( i,j ) = static_cast< T >( ( i==j )? 1.0 : 0.0 );
     } // END for all columns
  } // END for all rows

  return ( In );
}

//-----------------------------------------------------------------------------
template < typename T >
Matrix< T > Matrix< T >::zeros( int nrows, int ncols )
{
  Matrix< T > M( nrows, ncols );
  M.fill( static_cast< T >( 0 ) );
  return ( M );
}

//-----------------------------------------------------------------------------
template < typename T >
Matrix< T > Matrix< T >::ones( int nrows, int ncols )
{
  Matrix< T > M( nrows, ncols );
  M.fill( static_cast< T >( 1 ) );
  return ( M );
}

//-----------------------------------------------------------------------------
// PRIVATE HELPER METHODS
//-----------------------------------------------------------------------------
template < typename T >
void Matrix< T >::copy( const Matrix< T >& rhs )
{
  bool do_allocate = (m_rows != rhs.m_rows) || (m_cols != rhs.m_cols);

  if ( do_allocate ) {

    assert( m_data == AXOM_NULLPTR );

    m_rows = rhs.m_rows;
    m_cols = rhs.m_cols;
    m_data = new T[ m_rows*m_cols ];
  }

  assert( m_rows==rhs.m_rows );
  assert( m_cols==rhs.m_cols );
  assert( m_data != AXOM_NULLPTR );

  const int nitems   = m_rows*m_cols;
  const int bytesize = nitems*sizeof(T);
  memcpy( m_data, rhs.m_data, bytesize );
}

//-----------------------------------------------------------------------------
template < typename T >
void Matrix< T >::clear( )
{
  delete [ ] m_data;
  m_data = AXOM_NULLPTR;
  m_rows = m_cols = 0;
}

//-----------------------------------------------------------------------------
// OVERLOADED OPERATORS
//-----------------------------------------------------------------------------

template < typename T >
Matrix< T > operator+( const Matrix< T >& A, const Matrix< T >& B )
{
  const int nrows = A.getNumRows();
  const int ncols = A.getNumColumns();
  Matrix< T > C( nrows, ncols );

  add( A, B, C );

  return ( C );
}

//-----------------------------------------------------------------------------
template < typename T >
Matrix< T > operator-( const Matrix< T >& A, const Matrix< T >& B )
{
  const int nrows = A.getNumRows();
  const int ncols = A.getNumColumns();
  Matrix< T > C( nrows, ncols );

  subtract( A, B, C );

  return ( C );
}

//-----------------------------------------------------------------------------
template < typename T >
Matrix< T > operator*( const Matrix< T >& A, const T& scalar )
{
  Matrix< T > C = A;
  scalar_multiply( C, scalar );
  return ( C );
}

//-----------------------------------------------------------------------------
template < typename T >
Matrix< T > operator*( const Matrix< T >& A, const T* x )
{
  const int nrows= A.getNumRows();

  Matrix< T > b( nrows, 1 );
  vector_multiply( A, x, b.data() );
  return( b );
}

//-----------------------------------------------------------------------------
template < typename T >
Matrix< T > operator*( const Matrix< T >& A, const Matrix< T >& B )
{
  const int nrows = A.getNumRows();
  const int ncols = B.getNumColumns();

  Matrix< T > C(nrows,ncols);
  matrix_multiply( A, B, C );
  return( C );
}

//-----------------------------------------------------------------------------
// FREE METHODS
//-----------------------------------------------------------------------------
template < typename T >
void add( const Matrix< T >& A, const Matrix< T >&B, Matrix< T >& C )
{
  assert( A.getNumRows() == B.getNumRows() );
  assert( A.getNumColumns() == B.getNumColumns() );
  assert( C.getNumRows() == B.getNumRows() );
  assert( C.getNumColumns() == B.getNumColumns() );

  if ( A.getNumRows()    != B.getNumRows() ||
       A.getNumColumns() != B.getNumColumns() ||
       C.getNumRows()    != B.getNumRows() ||
       C.getNumColumns() != B.getNumColumns() ) {

    // matrix dimensions do not match
    C = Matrix< T >::zeros(1,1);
    return;
  }

  typedef typename Matrix< T >::IndexType IndexType;

  const int nrows = A.getNumRows();
  const int ncols = A.getNumColumns();
  const int N     = nrows*ncols;

  T* target = C.data();

  const T* sourceA = A.data();
  const T* sourceB = B.data();
  for ( IndexType i=0; i < N; ++i ) {
     target[ i ] = sourceA[ i ] + sourceB[ i ];
  }

}

//-----------------------------------------------------------------------------
template < typename T >
void subtract( const Matrix< T >& A, const Matrix< T >& B, Matrix< T >& C )
{
  assert( A.getNumRows() == B.getNumRows() );
  assert( A.getNumColumns() == B.getNumColumns() );
  assert( C.getNumRows() == B.getNumRows() );
  assert( C.getNumColumns() == B.getNumColumns() );

  if ( A.getNumRows()    != B.getNumRows() ||
       A.getNumColumns() != B.getNumColumns() ||
       C.getNumRows()    != B.getNumRows() ||
       C.getNumColumns() != B.getNumColumns() ) {

      // matrix dimensions do not match
      C = Matrix< T >::zeros(1,1);
      return;
  }

    typedef typename Matrix< T >::IndexType IndexType;

    const int nrows = A.getNumRows();
    const int ncols = A.getNumColumns();
    const int N     = nrows*ncols;

    T* target = C.data();

    const T* sourceB = B.data();
    const T* sourceA = A.data();
    for ( IndexType i=0; i < N; ++i ) {
       target[ i ] = sourceA[ i ] - sourceB[ i ] ;
    }

}

//-----------------------------------------------------------------------------
template < typename T >
void scalar_multiply( Matrix< T >& A, const T& c )
{
  // matrix-scalar multiplication
  const int nrows = A.getNumRows();
  const int ncols = A.getNumColumns();
  const int N     = nrows*ncols;

  typedef typename Matrix< T >::IndexType IndexType;

  T* target = A.data();
  for ( IndexType i=0; i < N; ++i ) {
     target[ i ] *= c;
  }

}

//-----------------------------------------------------------------------------
template < typename T >
void vector_multiply( const Matrix< T >& A, const T* x, T* b )
{
  // matrix-vector multiplication
  const int nrows = A.getNumRows();
  const int ncols = A.getNumColumns();

  typedef typename Matrix< T >::IndexType IndexType;

  for ( IndexType i=0; i < nrows; ++i ) {

    double sum = 0.0;
    for ( IndexType j=0; j < ncols; ++j ) {
       sum += A( i,j )*x[ j ];
    } // END for all columns

    b[ i ] = sum;

  } // END for all rows

}

//-----------------------------------------------------------------------------
template < typename T >
void matrix_multiply( const Matrix< T >& A,
                      const Matrix< T >& B,
                      Matrix< T >& C )
{
  // basic matrix-matrix multiplication
  assert( A.getNumColumns() == B.getNumRows() );

  if ( A.getNumColumns() != B.getNumRows()   ||
       C.getNumRows() != A.getNumRows()      ||
       C.getNumColumns() != B.getNumColumns()   ) {

     C = Matrix< T >::zeros( 1,1 );
     return;
  }

  typedef typename Matrix< T >::IndexType IndexType;

  const int nk    = A.getNumColumns();
  const int nrows = C.getNumRows();
  const int ncols = C.getNumColumns();

  for ( IndexType i=0; i < nrows; ++i ) {
     for ( IndexType j=0; j < ncols; ++j ) {

        C( i,j ) = static_cast< T >( 0 );
        for ( IndexType k=0; k < nk; ++k ) {
           C( i,j ) += A(i,k) * B(k,j);
        } // END for all internal

     } // END for all columns
  } // END for all rows

}

//-----------------------------------------------------------------------------
template < typename T >
Matrix< T > transpose( const Matrix< T >& A )
{
  const int ncols = A.getNumColumns();
  const int nrows = A.getNumRows();

  typedef typename Matrix< T >::IndexType IndexType;

  Matrix< T > M( ncols, nrows );

  for ( IndexType i=0; i < nrows; ++i ) {
     for ( IndexType j=0; j < ncols; ++j ) {
        M( j,i ) = A( i,j );
     } // END for all columns in A
  } // END for all rows in A

  return ( M );
}

//-----------------------------------------------------------------------------
template < typename T >
Matrix< T > lower_triangular( const Matrix< T >& A, bool unit_diagonal )
{
  assert( A.isSquare() );

  if ( !A.isSquare() ) {
    return Matrix< T >::zeros( 1,1 );
  }

  const int N  = A.getNumRows();

  Matrix< T > L( N,N );

  typedef typename Matrix< T >::IndexType IndexType;

  for ( IndexType i=0; i < N; ++i ) {
     for ( IndexType j=0; j < N; ++j ) {

        if ( i==j ) {

           L( i,j ) = ( unit_diagonal )? static_cast< T >( 1 ) : A( i,j );

        } else {

           L( i,j ) = ( i > j )? A( i,j ) : static_cast< T >( 0 );
        }

     } // END for all columns
  } // END for all rows

  return ( L );
}

//-----------------------------------------------------------------------------
template < typename T >
Matrix< T > upper_triangular( const Matrix< T >& A, bool unit_diagonal )
{
  assert( A.isSquare() );

  if ( !A.isSquare() ) {
    return Matrix< T >::zeros( 1,1 );
  }

  const int N = A.getNumRows();

  Matrix< T > U( N,N );

  typedef typename Matrix< T >::IndexType IndexType;

  for ( IndexType i=0; i < N; ++i ) {
     for ( IndexType j=0; j < N; ++j ) {

        if ( i==j ) {

          U( i,j ) = ( unit_diagonal )? static_cast< T >( 1 ) : A( i,j );

        } else {

          U( i,j ) = ( i < j )? A( i,j ) : static_cast< T >( 0 );

        }

     } // END for all columns
  } // END for all rows

  return ( U );
}

//-----------------------------------------------------------------------------
template < typename T >
std::ostream& operator<<( std::ostream& os, const Matrix< T >& M )
{
  const int nrows = M.getNumRows();
  const int ncols = M.getNumColumns();

  typedef typename Matrix< T >::IndexType IndexType;

  for ( IndexType i=0; i < nrows; ++i ) {
     os << "[ ";
     for ( int j=0; j < ncols; ++j ) {
        os << M( i,j ) << " ";
     } // END for all j

     os << "]\n";

  } // END for all i

  return ( os );
}

} /* end namespace numerics */
} /* end namespace axom */

#endif /* AXOM_MATRIX_HPP_ */
