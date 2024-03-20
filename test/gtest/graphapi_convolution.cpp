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
#include <miopen/algorithm.hpp>
#include <miopen/miopen.h>
#include <miopen/graphapi/graphapi_convolution.hpp>

#include <vector>
#include <cstdint>
#include <tuple>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using GraphApiConvolutionDesctiptorTuple = std::tuple<bool,
                                                      miopenDataType_t,
                                                      miopenConvolutionMode_t,
                                                      int64_t,
                                                      std::vector<int64_t>,
                                                      std::vector<int64_t>,
                                                      std::vector<int64_t>,
                                                      std::vector<int64_t>>;

class GraphApiConvolutionDescriptor
    : public testing::TestWithParam<GraphApiConvolutionDesctiptorTuple>
{
protected:
    void SetUp() override
    {
        std::tie(attrsValid,
                 compType,
                 mode,
                 spatialDims,
                 dilations,
                 filterStrides,
                 prePaddings,
                 postPaddings) = GetParam();
    }
    miopenDataType_t compType;
    miopenConvolutionMode_t mode;
    int64_t spatialDims;
    std::vector<int64_t> dilations;
    std::vector<int64_t> filterStrides;
    std::vector<int64_t> prePaddings;
    std::vector<int64_t> postPaddings;
    bool attrsValid;
};

TEST_P(GraphApiConvolutionDescriptor, BuilderValidateAttributes)
{
    bool thrown = false;
    try
    {
        auto conv = miopen::graphapi::ConvolutionBuilder()
                        .setCompType(compType)
                        .setMode(mode)
                        .setSpatialDims(spatialDims)
                        .setDilations(dilations)
                        .setFilterStrides(filterStrides)
                        .setPrePaddings(prePaddings)
                        .setPostPaddings(postPaddings)
                        .build();
    }
    catch(...)
    {
        thrown = true;
    }
    EXPECT_NE(thrown, attrsValid) << "Builder failure";
}

TEST_P(GraphApiConvolutionDescriptor, RVBuilderMissingSetter)
{
    EXPECT_ANY_THROW({
        auto conv = miopen::graphapi::ConvolutionBuilder()
                        .setMode(mode)
                        .setSpatialDims(spatialDims)
                        .setDilations(dilations)
                        .setFilterStrides(filterStrides)
                        .setPrePaddings(prePaddings)
                        .setPostPaddings(postPaddings)
                        .build();
    }) << "Builder failure on missing "
          "graphapi::ConvolutionBuilder::setCompType() call";

    EXPECT_ANY_THROW({
        auto conv = miopen::graphapi::ConvolutionBuilder()
                        .setCompType(compType)
                        .setSpatialDims(spatialDims)
                        .setDilations(dilations)
                        .setFilterStrides(filterStrides)
                        .setPrePaddings(prePaddings)
                        .setPostPaddings(postPaddings)
                        .build();
    }) << "Builder failure on missing "
          "graphapi::ConvolutionBuilder::setMode() call";

    EXPECT_ANY_THROW({
        auto conv = miopen::graphapi::ConvolutionBuilder()
                        .setCompType(compType)
                        .setMode(mode)
                        .setDilations(dilations)
                        .setFilterStrides(filterStrides)
                        .setPrePaddings(prePaddings)
                        .setPostPaddings(postPaddings)
                        .build();
    }) << "Builder failure on missing "
          "graphapi::ConvolutionBuilder::setSpatialDims() call";

    EXPECT_ANY_THROW({
        auto conv = miopen::graphapi::ConvolutionBuilder()
                        .setCompType(compType)
                        .setMode(mode)
                        .setSpatialDims(spatialDims)
                        .setFilterStrides(filterStrides)
                        .setPrePaddings(prePaddings)
                        .setPostPaddings(postPaddings)
                        .build();
    }) << "Builder failure on missing "
          "graphapi::ConvolutionBuilder::setDilations() call";

    EXPECT_ANY_THROW({
        auto conv = miopen::graphapi::ConvolutionBuilder()
                        .setCompType(compType)
                        .setMode(mode)
                        .setSpatialDims(spatialDims)
                        .setDilations(dilations)
                        .setPrePaddings(prePaddings)
                        .setPostPaddings(postPaddings)
                        .build();
    }) << "Builder failure on missing "
          "graphapi::ConvolutionBuilder::setFilterStrides() call";

    EXPECT_ANY_THROW({
        auto conv = miopen::graphapi::ConvolutionBuilder()
                        .setCompType(compType)
                        .setMode(mode)
                        .setSpatialDims(spatialDims)
                        .setDilations(dilations)
                        .setFilterStrides(filterStrides)
                        .setPostPaddings(postPaddings)
                        .build();
    }) << "Builder failure on missing "
          "graphapi::ConvolutionBuilder::setPrePaddings() call";

    EXPECT_ANY_THROW({
        auto conv = miopen::graphapi::ConvolutionBuilder()
                        .setCompType(compType)
                        .setMode(mode)
                        .setSpatialDims(spatialDims)
                        .setDilations(dilations)
                        .setFilterStrides(filterStrides)
                        .setPrePaddings(prePaddings)
                        .build();
    }) << "Builder failure on missing "
          "graphapi::ConvolutionBuilder::setPostPaddings() call";
}

