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

#include "../driver/tensor_driver.hpp"
#include "cpu_addlayernorm.hpp"
#include "get_handle.hpp"
#include "random.hpp"
#include "tensor_holder.hpp"
#include "verify.hpp"
#include <gtest/gtest.h>
#include <miopen/addlayernorm.hpp>
#include <miopen/miopen.h>

struct AddLayerNormTestCase
{
    size_t N;
    size_t C;
    size_t D;
    size_t H;
    size_t W;
    size_t nomalized_dim;
    float eps;
    miopenNormMode_t ln_mode;
    friend std::ostream& operator<<(std::ostream& os, const AddLayerNormTestCase& tc)
    {
        return os << " N:" << tc.N << " C:" << tc.C << " D:" << tc.D << " H:" << tc.H
                  << " W:" << tc.W << " dim:" << tc.nomalized_dim << " eps:" << tc.eps
                  << " LayerNorm_mode:" << tc.ln_mode;
    }

    std::vector<size_t> GetInput()
    {
        if((N != 0) && (C != 0) && (D != 0) && (H != 0) && (W != 0))
        {
            return std::vector<size_t>({N, C, D, H, W});
        }
        else if((N != 0) && (C != 0) && (H != 0) && (W != 0))
        {
            return std::vector<size_t>({N, C, H, W});
        }
        else if((N != 0) && (C != 0) && (W != 0))
        {
            return std::vector<size_t>({N, C, W});
        }
        else if((N != 0) && (W != 0))
        {
            return std::vector<size_t>({N, W});
        }
        else
        {
            std::cout << "Error Input Tensor Lengths\n" << std::endl;
            return std::vector<size_t>({0});
        }
    }
};

std::vector<AddLayerNormTestCase> AddLayerNormTestConfigs()
{ // n c d h w nomalized_dim eps ln_mode
    // clang-format off
    return {
        { 32,   1,   32,  32,  32  , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},   // 32x32x32 based on VoxNet arch
        { 32,   1,   14,  14,  14  , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},
        { 32,  32,   14,  14,  14  , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},
        { 32,  32,   12,  12,  12  , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},
        { 32,  32,   6,   6,   6   , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},
        { 256,  1,   32,  32,  32  , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},   // 32x32x32 based on VoxNet arch
        { 256, 32,   14,  14,  14  , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},
        { 256, 32,   12,  12,  12  , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},
        { 256, 32,   6,   6,   6   , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},
        { 512,  1,   32,  32,  32  , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},   // 32x32x32 based on VoxNet arch
        { 512, 32,   14,  14,  14  , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},
        { 512, 32,   12,  12,  12  , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},
        { 512, 32,   6,   6,   6   , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},
        { 32,   2,   32,  57,  125 , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},    // Hand-gesture recognition CVPR 2015 paper High Res Net Path
        { 32,  32,   14,  25,  59  , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},
        { 32,  32,   6,   10,  27  , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},
        { 32,  32,   4,   6,   11  , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},
        { 32,  32,   2,   2,   3   , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},
        { 32,  32,   32,  28,  62  , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},    // Hand-gesture recognition CVPR 2015 paper Low Res Net Path
        { 32,  32,   14,  12,  29  , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},
        { 32,  32,   6,   4,   12  , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},
        { 32,  32,   4,   2,   2   , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},
        { 16,  32,   6,   50,  50  , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},    // Multi-view 3D convnet
        { 1,    3,   8,   240, 320 , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},     // 3D convet on video
        { 1,    3,   16,  240, 320 , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},     // 3D convet on video
        { 1,    3,   8,   128, 171 , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},     // 3D convet on video
        { 1,    3,   16,  128, 171 , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},     // 3D convet on video
        { 1,    3,   8,   112, 112 , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},     // 3D convet on video
        { 1,    3,   16,  112, 112 , 4, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},     // 3D convet on video
        { 32,   1,   32,  32,  32  , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},          // 32x32x32 based on VoxNet arch
        { 32,   1,   14,  14,  14  , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},
        { 32,  32,   14,  14,  14  , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},
        { 32,  32,   12,  12,  12  , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},
        { 32,  32,   6,   6,   6   , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},
        { 256,  1,   32,  32,  32  , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},          // 32x32x32 based on VoxNet arch
        { 256, 32,   14,  14,  14  , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},
        { 256, 32,   12,  12,  12  , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},
        { 256, 32,   6,   6,   6   , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},
        { 512,  1,   32,  32,  32  , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},          // 32x32x32 based on VoxNet arch
        { 512, 32,   14,  14,  14  , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},
        { 512, 32,   12,  12,  12  , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},
        { 512, 32,   6,   6,   6   , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},
        { 32,   2,   32,  57,  125 , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},           // Hand-gesture recognition CVPR 2015 paper High Res Net Path
        { 32,  32,   14,  25,  59  , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},
        { 32,  32,   6,   10,  27  , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},
        { 32,  32,   4,   6,   11  , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},
        { 32,  32,   2,   2,   3   , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},
        { 32,  32,   32,  28,  62  , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},           // Hand-gesture recognition CVPR 2015 paper Low Res Net Path
        { 32,  32,   14,  12,  29  , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},
        { 32,  32,   6,   4,   12  , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},
        { 32,  32,   4,   2,   2   , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},
        { 16,  32,   6,   50,  50  , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},           // Multi-view 3D convnet
        { 1,   3,    8,   240, 320 , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},            // 3D convet on video
        { 1,   3,    16,  240, 320 , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},            // 3D convet on video
        { 1,   3,    8,   128, 171 , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},            // 3D convet on video
        { 1,   3,    16,  128, 171 , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},            // 3D convet on video
        { 1,   3,    8,   112, 112 , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},            // 3D convet on video
        { 1,   3,    16,  112, 112 , 4, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},            // 3D convet on video
        {32,   4,    0,   4,   256 , 1, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},
        {64,   4,    0,   4,   256 , 1, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},
        {32,   4,    0,   4,   256 , 1, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},
        {64,   4,    0,   4,   256 , 1, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},
        {32,   0,    0,   0,   256 , 1, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},
        {64,   0,    0,   0,   256 , 1, 1e-5, MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD},
        {32,   0,    0,   0,   256 , 1, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD},
        {64,   0,    0,   0,   256 , 1, 1e-5, MIOPEN_WEIGHT_BIAS_FUSED_ADD}
      };
    // clang-format on
}

