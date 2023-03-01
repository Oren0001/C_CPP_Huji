// Matrix.h

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cmath>

/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims {
    int rows, cols;
} matrix_dims;

/**
 * A class which represents a Matrix of rowsXcols.
 */
class Matrix {
 private:
  /* keeps all matrix elements. */
  float *_data;

  /* A struct that represents the number of rows and cols in the matrix. */
  matrix_dims _dims;

  /* The size of the matrix. */
  int _size;

 public:
  /**
   * Constructs a Matrix with dimensions rowsXcols.
   * All elements are initialized to 0.
   * @param rows Number of rows.
   * @param cols Number of Columns.
   */
  Matrix (const int &rows, const int &cols);

  /**
   * Constructs a 1X1 Matrix. The single element is initialized to 0.
   */
  Matrix ();

  /**
   * A Matrix copy constructor.
   * @param m A matrix to construct from.
   */
  Matrix (const Matrix &m);

  /** A Matrix destructor. */
  ~Matrix ()
  {
    delete[] _data;
  }

  /**
   * @return The amount of rows a matrix has.
   */
  int get_rows () const
  {
    return _dims.rows;
  }

  /**
   * @return The amount of columns a matrix has.
   */
  int get_cols () const
  {
    return _dims.cols;
  }

  /**
   * Transforms a matrix into its transpose matrix.
   * e.g.
   * Matrix a(5,4), b(4,5);
   * a.transpose(); // a.get_rows == 4, a.get_cols == 5
   * b.transpose().transpose(); // b is same as before
   * @return A reference this matrix.
   */
  Matrix &transpose ();

  /**
   * Transforms a matrix into a column vector.
   * e.g.:
   * Matrix m(5,4);
   * m.vectorize(); // m.get_cols() == 1, m.get_rows() == 20
   * @return A reference to this matrix.
   */
  Matrix &vectorize ();

  /**
   * Prints matrix elements with a space after each element,
   * and a newline after each row.
   */
  void plain_print () const;

  /**
   * Multiplies this matrix and another matrix - m.
   * @param m A matrix to multiply with.
   * @return A matrix that is the product.
   */
  Matrix dot (Matrix &rhs) const;

  /**
   * @return The Frobenius norm of the given matrix.
   */
  float norm () const;

  /**
   * Reads the entire content of a binary file to the given matrix.
   * The matrix contains float numbers which we assume are of size 4,
   * therefore each 4 bytes of the file will be represented as a float
   * number in the matrix.
   * @param is An input stream which we will read from.
   * @param m A reference to a matrix which will be filled entirely.
   */
  friend std::istream &read_binary_file (std::istream &is, const Matrix &m);

  /**
   * Adds each coordinate of this matrix to it's corresponding
   * coordinate in another matrix.
   * @param m_2 A reference to a matrix.
   * @return A new matrix which contains the sum of the given matrices.
   */
  Matrix operator+ (const Matrix &rhs) const;

  /**
   * Assigns a matrix to this one.
   * @param m_2 A matrix to assign.
   * @return The matrix after assignment.
   */
  Matrix &operator= (const Matrix &rhs);

  /**
   * Multiplies this matrix and another matrix.
   * @param m_2 A matrix to multiply with.
   * @return A new matrix that equals the product of the matrices.
   */
  Matrix operator* (const Matrix &rhs) const;

  /**
   * Scalar multiplication on the right.
   * @param c A scalar to multiply with.
   * @return A new matrix with the product values.
   */
  Matrix operator* (const float &c) const;

  /**
   * Scalar multiplication on the left.
   * @param c A scalar to multiply with.
   * @return A new matrix with the product values.
   */
  friend Matrix operator* (const float &c, const Matrix &m)
  {
    return m * c;
  }

  /**
   * Operates as matrix addition accumulation.
   * @param m_2 A matrix to add to this matrix.
   * @return A reference to this matrix after the addition.
   */
  Matrix &operator+= (const Matrix &rhs);

  /**
   * @param i Number of row.
   * @param j Number of column.
   * @return A reference to the element positioned at i,j.
   */
  float &operator() (const int &i, const int &j);

  /**
   * @param i Number of row.
   * @param j Number of column.
   * @return A const reference to the element positioned at i,j.
   */
  float operator() (const int &i, const int &j) const;

  /**
   * @param i The index of the element to return.
   * @return A reference to the i'th element in the matrix.
   */
  float &operator[] (const int &i);

  /**
   * @param i The index of the element to return.
   * @return A const reference to the i'th element in the matrix.
   */
  float operator[] (const int &i) const;

  /**
   * Prints the image that is represented by the given matrix.
   * @param os A reference to an output stream.
   * @param m A reference to a matrix.
   * @return os.
   */
  friend std::ostream &operator<< (std::ostream &os, const Matrix &m);
};

#endif //MATRIX_H
