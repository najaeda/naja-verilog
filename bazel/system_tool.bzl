# SPDX-FileCopyrightText: 2023 The Naja verilog authors <https://github.com/najaeda/naja-verilog/blob/main/AUTHORS>
#
# SPDX-License-Identifier: Apache-2.0

"""Repository rule locating a system-installed tool via PATH.

Used for `bison`, `flex`, and `m4`. This mirrors the pragmatic
system-wrapping pattern naja's own Bazel migration uses for TBB/Boost
(see internal/bazel-migration-plan.md in the naja repo) rather than
vendoring/building these toolchains from source under Bazel.
"""

def _system_tool_repository_impl(repository_ctx):
    tool = repository_ctx.attr.tool
    found = repository_ctx.which(tool)
    if not found:
        fail(("{} not found on PATH; required to build naja-verilog's " +
              "Bison/Flex parser. Is it installed? (brew install {} / " +
              "apt install {})").format(tool, tool, tool))

    repository_ctx.symlink(found, tool)
    repository_ctx.file(
        "BUILD.bazel",
        'exports_files(["{}"])\n'.format(tool),
    )

system_tool_repository = repository_rule(
    implementation = _system_tool_repository_impl,
    attrs = {
        "tool": attr.string(mandatory = True, doc = "Tool name to locate on PATH."),
    },
    local = True,
    doc = "Locates a system-installed tool (bison/flex/m4) via PATH.",
)

# Homebrew's flex keg and Xcode both ship a FlexLexer.h, and they disagree
# on the yyFlexLexer::LexerInput/LexerOutput signatures -- the angle-bracket
# `#include <FlexLexer.h>` in VerilogScanner.h silently picks up whichever
# one is first on the system include path, which can mismatch the flex
# *binary* actually used to generate the scanner and fail with "out-of-line
# definition ... does not match any declaration in 'yyFlexLexer'". This rule
# finds the FlexLexer.h that matches the `flex` binary resolved on PATH:
# sibling `../include` of its
# `prefix/bin/flex` layout (true for Homebrew's `opt/flex` symlink prefix;
# for a plain `/usr/bin/flex` this resolves to `/usr/include`, where
# distros already ship the matching header).
def _flex_headers_repository_impl(repository_ctx):
    flex = repository_ctx.which("flex")
    if not flex:
        fail("flex not found on PATH; required to build naja-verilog's Verilog scanner.")

    candidate = flex.dirname.dirname.get_child("include")
    if not candidate.get_child("FlexLexer.h").exists:
        fail(("Could not find FlexLexer.h alongside the resolved `flex` " +
              "binary ({}); expected it at {}. On Debian/Ubuntu, install " +
              "the `libfl-dev` package. On macOS with Homebrew, ensure " +
              "`$(brew --prefix flex)/bin` precedes `/usr/bin` on " +
              "PATH.").format(flex, candidate))

    repository_ctx.symlink(candidate, "include")
    repository_ctx.file(
        "BUILD.bazel",
        """\
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "headers",
    hdrs = ["include/FlexLexer.h"],
    includes = ["include"],
)
""",
    )

flex_headers_repository = repository_rule(
    implementation = _flex_headers_repository_impl,
    local = True,
    doc = "Locates the FlexLexer.h matching the system `flex` binary.",
)
