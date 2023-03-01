#include "Matrix.h"

#define DEFAULT_ROW_SIZE 1
#define DEFAULT_COL_SIZE 1
#define ERROR_FORMAT "Error: "
#define FLOAT_SIZE 4
#define PRINT_THRESHOLD 0.1
#define ABOVE_THRESHOLD_PRINT "  "
#define BELOW_THRESHOLD_PRINT "**"
#define READ_ERROR_MSG "Could not read binary file."
#define SIZE_ERROR_MSG "Matrices size are not equal."
#define OUT_OF_BOUNDS "Matrix index is out of bounds."

using std::cerr;
using std::endl;

/**
* Constructs a Matrix with dimensions rowsXcols.
* All elements are initialized to 0.
* @param rows Number of rows.
* @param cols Number of Columns.
*/
Matrix::Matrix (const int &rows, const int &cols) :
  _dims{rows, cols}
{
  if ((rows < DEFAULT_ROW_SIZE) || (cols < DEFAULT_COL_SIZE))
    {
      cerr << ERROR_FORMAT << "Invalid input." << endl;
      exit (EXIT_FAILURE);
    }
  _data = new (std::nothrow) float[rows * cols] ();
  if (_data == nullptr)
    {
      cerr << ERROR_FORMAT << "Allocation failure." << endl;
      exit (EXIT_FAILURE);
    }
  _size = rows * cols;
}

/**
* Constructs a 1X1 Matrix. The single element is initialized to 0.
*/
Matrix::Matrix () : Matrix (DEFAULT_ROW_SIZE, DEFAULT_COL_SIZE)
{}

/**
   * A Matrix copy constructor.
   * @param m A matrix to construct from.
   */
Matrix::Matrix (const Matrix &m) : Matrix (m.get_rows (), m.get_cols ())
{
  for (int i = 0; i < (m._size); ++i)
    this->_data[i] = m._data[i];
}

/**
* Transforms a matrix into its transpose matrix.
* e.g.
* Matrix a(5,4), b(4,5);
* a.transpose(); // a.get_rows == 4, a.get_cols == 5
* b.transpose().transpose(); // b is same as before
* @return A reference to this matrix.
*/
Matrix &Matrix::transpose ()
{
  Matrix old_matrix (*this);
  this->_dims.rows = old_matrix.get_cols ();
  this->_dims.cols = old_matrix.get_rows ();
  int new_row, new_col, new_index;
  for (int index = 0; index < this->_size; ++index)
    {
      new_row = index % old_matrix.get_cols ();
      new_col = index / old_matrix.get_cols ();
      new_index = new_row * this->_dims.cols + new_col;
      this->_data[new_index] = old_matrix._data[index];
    }
  return *this;
}

/**
 * Transforms a matrix into a column vector.
 * e.g.:
 * Matrix m(5,4);
 * m.vectorize(); // m.get_cols() == 1, m.get_rows() == 20
 * @return A reference to this matrix.
 */
Matrix &Matrix::vectorize ()
{
  _dims.rows = _size;
  _dims.cols = 1;
  return *this;
}

/**
 * Prints matrix elements with a space after each element,
 * and a newline after each row.
 */
void Matrix::plain_print () const
{
  for (int i = 0; i < get_rows (); ++i)
    {
      for (int j = 0; j < get_cols (); ++j)
        {
          std::cout << (*this) (i, j) << " ";
        }
      std::cout << endl;
    }
}

/**
 * Multiplies this matrix and another matrix - m.
 * @param m A matrix to multiply with.
 * @return A matrix that is the product.
 */
Matrix Matrix::dot (Matrix &rhs) const
{
  if (this->_size != rhs._size)
    {
      cerr << ERROR_FORMAT << SIZE_ERROR_MSG << endl;
      exit (EXIT_FAILURE);
    }
  Matrix res (this->get_rows (), this->get_cols ());
  for (int i = 0; i < this->_size; ++i)
    {
      res._data[i] = this->_data[i] * rhs._data[i];
    }
  return res;
}

/**
 * @return The Frobenius norm of the given matrix.
 */
float Matrix::norm () const
{
  float sum = 0;
  for (int i = 0; i < _size; ++i)
    {
      sum += _data[i] * _data[i];
    }
  return std::sqrt (sum);
}

/**
 * Reads the entire content of a binary file to the given matrix.
 * The matrix contains float numbers which we assume are of size 4,
 * therefore each 4 bytes of the file will be represented as a float
 * number in the matrix.
 * @param is An input stream which we will read from.
 * @param m A reference to a matrix which will be filled entirely.
 */
std::istream &read_binary_file (std::istream &is, const Matrix &m)
{
  is.seekg (0, std::istream::end);
  long file_bytes_num = is.tellg ();
  long matrix_bytes_num = m._size * FLOAT_SIZE;
  if (file_bytes_num < matrix_bytes_num)
    {
      cerr << ERROR_FORMAT << READ_ERROR_MSG << endl;
      exit (EXIT_FAILURE);
    }
  is.seekg (0, std::istream::beg);
  is.read ((char *) m._data, matrix_bytes_num);
  if (is.fail ())
    {
      cerr << ERROR_FORMAT << READ_ERROR_MSG << endl;
      exit (EXIT_FAILURE);
    }
  return is;
}

