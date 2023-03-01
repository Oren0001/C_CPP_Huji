#include "MlpNetwork.h"

#define DIGITS_NUM 10

/**
* Applies the enitre network on img
* @param img A matrix of one column.
* @return A digit struct.
*/
digit MlpNetwork::operator() (const Matrix &img) const
{
  Matrix output_vec = img;
  ActivationType act_type;

  // find the output vector.
  for (int i = 0; i < MLP_SIZE; ++i)
    {
      act_type = (i == MLP_SIZE - 1) ? SOFTMAX : RELU;
      Dense layer (_weights[i], _biases[i], act_type);
      auto activation = layer.get_activation ();
      output_vec = activation (layer (output_vec));
    }

  // find the digit.
  digit res{0, output_vec[0]};
  for (int i = 1; i < DIGITS_NUM; ++i)
    {
      if (res.probability < output_vec[i])
        {
          res.value = i;
          res.probability = output_vec[i];
        }
    }

  return res;
}