TEST_P(GraphApiConvolutionDescriptor, LVBuilderMissingSetter)
{
    EXPECT_ANY_THROW({
        miopen::graphapi::ConvolutionBuilder builder;
        auto conv = builder.setMode(mode)
                        .setSpatialDims(spatialDims)
                        .setDilations(dilations)
                        .setFilterStrides(filterStrides)
                        .setPrePaddings(prePaddings)
                        .setPostPaddings(postPaddings)
                        .build();
    }) << "Builder failure on missing "
          "graphapi::ConvolutionBuilder::setCompType() call";

    EXPECT_ANY_THROW({
        miopen::graphapi::ConvolutionBuilder builder;
        auto conv = builder.setCompType(compType)
                        .setSpatialDims(spatialDims)
                        .setDilations(dilations)
                        .setFilterStrides(filterStrides)
                        .setPrePaddings(prePaddings)
                        .setPostPaddings(postPaddings)
                        .build();
    }) << "Builder failure on missing "
          "graphapi::ConvolutionBuilder::setMode() call";

    EXPECT_ANY_THROW({
        miopen::graphapi::ConvolutionBuilder builder;
        auto conv = builder.setCompType(compType)
                        .setMode(mode)
                        .setDilations(dilations)
                        .setFilterStrides(filterStrides)
                        .setPrePaddings(prePaddings)
                        .setPostPaddings(postPaddings)
                        .build();
    }) << "Builder failure on missing "
          "graphapi::ConvolutionBuilder::setSpatialDims() call";

    EXPECT_ANY_THROW({
        miopen::graphapi::ConvolutionBuilder builder;
        auto conv = builder.setCompType(compType)
                        .setMode(mode)
                        .setSpatialDims(spatialDims)
                        .setFilterStrides(filterStrides)
                        .setPrePaddings(prePaddings)
                        .setPostPaddings(postPaddings)
                        .build();
    }) << "Builder failure on missing "
          "graphapi::ConvolutionBuilder::setDilations() call";

    EXPECT_ANY_THROW({
        miopen::graphapi::ConvolutionBuilder builder;
        auto conv = builder.setCompType(compType)
                        .setMode(mode)
                        .setSpatialDims(spatialDims)
                        .setDilations(dilations)
                        .setPrePaddings(prePaddings)
                        .setPostPaddings(postPaddings)
                        .build();
    }) << "Builder failure on missing "
          "graphapi::ConvolutionBuilder::setFilterStrides() call";

    EXPECT_ANY_THROW({
        miopen::graphapi::ConvolutionBuilder builder;
        auto conv = builder.setCompType(compType)
                        .setMode(mode)
                        .setSpatialDims(spatialDims)
                        .setDilations(dilations)
                        .setFilterStrides(filterStrides)
                        .setPostPaddings(postPaddings)
                        .build();
    }) << "Builder failure on missing "
          "graphapi::ConvolutionBuilder::setPrePaddings() call";

    EXPECT_ANY_THROW({
        miopen::graphapi::ConvolutionBuilder builder;
        auto conv = builder.setCompType(compType)
                        .setMode(mode)
                        .setSpatialDims(spatialDims)
                        .setDilations(dilations)
                        .setFilterStrides(filterStrides)
                        .setPrePaddings(prePaddings)
                        .build();
    }) << "Builder failure on missing "
          "graphapi::ConvolutionBuilder::setPostPaddings() call";
}

