/*
 * MIT License
 *
 * Copyright (c) 2021 Chase (llxiaoyuan) - original oxorany
 * Copyright (c) 2026 Quoc Toan Dev - modifications and remake
 *
 * This software is a remake / derivative work of oxorany
 * (https://github.com/llxiaoyuan/oxorany).
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice(s) and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef QUOCTOANDEV_H
#define QUOCTOANDEV_H

#if defined(_MSVC_LANG)
#  define QTD_CPLUSPLUS _MSVC_LANG
#else
#  define QTD_CPLUSPLUS __cplusplus
#endif

#if QTD_CPLUSPLUS >= 201703L
#  define QTD_NODISCARD [[nodiscard]]
#else
#  define QTD_NODISCARD
#endif

#if _KERNEL_MODE
#  include <ntddk.h>
   typedef UINT8   _obf__uint8_t;
   typedef UINT32  _obf__uint32_t;
   typedef UINT64  _obf__uint64_t;
   typedef SIZE_T  _obf__size_t;
#else
#  include <stdint.h>
   typedef uint8_t  _obf__uint8_t;
   typedef uint32_t _obf__uint32_t;
   typedef uint64_t _obf__uint64_t;
   typedef size_t   _obf__size_t;
#endif

#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__) || \
    defined(__aarch64__) || defined(__LP64__)
#  define QTD_ENVIRONMENT64
#else
#  define QTD_ENVIRONMENT32
#endif

#ifdef _MSC_VER
#  define QTD_FORCEINLINE __forceinline
#else
#  define QTD_FORCEINLINE __attribute__((always_inline)) inline
#endif

#ifdef _DEBUG
#  define QTD_ENC(any)     (any)
#  define QTD_ENC_FLT(any) (any)
#else
#  define QTD_ENC(any) \
     _obf_qtd_simple_::qtd_simple< \
       decltype(_obf_qtd_simple_::typeofs(any)), \
       _obf_qtd_simple_::array_size(any) \
     >(any, _obf_::make_index_sequence<sizeof(decltype(any))>()).get()

#  if QTD_CPLUSPLUS >= 202002L
#    define QTD_ENC_FLT(any) \
       _obf_::_Bit_cast<decltype(any)>( \
         QTD_ENC(( \
           _obf_::integral_constant< \
             typename _obf_::type_cast<decltype(any)>::type, \
             _obf_::_Bit_cast<typename _obf_::type_cast<decltype(any)>::type>(any) \
           >::value \
         )) \
       )
#  else
#    define QTD_ENC_FLT(any) QTD_ENC(any)
#  endif
#endif

namespace _obf_ {

    template<typename T> struct type_cast { using type = T; };
    template<> struct type_cast<float>  { using type = _obf__uint32_t; };
    template<> struct type_cast<double> { using type = _obf__uint64_t; };

    template<class _Ty, _Ty _Val>
    struct integral_constant {
        static constexpr _Ty value = _Val;
        using value_type = _Ty;
        using type       = integral_constant;
        constexpr operator value_type() const noexcept { return value; }
        QTD_NODISCARD constexpr value_type operator()() const noexcept { return value; }
    };

#if QTD_CPLUSPLUS >= 202002L
    template<class _To, class _From>
    QTD_NODISCARD constexpr _To _Bit_cast(const _From& _Val) noexcept {
        return __builtin_bit_cast(_To, _Val);
    }
#endif

    template<_obf__size_t... Ints>
    struct index_sequence {
        using type       = index_sequence;
        using value_type = _obf__size_t;
        static constexpr _obf__size_t size() noexcept { return sizeof...(Ints); }
    };
    template<class S1, class S2> struct _merge_and_renumber;
    template<_obf__size_t... I1, _obf__size_t... I2>
    struct _merge_and_renumber<index_sequence<I1...>, index_sequence<I2...>>
        : index_sequence<I1..., (sizeof...(I1) + I2)...> {};
    template<_obf__size_t N>
    struct make_index_sequence
        : _merge_and_renumber<
            typename make_index_sequence<N / 2>::type,
            typename make_index_sequence<N - N / 2>::type> {};
    template<> struct make_index_sequence<0> : index_sequence<> {};
    template<> struct make_index_sequence<1> : index_sequence<0> {};

} // namespace _obf_

namespace _obf_qtd_simple_ {

    _obf__size_t& X();
    _obf__size_t& Y();
    _obf__size_t& Z();

    static constexpr _obf__size_t base_key = 
        ((_obf__size_t)__TIME__[7] - '0') +
        ((_obf__size_t)__TIME__[6] - '0') * 10 +
        ((_obf__size_t)__TIME__[4] - '0') * 60 +
        ((_obf__size_t)__TIME__[3] - '0') * 600 +
        ((_obf__size_t)__TIME__[1] - '0') * 3600 +
        ((_obf__size_t)__TIME__[0] - '0') * 36000;

    static QTD_FORCEINLINE constexpr _obf__uint8_t rol8(_obf__uint8_t v, _obf__uint8_t n) {
        n &= 7u;
        return n == 0u ? v
                       : static_cast<_obf__uint8_t>((v << n) | (v >> (8u - n)));
    }
    static QTD_FORCEINLINE constexpr _obf__uint8_t ror8(_obf__uint8_t v, _obf__uint8_t n) {
        n &= 7u;
        return n == 0u ? v
                       : static_cast<_obf__uint8_t>((v >> n) | (v << (8u - n)));
    }

    template<_obf__size_t base>
    static QTD_FORCEINLINE constexpr _obf__uint8_t subkey(_obf__size_t i) {
#ifdef QTD_ENVIRONMENT64
        _obf__size_t k = base ^ (i * static_cast<_obf__size_t>(0x9e3779b97f4a7c15ULL));
        k ^= (k >> 30);
        k *= static_cast<_obf__size_t>(0xbf58476d1ce4e5b9ULL);
        k ^= (k >> 27);
        k *= static_cast<_obf__size_t>(0x94d049bb133111ebULL);
        k ^= (k >> 31);
#else
        _obf__size_t k = base ^ (i * static_cast<_obf__size_t>(0x9e3779b9UL));
        k ^= (k >> 16);
        k *= static_cast<_obf__size_t>(0x85ebca6bUL);
        k ^= (k >> 13);
        k *= static_cast<_obf__size_t>(0xc2b2ae35UL);
        k ^= (k >> 16);
#endif
        return static_cast<_obf__uint8_t>(k);
    }

    template<typename T>
    static QTD_FORCEINLINE constexpr T mba_add(T a, T b) {
        return (a ^ b) + 2 * (a & b);
    }
    template<typename T>
    static QTD_FORCEINLINE constexpr T mba_sub(T a, T b) {
        return (a ^ ~b) + 2 * (a & ~b) + 1;
    }
    template<typename T>
    static QTD_FORCEINLINE constexpr T mba_xor(T a, T b) {
        return (a | b) - (a & b);
    }

    template<_obf__size_t key>
    static QTD_FORCEINLINE constexpr _obf__uint8_t encrypt_byte(
        _obf__uint8_t c, _obf__size_t i)
    {
        constexpr _obf__size_t key2 = key ^ static_cast<_obf__size_t>(0x9e3779b9UL);
        const _obf__uint8_t k1  = subkey<key >(i);
        const _obf__uint8_t k2  = subkey<key2>(i);
        const _obf__uint8_t rot = mba_xor(k1, k2) & 7u;

        const _obf__uint8_t r1 = mba_xor(c, k1);
        const _obf__uint8_t r2 = static_cast<_obf__uint8_t>(mba_add(r1, k2));
        return rol8(r2, rot);
    }

    template<_obf__size_t key>
    static QTD_FORCEINLINE constexpr _obf__uint8_t decrypt_byte(
        _obf__uint8_t c, _obf__size_t i)
    {
        constexpr _obf__size_t key2 = key ^ static_cast<_obf__size_t>(0x9e3779b9UL);
        const _obf__uint8_t k1  = subkey<key >(i);
        const _obf__uint8_t k2  = subkey<key2>(i);
        const _obf__uint8_t rot = mba_xor(k1, k2) & 7u;

        const _obf__uint8_t r2 = ror8(c, rot);
        const _obf__uint8_t r1 = static_cast<_obf__uint8_t>(mba_sub(r2, k2));
        return static_cast<_obf__uint8_t>(mba_xor(r1, k1));
    }

    template<typename return_type, _obf__size_t key, _obf__size_t size>
    static QTD_FORCEINLINE const return_type decrypt(_obf__uint8_t(&buffer)[size]) {
#ifndef QTD_DISABLE_OBFUSCATION

        volatile _obf__size_t* vX = const_cast<volatile _obf__size_t*>(&X());
        volatile _obf__size_t* vY = const_cast<volatile _obf__size_t*>(&Y());
        volatile _obf__size_t* vZ = const_cast<volatile _obf__size_t*>(&Z());

        constexpr _obf__size_t ST_INIT    = 0x3F8A;
        constexpr _obf__size_t ST_LOOP    = 0x7C21;
        constexpr _obf__size_t ST_DECRYPT = 0x9D4E;
        constexpr _obf__size_t ST_STORE   = 0x1B59;
        constexpr _obf__size_t ST_EXIT    = 0xE60F;
        
        constexpr _obf__size_t ST_FAKE_A  = 0x1234;
        constexpr _obf__size_t ST_FAKE_B  = 0x5678;
        constexpr _obf__size_t ST_FAKE_C  = 0x9ABC;
        constexpr _obf__size_t ST_FAKE_D  = 0xDEF0;
        constexpr _obf__size_t ST_TRAP    = 0xDEAD;

        _obf__size_t state_key = *vZ;
        volatile _obf__size_t enc_state = ST_INIT ^ state_key;
        
        volatile _obf__size_t i = 0;
        volatile _obf__uint8_t* v_buffer = buffer;
        
        _obf__uint8_t source = 0;
        _obf__uint8_t decrypted = 0;
        
        _obf__size_t dummy_var = 0; 

        while (true) {
            _obf__size_t x_val = *vX;
            _obf__size_t y_val = *vY;
            state_key = *vZ; 
            _obf__size_t current_state = enc_state ^ state_key;

            _obf__size_t P1 = (((x_val * x_val) + x_val) % 2) == 0;
            _obf__size_t P2 = mba_xor(y_val, state_key) == ((y_val | state_key) - (y_val & state_key));
            _obf__size_t P3 = (dummy_var ^ dummy_var) == 0;

            switch (current_state) {
                case ST_INIT:
                    i = 0;
                    dummy_var = mba_xor(x_val, 0x5A5A5A5A);
                    enc_state = (P1 ? ST_LOOP : ST_FAKE_A) ^ state_key;
                    break;

                case ST_FAKE_A:
                    dummy_var = mba_add(dummy_var, mba_sub(y_val, state_key));
                    if (P3) {
                        enc_state = ST_LOOP ^ state_key;
                    } else {
                        enc_state = ST_TRAP ^ state_key;
                    }
                    break;

                case ST_LOOP:
                    if (i >= size) {
                        enc_state = ST_EXIT ^ state_key;
                    } else {
                        enc_state = (P2 ? ST_DECRYPT : ST_FAKE_B) ^ state_key;
                    }
                    break;

                case ST_FAKE_B:
                    dummy_var = mba_xor(dummy_var, i);
                    enc_state = ST_DECRYPT ^ state_key;
                    break;

                case ST_DECRYPT:
                    source = v_buffer[i];
                    if (P1) {
                        decrypted = decrypt_byte<key>(source, i);
                    } else {
                        decrypted = decrypt_byte<key>(source, mba_sub(i, static_cast<_obf__size_t>(0)));
                    }
                    enc_state = (P3 ? ST_STORE : ST_FAKE_C) ^ state_key;
                    break;

                case ST_FAKE_C:
                    dummy_var = mba_add(dummy_var, source);
                    enc_state = ST_STORE ^ state_key;
                    break;

                case ST_STORE:
                    v_buffer[i] = decrypted;
                    i = mba_add(i, static_cast<_obf__size_t>(1));
                    enc_state = (P2 ? ST_LOOP : ST_FAKE_D) ^ state_key;
                    break;

                case ST_FAKE_D:
                    dummy_var = mba_sub(dummy_var, decrypted);
                    enc_state = ST_LOOP ^ state_key;
                    break;

                case ST_EXIT:
                    return reinterpret_cast<return_type>(buffer);

                case ST_TRAP:
                default:
                    if (!P1) {
                        v_buffer[i] = mba_xor(source, decrypted);
                        enc_state = ST_LOOP ^ state_key;
                    } else {
                        enc_state = ST_TRAP ^ state_key;
                    }
                    break;
            }
        }

#else
        for (volatile _obf__size_t i = 0; i < size; i++) {
            buffer[i] = decrypt_byte<key>(buffer[i], i);
        }
        return reinterpret_cast<return_type>(buffer);
#endif
    }

    static QTD_FORCEINLINE constexpr _obf__size_t align(_obf__size_t n, _obf__size_t a) {
        return (n + a - 1) & ~(a - 1);
    }

    template<typename T, _obf__size_t size>
    static QTD_FORCEINLINE constexpr _obf__size_t array_size(const T(&)[size]) { return size; }
    template<typename T>
    static QTD_FORCEINLINE constexpr _obf__size_t array_size(T) { return 0; }

    template<typename T, _obf__size_t size>
    static inline T typeofs(const T(&)[size]);
    template<typename T>
    static inline T typeofs(T);

    template<typename any_t, _obf__size_t ary_size>
    class qtd_simple {
        static constexpr _obf__size_t size = align(ary_size * sizeof(any_t), 16);
        static constexpr _obf__size_t key  = base_key;

        alignas(0x10) _obf__uint8_t buffer[size];

    public:
        template<_obf__size_t... indices>
        QTD_FORCEINLINE constexpr qtd_simple(
            const any_t(&any)[ary_size],
            _obf_::index_sequence<indices...>
        ) : buffer{ encrypt_byte<key>(((_obf__uint8_t*)&any)[indices], indices)... } {}

        ~qtd_simple() {
            volatile _obf__uint8_t* p = buffer;
            for (_obf__size_t idx = 0; idx < size; ++idx) {
                p[idx] = 0;
            }
        }

        QTD_NODISCARD QTD_FORCEINLINE const any_t* get() {
            return decrypt<const any_t*, key>(buffer);
        }
    };

    template<typename any_t>
    class qtd_simple<any_t, 0> {
        static constexpr _obf__size_t size = align(sizeof(any_t), 16);
        static constexpr _obf__size_t key  = base_key;

        alignas(0x10) _obf__uint8_t buffer[size];

    public:
        template<_obf__size_t... indices>
        QTD_FORCEINLINE constexpr qtd_simple(
            any_t any,
            _obf_::index_sequence<indices...>
        ) : buffer{ encrypt_byte<key>(reinterpret_cast<_obf__uint8_t*>(&any)[indices], indices)... } {}

        ~qtd_simple() {
            volatile _obf__uint8_t* p = buffer;
            for (_obf__size_t idx = 0; idx < size; ++idx) {
                p[idx] = 0;
            }
        }

        QTD_NODISCARD QTD_FORCEINLINE const any_t get() {
            return *decrypt<const any_t*, key>(buffer);
        }
    };

} // namespace _obf_qtd_simple_

#endif // QUOCTOANDEV_H
