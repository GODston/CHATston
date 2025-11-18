# wxWidgets configuration

set(wxBUILD_SHARED OFF)

# Disable wxWidgets components you don't need
set(wxBUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(wxBUILD_TESTS OFF CACHE BOOL "" FORCE)
set(wxBUILD_DEMOS OFF CACHE BOOL "" FORCE)
set(wxBUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)

# Only build the libraries you actually use
set(wxUSE_STL ON CACHE BOOL "" FORCE)
set(wxUSE_LIBJPEG OFF CACHE BOOL "" FORCE)
set(wxUSE_LIBTIFF OFF CACHE BOOL "" FORCE)
# Keep PNG support for UI images (icons, buttons, etc)
# set(wxUSE_LIBPNG OFF CACHE BOOL "" FORCE)
set(wxUSE_ZLIB builtin CACHE STRING "" FORCE)
set(wxUSE_EXPAT builtin CACHE STRING "" FORCE)
set(wxUSE_REGEX builtin CACHE STRING "" FORCE)