TEST_P(GraphApiConvolutionDescriptor, BuilderCopyValues)
{
    auto srcDilations     = dilations;
    auto srcFilterStrides = filterStrides;
    auto srcPrePaddings   = prePaddings;
    auto srcPostPaddings  = postPaddings;

    auto srcDilationsAddress     = srcDilations.data();
    auto srcFilterStridesAddress = srcFilterStrides.data();
    auto srcPrePaddingsAddress   = srcPrePaddings.data();
    auto srcPostPaddingAddress   = srcPostPaddings.data();

    bool thrown = false;
    miopen::graphapi::Convolution conv;
    try
    {
        miopen::graphapi::ConvolutionBuilder builder;
        conv = builder.setCompType(compType)
                   .setMode(mode)
                   .setSpatialDims(spatialDims)
                   .setDilations(srcDilations)
                   .setFilterStrides(srcFilterStrides)
                   .setPrePaddings(srcPrePaddings)
                   .setPostPaddings(srcPostPaddings)
                   .build();
    }
    catch(...)
    {
        thrown = true;
    }
    EXPECT_NE(thrown, attrsValid) << "graphapi::ConvolutionBuilder failure";

    if(!attrsValid)
        return;

    EXPECT_EQ(conv.getCompType(), compType)
        << "graphapi::ConvolutionBuilder::setCompType didn't set parameter correctly";
    EXPECT_EQ(conv.getMode(), mode)
        << "graphapi::ConvolutionBuilder::setMode didn't set parameter correctly";
    EXPECT_EQ(conv.getSpatialDims(), spatialDims)
        << "graphapi::ConvolutionBuilder::setSpatialDims didn't set parameter correctly";
    EXPECT_THAT(conv.getDilations(), testing::ContainerEq(dilations))
        << "graphapi::ConvolutionBuilder::setDilations didn't set parameter correctly";
    EXPECT_THAT(conv.getFilterStrides(), testing::ContainerEq(filterStrides))
        << "graphapi::ConvolutionBuilder::setFilterStrides didn't set parameter correctly";
    EXPECT_THAT(conv.getPrePaddings(), testing::ContainerEq(prePaddings))
        << "graphapi::ConvolutionBuilder::setPrePaddings didn't set parameter correctly";
    EXPECT_THAT(conv.getPostPaddings(), testing::ContainerEq(postPaddings))
        << "graphapi::ConvolutionBuilder::setPostPaddings didn't set parameter correctly";

    EXPECT_NE(conv.getDilations().data(), srcDilationsAddress)
        << "graphapi::ConvolutionBuilder::setDilations unexpectedly moved the parameter";
    EXPECT_NE(conv.getFilterStrides().data(), srcFilterStridesAddress)
        << "graphapi::ConvolutionBuilder::setFilterStrides unexpectedly moved the parameter";
    EXPECT_NE(conv.getPrePaddings().data(), srcPrePaddingsAddress)
        << "graphapi::ConvolutionBuilder::setPrePaddings unexpectedly moved the parameter";
    EXPECT_NE(conv.getPostPaddings().data(), srcPostPaddingAddress)
        << "graphapi::ConvolutionBuilder::setPostPaddings unexpectedly moved the parameter";
}

TEST_P(GraphApiConvolutionDescriptor, BuilderMoveValues)
{
    auto srcDilations     = dilations;
    auto srcFilterStrides = filterStrides;
    auto srcPrePaddings   = prePaddings;
    auto srcPostPaddings  = postPaddings;

    auto srcDilationsAddress     = srcDilations.data();
    auto srcFilterStridesAddress = srcFilterStrides.data();
    auto srcPrePaddingsAddress   = srcPrePaddings.data();
    auto srcPostPaddingAddress   = srcPostPaddings.data();

    bool thrown = false;
    miopen::graphapi::Convolution conv;
    try
    {
        conv = miopen::graphapi::ConvolutionBuilder()
                   .setCompType(compType)
                   .setMode(mode)
                   .setSpatialDims(spatialDims)
                   .setDilations(std::move(srcDilations))
                   .setFilterStrides(std::move(srcFilterStrides))
                   .setPrePaddings(std::move(srcPrePaddings))
                   .setPostPaddings(std::move(srcPostPaddings))
                   .build();
    }
    catch(...)
    {
        thrown = true;
    }
    EXPECT_NE(thrown, attrsValid) << "graphapi::ConvolutionBuilder failure";

    if(!attrsValid)
        return;

    EXPECT_EQ(conv.getCompType(), compType)
        << "graphapi::ConvolutionBuilder::setCompType didn't set the parameter correctly";
    EXPECT_EQ(conv.getMode(), mode)
        << "graphapi::ConvolutionBuilder::setMode didn't set the parameter correctly";
    EXPECT_EQ(conv.getSpatialDims(), spatialDims)
        << "graphapi::ConvolutionBuilder::setSpatialDims didn't set the parameter correctly";
    EXPECT_THAT(conv.getDilations(), testing::ContainerEq(dilations))
        << "graphapi::ConvolutionBuilder::setDilations didn't set the parameter correctly";
    EXPECT_THAT(conv.getFilterStrides(), testing::ContainerEq(filterStrides))
        << "graphapi::ConvolutionBuilder::setFilterStrides didn't set parameter correctly";
    EXPECT_THAT(conv.getPrePaddings(), testing::ContainerEq(prePaddings))
        << "graphapi::ConvolutionBuilder::setPrePaddings didn't set parameter correctly";
    EXPECT_THAT(conv.getPostPaddings(), testing::ContainerEq(postPaddings))
        << "graphapi::ConvolutionBuilder::setPostPaddings didn't set parameter correctly";

    EXPECT_EQ(conv.getDilations().data(), srcDilationsAddress)
        << "graphapi::ConvolutionBuilder::setDilations didn't move the parameter";
    EXPECT_EQ(conv.getFilterStrides().data(), srcFilterStridesAddress)
        << "graphapi::ConvolutionBuilder::setFilterStrides didn't move the parameter";
    EXPECT_EQ(conv.getPrePaddings().data(), srcPrePaddingsAddress)
        << "graphapi::ConvolutionBuilder::setPrePaddings didn't move the parameter";
    EXPECT_EQ(conv.getPostPaddings().data(), srcPostPaddingAddress)
        << "graphapi::ConvolutionBuilder::setPostPaddings didn't move the parameter";
}

