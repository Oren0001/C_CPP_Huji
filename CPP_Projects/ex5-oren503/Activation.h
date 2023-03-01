//Activation.h
#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"

/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */
enum ActivationType {
    RELU,
    SOFTMAX
};

/**
 * A class which represents an activation function.
 */
class Activation {
 private:
  /* The type of the activation function. */
  const ActivationType _type;

 public:
  /**
   * Accepts an ActivationType enum with one of two legal values:
     ReLU/Softmax. Defines this instance's activation accordingly.
   * @param act_type The type of the activation function.
   */
  Activation (const ActivationType act_type) : _type (act_type)
  {}

  /**
   * @return The activation type (ReLU or Softmax).
   */
  ActivationType get_activation_type () const
  {
    return _type;
  }

  /**
   * Applies the appropriate activation function on rhs.
   * The operator wont change rhs.
   * @param rhs A matrix of 1 column.
   * @return A new matrix which is the result of applying the activation
   *         function on rhs.
   */
  Matrix operator() (const Matrix &rhs) const;
};

#endif //ACTIVATION_H
