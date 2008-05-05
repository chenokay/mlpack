
librule(
    sources = ["discrete.cc", "geometry.cc", "statistics.cc"],
    headers = ["discrete.h", "geometry.h", "statistics.h", "kernel.h", "math.h"],
    deplibs = ["fastlib/base:base", "fastlib/col:col"]
    )

binrule(
    name = "math_test",
    sources = ["math_test.cc"],
    deplibs = [":math"])

