/* SPDX-License-Identifier: GPL-3.0-or-later WITH GCC-exception-3.1 */
/* Copyright © 2023-2024 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH
 *                       Matthias Kretz <m.kretz@gsi.de>
 */

#ifndef PROTOTYPE_X86_DETAIL_H_
#define PROTOTYPE_X86_DETAIL_H_

#include "detail.h"

#if _GLIBCXX_SIMD_HAVE_SSE

#pragma GCC push_options
// ensure GCC knows about the __builtin_ia32_* calls
#pragma GCC target("sse2", "sse3", "ssse3", "sse4.1", "sse4.2", "avx", "avx2", "bmi", "bmi2")
#pragma GCC pop_options

namespace std::__detail
{
  struct _MachineFlags
  {
    uint64_t _M_have_mmx : 1 = _GLIBCXX_SIMD_HAVE_MMX;

    uint64_t _M_have_sse : 1 = _GLIBCXX_SIMD_HAVE_SSE;

    uint64_t _M_have_sse2 : 1 = _GLIBCXX_SIMD_HAVE_SSE2;

    uint64_t _M_have_sse3 : 1 = _GLIBCXX_SIMD_HAVE_SSE3;

    uint64_t _M_have_ssse3 : 1 = _GLIBCXX_SIMD_HAVE_SSSE3;

    uint64_t _M_have_sse4_1 : 1 = _GLIBCXX_SIMD_HAVE_SSE4_1;

    uint64_t _M_have_sse4_2 : 1 = _GLIBCXX_SIMD_HAVE_SSE4_2;

    uint64_t _M_have_xop : 1 = _GLIBCXX_SIMD_HAVE_XOP;

    uint64_t _M_have_avx : 1 = _GLIBCXX_SIMD_HAVE_AVX;

    uint64_t _M_have_avx2 : 1 = _GLIBCXX_SIMD_HAVE_AVX2;

    uint64_t _M_have_bmi : 1 = _GLIBCXX_SIMD_HAVE_BMI;

    uint64_t _M_have_bmi2 : 1 = _GLIBCXX_SIMD_HAVE_BMI2;

    uint64_t _M_have_lzcnt : 1 = _GLIBCXX_SIMD_HAVE_LZCNT;

    uint64_t _M_have_sse4a : 1 = _GLIBCXX_SIMD_HAVE_SSE4A;

    uint64_t _M_have_fma : 1 = _GLIBCXX_SIMD_HAVE_FMA;

    uint64_t _M_have_fma4 : 1 = _GLIBCXX_SIMD_HAVE_FMA4;

    uint64_t _M_have_f16c : 1 = _GLIBCXX_SIMD_HAVE_F16C;

    uint64_t _M_have_popcnt : 1 = _GLIBCXX_SIMD_HAVE_POPCNT;

    uint64_t _M_have_avx512f : 1 = _GLIBCXX_SIMD_HAVE_AVX512F;

    uint64_t _M_have_avx512dq : 1 = _GLIBCXX_SIMD_HAVE_AVX512DQ;

    uint64_t _M_have_avx512vl : 1 = _GLIBCXX_SIMD_HAVE_AVX512VL;

    uint64_t _M_have_avx512bw : 1 = _GLIBCXX_SIMD_HAVE_AVX512BW;

    uint64_t _M_have_avx512bitalg : 1 = _GLIBCXX_SIMD_HAVE_AVX512BITALG;

    uint64_t _M_have_avx512vbmi : 1 = _GLIBCXX_SIMD_HAVE_AVX512VBMI;

    uint64_t _M_have_avx512vbmi2 : 1 = _GLIBCXX_SIMD_HAVE_AVX512VBMI2;

    uint64_t _M_have_avx512ifma : 1 = _GLIBCXX_SIMD_HAVE_AVX512IFMA;

    uint64_t _M_have_avx512cd : 1 = _GLIBCXX_SIMD_HAVE_AVX512CD;

    uint64_t _M_have_avx512vnni : 1 = _GLIBCXX_SIMD_HAVE_AVX512VNNI;

    uint64_t _M_have_avx512vpopcntdq : 1 = _GLIBCXX_SIMD_HAVE_AVX512VPOPCNTDQ;

    uint64_t _M_have_avx512vp2intersect : 1 = _GLIBCXX_SIMD_HAVE_AVX512VP2INTERSECT;

    uint64_t _M_have_avx512fp16 : 1 = _GLIBCXX_SIMD_HAVE_AVX512FP16;

