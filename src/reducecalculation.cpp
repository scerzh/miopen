/*******************************************************************************
 *
 * MIT License
 *
 * Copyright (c) 2024 Advanced Micro Devices, Inc.
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

#include <miopen/datatype.hpp>
#include <miopen/find_solution.hpp>
#include <miopen/float_equal.hpp>
#include <miopen/kernel_cache.hpp>
#include <miopen/reduce/invoke_params.hpp>
#include <miopen/reduce/solvers.hpp>
#include <miopen/reducecalculation.hpp>
#include <miopen/tensor.hpp>

namespace miopen {

std::size_t GetReduceCalculationWorkspaceSize(const Handle& handle,
                                              const TensorDescriptor& xDesc,
                                              const TensorDescriptor& yDesc,
                                              int32_t dim,
                                              miopenReduceCalculationOp_t reduceCalculationOp)
{
    if(reduceCalculationOp == MIOPEN_REDUCE_CALCULATION_SUM)
    {
        auto ctx           = ExecutionContext{&handle};
        const auto problem = reduce::ProblemDescriptionCalculation{
            MIOPEN_REDUCE_CALCULATION_NOT_PROPAGATE_NAN, xDesc, yDesc, dim, reduceCalculationOp};

        const auto algo    = AlgorithmName{"SumForward"};
        const auto solvers = solver::SolverContainer<solver::reduce::SumForward>{};

        auto pair_size_vector = solvers.GetWorkspaceSizes(ctx, problem);

        return pair_size_vector.empty() ? static_cast<size_t>(-1) : pair_size_vector.front().second;
    }
    else if(reduceCalculationOp == MIOPEN_REDUCE_CALCULATION_PROD)
    {
        auto ctx           = ExecutionContext{&handle};
        const auto problem = reduce::ProblemDescriptionCalculation{
            MIOPEN_REDUCE_CALCULATION_NOT_PROPAGATE_NAN, xDesc, yDesc, dim, reduceCalculationOp};

        const auto algo    = AlgorithmName{"ProdForward"};
        const auto solvers = solver::SolverContainer<solver::reduce::ProdForward>{};

        auto pair_size_vector = solvers.GetWorkspaceSizes(ctx, problem);

        return pair_size_vector.empty() ? static_cast<size_t>(-1) : pair_size_vector.front().second;
    }

    return static_cast<size_t>(-1);
}

miopenStatus_t ReduceCalculationForward(const Handle& handle,
                                        Data_t workspace,
                                        size_t workspaceSizeInBytes,
                                        const TensorDescriptor& xDesc,
                                        ConstData_t x,
                                        const TensorDescriptor& yDesc,
                                        Data_t y,
                                        miopenReduceCalculationNanPropagation_t nanPropagation,
                                        int32_t dim,
                                        miopenReduceCalculationOp_t reduceCalculationOp)
{

    if(reduceCalculationOp == MIOPEN_REDUCE_CALCULATION_SUM)
    {
        const auto problem = reduce::ProblemDescriptionCalculation{
            nanPropagation, xDesc, yDesc, dim, reduceCalculationOp};

        const auto invoke_params = [&]() {
            auto tmp           = reduce::CalculationInvokeParams{};
            tmp.type           = InvokeType::Run;
            tmp.xDesc          = &xDesc;
            tmp.yDesc          = &yDesc;
            tmp.x              = x;
            tmp.y              = y;
            tmp.workspace      = workspace;
            tmp.workspace_size = workspaceSizeInBytes;
            tmp.nanPropagation = nanPropagation;
            tmp.dim            = dim;
            return tmp;
        }();

        const auto algo    = AlgorithmName{"SumForward"};
        const auto solvers = solver::SolverContainer<solver::reduce::SumForward>{};

        solvers.ExecutePrimitive(handle, problem, algo, invoke_params);

        return miopenStatusSuccess;
    }
    else if(reduceCalculationOp == MIOPEN_REDUCE_CALCULATION_PROD)
    {
        const auto problem = reduce::ProblemDescriptionCalculation{
            nanPropagation, xDesc, yDesc, dim, reduceCalculationOp};

        const auto invoke_params = [&]() {
            auto tmp           = reduce::CalculationInvokeParams{};
            tmp.type           = InvokeType::Run;
            tmp.xDesc          = &xDesc;
            tmp.yDesc          = &yDesc;
            tmp.x              = x;
            tmp.y              = y;
            tmp.workspace      = workspace;
            tmp.workspace_size = workspaceSizeInBytes;
            tmp.nanPropagation = nanPropagation;
            tmp.dim            = dim;
            return tmp;
        }();

        const auto algo    = AlgorithmName{"ProdForward"};
        const auto solvers = solver::SolverContainer<solver::reduce::ProdForward>{};

        solvers.ExecutePrimitive(handle, problem, algo, invoke_params);

        return miopenStatusSuccess;
    }

    return miopenStatusUnsupportedOp;
}

} // namespace miopen
