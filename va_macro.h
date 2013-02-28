// va_macro.h, 2013-02-28 21:30:00
// VA_MACRO - C preprocessor helpers to write macros with finite variadic arguments
// Copyright (c) 2013 Julien Duminil
// This file is released under the MIT License (http://opensource.org/licenses/MIT)

#ifndef __VA_MACRO_H__
#define __VA_MACRO_H__

// TOKEN_PASTING_SUBSTITUTION: Concat names after macro substitution
#define TOKEN_PASTING_SUBSTITUTION(_MACRO_1, _MACRO_2) TOKEN_PASTING_SUBSTITUTION_1(_MACRO_1, _MACRO_2)
#define TOKEN_PASTING_SUBSTITUTION_1(_MACRO_1, _MACRO_2) TOKEN_PASTING_SUBSTITUTION_2(_MACRO_1, _MACRO_2) // Additional indirection to help MSVS compiler
#define TOKEN_PASTING_SUBSTITUTION_2(_MACRO_1, _MACRO_2) _MACRO_1##_MACRO_2

// VA_ARGS_COUNT: Return the number of variadic arguments
#define VA_ARGS_COUNT(...) TOKEN_PASTING_SUBSTITUTION(VA_ARGS_COUNT_IMPL_, VA_ARGS_SKIP((__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 0)))(__VA_ARGS__)
#define VA_ARGS_COUNT_IMPL_0(...) VA_ARGS_SKIP((VA_ARGS_ADD_COMMA __VA_ARGS__ (), , , , , , , 0, 1)) // __VA_ARGS__ has no comma here!
#define VA_ARGS_COUNT_IMPL_1(...) VA_ARGS_SKIP((__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)) // __VA_ARGS__ has at least one comma here!
#define VA_ARGS_ADD_COMMA() ,
#define VA_ARGS_SKIP(_TUPLE) VA_ARGS_SKIP_IMPL _TUPLE
#define VA_ARGS_SKIP_IMPL(_SKIP_1, _SKIP_2, _SKIP_3, _SKIP_4, _SKIP_5, _SKIP_6, _SKIP_7, _SKIP_8, _KEEP, ...) _KEEP

// VA_ARGS_MACRO: Dispatch arguments to the right macro depending on the number of arguments
#define VA_ARGS_MACRO(_MACRO, ...) VA_ARGS_MACRO_IMPL(TOKEN_PASTING_SUBSTITUTION(_MACRO, VA_ARGS_COUNT(__VA_ARGS__)), (__VA_ARGS__))
#define VA_ARGS_MACRO_IMPL(_MACRO, _TUPLE) _MACRO _TUPLE

// VA_ARGS_1: Return the 1st argument of a variadic argument list
#define VA_ARGS_1(...) VA_ARGS_MACRO(VA_ARGS_1_, __VA_ARGS__)
#define VA_ARGS_1_1(_ARG_1) _ARG_1
#define VA_ARGS_1_2(_ARG_1, ...) _ARG_1
#define VA_ARGS_1_3 VA_ARGS_1_2
#define VA_ARGS_1_4 VA_ARGS_1_2
#define VA_ARGS_1_5 VA_ARGS_1_2
#define VA_ARGS_1_6 VA_ARGS_1_2
#define VA_ARGS_1_7 VA_ARGS_1_2
#define VA_ARGS_1_8 VA_ARGS_1_2

// VA_ARGS_2: Return the 2nd argument of a variadic argument list
#define VA_ARGS_2(...) VA_ARGS_MACRO(VA_ARGS_2_, __VA_ARGS__)
#define VA_ARGS_2_2(_ARG_1, _ARG_2) _ARG_2
#define VA_ARGS_2_3(_ARG_1, _ARG_2, ...) _ARG_2
#define VA_ARGS_2_4 VA_ARGS_2_3
#define VA_ARGS_2_5 VA_ARGS_2_3
#define VA_ARGS_2_6 VA_ARGS_2_3
#define VA_ARGS_2_7 VA_ARGS_2_3
#define VA_ARGS_2_8 VA_ARGS_2_3

// VA_ARGS_3: Return the 3rd argument of a variadic argument list
#define VA_ARGS_3(...) VA_ARGS_MACRO(VA_ARGS_3_, __VA_ARGS__)
#define VA_ARGS_3_3(_ARG_1, _ARG_2, _ARG_3) _ARG_3
#define VA_ARGS_3_4(_ARG_1, _ARG_2, _ARG_3, ...) _ARG_3
#define VA_ARGS_3_5 VA_ARGS_3_4
#define VA_ARGS_3_6 VA_ARGS_3_4
#define VA_ARGS_3_7 VA_ARGS_3_4
#define VA_ARGS_3_8 VA_ARGS_3_4

// VA_ARGS_4: Return the 4th argument of a variadic argument list
#define VA_ARGS_4(...) VA_ARGS_MACRO(VA_ARGS_4_, __VA_ARGS__)
#define VA_ARGS_4_4(_ARG_1, _ARG_2, _ARG_3, _ARG_4) _ARG_4
#define VA_ARGS_4_5(_ARG_1, _ARG_2, _ARG_3, _ARG_4, ...) _ARG_4
#define VA_ARGS_4_6 VA_ARGS_4_5
#define VA_ARGS_4_7 VA_ARGS_4_5
#define VA_ARGS_4_8 VA_ARGS_4_5

// VA_ARGS_5: Return the 5th argument of a variadic argument list
#define VA_ARGS_5(...) VA_ARGS_MACRO(VA_ARGS_5_, __VA_ARGS__)
#define VA_ARGS_5_5(_ARG_1, _ARG_2, _ARG_3, _ARG_4, _ARG_5) _ARG_5
#define VA_ARGS_5_6(_ARG_1, _ARG_2, _ARG_3, _ARG_4, _ARG_5, ...) _ARG_5
#define VA_ARGS_5_7 VA_ARGS_5_6
#define VA_ARGS_5_8 VA_ARGS_5_6

// VA_ARGS_6: Return the 6th argument of a variadic argument list
#define VA_ARGS_6(...) VA_ARGS_MACRO(VA_ARGS_6_, __VA_ARGS__)
#define VA_ARGS_6_6(_ARG_1, _ARG_2, _ARG_3, _ARG_4, _ARG_5, _ARG_6) _ARG_6
#define VA_ARGS_6_7(_ARG_1, _ARG_2, _ARG_3, _ARG_4, _ARG_5, _ARG_6, ...) _ARG_6
#define VA_ARGS_6_8 VA_ARGS_6_7

// VA_ARGS_7: Return the 7th argument of a variadic argument list
#define VA_ARGS_7(...) VA_ARGS_MACRO(VA_ARGS_7_, __VA_ARGS__)
#define VA_ARGS_7_7(_ARG_1, _ARG_2, _ARG_3, _ARG_4, _ARG_5, _ARG_6, _ARG_7) _ARG_7
#define VA_ARGS_7_8(_ARG_1, _ARG_2, _ARG_3, _ARG_4, _ARG_5, _ARG_6, _ARG_7, ...) _ARG_7

// VA_ARGS_8: Return the 8th argument of a variadic argument list
#define VA_ARGS_8(...) VA_ARGS_MACRO(VA_ARGS_8_, __VA_ARGS__)
#define VA_ARGS_8_8(_ARG_1, _ARG_2, _ARG_3, _ARG_4, _ARG_5, _ARG_6, _ARG_7, _ARG_8) _ARG_8

#endif // __VA_MACRO_H__
