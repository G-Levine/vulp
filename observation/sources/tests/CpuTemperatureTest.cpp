/*
 * Copyright 2022 Stéphane Caron
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
#include "vulp/observation/sources/CpuTemperature.h"

namespace vulp::observation::sources {

TEST(CpuTemperature, WriteOnce) {
  CpuTemperature cpu_temperature;
  Dictionary observation;
  cpu_temperature.write(observation);
  double temperature = observation(cpu_temperature.prefix());
  ASSERT_GT(temperature, 0.0);
  ASSERT_LT(temperature, 100.0);
}

}  // namespace vulp::observation::sources