template <typename T = float>
struct AddLayerNormTest : public ::testing::TestWithParam<AddLayerNormTestCase>
{
protected:
    void SetUp() override
    {
        auto&& handle       = get_handle();
        addlayernorm_config = GetParam();
        auto gen_value      = [](auto...) { return prng::gen_descreet_uniform_sign<T>(1e-2, 100); };

        nomalized_dim = addlayernorm_config.nomalized_dim;
        eps           = addlayernorm_config.eps;
        ln_mode       = addlayernorm_config.ln_mode;

        auto in_dim = addlayernorm_config.GetInput();

        x  = tensor<T>{in_dim}.generate(gen_value);
        x2 = tensor<T>{in_dim}.generate(gen_value);

        std::vector<size_t> inner_dim;
        if(nomalized_dim == in_dim.size())
            inner_dim = {1};
        else
            inner_dim = {in_dim.begin() + nomalized_dim, in_dim.end()};

        if(ln_mode == MIOPEN_ELEMENTWISE_AFFINE_FUSED_ADD)
        {
            auto gen_one  = [&](auto...) { return 1; };
            auto gen_zero = [&](auto...) { return 0; };
            weight        = tensor<T>{inner_dim}.generate(gen_one);
            bias          = tensor<T>{inner_dim}.generate(gen_zero);
        }
        else
        {
            weight = tensor<T>{inner_dim}.generate(gen_value);
            bias   = tensor<T>{inner_dim}.generate(gen_value);
        }

        std::vector<size_t> outer_dim;
        if(nomalized_dim == 0)
            outer_dim = {1};
        else
            outer_dim = {in_dim.begin(), in_dim.end() - (in_dim.size() - nomalized_dim)};

        y    = tensor<T>{in_dim};
        mean = tensor<T>{outer_dim};
        rstd = tensor<T>{outer_dim};
        std::fill(y.begin(), y.end(), std::numeric_limits<T>::quiet_NaN());
        std::fill(mean.begin(), mean.end(), std::numeric_limits<T>::quiet_NaN());
        std::fill(rstd.begin(), rstd.end(), std::numeric_limits<T>::quiet_NaN());

        ref_y    = tensor<T>{in_dim};
        ref_mean = tensor<T>{outer_dim};
        ref_rstd = tensor<T>{outer_dim};
        std::fill(ref_y.begin(), ref_y.end(), std::numeric_limits<T>::quiet_NaN());
        std::fill(ref_mean.begin(), ref_mean.end(), std::numeric_limits<T>::quiet_NaN());
        std::fill(ref_rstd.begin(), ref_rstd.end(), std::numeric_limits<T>::quiet_NaN());

        x_dev      = handle.Write(x.data);
        x2_dev     = handle.Write(x2.data);
        weight_dev = handle.Write(weight.data);
        bias_dev   = handle.Write(bias.data);
        y_dev      = handle.Write(y.data);
        mean_dev   = handle.Write(mean.data);
        rstd_dev   = handle.Write(rstd.data);
    }
    void RunTest()
    {
        auto&& handle = get_handle();

        cpu_addlayernorm_forward<T>(
            x, x2, weight, bias, ref_y, ref_mean, ref_rstd, eps, nomalized_dim, ln_mode);
        miopenStatus_t status;

        status = miopen::AddLayerNormForward(handle,
                                             x.desc,
                                             x_dev.get(),
                                             x2.desc,
                                             x2_dev.get(),
                                             weight.desc,
                                             weight_dev.get(),
                                             bias.desc,
                                             bias_dev.get(),
                                             y.desc,
                                             y_dev.get(),
                                             mean.desc,
                                             mean_dev.get(),
                                             rstd.desc,
                                             rstd_dev.get(),
                                             ln_mode,
                                             eps,
                                             nomalized_dim);
        EXPECT_EQ(status, miopenStatusSuccess);

        y.data    = handle.Read<T>(y_dev, y.data.size());
        mean.data = handle.Read<T>(mean_dev, mean.data.size());
        rstd.data = handle.Read<T>(rstd_dev, rstd.data.size());
    }

