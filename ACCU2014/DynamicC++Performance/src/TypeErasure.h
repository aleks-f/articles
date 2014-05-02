#ifndef DynamicCPP_TypeErasure_included
#define DynamicCPP_TypeErasure_included

#include <boost/type_erasure/builtin.hpp>
#include <boost/type_erasure/operators.hpp>
#include <boost/type_erasure/any_cast.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/io/ios_state.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>

namespace mpl = boost::mpl;
using namespace boost::type_erasure;
using namespace boost::io;

// We capture the arguments by reference and require nothing
// except that each one must provide a stream insertion operator.
typedef boost::type_erasure::any<
	boost::mpl::vector<
	boost::type_erasure::typeid_<>,
	boost::type_erasure::ostreamable<>
	>,
	const boost::type_erasure::_self&
> any_printable;
typedef std::vector<any_printable> print_storage;

// print
//
// Writes values to a stream like the classic C printf function.  The
// arguments are formatted based on specifiers in the format string,
// which match the pattern:
//
// '%' [ argument-number '$' ] flags * [ width ] [ '.' precision ] [ type-code ] format-specifier
//
// Other characters in the format string are written to the stream unchanged.
// In addition the sequence, "%%" can be used to print a literal '%' character.
// Each component is explained in detail below
//
// argument-number:
//   The value must be between 1 and sizeof... T.  It indicates the
//   index of the argument to be formatted.  If no index is specified
//   the arguments will be processed sequentially.  If an index is
//   specified for one argument, then it must be specified for every argument.
//
// flags:
//   Consists of zero or more of the following:
//   '-': Left justify the argument
//   '+': Print a plus sign for positive integers
//   '0': Use leading 0's to pad instead of filling with spaces.
//   ' ': If the value doesn't begin with a sign, prepend a space
//   '#': Print 0x or 0 for hexadecimal and octal numbers.
//
// width:
//   Indicates the minimum width to print.  This can be either
//   an integer or a '*'.  an asterisk means to read the next
//   argument (which must have type int) as the width.
//
// precision:
//   For numeric arguments, indicates the number of digits to print.  For
//   strings (%s) the precision indicates the maximum number of characters
//   to print.  Longer strings will be truncated.  As with width
//   this can be either an integer or a '*'.  an asterisk means
//   to read the next argument (which must have type int) as
//   the width.  If both the width and the precision are specified
//   as '*', the width is read first.
//
// type-code:
//   This is ignored, but provided for compatibility with C printf.
//
// format-specifier:
//   Must be one of the following characters:
//   d, i, u: The argument is formatted as a decimal integer
//   o:       The argument is formatted as an octal integer
//   x, X:    The argument is formatted as a hexadecimal integer
//   p:       The argument is formatted as a pointer
//   f:       The argument is formatted as a fixed point decimal
//   e, E:    The argument is formatted in exponential notation
//   g, G:    The argument is formatted as either fixed point or using
//            scientific notation depending on its magnitude
//   c:       The argument is formatted as a character
//   s:       The argument is formatted as a string
//
template<class... T>
void print(std::ostream& os, const char * format, const T&... t)
{
	// capture the arguments
	print_storage args = { any_printable(t)... };
	// and forward to the real implementation
	print_impl(os, format, args);
}

// This overload of print with no explicit stream writes to std::cout.
template<class... T>
void print(const char * format, const T&... t)
{
	print(std::cout, format, t...);
}

#endif // DynamicCPP_TypeErasure_included