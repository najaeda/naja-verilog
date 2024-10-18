#[=======================================================================[.rst:
# SPDX-FileCopyrightText: 2023 The Naja verilog authors <https://github.com/najaeda/naja-verilog/blob/main/AUTHORS>
#
# SPDX-License-Identifier: Apache-2.0
FindNajaVerilog
-----------

Find the naja libraries.

``NajaVerilog::NajaVerilog``
  Naja Verilog libraries

^^^^^^^^^^^^^^^^

This module will set the following variables in your project:

``NajaVerilog_FOUND``
  true if Naja Verilog headers and libraries were found
``NAJA_VERILOG_INCLUDE_DIRS``
  list of the include directories needed to use Naja Verilog
``NAJA_VERILOG_LIBRARIES``
  Naja Verilog libraries to be linked

Cache variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``NAJA_VERILOG_INCLUDE_DIR``
  the directory containing Naja Verilog headers
``NAJA_VERILOG_LIBRARY``
  path to the Naja Verilog library
``NAJA_VERILOG_LIBRARY``
  path to the Naja Verilog library
#]=======================================================================]

find_path(NAJA_VERILOG_INCLUDE_DIR NAMES VerilogConstructor.h
   HINTS
   $ENV{NAJA_INSTALL}/include
)

find_library(NAJA_VERILOG_LIBRARY NAMES naja_verilog
   HINTS
   $ENV{NAJA_INSTALL}/lib
)

set(NAJA_VERILOG_INCLUDE_DIRS ${NAJA_VERILOG_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(NajaVerilog
  REQUIRED_VARS NAJA_VERILOG_INCLUDE_DIR NAJA_VERILOG_LIBRARY)

if(NajaVerilog_FOUND AND NOT TARGET NajaVerilog::NajaVerilog)
  add_library(NajaVerilog::NajaVerilog UNKNOWN IMPORTED)
  set_target_properties(NajaVerilog::NajaVerilog PROPERTIES
    IMPORTED_LOCATION "${NAJA_VERILOG_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${NAJA_VERILOG_INCLUDE_DIRS}")
endif()

mark_as_advanced(NAJA_VERILOG_INCLUDE_DIR NAJA_VERILOG_LIBRARY)