TEST_P(GraphApiConvolutionDescriptor, CFunctions)
{
    // clang-format off
    // Create Desctiptor
    miopenBackendDescriptor_t descrConv;
    miopenStatus_t status = miopenBackendCreateDescriptor(MIOPEN_BACKEND_CONVOLUTION_DESCRIPTOR, &descrConv);
    ASSERT_EQ(status, miopenStatusSuccess) << "MIOPEN_BACKEND_CONVOLUTION_DESCRIPTOR wasn't created";
    ASSERT_NE(descrConv, nullptr) << "A null MIOPEN_BACKEND_CONVOLUTION_DESCRIPTOR was created";

    // Finalize before setting attributes
    status = miopenBackendFinalize(descrConv);
    if(status == miopenStatusSuccess)
    {
        miopenBackendDestroyDescriptor(descrConv);
        FAIL() << "MIOPEN_BACKEND_CONVOLUTION_DESCRIPTOR was finalized without setting attributes";
    }

    // Set compType
    bool allParamsSet = true;
    char twoBytes[2] = {0, 0};
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_COMP_TYPE, MIOPEN_TYPE_BOOLEAN, 1, &compType);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_COMP_TYPE was set with invalid type";
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_COMP_TYPE, MIOPEN_TYPE_DATA_TYPE, 2, twoBytes);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_COMP_TYPE was set with invalid element count";
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_COMP_TYPE, MIOPEN_TYPE_DATA_TYPE, 1, nullptr);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_COMP_TYPE was set with null array of elements";
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_COMP_TYPE, MIOPEN_TYPE_DATA_TYPE, 1, &compType);
    if(attrsValid) // implementation may postpone validating values to finalize()
        EXPECT_EQ(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_COMP_TYPE wasn't set";
    allParamsSet = allParamsSet && (status == miopenStatusSuccess);

    // Set mode
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_CONV_MODE, MIOPEN_TYPE_BOOLEAN, 1, &mode);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_CONV_MODE was set with invalid type";
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_CONV_MODE, MIOPEN_TYPE_CONVOLUTION_MODE, 2, twoBytes);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_CONV_MODE was set with invalid element count";
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_CONV_MODE, MIOPEN_TYPE_CONVOLUTION_MODE, 1, nullptr);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_COMP_TYPE was set with null array of elements";
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_CONV_MODE, MIOPEN_TYPE_CONVOLUTION_MODE, 1, &mode);
    if(attrsValid) // implementation may postpone validating values to finalize()
        EXPECT_EQ(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_CONV_MODE wasn't set";
    allParamsSet = allParamsSet && (status == miopenStatusSuccess);

    // Set spatialDims
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_SPATIAL_DIMS, MIOPEN_TYPE_BOOLEAN, 1, &spatialDims);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_SPATIAL_DIMS was set with invalid type";
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_SPATIAL_DIMS, MIOPEN_TYPE_INT64, 2, twoBytes);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_SPATIAL_DIMS was set with invalid element count";
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_SPATIAL_DIMS, MIOPEN_TYPE_INT64, 1, nullptr);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_COMP_TYPE was set with null array of elements";
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_SPATIAL_DIMS, MIOPEN_TYPE_INT64, 1, &spatialDims);
    if(attrsValid) // implementation may postpone validating values to finalize()
        EXPECT_EQ(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_SPATIAL_DIMS wasn't set";
    allParamsSet = allParamsSet && (status == miopenStatusSuccess);

    // Set dilations
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_DILATIONS, MIOPEN_TYPE_BOOLEAN, dilations.size(), dilations.data());
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_DILATIONS was set with invalid type";
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_DILATIONS, MIOPEN_TYPE_INT64, 0, twoBytes);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_DILATIONS was set with invalid element count";
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_DILATIONS, MIOPEN_TYPE_INT64, dilations.size(), nullptr);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_COMP_TYPE was set with null array of elements";
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_DILATIONS, MIOPEN_TYPE_INT64, dilations.size(), dilations.data());
    if(attrsValid) // implementation may postpone validating values to finalize()
        EXPECT_EQ(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_DILATIONS wasn't set";
    allParamsSet = allParamsSet && (status == miopenStatusSuccess);

    // Set filterStrides
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_FILTER_STRIDES, MIOPEN_TYPE_BOOLEAN, filterStrides.size(), filterStrides.data());
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_FILTER_STRIDES was set with invalid type";
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_FILTER_STRIDES, MIOPEN_TYPE_INT64, 0, twoBytes);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_FILTER_STRIDES was set with invalid element count";
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_FILTER_STRIDES, MIOPEN_TYPE_INT64, filterStrides.size(), nullptr);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_FILTER_STRIDES was set with null array of elements";
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_FILTER_STRIDES, MIOPEN_TYPE_INT64, filterStrides.size(), filterStrides.data());
    if(attrsValid) // implementation may postpone validating values to finalize()
        EXPECT_EQ(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_FILTER_STRIDES wasn't set";
    allParamsSet = allParamsSet && (status == miopenStatusSuccess);

    // Set prePaddings
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_PRE_PADDINGS, MIOPEN_TYPE_BOOLEAN, prePaddings.size(), prePaddings.data());
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_PRE_PADDINGS was set with invalid type";
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_PRE_PADDINGS, MIOPEN_TYPE_INT64, 0, twoBytes);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_PRE_PADDINGS was set with invalid element count";
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_PRE_PADDINGS, MIOPEN_TYPE_INT64, prePaddings.size(), nullptr);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_PRE_PADDINGS was set with null array of elements";
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_PRE_PADDINGS, MIOPEN_TYPE_INT64, prePaddings.size(), prePaddings.data());
    if(attrsValid) // implementation may postpone validating values to finalize()
        EXPECT_EQ(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_PRE_PADDINGS wasn't set";
    allParamsSet = allParamsSet && (status == miopenStatusSuccess);

    // Set postPaddings
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_POST_PADDINGS, MIOPEN_TYPE_BOOLEAN, postPaddings.size(), postPaddings.data());
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_POST_PADDINGS was set with invalid type";
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_POST_PADDINGS, MIOPEN_TYPE_INT64, 0, twoBytes);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_POST_PADDINGS was set with invalid element count";
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_POST_PADDINGS, MIOPEN_TYPE_INT64, postPaddings.size(), nullptr);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_POST_PADDINGS was set with null array of elements";
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_POST_PADDINGS, MIOPEN_TYPE_INT64, postPaddings.size(), postPaddings.data());
    if(attrsValid) // implementation may postpone validating values to finalize()
        EXPECT_EQ(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_POST_PADDINGS wasn't set";
    allParamsSet = allParamsSet && (status == miopenStatusSuccess);

    // Get attibute before finalizing
    miopenDataType_t gotCompType;
    int64_t elementCount = 0;
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_COMP_TYPE, MIOPEN_TYPE_BOOLEAN, 1, &elementCount, &gotCompType);
    if(status == miopenStatusSuccess)
    {
        miopenBackendDestroyDescriptor(descrConv);
        FAIL() << "MIOPEN_ATTR_CONVOLUTION_COMP_TYPE was retrieved before finalize()";
    }

    if(!allParamsSet && attrsValid)
    {
        miopenBackendDestroyDescriptor(descrConv);
        FAIL() << "Not all attributes of MIOPEN_BACKEND_CONVOLUTION_DESCRIPTOR were set";
    }

    // Finalize
    status = miopenBackendFinalize(descrConv);
    if(attrsValid && status != miopenStatusSuccess)
    {
        miopenBackendDestroyDescriptor(descrConv);
        FAIL() << "MIOPEN_BACKEND_CONVOLUTION_DESCRIPTOR wasn't finalized";
    }
    else if(!attrsValid)
    {
        miopenBackendDestroyDescriptor(descrConv);
        ASSERT_NE(status, miopenStatusSuccess) << "MIOPEN_BACKEND_CONVOLUTION_DESCRIPTOR was finalized on invalid attributes";
        return; // no need to continue with not finalized descriptor
    }

    // Set Attributes after finalizing
    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_COMP_TYPE, MIOPEN_TYPE_DATA_TYPE, 1, &compType);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_COMP_TYPE was set after finalize()";

    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_CONV_MODE, MIOPEN_TYPE_CONVOLUTION_MODE, 1, &mode);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_CONV_MODE was set after finalize()";

    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_SPATIAL_DIMS, MIOPEN_TYPE_INT64, 1, &spatialDims);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_SPATIAL_DIMS was set after finalize()";

    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_DILATIONS, MIOPEN_TYPE_INT64, dilations.size(), dilations.data());
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_DILATIONS was set after finalize()";

    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_FILTER_STRIDES, MIOPEN_TYPE_INT64, filterStrides.size(), filterStrides.data());
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_FILTER_STRIDES was set after finalize()";

    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_PRE_PADDINGS, MIOPEN_TYPE_INT64, prePaddings.size(), prePaddings.data());
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_PRE_PADDINGS was set after finalize()";

    status = miopenBackendSetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_POST_PADDINGS, MIOPEN_TYPE_INT64, postPaddings.size(), postPaddings.data());
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_POST_PADDINGS was set after finalize()";

    // Get compType
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_COMP_TYPE, MIOPEN_TYPE_BOOLEAN, 1, &elementCount, &gotCompType);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_COMP_TYPE was retrieved with invalid type";
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_COMP_TYPE, MIOPEN_TYPE_DATA_TYPE, 2, &elementCount, twoBytes);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_COMP_TYPE was retrieved with invalid element count";
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_COMP_TYPE, MIOPEN_TYPE_DATA_TYPE, 1, &elementCount, nullptr);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_COMP_TYPE was retrieved with null array of elements";
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_COMP_TYPE, MIOPEN_TYPE_DATA_TYPE, 1, &elementCount, &gotCompType);
    EXPECT_EQ(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_COMP_TYPE wasn't retrieved";
    if(status == miopenStatusSuccess)
        EXPECT_EQ(gotCompType, compType) << "MIOPEN_ATTR_CONVOLUTION_COMP_TYPE set and retrieved values differ";

    // Get mode
    miopenConvolutionMode_t gotMode;
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_CONV_MODE, MIOPEN_TYPE_BOOLEAN, 1, &elementCount, &gotMode);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_CONV_MODE was retrieved with invalid type";
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_CONV_MODE, MIOPEN_TYPE_CONVOLUTION_MODE, 2, &elementCount, twoBytes);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_CONV_MODE was retrieved with invalid element count";
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_CONV_MODE, MIOPEN_TYPE_CONVOLUTION_MODE, 1, &elementCount, nullptr);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_CONV_MODE was retrieved with null array of elements";
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_CONV_MODE, MIOPEN_TYPE_CONVOLUTION_MODE, 1, &elementCount, &gotMode);
    EXPECT_EQ(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_CONV_MODE wasn't retrieved";
    if(status == miopenStatusSuccess)
        EXPECT_EQ(gotMode, mode) << "MIOPEN_ATTR_CONVOLUTION_CONV_MODE set and retrieved values differ";

    // Get spatialDims
    int64_t gotSpatialDims = 0;
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_SPATIAL_DIMS, MIOPEN_TYPE_BOOLEAN, 1, &elementCount, &gotSpatialDims);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_SPATIAL_DIMS was retrieved with invalid type";
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_SPATIAL_DIMS, MIOPEN_TYPE_INT64, 2, &elementCount, twoBytes);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_SPATIAL_DIMS was retrieved with invalid element count";
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_SPATIAL_DIMS, MIOPEN_TYPE_INT64, 1, &elementCount, nullptr);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_SPATIAL_DIMS was retrieved with null array of elements";
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_SPATIAL_DIMS, MIOPEN_TYPE_INT64, 1, &elementCount, &gotSpatialDims);
    EXPECT_EQ(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_SPATIAL_DIMS wasn't retrieved";
    if(status == miopenStatusSuccess)
        EXPECT_EQ(gotSpatialDims, spatialDims) << "MIOPEN_ATTR_CONVOLUTION_SPATIAL_DIMS set and retrieved values differ";

    // Get dilations
    std::vector<int64_t> gotDilations {dilations.size(), 0};
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_DILATIONS, MIOPEN_TYPE_BOOLEAN, gotDilations.size(), &elementCount, gotDilations.data());
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_DILATIONS was retrieved with invalid type";
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_DILATIONS, MIOPEN_TYPE_INT64, 0, &elementCount, twoBytes);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_DILATIONS was retrieved with invalid element count";
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_DILATIONS, MIOPEN_TYPE_INT64, gotDilations.size(), &elementCount, nullptr);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_DILATIONS was retrieved with null array of elements";
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_DILATIONS, MIOPEN_TYPE_INT64, gotDilations.size(), &elementCount, gotDilations.data());
    EXPECT_EQ(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_DILATIONS wasn't retrieved";
    if(status == miopenStatusSuccess)
        EXPECT_THAT(gotDilations, testing::ContainerEq(dilations)) << "MIOPEN_ATTR_CONVOLUTION_DILATIONS set and retrieved values differ";

    // Get filterStrides
    std::vector<int64_t> gotFilterStrides {filterStrides.size(), 0};
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_FILTER_STRIDES, MIOPEN_TYPE_BOOLEAN, gotFilterStrides.size(), &elementCount, gotFilterStrides.data());
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_FILTER_STRIDES was retrieved with invalid type";
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_FILTER_STRIDES, MIOPEN_TYPE_INT64, 0, &elementCount, twoBytes);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_FILTER_STRIDES was retrieved with invalid element count";
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_FILTER_STRIDES, MIOPEN_TYPE_INT64, gotFilterStrides.size(), &elementCount, nullptr);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_FILTER_STRIDES was retrieved with null array of elements";
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_FILTER_STRIDES, MIOPEN_TYPE_INT64, gotFilterStrides.size(), &elementCount, gotFilterStrides.data());
    EXPECT_EQ(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_FILTER_STRIDES wasn't retrieved";
    if(status == miopenStatusSuccess)
        EXPECT_THAT(gotFilterStrides, testing::ContainerEq(filterStrides)) << "MIOPEN_ATTR_CONVOLUTION_FILTER_STRIDES set and retrieved values differ";

    // Get prePaddings
    std::vector<int64_t> gotPrePaddings {prePaddings.size(), 0};
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_PRE_PADDINGS, MIOPEN_TYPE_BOOLEAN, gotPrePaddings.size(), &elementCount, gotPrePaddings.data());
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_PRE_PADDINGS was retrieved with invalid type";
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_PRE_PADDINGS, MIOPEN_TYPE_INT64, 0, &elementCount, twoBytes);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_PRE_PADDINGS was retrieved with invalid element count";
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_PRE_PADDINGS, MIOPEN_TYPE_INT64, gotPrePaddings.size(), &elementCount, nullptr);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_PRE_PADDINGS was retrieved with null array of elements";
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_PRE_PADDINGS, MIOPEN_TYPE_INT64, gotPrePaddings.size(), &elementCount, gotPrePaddings.data());
    EXPECT_EQ(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_PRE_PADDINGS wasn't retrieved";
    if(status == miopenStatusSuccess)
        EXPECT_THAT(gotPrePaddings, testing::ContainerEq(prePaddings)) << "MIOPEN_ATTR_CONVOLUTION_PRE_PADDINGS set and retrieved values differ";

    // Get postPaddings
    std::vector<int64_t> gotPostPaddings {postPaddings.size(), 0};
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_POST_PADDINGS, MIOPEN_TYPE_BOOLEAN, gotPostPaddings.size(), &elementCount, gotPostPaddings.data());
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_POST_PADDINGS was retrieved with invalid type";
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_POST_PADDINGS, MIOPEN_TYPE_INT64, 0, &elementCount, twoBytes);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_POST_PADDINGS was retrieved with invalid element count";
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_POST_PADDINGS, MIOPEN_TYPE_INT64, gotPostPaddings.size(), &elementCount, nullptr);
    EXPECT_NE(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_POST_PADDINGS was retrieved with null array of elements";
    status = miopenBackendGetAttribute(descrConv, MIOPEN_ATTR_CONVOLUTION_POST_PADDINGS, MIOPEN_TYPE_INT64, gotPostPaddings.size(), &elementCount, gotPostPaddings.data());
    EXPECT_EQ(status, miopenStatusSuccess) << "MIOPEN_ATTR_CONVOLUTION_POST_PADDINGS wasn't retrieved";
    if(status == miopenStatusSuccess)
        EXPECT_THAT(gotPostPaddings, testing::ContainerEq(postPaddings)) << "MIOPEN_ATTR_CONVOLUTION_POST_PADDINGS set and retrieved values differ";

    // Destroy description
    status = miopenBackendDestroyDescriptor(descrConv);
    EXPECT_EQ(status, miopenStatusSuccess) << "MIOPEN_BACKEND_CONVOLUTION_DESCRIPTOR destroyed with non-success status";
    // clang-format on
}

