#pragma once

#if !__cplusplus
    #error "c++ needed"
#endif

#include "nenviron.h"

template<class T> struct TraitName {
    static constexpr const char *const NAME = "";
};

template<class T> struct TraitType {
    static const NType TYPE = NTYPE_BLUR_STRUCT;
};

template<class T> struct TraitType<T * > {static const NType TYPE = NTYPE_BLUR_PTR;};
template<class T> struct TraitType<T **> {static const NType TYPE = NTYPE_BLUR_PTR;};

template<> struct TraitType<void    > {static const NType TYPE = NTYPE_VOID  ;};
template<> struct TraitType<bool    > {static const NType TYPE = NTYPE_BOOL  ;};
template<> struct TraitType<char    > {static const NType TYPE = NTYPE_CHAR8 ;};
template<> struct TraitType<char16_t> {static const NType TYPE = NTYPE_CHAR16;};
template<> struct TraitType<char32_t> {static const NType TYPE = NTYPE_CHAR32;};
template<> struct TraitType<int8_t  > {static const NType TYPE = NTYPE_INT8  ;};
template<> struct TraitType<int16_t > {static const NType TYPE = NTYPE_INT16 ;};
template<> struct TraitType<int32_t > {static const NType TYPE = NTYPE_INT32 ;};
template<> struct TraitType<int64_t > {static const NType TYPE = NTYPE_INT64 ;};
template<> struct TraitType<uint8_t > {static const NType TYPE = NTYPE_UINT8 ;};
template<> struct TraitType<uint16_t> {static const NType TYPE = NTYPE_UINT16;};
template<> struct TraitType<uint32_t> {static const NType TYPE = NTYPE_UINT32;};
template<> struct TraitType<uint64_t> {static const NType TYPE = NTYPE_UINT64;};
template<> struct TraitType<float   > {static const NType TYPE = NTYPE_FLOAT ;};
template<> struct TraitType<double  > {static const NType TYPE = NTYPE_DOUBLE;};

template<> struct TraitType<void     *> {static const NType TYPE = NTYPE_VOID_PTR  ;};
template<> struct TraitType<bool     *> {static const NType TYPE = NTYPE_BOOL_PTR  ;};
template<> struct TraitType<char     *> {static const NType TYPE = NTYPE_CHAR8_PTR ;};
template<> struct TraitType<char16_t *> {static const NType TYPE = NTYPE_CHAR16_PTR;};
template<> struct TraitType<char32_t *> {static const NType TYPE = NTYPE_CHAR32_PTR;};
template<> struct TraitType<int8_t   *> {static const NType TYPE = NTYPE_INT8_PTR  ;};
template<> struct TraitType<int16_t  *> {static const NType TYPE = NTYPE_INT16_PTR ;};
template<> struct TraitType<int32_t  *> {static const NType TYPE = NTYPE_INT32_PTR ;};
template<> struct TraitType<int64_t  *> {static const NType TYPE = NTYPE_INT64_PTR ;};
template<> struct TraitType<uint8_t  *> {static const NType TYPE = NTYPE_UINT8_PTR ;};
template<> struct TraitType<uint16_t *> {static const NType TYPE = NTYPE_UINT16_PTR;};
template<> struct TraitType<uint32_t *> {static const NType TYPE = NTYPE_UINT32_PTR;};
template<> struct TraitType<uint64_t *> {static const NType TYPE = NTYPE_UINT64_PTR;};
template<> struct TraitType<float    *> {static const NType TYPE = NTYPE_FLOAT_PTR ;};
template<> struct TraitType<double   *> {static const NType TYPE = NTYPE_DOUBLE_PTR;};

template<> struct TraitType<const void     *> {static const NType TYPE = NTYPE_VOID_PTR  ;};
template<> struct TraitType<const bool     *> {static const NType TYPE = NTYPE_BOOL_PTR  ;};
template<> struct TraitType<const char     *> {static const NType TYPE = NTYPE_CHAR8_PTR ;};
template<> struct TraitType<const char16_t *> {static const NType TYPE = NTYPE_CHAR16_PTR;};
template<> struct TraitType<const char32_t *> {static const NType TYPE = NTYPE_CHAR32_PTR;};
template<> struct TraitType<const int8_t   *> {static const NType TYPE = NTYPE_INT8_PTR  ;};
template<> struct TraitType<const int16_t  *> {static const NType TYPE = NTYPE_INT16_PTR ;};
template<> struct TraitType<const int32_t  *> {static const NType TYPE = NTYPE_INT32_PTR ;};
template<> struct TraitType<const int64_t  *> {static const NType TYPE = NTYPE_INT64_PTR ;};
template<> struct TraitType<const uint8_t  *> {static const NType TYPE = NTYPE_UINT8_PTR ;};
template<> struct TraitType<const uint16_t *> {static const NType TYPE = NTYPE_UINT16_PTR;};
template<> struct TraitType<const uint32_t *> {static const NType TYPE = NTYPE_UINT32_PTR;};
template<> struct TraitType<const uint64_t *> {static const NType TYPE = NTYPE_UINT64_PTR;};
template<> struct TraitType<const float    *> {static const NType TYPE = NTYPE_FLOAT_PTR ;};
template<> struct TraitType<const double   *> {static const NType TYPE = NTYPE_DOUBLE_PTR;};

#define AddTrait(name)                                          \
/**/    template<> struct TraitName<name *> {                   \
/**/        static constexpr const char *const NAME = #name;    \
/**/    };                                                      \
/**/    template<> struct TraitType<name *> {                   \
/**/        static const NType TYPE = 0;                        \
/**/    }
