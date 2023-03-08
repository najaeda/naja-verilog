#[=======================================================================[.rst:
FindNajaVerilog
-----------

Find the naja libraries.

``Naja::NajaVerilog``
  Naja Verilog libraries

^^^^^^^^^^^^^^^^

This module will set the following variables in your project:

``Naja_Verilog_FOUND``
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

mark_as_advanced(NAJA_VERILOG_INCLUDE_DIR NAJA_VERILOG_LIBRARY)

if(Naja_Verilog_FOUND AND NOT TARGET Naja::NajaVerilog)
  add_library(Naja::NajaVerilog UNKNOWN IMPORTED)
  set_target_properties(Naja::NajaVerilog PROPERTIES
    IMPORTED_LOCATION "${NAJA_VERILOG_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${NAJA_VERILOG_INCLUDE_DIRS}")
endif()