    uint64_t _M_padding = 0;
  };

  static_assert(sizeof(_MachineFlags) == sizeof(uint64_t) * 2);

  template <__vectorizable _Tp>
    struct __x86_builtin_int;

  template <__vectorizable _Tp>
    using __x86_builtin_int_t = typename __x86_builtin_int<_Tp>::type;

  template <__vectorizable _Tp>
    requires(sizeof(_Tp) == 1)
    struct __x86_builtin_int<_Tp>
    { using type = char; };

  template <__vectorizable _Tp>
    requires(sizeof(_Tp) == 2)
    struct __x86_builtin_int<_Tp>
    { using type = short; };

  template <__vectorizable _Tp>
    requires(sizeof(_Tp) == 4)
    struct __x86_builtin_int<_Tp>
    { using type = int; };

  template <__vectorizable _Tp>
    requires(sizeof(_Tp) == 8)
    struct __x86_builtin_int<_Tp>
    { using type = long long; };

  /**
   * Return __x with suitable type for Intel intrinsics. If __x is smaller than a full XMM register,
   * then a zero-padded 16-Byte object will be returned.
   */
  template <__vec_builtin _TV>
    _GLIBCXX_SIMD_INTRINSIC constexpr auto
    __to_x86_intrin(_TV __x)
    {
      static_assert(sizeof(_TV) <= 64);
      using _Tp = __value_type_of<_TV>;
      using _Rp = conditional_t<is_floating_point_v<_Tp>, _Tp, long long>;
      if constexpr (sizeof(_TV) < 16)
        {
          using _Up = __make_signed_int_t<_TV>;
          __vec_builtin_type_bytes<_Up, 16> __tmp = {__builtin_bit_cast(_Up, __x)};
          return reinterpret_cast<__vec_builtin_type_bytes<_Rp, 16>>(__tmp);
        }
      else if constexpr (is_same_v<_Tp, _Rp>)
        return __x;
      else
        return reinterpret_cast<__vec_builtin_type_bytes<_Rp, sizeof(_TV)>>(__x);
    }

  _GLIBCXX_SIMD_INTRINSIC int
  __movmsk(__vec_builtin_sizeof<8, 16> auto __x) noexcept
  { return __builtin_ia32_movmskpd(reinterpret_cast<__v2double>(__x)); }

  _GLIBCXX_SIMD_INTRINSIC int
  __movmsk(__vec_builtin_sizeof<8, 32> auto __x) noexcept
  { return __builtin_ia32_movmskpd256(reinterpret_cast<__v4double>(__x)); }