/**
 * Adds each coordinate of this matrix to it's corresponding
 * coordinate in another matrix.
 * @param m_2 A reference to a matrix.
 * @return A new matrix which contains the sum of the given matrices.
 */
Matrix Matrix::operator+ (const Matrix &rhs) const
{
  if (this->_size != rhs._size)
    {
      cerr << ERROR_FORMAT << SIZE_ERROR_MSG << endl;
      exit (EXIT_FAILURE);
    }
  Matrix res (this->get_rows (), this->get_cols ());
  for (int i = 0; i < this->_size; ++i)
    {
      res._data[i] = this->_data[i] + rhs._data[i];
    }
  return res;
}

/**
 * Assigns a matrix to this one.
 * @param m_2 A matrix to assign.
 * @return The matrix after assignment.
 */
Matrix &Matrix::operator= (const Matrix &rhs)
{
  if (this != &rhs)
    {
      // assign matrix data
      if (this->_size != rhs._size)
        {
          delete[] this->_data;
          this->_data = new float[rhs._size];
        }
      for (int i = 0; i < rhs._size; ++i)
        this->_data[i] = rhs._data[i];

      // assign matrix fields
      this->_dims.rows = rhs.get_rows ();
      this->_dims.cols = rhs.get_cols ();
      this->_size = rhs._size;
    }
  return *this;
}

/**
* Multiplies this matrix and another matrix.
* @param m_2 A matrix to multiply with.
* @return A new matrix that is the product of the matrices.
*/
Matrix Matrix::operator* (const Matrix &rhs) const
{
  if (this->get_cols () != rhs.get_rows ())
    {
      cerr << ERROR_FORMAT
           << "Number of columns in the first matrix does not "
              "equal to the number of rows in the second matrix" << endl;
      exit (EXIT_FAILURE);
    }
  Matrix res (this->get_rows (), rhs.get_cols ());
  int i = 0;
  float new_element;
  while (i < this->get_rows ())
    {
      for (int j = 0; j < rhs.get_cols (); ++j)
        {
          new_element = 0;
          for (int k = 0; k < rhs.get_rows (); ++k)
            {
              new_element += (*this) (i, k) * rhs (k, j);
            }
          res._data[i * res.get_cols () + j] = new_element;
        }
      i++;
    }
  return res;
}

/**
 * Scalar multiplication on the right.
 * @param c A scalar to multiply with.
 * @return A new matrix with the product values.
 */
Matrix Matrix::operator* (const float &c) const
{
  Matrix res (this->get_rows (), this->get_cols ());
  for (int i = 0; i < this->_size; ++i)
    {
      res._data[i] = this->_data[i] * c;
    }
  return res;
}

/**
 * Operates as matrix addition accumulation.
 * @param m_2 A matrix to add to this matrix.
 * @return A reference to this matrix after the addition.
 */
Matrix &Matrix::operator+= (const Matrix &rhs)
{
  if (this->_size != rhs._size)
    {
      cerr << ERROR_FORMAT << SIZE_ERROR_MSG << endl;
      exit (EXIT_FAILURE);
    }
  for (int i = 0; i < this->_size; ++i)
    {
      this->_data[i] += rhs._data[i];
    }
  return *this;
}

/**
 * @param i Number of row.
 * @param j Number of column.
 * @return A reference to the element positioned at i,j.
 */
float &Matrix::operator() (const int &i, const int &j)
{
  if ((i < 0) || (j < 0) || (i >= get_rows ()) || (j >= get_cols ()))
    {
      cerr << ERROR_FORMAT << OUT_OF_BOUNDS << endl;
      exit (EXIT_FAILURE);
    }
  return _data[i * get_cols () + j];
}

/**
 * @param i Number of row.
 * @param j Number of column.
 * @return A const reference to the element positioned at i,j.
 */
float Matrix::operator() (const int &i, const int &j) const
{
  if ((i < 0) || (j < 0) || (i >= get_rows ()) || (j >= get_cols ()))
    {
      cerr << ERROR_FORMAT << OUT_OF_BOUNDS << endl;
      exit (EXIT_FAILURE);
    }
  return _data[i * get_cols () + j];
}

/**
 * @param i The index of the element to return.
 * @return A reference to the i'th element in the matrix.
 */
float &Matrix::operator[] (const int &i)
{
  if ((i < 0) || (i >= _size))
    {
      cerr << ERROR_FORMAT << OUT_OF_BOUNDS << endl;
      exit (EXIT_FAILURE);
    }
  return _data[i];
}

/**
 * @param i The index of the element to return.
 * @return A const reference to the i'th element in the matrix.
 */
float Matrix::operator[] (const int &i) const
{
  if ((i < 0) || (i >= _size))
    {
      cerr << ERROR_FORMAT << OUT_OF_BOUNDS << endl;
      exit (EXIT_FAILURE);
    }
  return _data[i];
}

/**
 * Prints the image that is represented by the given matrix.
 * @param os A reference to an output stream.
 * @param m A reference to a matrix.
 * @return os.
 */
std::ostream &operator<< (std::ostream &os, const Matrix &m)
{
  for (int i = 0; i < m.get_rows (); ++i)
    {
      for (int j = 0; j < m.get_cols (); ++j)
        {
          if (m (i, j) >= PRINT_THRESHOLD)
            os << ABOVE_THRESHOLD_PRINT;
          else
            os << BELOW_THRESHOLD_PRINT;
        }
      os << endl;
    }
  return os;
}