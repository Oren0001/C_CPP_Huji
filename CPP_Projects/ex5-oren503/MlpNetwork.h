//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Dense.h"
#include "Digit.h"

#define MLP_SIZE 4

const matrix_dims img_dims = {28, 28};
const matrix_dims weights_dims[] = {{128, 784},
                                    {64,  128},
                                    {20,  64},
                                    {10,  20}};
const matrix_dims bias_dims[] = {{128, 1},
                                 {64,  1},
                                 {20,  1},
                                 {10,  1}};

/**
 * This class will be used to arrange the different layers into a network
 * structure, and will allow to insert an input to the network
 * and receive the appropriate output.
 */
class MlpNetwork {
 private:
  /* A reference to an array of 4 different const weights. */
  const Matrix (&_weights)[MLP_SIZE];

  /* A reference to an array of 4 different const biases. */
  const Matrix (&_biases)[MLP_SIZE];

 public:
  /**
   * Initializes the MlpNetwork with the given weights and biases.
   * @param weights A reference to an array of 4 different const weights.
   * @param biases A reference to an array of 4 different const biases.
   */
  MlpNetwork (const Matrix (&weights)[MLP_SIZE],
              const Matrix (&biases)[MLP_SIZE]) :
      _weights (weights), _biases (biases)
  {}

  /**
   * Applies the enitre network on img
   * @param img A matrix of one column.
   * @return A digit struct.
   */
  digit operator() (const Matrix &img) const;
};

#endif // MLPNETWORK_H