  _GLIBCXX_SIMD_INTRINSIC int
  __movmsk(__vec_builtin_sizeof<4, 8> auto __x) noexcept
  {
#if defined  __x86_64__ and defined __BMI2__
    return __builtin_ia32_pext_di(__builtin_bit_cast(unsigned long long, __x),
                                  0x80808080'80808080ULL);
#else
    using _Float2 [[gnu::vector_size(8)]] = float;
    const _Float2 __tmp = __builtin_bit_cast(_Float2, __x);
    return __builtin_ia32_movmskps(__builtin_shufflevector(__tmp, _Float2(), 0, 1, 2, 3));
#endif
  }

  _GLIBCXX_SIMD_INTRINSIC int
  __movmsk(__vec_builtin_sizeof<4, 16> auto __x) noexcept
  { return __builtin_ia32_movmskps(reinterpret_cast<__v4float>(__x)); }

  _GLIBCXX_SIMD_INTRINSIC int
  __movmsk(__vec_builtin_sizeof<4, 32> auto __x) noexcept
  { return __builtin_ia32_movmskps256(reinterpret_cast<__v8float>(__x)); }

  template <__vec_builtin _TV, auto _Flags = _MachineFlags()>
    requires (sizeof(__value_type_of<_TV>) <= 2)
    _GLIBCXX_SIMD_ALWAYS_INLINE inline int
    __movmsk(_TV __x) noexcept
    {
      static_assert(__width_of<decltype(__x)> > 1);
      if constexpr (sizeof(__x) == 32)
        return __builtin_ia32_pmovmskb256(reinterpret_cast<__v32char>(__x));
      else if constexpr (sizeof(__x) == 16)
        return __builtin_ia32_pmovmskb128(reinterpret_cast<__v16char>(__x));
      else if constexpr (sizeof(__x) == 8)
        {
          using _Int2 [[gnu::vector_size(8)]] = int;
          const _Int2 __tmp = __builtin_bit_cast(_Int2, __x);
          return __builtin_ia32_pmovmskb128(
                   reinterpret_cast<__v16char>(
                     __builtin_shufflevector(__tmp, _Int2(), 0, 1, 2, 3)));
        }
      else if constexpr (sizeof(__x) == 4)
        {
          if constexpr (_Flags._M_have_bmi2)
            return __builtin_ia32_pext_si(__builtin_bit_cast(unsigned int, __x), 0x80808080u);
          using _Int1 [[gnu::vector_size(4)]] = int;
          const _Int1 __tmp = __builtin_bit_cast(_Int1, __x);
          return __builtin_ia32_pmovmskb128(
                   reinterpret_cast<__v16char>(
                     __builtin_shufflevector(__tmp, _Int1(), 0, 1, 1, 1)));
        }
      else if constexpr (sizeof(__x) == 2)
        {
          auto __bits = __builtin_bit_cast(unsigned short, __x);
          if constexpr (_Flags._M_have_bmi2)
            return __builtin_ia32_pext_si(__bits, 0x00008080u);
          else
            return ((__bits >> 7) & 1) | ((__bits & 0x8000) >> 14);
        }
      else
        __assert_unreachable<decltype(__x)>();
    }

  // calling the andnot builtins inhibits some optimizations, whereas GCC seems to be perfectly able
  // to choose andn instructions by itself without any help
#if 0 // not defined __clang__
  // overload __vec_andnot from detail.h
  template <__vec_builtin _TV>
    requires (sizeof(_TV) >= 16)
    _GLIBCXX_SIMD_INTRINSIC constexpr _TV
    __vec_andnot(_TV __a, _TV __b)
    {
      using _Tp = __value_type_of<_TV>;
      using _UV = __vec_builtin_type<__make_unsigned_int_t<_Tp>, __width_of<_TV>>;
      if (__builtin_is_constant_evaluated()
            or (__builtin_constant_p(__a) and __builtin_constant_p(__b)))
        return reinterpret_cast<_TV>(~reinterpret_cast<_UV>(__a) & reinterpret_cast<_UV>(__b));
      else
        return reinterpret_cast<_TV>([&] [[__gnu__::__always_inline__]] {
          if constexpr (sizeof(_TV) == 16 and is_same_v<_Tp, float>)
            return __builtin_ia32_andnps(__a, __b);
          else if constexpr (sizeof(_TV) == 16 and is_same_v<_Tp, double>)
            return __builtin_ia32_andnpd(__a, __b);
          else if constexpr (sizeof(_TV) == 16 and is_integral_v<_Tp>)
            return __builtin_ia32_pandn128(reinterpret_cast<__v2llong>(__a),
                                           reinterpret_cast<__v2llong>(__b));
          else if constexpr (sizeof(_TV) == 32 and is_same_v<_Tp, float>)
            return __builtin_ia32_andnps256(__a, __b);
          else if constexpr (sizeof(_TV) == 32 and is_same_v<_Tp, double>)
            return __builtin_ia32_andnpd256(__a, __b);
          else if constexpr (sizeof(_TV) == 32 and is_integral_v<_Tp> and __have_avx2)
            return __builtin_ia32_andnotsi256(reinterpret_cast<__v4llong>(__a),
                                              reinterpret_cast<__v4llong>(__b));
          else if constexpr (sizeof(_TV) == 32 and is_integral_v<_Tp>)
            return __builtin_ia32_andnpd256(reinterpret_cast<__v4double>(__a),
                                            reinterpret_cast<__v4double>(__b));
          else if constexpr (sizeof(_TV) == 64 and is_same_v<_Tp, float> and __have_avx512dq)
            return __builtin_ia32_andnps512_mask(__a, __b, __v16float{}, -1);
          else if constexpr (sizeof(_TV) == 64 and is_same_v<_Tp, double> and __have_avx512dq)
            return __builtin_ia32_andnpd512_mask(__a, __b, __v8double{}, -1);
          else if constexpr (sizeof(_TV) == 64)
            return __builtin_ia32_pandnd512_mask(
                     reinterpret_cast<__v16int>(__a), reinterpret_cast<__v16int>(__b),
                     __v16int{}, -1);
        }());
    }
#endif // not __clang__
}

#endif  // _GLIBCXX_SIMD_HAVE_SSE
#endif  // PROTOTYPE_X86_DETAIL_H_
