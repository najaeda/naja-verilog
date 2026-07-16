# SPDX-FileCopyrightText: 2023 The Naja verilog authors <https://github.com/najaeda/naja-verilog/blob/main/AUTHORS>
#
# SPDX-License-Identifier: Apache-2.0

"""Custom Bison/Flex codegen rules for the naja-verilog parser.

Community rules (`rules_bison`/`rules_flex`) were tried first, but their
`bison_action()` helper only declares the primary `{name}.cc`/`{name}.h`
outputs -- it doesn't know about `location.hh`, the auxiliary header the
`%skeleton "lalr1.cc"` + `%define api.namespace` combination in
VerilogParser.yy also emits (confirmed empirically: a direct `bison -d -v
--output=... --defines=...` run also produces `location.hh`,
`position.hh`, `stack.hh`, and a `.output` report alongside the two
declared files; `naja/src/VerilogScanner.h` directly `#include
"location.hh"`, so it has to be a real, captured Bazel output, not just
tolerated stray sandbox garbage). `position.hh`/`stack.hh`/the report
are unused by any naja-verilog source (grepped) and are left undeclared.

Runs the system-installed `bison` and `flex` via `system_tool.bzl`.
"""

def _verilog_bison_parser_impl(ctx):
    # Declared under src/ so the consuming cc_library only ever needs
    # `includes = ["src"]` -- Bazel
    # disallows `includes = ["."]` at the workspace root as a workspace-wide
    # include-everything footgun.
    out_cpp = ctx.actions.declare_file("src/" + ctx.attr.name + ".cpp")
    out_hpp = ctx.actions.declare_file("src/" + ctx.attr.name + ".hpp")
    out_location = ctx.actions.declare_file("src/location.hh")

    args = ctx.actions.args()
    args.add_all(["-d", "-v", "-Wconflicts-sr", "-Wconflicts-rr"])
    args.add("--output=" + out_cpp.path)
    args.add("--defines=" + out_hpp.path)
    args.add(ctx.file.src)

    ctx.actions.run(
        executable = ctx.executable._bison,
        arguments = [args],
        inputs = [ctx.file.src],
        tools = [ctx.executable._m4],
        outputs = [out_cpp, out_hpp, out_location],
        env = {"M4": ctx.executable._m4.path},
        mnemonic = "BisonVerilogParser",
        progress_message = "Generating Verilog parser from %s" % ctx.file.src.short_path,
    )
    return [
        DefaultInfo(files = depset([out_cpp, out_hpp, out_location])),
        OutputGroupInfo(
            cc_srcs = depset([out_cpp]),
            cc_hdrs = depset([out_hpp, out_location]),
        ),
    ]

verilog_bison_parser = rule(
    implementation = _verilog_bison_parser_impl,
    attrs = {
        "src": attr.label(mandatory = True, allow_single_file = [".yy"]),
        "_bison": attr.label(
            default = "@bison_tool//:bison",
            allow_single_file = True,
            executable = True,
            cfg = "exec",
        ),
        "_m4": attr.label(
            default = "@m4_tool//:m4",
            allow_single_file = True,
            executable = True,
            cfg = "exec",
        ),
    },
    doc = "Generates VerilogParser.{cpp,hpp} + location.hh via system bison.",
)

def _verilog_flex_scanner_impl(ctx):
    out_cpp = ctx.actions.declare_file("src/" + ctx.attr.name + ".cpp")

    args = ctx.actions.args()
    args.add("--c++")
    args.add("--outfile=" + out_cpp.path)
    args.add(ctx.file.src)

    ctx.actions.run(
        executable = ctx.executable._flex,
        arguments = [args],
        inputs = [ctx.file.src],
        outputs = [out_cpp],
        mnemonic = "FlexVerilogScanner",
        progress_message = "Generating Verilog scanner from %s" % ctx.file.src.short_path,
    )
    return [
        DefaultInfo(files = depset([out_cpp])),
        OutputGroupInfo(cc_srcs = depset([out_cpp])),
    ]

verilog_flex_scanner = rule(
    implementation = _verilog_flex_scanner_impl,
    attrs = {
        "src": attr.label(mandatory = True, allow_single_file = [".ll"]),
        "_flex": attr.label(
            default = "@flex_tool//:flex",
            allow_single_file = True,
            executable = True,
            cfg = "exec",
        ),
    },
    doc = "Generates VerilogScanner.cpp via system flex.",
)
