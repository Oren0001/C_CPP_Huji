#ifndef C___PROJECT_DENSE_H
#define C___PROJECT_DENSE_H

#include "Activation.h"

/**
 * Represents the layers in the Neurons network.
 */
class Dense {
 private:
  /* The weights of the layer. */
  const Matrix &_weights;

  /* The bias of the layer. */
  const Matrix &_bias;

  /* A class of activation. */
  const Activation _act;

 public:
  /**
   * Initializes a new layer with the given parameters.
   * @param weights The weights of the layer.
   * @param bias The bias of the layer.
   * @param act_type An activation type (ReLU or Softmax).
   */
  Dense (const Matrix &weights, const Matrix &bias,
         const ActivationType act_type) :
      _weights (weights), _bias (bias), _act (act_type)
  {}

  /**
   * @return The weights of this layer.
   */
  const Matrix &get_weights () const
  {
    return _weights;
  }

  /**
   * @return The bias of this layer.
   */
  const Matrix &get_bias () const
  {
    return _bias;
  }

  /**
   * @return The activation class of this layer.
   */
  const Activation &get_activation () const
  {
    return _act;
  }

  /**
   * Applies the layer on rhs.
   * @param rhs A matrix of one column.
   * @return A new matrix which is the result of applying the layer on rhs.
   */
  Matrix operator() (const Matrix &rhs) const;
};

#endif //C___PROJECT_DENSE_H