    void Verify()
    {
        // Computation error of fp16 is ~2^13 (=8192) bigger than
        // the one of fp32 because mantissa is shorter by 13 bits.
        auto threshold = std::is_same<T, float>::value ? 1.5e-5 : 8.2e-2;

        // bf16 mantissa has 7 bits, by 3 bits shorter than fp16.
        if(std::is_same<T, bfloat16>::value)
            threshold *= 8.0;

        auto error = miopen::rms_range(ref_y, y);

        EXPECT_TRUE(miopen::range_distance(ref_y) == miopen::range_distance(y));
        EXPECT_TRUE(error < threshold * 4)
            << "Error y beyond tolerance Error:" << error << ",  Threshold x 4: " << threshold * 4;

        error = miopen::rms_range(ref_mean, mean);
        EXPECT_TRUE(miopen::range_distance(ref_mean) == miopen::range_distance(mean));
        EXPECT_TRUE(error < threshold)
            << "Error mean beyond tolerance Error:" << error << ",  Threshold: " << threshold;

        error = miopen::rms_range(ref_rstd, rstd);
        EXPECT_TRUE(miopen::range_distance(ref_rstd) == miopen::range_distance(rstd));
        EXPECT_TRUE(error < threshold * 16) << "Error rstd beyond tolerance Error:" << error
                                            << ",  Threshold x 16: " << threshold * 16;
    }
    AddLayerNormTestCase addlayernorm_config;

    tensor<T> x;
    tensor<T> x2;
    tensor<T> weight;
    tensor<T> bias;
    tensor<T> y;
    tensor<T> mean;
    tensor<T> rstd;

    tensor<T> ref_y;
    tensor<T> ref_mean;
    tensor<T> ref_rstd;

    miopen::Allocator::ManageDataPtr x_dev;
    miopen::Allocator::ManageDataPtr x2_dev;
    miopen::Allocator::ManageDataPtr weight_dev;
    miopen::Allocator::ManageDataPtr bias_dev;
    miopen::Allocator::ManageDataPtr y_dev;
    miopen::Allocator::ManageDataPtr mean_dev;
    miopen::Allocator::ManageDataPtr rstd_dev;

    size_t nomalized_dim;
    float eps;
    miopenNormMode_t ln_mode;
};
