#pragma once
#ifndef BE_CTABLE_VERSION_HPP_
#define BE_CTABLE_VERSION_HPP_

#include <be/core/macros.hpp>

#define BE_CTABLE_VERSION_MAJOR 0
#define BE_CTABLE_VERSION_MINOR 1
#define BE_CTABLE_VERSION_REV 3

/*!! include('common/version', 'be::ctable') !! 6 */
/* ################# !! GENERATED CODE -- DO NOT MODIFY !! ################# */
#define BE_CTABLE_VERSION (BE_CTABLE_VERSION_MAJOR * 100000 + BE_CTABLE_VERSION_MINOR * 1000 + BE_CTABLE_VERSION_REV)
#define BE_CTABLE_VERSION_STRING "be::ctable " BE_STRINGIFY(BE_CTABLE_VERSION_MAJOR) "." BE_STRINGIFY(BE_CTABLE_VERSION_MINOR) "." BE_STRINGIFY(BE_CTABLE_VERSION_REV)

/* ######################### END OF GENERATED CODE ######################### */

#endif
