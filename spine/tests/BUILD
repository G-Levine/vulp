# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_test(
    name = "tests",
    srcs = glob([
        "*.cpp",
        "*.h",
    ]),
    deps = [
        "//actuation:mock_interface",
        "//observation/tests:observers",
        "//observation:observer_pipeline",
        "//spine:agent_interface",
        "//spine:position_commands",
        "//spine:spine",
        "//spine:state_machine",
        "//spine:stop_commands",
        "//utils:random_string",
        "@googletest//:main",
        "@palimpsest",
    ],
)

py_test(
    name = "spine_interface_test",
    srcs = ["spine_interface_test.py"],
    deps = [
        "//spine:python",
    ],
)

add_lint_tests(enable_clang_format_lint = True)