INSTANTIATE_TEST_SUITE_P(
    GraphApiConvolutionDescriptor,
    GraphApiConvolutionDescriptor,
    testing::Values(
        GraphApiConvolutionDesctiptorTuple{
            true, miopenInt8, miopenConvolution, 2, {5, 6}, {20, 21}, {3, 4}, {1, 2}},
        GraphApiConvolutionDesctiptorTuple{
            false, miopenInt8, miopenConvolution, 3, {1, 1}, {1, 1}, {0, 0}, {0, 0}},
        GraphApiConvolutionDesctiptorTuple{
            false, miopenInt8, miopenConvolution, 2, {1, 1, 1}, {1, 1}, {0, 0}, {0, 0}},
        GraphApiConvolutionDesctiptorTuple{
            false, miopenInt8, miopenConvolution, 2, {1, 1}, {1, 1, 1}, {0, 0}, {0, 0}},
        GraphApiConvolutionDesctiptorTuple{
            false, miopenInt8, miopenConvolution, 2, {1, 1}, {1, 1}, {0, 0, 0}, {0, 0}},
        GraphApiConvolutionDesctiptorTuple{
            false, miopenInt8, miopenConvolution, 2, {1, 1}, {1, 1}, {0, 0}, {0, 0, 0}},
        GraphApiConvolutionDesctiptorTuple{
            false, miopenInt8, miopenConvolution, 2, {1, 0}, {1, 1}, {0, 0}, {0, 0}},
        GraphApiConvolutionDesctiptorTuple{
            false, miopenInt8, miopenConvolution, 2, {1, 1}, {1, 0}, {0, 0}, {0, 0}},
        GraphApiConvolutionDesctiptorTuple{
            false, miopenInt8, miopenConvolution, 2, {1, 1}, {1, 1}, {-1, 0}, {0, 0}},
        GraphApiConvolutionDesctiptorTuple{
            false, miopenInt8, miopenConvolution, 2, {1, 1}, {1, 1}, {0, 0}, {0, -1}}));

