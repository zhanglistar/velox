/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "gtest/gtest.h"

#include "velox/common/base/Exceptions.h"
#include "velox/expression/EvalCtx.h"
#include "velox/vector/tests/VectorTestBase.h"

using namespace facebook::velox;
using namespace facebook::velox::exec;
using namespace facebook::velox::test;

class EvalCtxTest : public testing::Test, public VectorTestBase {
 protected:
  std::unique_ptr<core::ExecCtx> execCtx_{
      std::make_unique<core::ExecCtx>(pool_.get(), nullptr)};
};

TEST_F(EvalCtxTest, selectivityVectors) {
  EvalCtx context(execCtx_.get());
  SelectivityVector all100(100, true);
  SelectivityVector none100(100, false);

  // Not initialized, initially nullptr.
  LocalSelectivityVector local1(context);
  EXPECT_TRUE(!local1.get());

  // Specify initialization in get()
  EXPECT_EQ(all100, *local1.get(100, true));

  // The get() stays the same.
  EXPECT_EQ(all100, *local1.get());

  // Initialize from other in get().
  EXPECT_EQ(none100, *local1.get(none100));

  // Init from existing
  LocalSelectivityVector local2(context, all100);
  EXPECT_EQ(all100, *local2.get());
}
