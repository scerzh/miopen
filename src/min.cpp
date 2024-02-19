/*******************************************************************************
 *
 * MIT License
 *
 * Copyright (c) 2023 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *******************************************************************************/

#include <miopen/min.hpp>
#include <miopen/datatype.hpp>
#include <miopen/find_solution.hpp>
#include <miopen/float_equal.hpp>
#include <miopen/kernel_cache.hpp>
#include <miopen/reduce/invoke_params.hpp>
#include <miopen/reduce/solvers.hpp>
#include <miopen/tensor.hpp>

namespace miopen {

miopenStatus_t MinForward(Handle& handle,
                          const TensorDescriptor& xDesc,
                          ConstData_t x,
                          const TensorDescriptor& yDesc,
                          Data_t y,
                          const TensorDescriptor& indiceDesc,
                          Data_t indice,
                          int32_t dim)
{
    const auto problem = reduce::ProblemDescription{xDesc, yDesc, indiceDesc, dim};

    const auto invoke_params = [&]() {
        auto tmp       = reduce::InvokeParams{};
        tmp.type       = InvokeType::Run;
        tmp.xDesc      = &xDesc;
        tmp.yDesc      = &yDesc;
        tmp.indiceDesc = &indiceDesc;
        tmp.x          = x;
        tmp.y          = y;
        tmp.indice     = indice;
        tmp.dim        = dim;
        return tmp;
    }();

    const auto algo    = AlgorithmName{"MinForward"};
    const auto solvers = solver::SolverContainer<solver::reduce::MinForward>{};

    solvers.ExecutePrimitive(handle, problem, algo, invoke_params);

    return miopenStatusSuccess;
}

} // namespace miopen