template <typename T>
class GraphApiOperationConvolution : public testing::Test
{
protected:
    using TestCase = std::tuple<bool,
                                miopen::graphapi::Convolution*,
                                miopen::graphapi::Tensor*,
                                miopen::graphapi::Tensor*,
                                miopen::graphapi::Tensor*,
                                const char*>;

    void SetUp() override
    {
        testCases = {TestCase{true, &this->convolution, &this->x, &this->y, &this->w, ""},
                     TestCase{false, nullptr, &this->x, &this->y, &this->w, "convolution"},
                     TestCase{false, &this->convolution, nullptr, &this->y, &this->w, "X tensor"},
                     TestCase{false, &this->convolution, &this->x, nullptr, &this->w, "Y tensor"},
                     TestCase{false, &this->convolution, &this->x, &this->y, nullptr, "W tensor"}};
    }

    miopen::graphapi::Convolution convolution;
    miopen::graphapi::Tensor x;
    miopen::graphapi::Tensor y;
    miopen::graphapi::Tensor w;
    double dAlpha = 1.0;
    double dBeta  = 0.0;
    float fAlpha  = 1.0f;
    float fBeta   = 0.0f;

    std::array<TestCase, 5> testCases;
};

using GraphApiOperationConvolutionClasses =
    testing::Types<miopen::graphapi::OperationConvolutionForwardBuilder,
                   miopen::graphapi::OperationConvolutionBackwardDataBuilder,
                   miopen::graphapi::OperationConvolutionBackwardFilterBuilder>;

