/*
   +----------------------------------------------------------------------+
   | PHP Version 7                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2016 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: zzu_softboy <zzu_softboy@163.com>                           |
   +----------------------------------------------------------------------+
*/

#ifndef PHP_KERNEL_CLI_TERMINAL_H
#define PHP_KERNEL_CLI_TERMINAL_H

#include <utility>
#include <map>

namespace php{
namespace kernel{
namespace io{

enum class TerminalColor
{
   Default,
   Black,
   Red,
   Green,
   Yellow,
   Blue,
   Magenta,
   Cyan,
   LightGray,
   DarkGray,
   LightRed,
   LightGreen,
   LightYellow,
   LightBlue,
   LightMagenta,
   LightCyan,
   White
};


}//io
}//kernel
}//php

#endif //PHP_KERNEL_CLI_TERMINAL_H