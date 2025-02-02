/*
 * Copyright 2023 Inria
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

#include "vulp/observation/Source.h"

#include <palimpsest/Dictionary.h>

#include <Eigen/Geometry>
#include <map>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "vulp/observation/observe_imu.h"

using palimpsest::Dictionary;

namespace vulp::observation::tests {

TEST(Source, UnknownPrefix) {
  Source source;
  ASSERT_EQ(source.prefix().rfind("unknown", 0), 0);
}

TEST(Source, WritesNothing) {
  Source source;
  Dictionary observation;
  source.write(observation);
  ASSERT_TRUE(observation.is_empty());
}

}  // namespace vulp::observation::tests