TYPED_TEST_SUITE(GraphApiOperationConvolution, GraphApiOperationConvolutionClasses);

TYPED_TEST(GraphApiOperationConvolution, BuilderValidateAttributes)
{
    for(auto [attrsValid, convolution, x, y, w, message] : this->testCases)
    {
        if(attrsValid)
        {
            EXPECT_NO_THROW({
                auto op = TypeParam()
                              .setConvolution(convolution)
                              .setX(x)
                              .setY(y)
                              .setW(w)
                              .setAlpha(this->dAlpha)
                              .setBeta(this->dBeta)
                              .build();
            }) << "Builder didn't validate correct attributes";
        }
        else
        {
            EXPECT_ANY_THROW({
                auto op = TypeParam()
                              .setConvolution(convolution)
                              .setX(x)
                              .setY(y)
                              .setW(w)
                              .setAlpha(this->dAlpha)
                              .setBeta(this->dBeta)
                              .build();
            }) << "Builder validated incorrect "
               << message;
        }
    }
}

TYPED_TEST(GraphApiOperationConvolution, BuilderMissingSetter)
{
    for(auto [attrsValid, convolution, x, y, w, message] : this->testCases)
    {
        EXPECT_ANY_THROW({
            auto op = TypeParam()
                          .setX(&this->x)
                          .setY(&this->y)
                          .setW(&this->w)
                          .setAlpha(this->dAlpha)
                          .setBeta(this->dBeta)
                          .build();
        }) << "Builder validated attributes despite missing"
              "setConvolution() call";

        EXPECT_ANY_THROW({
            auto op = TypeParam()
                          .setConvolution(convolution)
                          .setY(y)
                          .setW(w)
                          .setAlpha(this->dAlpha)
                          .setBeta(this->dBeta)
                          .build();
        }) << "Builder validated attributes despite missing"
              "setX() call";

        EXPECT_ANY_THROW({
            auto op = TypeParam()
                          .setConvolution(convolution)
                          .setX(x)
                          .setW(w)
                          .setAlpha(this->dAlpha)
                          .setBeta(this->dBeta)
                          .build();
        }) << "Builder validated attributes despite missing"
              "setY() call";

        EXPECT_ANY_THROW({
            auto op = TypeParam()
                          .setConvolution(convolution)
                          .setX(x)
                          .setY(y)
                          .setAlpha(this->dAlpha)
                          .setBeta(this->dBeta)
                          .build();
        }) << "Builder validated attributes despite missing"
              "setW() call";

        EXPECT_ANY_THROW({
            auto op = TypeParam()
                          .setConvolution(convolution)
                          .setX(x)
                          .setY(y)
                          .setW(w)
                          .setBeta(this->dBeta)
                          .build();
        }) << "Builder validated attributes despite missing"
              "setAlpha() call";

        EXPECT_ANY_THROW({
            auto op = TypeParam()
                          .setConvolution(convolution)
                          .setX(x)
                          .setY(y)
                          .setW(w)
                          .setAlpha(this->dAlpha)
                          .build();
        }) << "Builder validated attributes despite missing"
              "setBeta() call";
    }
}
