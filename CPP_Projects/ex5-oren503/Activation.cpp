#include "Activation.h"

/**
 * Applies the Softmax function on m.
 * @param m A copy of rhs - a matrix of one column.
 * @return A reference to m.
 */
Matrix &softmax (Matrix &m)
{
  int m_size = m.get_rows () * m.get_cols ();
  float c = 0;
  for (int i = 0; i < m_size; ++i)
    {
      c += std::exp (m[i]);
    }
  c = 1 / c;
  for (int i = 0; i < m_size; ++i)
    {
      m[i] = c * std::exp (m[i]);
    }
  return m;
}

/**
 * Applies the ReLU function on m.
 * @param m A copy of rhs - a matrix of one column.
 * @return A reference to m.
 */
Matrix &relu (Matrix &m)
{
  int m_size = m.get_rows () * m.get_cols ();
  for (int i = 0; i < m_size; ++i)
    {
      if (m[i] < 0)
        {
          m[i] = 0;
        }
    }
  return m;
}

/**
* Applies the appropriate activation function on rhs.
* The operator wont change rhs.
* @param rhs A matrix of 1 column.
* @return A new matrix which is the result of applying the activation
*         function on rhs.
*/
Matrix Activation::operator() (const Matrix &rhs) const
{
  Matrix m (rhs);
  switch (_type)
    {
      case RELU:
        return relu (m);
      case SOFTMAX:
        return softmax (m);
      default:
        std::cerr << "Error: Invalid activation type." << std::endl;
        exit(EXIT_FAILURE);
    }
}
