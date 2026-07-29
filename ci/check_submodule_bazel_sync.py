#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2026 The Naja verilog authors <https://github.com/najaeda/naja-verilog/blob/main/AUTHORS>
#
# SPDX-License-Identifier: Apache-2.0

"""Check that CMake and Bazel pin the same googletest release."""

import re
import subprocess
import sys
from pathlib import Path


REPO_ROOT = Path(__file__).resolve().parent.parent
MODULE_BAZEL = REPO_ROOT / "MODULE.bazel"
GOOGLETEST_PATH = "thirdparty/googletest"
GOOGLETEST_REMOTE = "https://github.com/google/googletest.git"


def run(args: list[str]) -> str:
    return subprocess.run(
        args,
        cwd=REPO_ROOT,
        check=True,
        capture_output=True,
        text=True,
    ).stdout.strip()


def googletest_module_version() -> str:
    match = re.search(
        r'bazel_dep\(\s*name\s*=\s*"googletest"\s*,\s*version\s*=\s*"([^"]+)"',
        MODULE_BAZEL.read_text(),
    )
    if not match:
        raise RuntimeError("No googletest bazel_dep version found in MODULE.bazel")
    return match.group(1)


def googletest_submodule_commit() -> str:
    entry = run(["git", "ls-tree", "HEAD", "--", GOOGLETEST_PATH])
    fields = entry.split()
    if len(fields) < 3 or fields[0] != "160000":
        raise RuntimeError(f"{GOOGLETEST_PATH} is not a git submodule")
    return fields[2]


def upstream_tag_commit(version: str) -> str:
    tag = f"refs/tags/v{version}"
    output = run(
        [
            "git",
            "ls-remote",
            "--tags",
            GOOGLETEST_REMOTE,
            tag,
            f"{tag}^{{}}",
        ]
    )
    refs = {}
    for line in output.splitlines():
        commit, ref = line.split()
        refs[ref] = commit
    commit = refs.get(f"{tag}^{{}}") or refs.get(tag)
    if not commit:
        raise RuntimeError(f"Upstream googletest tag v{version} was not found")
    return commit


def main() -> int:
    version = googletest_module_version()
    submodule_commit = googletest_submodule_commit()
    tag_commit = upstream_tag_commit(version)

    if submodule_commit != tag_commit:
        print(
            "googletest dependency pins differ:\n"
            f"  CMake submodule: {submodule_commit}\n"
            f"  Bazel version:   {version} (v{version} = {tag_commit})",
            file=sys.stderr,
        )
        return 1

    print(
        f"OK: CMake and Bazel both pin googletest v{version} "
        f"({submodule_commit})"
    )
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (RuntimeError, subprocess.CalledProcessError) as error:
        print(f"Dependency sync check failed: {error}", file=sys.stderr)
        sys.exit(1)
