#include "TypeInfo.test1.h"

IMPL_TYPE_INFO(test, W)
IMPL_TYPE_INFO(test, X, test::W)
IMPL_TYPE_INFO(test, Y, test::W)
IMPL_TYPE_INFO(test, Z, test::X, test::Y)

IMPL_IDENTIFIABLE(test, A)
IMPL_IDENTIFIABLE(test, B, test::A)
IMPL_IDENTIFIABLE(test, C, test::B)
IMPL_IDENTIFIABLE(test, D, test::X, test::Y)

IMPL_TYPE_INFO(test::T, U)
