#pragma once
//------------------------------------------------------------------------------
//
//   Copyright 2018-2019 Fetch.AI Limited
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
//
//------------------------------------------------------------------------------

#include "ml/ops/ops.hpp"

#include <cassert>
#include <memory>
#include <vector>

namespace fetch {
namespace ml {
namespace ops {

template <class T>
class AvgPool1D : public Ops<T>
{
public:
  using TensorType    = T;
  using SizeType      = fetch::math::SizeType;
  using DataType      = typename TensorType::Type;
  using ArrayPtrType  = std::shared_ptr<TensorType>;
  using VecTensorType = typename Ops<T>::VecTensorType;
  using SPType        = OpAvgPool1DSaveableParams<T>;
  using MyType        = AvgPool1D<TensorType>;

  AvgPool1D(SizeType const kernel_size, SizeType const stride_size)
    : kernel_size_{kernel_size}
    , stride_size_{stride_size}
  {}

  explicit AvgPool1D(SPType const &sp)
    : Ops<T>(sp)
  {
    kernel_size_ = sp.kernel_size;
    stride_size_ = sp.stride_size;
  }

  ~AvgPool1D() override = default;

  std::shared_ptr<OpsSaveableParams> GetOpSaveableParams() override
  {
    SPType sp{};
    sp.kernel_size = kernel_size_;
    sp.stride_size = stride_size_;
    return std::make_shared<SPType>(sp);
  }

  std::shared_ptr<fetch::ml::ops::Ops<TensorType>> MakeSharedCopy(
      std::shared_ptr<fetch::ml::ops::Ops<TensorType>> me) override
  {
    FETCH_UNUSED(me);
    assert(me.get() == this);

    auto copyshare = std::make_shared<MyType>(*this);  // calls default copy constructor of MyType

    return copyshare;
  }
  /**
   * Applies 1D avg pooling of kernel_size_ for each channel described here:
   * http://ais.uni-bonn.de/papers/icann2010_maxpool.pdf
   * @param inputs vector of tensor references where at:
   * inputs[0] = input_data[input_channels x input_height]
   * @param output tensor of size [input_channels=output_channels x number_of_stride_sized_steps]
   * @return: output tensor parameter
   */
  void Forward(VecTensorType const &inputs, TensorType &output) override
  {
    assert(inputs.size() == 1);
    // Input must be a 3D tensor [C x W x N]
    assert(inputs.at(0)->shape().size() == 3);
    assert(output.shape() == ComputeOutputShape(inputs));

    SizeType iter;
    DataType sum;
    auto     cnt    = static_cast<DataType>(kernel_size_);
    auto     out_it = output.begin();

    for (SizeType n_i{0}; n_i < output.shape().at(2); n_i++)  // iterate over batch
    {
      // output_channels = input_channels
      for (SizeType i{0}; i < output.shape().at(1); i++)  // Iterate over kernel stride
      {
        iter = i * stride_size_;
        for (SizeType c{0}; c < output.shape().at(0); ++c)  // Iterate over output channels
        {
          sum = static_cast<DataType>(0);

          // Get sum of value on kernel_size_ window
          for (SizeType j{0}; j < kernel_size_; j++)  // Iterate over kernel width
          {
            sum += inputs.at(0)->At(c, iter + j, n_i);
          }

          // Set average value for each [kernel_size_] window to output
          *out_it = sum / cnt;
          ++out_it;
        }
      }
    }
  }

  /**
   * Computes gradient of 1D avg pooling of kernel_size_ for each channel described here:
   * http://ais.uni-bonn.de/papers/icann2010_maxpool.pdf
   * Error signal of avg pool is passed to each node divided by kernel size
   * @param inputs vector of tensor references where at:
   * inputs[0] = input_data[input_channels x input_height]
   * @param error_signal tensor of size [output_channels=input_channels x
   * number_of_stride_sized_steps]
   * @return: output vector of tensors with back propagated error signal
   * output[0]=input_error[inputs[0].shape]
   */
  std::vector<TensorType> Backward(VecTensorType const &inputs,
                                   TensorType const &   error_signal) override
  {
    assert(inputs.size() == 1);
    assert(error_signal.shape() == ComputeOutputShape(inputs));

    TensorType return_signal{inputs.at(0)->shape()};

    auto output_shape = error_signal.shape();

    SizeType iter;
    auto     cnt = static_cast<DataType>(kernel_size_);

    auto er_it = error_signal.cbegin();

    for (SizeType n_i{0}; n_i < output_shape.at(2); n_i++)  // iterate over batch
    {
      for (SizeType i{0}; i < output_shape.at(1); i++)  // Iterate over kernel stride
      {
        iter = i * stride_size_;
        for (SizeType c{0}; c < output_shape.at(0); ++c)  // Iterate over output channels
        {

          // Add error signal divided by kernel size
          for (SizeType j{0}; j < kernel_size_; j++)  // Iterate over kernel width
          {
            return_signal(c, iter + j, n_i) += *er_it / cnt;
          }

          ++er_it;
        }
      }
    }

    return {return_signal};
  }

  std::vector<SizeType> ComputeOutputShape(VecTensorType const &inputs) const override
  {
    std::vector<SizeType> output_shape;

    // output_shape_[0]=number of output channels
    output_shape.emplace_back(inputs.at(0)->shape().at(0));
    // output_shape_[1]=number of stride_size steps over input size
    output_shape.emplace_back((inputs.at(0)->shape().at(1) - (kernel_size_ - stride_size_)) /
                              stride_size_);
    // output_shape_[2]=batch dimension
    output_shape.emplace_back(inputs.at(0)->shape().at(2));
    return output_shape;
  }

  static constexpr OpType OpCode()
  {
    return OpType::OP_AVG_POOL_1D;
  }
  static constexpr char const *DESCRIPTOR = "AvgPool1D";

private:
  SizeType kernel_size_;
  SizeType stride_size_;
};

}  // namespace ops
}  // namespace ml
}  // namespace fetch
