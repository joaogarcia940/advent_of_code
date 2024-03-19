load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "part1",
    srcs = [
        "part1.cpp",
    ],
    deps = [":utils"],
)

cc_binary(
    name = "part2",
    srcs = [
        "part2.cpp",
    ],
    deps = [":utils"],
)

cc_binary(
    name = "experiments",
    srcs = [
        "experiments.cpp",
    ],
)

cc_library(
    name = "utils",
    srcs = ["utils.h"],
    visibility = ["//visibility:public"],
    deps = ["//utils"],
)
