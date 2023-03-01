#include "Dense.h"

/**
* Applies the layer on rhs.
* @param rhs A matrix of one column.
* @return A new matrix which is the result of applying the layer on rhs.
*/
Matrix Dense::operator() (const Matrix &rhs) const
{
  Matrix res = _weights * rhs;
  res += _bias;
  return _act (res);
}
