# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "control",
    deps = [
        ":controller",
    ],
)

cc_library(
    name = "controller",
    hdrs = ["Controller.h"],
    deps = [
        "@palimpsest",
    ],
    include_prefix = "vulp/control",
)

add_lint_tests(enable_clang_format_lint = True)
