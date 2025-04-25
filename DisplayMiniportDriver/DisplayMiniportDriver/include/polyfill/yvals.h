// yvals.h internal header

// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once
#if defined _STL70_
#include "_70_yvals.h"
#elif defined _STL100_
#include "_100_yvals.h"
#elif defined _STL110_
#include "_110_yvals.h"
#else


/* yvals.h values header for Microsoft C/C++ */
#ifndef _YVALS
#define _YVALS
#include <yvals_core.h>
#include <use_ansi.h>
#include <vcruntime.h>


#define _CRT_WARN           0
#define _CRT_ERROR          1
#define _CRT_ASSERT         2
#define _CRT_ERRCNT         3


//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// CRT DLL Export/Import Macros
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// used to annotate symbols exported from msvcp140
#ifndef _CRTIMP2
    #if defined CRTDLL2 && defined _CRTBLD
        #define _CRTIMP2 __declspec(dllexport)
    #else
        #define _CRTIMP2
    #endif
#endif

// used to annotate symbols exported from msvcp140_1
#ifndef _CRT_SATELLITE_1
    #if defined _BUILDING_SATELLITE_1 && defined _CRTBLD
        #define _CRT_SATELLITE_1 __declspec(dllexport)
    #else
        #define _CRT_SATELLITE_1
    #endif
#endif

// used to annotate symbols exported from msvcp140_2
#ifndef _CRT_SATELLITE_2
    #if defined _BUILDING_SATELLITE_2 && defined _CRTBLD
        #define _CRT_SATELLITE_2 __declspec(dllexport)
    #else
        #define _CRT_SATELLITE_2
    #endif
#endif

// Symbols exported from msvcp140_codecvt_ids are annotated with _CRT_SATELLITE_CODECVT_IDS, except for symbols which
// are data members of classes exported from msvcp140 that must themselves be exported from msvcp140_codecvt_ids, which
// are annotated with _CRT_SATELLITE_CODECVT_IDS_NOIMPORT.
#ifndef _CRT_SATELLITE_CODECVT_IDS
    #if defined _BUILDING_SATELLITE_CODECVT_IDS && defined _CRTBLD
        #define _CRT_SATELLITE_CODECVT_IDS __declspec(dllexport)
        #define _CRT_SATELLITE_CODECVT_IDS_NOIMPORT __declspec(dllexport)
    #elif defined(_DLL)
        #define _CRT_SATELLITE_CODECVT_IDS __declspec(dllimport)
        #define _CRT_SATELLITE_CODECVT_IDS_NOIMPORT
    #else
        #define _CRT_SATELLITE_CODECVT_IDS
        #define _CRT_SATELLITE_CODECVT_IDS_NOIMPORT
    #endif
#endif

#ifndef _CONCRTIMP
    #if defined CONCRTDLL && defined _CRTBLD
        #define _CONCRTIMP __declspec(dllexport)
    #else
        #if defined _DLL && !defined _STATIC_CPPLIB
            #define _CONCRTIMP __declspec(dllimport)
        #else
            #define _CONCRTIMP
        #endif
    #endif
#endif

#ifndef _MRTIMP2
    #if defined CRTDLL2 && defined _CRTBLD
        #define _MRTIMP2 __declspec(dllexport)
    #elif defined MRTDLL && defined _CRTBLD
        #define _MRTIMP2 _MRTIMP
    #else
        #define _MRTIMP2
    #endif
#endif

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

/* Define _CRTIMP2 */
#ifndef _CRTIMP2
#if defined(_DLL) && !defined(_STATIC_CPPLIB)
#define _CRTIMP2 __declspec(dllimport)
#else	/* ndef _DLL */
#define _CRTIMP2
#endif	/* _DLL */
#endif	/* _CRTIMP2 */

#ifdef  _MSC_VER
#pragma pack(push,8)
#endif  /* _MSC_VER */

#include <stlshared.h>

#pragma warning(disable: 4018 4114 4146 4244 4245) /* NOTE: These leak out into user code. */
#pragma warning(disable: 4663 4664 4665)           /* NOTE: These leak out into user code. */
#pragma warning(disable: 4237 4284 4290 4514)      /* NOTE: These leak out into user code. */
	   /* NAMESPACE */
#if defined(__cplusplus)
  #ifndef _STD
    #define _STD			std::
  #endif
  #ifndef _STD_BEGIN
    #define _STD_BEGIN	namespace std {
  #endif
  #ifndef _STD_END
    #define _STD_END		};
  #endif
  #ifndef _STD_USING
    #define _STD_USING
  #endif
#else
  #ifndef _STD
    #define _STD			::
  #endif
  #ifndef _STD_BEGIN
    #define _STD_BEGIN
  #endif
  #ifndef _STD_END
    #define _STD_END
  #endif
#endif /* __cplusplus */
_STD_BEGIN
		/* TYPE bool */
#if defined(__cplusplus)
	typedef bool _Bool;
#endif /* __cplusplus */
	   /* INTEGER PROPERTIES */
#define _MAX_EXP_DIG	8	/* for parsing numerics */
#define _MAX_INT_DIG	32
#define _MAX_SIG_DIG	36
#define _MAX_SIG_DIG_V2 768
		/* STDIO PROPERTIES */
#define _Filet _iobuf

#ifndef _FPOS_T_DEFINED
#define _FPOSOFF(fp)	((long)(fp))
#endif /* _FPOS_T_DEFINED */

		/* NAMING PROPERTIES */
#if defined(__cplusplus)
#define _C_LIB_DECL extern "C" {
#define _END_C_LIB_DECL }
#else
#define _C_LIB_DECL
#define _END_C_LIB_DECL
#endif /* __cplusplus */
#define _CDECL

	   /* MISCELLANEOUS MACROS */
#define _L(c)	L##c
#define _Mbstinit(x)	mbstate_t x = {0}
#define _MAX	_cpp_max
#define _MIN	_cpp_min
#if defined(_DLL) && !defined(_MANAGED)
#define _DLL_CPPLIB_STDHPP64
#endif
_STD_END
#ifdef  _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#ifdef _ITERATOR_DEBUG_LEVEL // A. _ITERATOR_DEBUG_LEVEL is already defined.

// A1. Validate _ITERATOR_DEBUG_LEVEL.
#if _ITERATOR_DEBUG_LEVEL > 2 && defined(_DEBUG)
#error _ITERATOR_DEBUG_LEVEL > 2 is not supported in debug mode.
#elif _ITERATOR_DEBUG_LEVEL > 1 && !defined(_DEBUG)
#error _ITERATOR_DEBUG_LEVEL > 1 is not supported in release mode.
#endif

// A2. Inspect _HAS_ITERATOR_DEBUGGING.
#ifdef _HAS_ITERATOR_DEBUGGING // A2i. _HAS_ITERATOR_DEBUGGING is already defined, validate it.
#if _ITERATOR_DEBUG_LEVEL == 2 && _HAS_ITERATOR_DEBUGGING != 1
#error _ITERATOR_DEBUG_LEVEL == 2 must imply _HAS_ITERATOR_DEBUGGING == 1.
#elif _ITERATOR_DEBUG_LEVEL < 2 && _HAS_ITERATOR_DEBUGGING != 0
#error _ITERATOR_DEBUG_LEVEL < 2 must imply _HAS_ITERATOR_DEBUGGING == 0.
#endif
#else // A2ii. _HAS_ITERATOR_DEBUGGING is not yet defined, derive it.
#if _ITERATOR_DEBUG_LEVEL == 2
#define _HAS_ITERATOR_DEBUGGING 1
#else
#define _HAS_ITERATOR_DEBUGGING 0
#endif
#endif // _HAS_ITERATOR_DEBUGGING

// A3. Inspect _SECURE_SCL.
#ifdef _SECURE_SCL // A3i. _SECURE_SCL is already defined, validate it.
#if _ITERATOR_DEBUG_LEVEL > 0 && _SECURE_SCL != 1
#error _ITERATOR_DEBUG_LEVEL > 0 must imply _SECURE_SCL == 1.
#elif _ITERATOR_DEBUG_LEVEL == 0 && _SECURE_SCL != 0
#error _ITERATOR_DEBUG_LEVEL == 0 must imply _SECURE_SCL == 0.
#endif
#else // A3ii. _SECURE_SCL is not yet defined, derive it.
#if _ITERATOR_DEBUG_LEVEL > 0
#define _SECURE_SCL 1
#else
#define _SECURE_SCL 0
#endif
#endif // _SECURE_SCL

#else // B. _ITERATOR_DEBUG_LEVEL is not yet defined.

// B1. Inspect _HAS_ITERATOR_DEBUGGING.
#ifdef _HAS_ITERATOR_DEBUGGING // B1i. _HAS_ITERATOR_DEBUGGING is already defined, validate it.
#if _HAS_ITERATOR_DEBUGGING > 1
#error _HAS_ITERATOR_DEBUGGING must be either 0 or 1.
#elif _HAS_ITERATOR_DEBUGGING == 1 && !defined(_DEBUG)
#error _HAS_ITERATOR_DEBUGGING == 1 is not supported in release mode.
#endif
#else // B1ii. _HAS_ITERATOR_DEBUGGING is not yet defined, default it.
#ifdef _DEBUG
#define _HAS_ITERATOR_DEBUGGING 1
#else
#define _HAS_ITERATOR_DEBUGGING 0
#endif
#endif // _HAS_ITERATOR_DEBUGGING

// B2. Inspect _SECURE_SCL.
#ifdef _SECURE_SCL // B2i. _SECURE_SCL is already defined, validate it.
#if _SECURE_SCL > 1
#error _SECURE_SCL must be either 0 or 1.
#endif
#else // B2ii. _SECURE_SCL is not yet defined, default it.
#if _HAS_ITERATOR_DEBUGGING == 1
#define _SECURE_SCL 1
#else
#define _SECURE_SCL 0
#endif
#endif // _SECURE_SCL

// B3. Derive _ITERATOR_DEBUG_LEVEL.
#if _HAS_ITERATOR_DEBUGGING
#define _ITERATOR_DEBUG_LEVEL 2
#elif _SECURE_SCL
#define _ITERATOR_DEBUG_LEVEL 1
#else
#define _ITERATOR_DEBUG_LEVEL 0
#endif

#endif // _ITERATOR_DEBUG_LEVEL

#ifndef _CONTAINER_DEBUG_LEVEL
#if _ITERATOR_DEBUG_LEVEL == 0
#define _CONTAINER_DEBUG_LEVEL 0
#else // ^^^ _ITERATOR_DEBUG_LEVEL == 0 // _ITERATOR_DEBUG_LEVEL != 0 vvv
#define _CONTAINER_DEBUG_LEVEL 1
#endif // _ITERATOR_DEBUG_LEVEL == 0
#endif // _CONTAINER_DEBUG_LEVEL


#ifndef _STL_CRT_SECURE_INVALID_PARAMETER
#ifdef _DEBUG // avoid emitting unused long strings for function names; see GH-1956
#define _STL_CRT_SECURE_INVALID_PARAMETER(expr) ::_invalid_parameter(_CRT_WIDE(#expr), L"", __FILEW__, __LINE__, 0)
#else // _DEBUG
// #define _STL_CRT_SECURE_INVALID_PARAMETER(expr) _CRT_SECURE_INVALID_PARAMETER(expr)
#define _STL_CRT_SECURE_INVALID_PARAMETER(expr) 
#endif // _DEBUG
#endif // _STL_CRT_SECURE_INVALID_PARAMETER

#define _STL_REPORT_ERROR(mesg)                  \
    do {                                         \
        /*_RPTF0(_CRT_ASSERT, mesg);*/               \
        _STL_CRT_SECURE_INVALID_PARAMETER(mesg); \
    } while (false)

#ifdef __clang__
#define _STL_VERIFY(cond, mesg)                                                            \
    _Pragma("clang diagnostic push") _Pragma("clang diagnostic ignored \"-Wassume\"") do { \
        if (cond) { /* contextually convertible to bool paranoia */                        \
        } else {                                                                           \
            _STL_REPORT_ERROR(mesg);                                                       \
        }                                                                                  \
                                                                                           \
        _Analysis_assume_(cond);                                                           \
    }                                                                                      \
    while (false)                                                                          \
    _Pragma("clang diagnostic pop")
#else // ^^^ Clang // MSVC vvv
#define _STL_VERIFY(cond, mesg)                                     \
    do {                                                            \
        if (cond) { /* contextually convertible to bool paranoia */ \
        } else {                                                    \
            _STL_REPORT_ERROR(mesg);                                \
        }                                                           \
                                                                    \
        _Analysis_assume_(cond);                                    \
    } while (false)
#endif // ^^^ MSVC ^^^

#ifdef _DEBUG
#define _STL_ASSERT(cond, mesg) _STL_VERIFY(cond, mesg)
#else // ^^^ _DEBUG ^^^ // vvv !_DEBUG vvv
#define _STL_ASSERT(cond, mesg) _Analysis_assume_(cond)
#endif // _DEBUG

#ifdef _ENABLE_STL_INTERNAL_CHECK
#define _STL_INTERNAL_CHECK(...) _STL_VERIFY(__VA_ARGS__, "STL internal check: " #__VA_ARGS__)
#else // ^^^ _ENABLE_STL_INTERNAL_CHECK ^^^ // vvv !_ENABLE_STL_INTERNAL_CHECK vvv
#define _STL_INTERNAL_CHECK(...) _Analysis_assume_(__VA_ARGS__)
#endif // _ENABLE_STL_INTERNAL_CHECK

#ifndef _ENABLE_ATOMIC_REF_ALIGNMENT_CHECK
#ifdef _DEBUG
#define _ENABLE_ATOMIC_REF_ALIGNMENT_CHECK 1
#else // ^^^ _DEBUG ^^^ // vvv !_DEBUG vvv
#define _ENABLE_ATOMIC_REF_ALIGNMENT_CHECK 0
#endif // _DEBUG
#endif // _ENABLE_ATOMIC_REF_ALIGNMENT_CHECK

#if _ENABLE_ATOMIC_REF_ALIGNMENT_CHECK
#define _ATOMIC_REF_CHECK_ALIGNMENT(cond, mesg) _STL_VERIFY(cond, mesg)
#else
#define _ATOMIC_REF_CHECK_ALIGNMENT(cond, mesg) _Analysis_assume_(cond)
#endif

#define _WARNING_MESSAGE(NUMBER, MESSAGE) __FILE__ "(" _CRT_STRINGIZE(__LINE__) "): warning " NUMBER ": " MESSAGE

#if defined(_M_CEE_PURE) && !defined(_SILENCE_CLR_PURE_DEPRECATION_WARNING)
#pragma message(_WARNING_MESSAGE("STL4001", "/clr:pure is deprecated and will be REMOVED."))
#endif

#ifndef _MRTIMP2_PURE
#ifdef _M_CEE_PURE
#define _MRTIMP2_PURE
#else
#define _MRTIMP2_PURE _MRTIMP2
#endif
#endif // _MRTIMP2_PURE

#if defined(_DLL) && !defined(_STATIC_CPPLIB) && !defined(_M_CEE_PURE)
#define _DLL_CPPLIB
#endif

#ifndef _CRTIMP2_PURE
#ifdef _M_CEE_PURE
#define _CRTIMP2_PURE
#else
#define _CRTIMP2_PURE _CRTIMP2
#endif
#endif // _CRTIMP2_PURE

#ifdef _CRTBLD
// These functions are for enabling STATIC_CPPLIB functionality
#define _cpp_stdin         (__acrt_iob_func(0))
#define _cpp_stdout        (__acrt_iob_func(1))
#define _cpp_stderr        (__acrt_iob_func(2))
#define _cpp_isleadbyte(c) (__pctype_func()[static_cast<unsigned char>(c)] & _LEADBYTE)
#endif // _CRTBLD

#ifndef _CRTIMP2_IMPORT
#if defined(CRTDLL2) && defined(_CRTBLD)
#define _CRTIMP2_IMPORT __declspec(dllexport)
#elif defined(_DLL) && !defined(_STATIC_CPPLIB)
#define _CRTIMP2_IMPORT __declspec(dllimport)
#else
#define _CRTIMP2_IMPORT
#endif
#endif // _CRTIMP2_IMPORT

#ifndef _CRTIMP2_PURE_IMPORT
#ifdef _M_CEE_PURE
#define _CRTIMP2_PURE_IMPORT
#else
#define _CRTIMP2_PURE_IMPORT _CRTIMP2_IMPORT
#endif
#endif // _CRTIMP2_PURE_IMPORT

#ifndef _CRTIMP2_PURE_IMPORT_UNLESS_CODECVT_ID_SATELLITE
#ifdef _BUILDING_SATELLITE_CODECVT_IDS
#define _CRTIMP2_PURE_IMPORT_UNLESS_CODECVT_ID_SATELLITE
#else
#define _CRTIMP2_PURE_IMPORT_UNLESS_CODECVT_ID_SATELLITE _CRTIMP2_PURE_IMPORT
#endif
#endif // _CRTIMP2_PURE_IMPORT_UNLESS_CODECVT_ID_SATELLITE

#ifndef _CRTDATA2_IMPORT
#if defined(MRTDLL) && defined(_CRTBLD)
#define _CRTDATA2_IMPORT
#else
#define _CRTDATA2_IMPORT _CRTIMP2_IMPORT
#endif
#endif // _CRTDATA2_IMPORT

#define _LOCK_LOCALE         0
#define _LOCK_MALLOC         1
#define _LOCK_STREAM         2
#define _LOCK_DEBUG          3
#define _LOCK_AT_THREAD_EXIT 4

#ifndef _STD_ATOMIC_ALWAYS_USE_CMPXCHG16B
#if _STL_WIN32_WINNT >= _STL_WIN32_WINNT_WINBLUE && defined(_WIN64)
#define _STD_ATOMIC_ALWAYS_USE_CMPXCHG16B 1
#else // ^^^ modern 64-bit // less modern or 32-bit vvv
#define _STD_ATOMIC_ALWAYS_USE_CMPXCHG16B 0
#endif // _STL_WIN32_WINNT >= _STL_WIN32_WINNT_WINBLUE && defined(_WIN64)
#endif // _STD_ATOMIC_ALWAYS_USE_CMPXCHG16B

#if _STD_ATOMIC_ALWAYS_USE_CMPXCHG16B == 0 && defined(_M_ARM64)
#error ARM64 requires _STD_ATOMIC_ALWAYS_USE_CMPXCHG16B to be 1.
#endif // _STD_ATOMIC_ALWAYS_USE_CMPXCHG16B == 0 && defined(_M_ARM64)

#if _STD_ATOMIC_ALWAYS_USE_CMPXCHG16B == 1 && !defined(_WIN64)
#error _STD_ATOMIC_ALWAYS_USE_CMPXCHG16B == 1 requires 64-bit.
#endif // _STD_ATOMIC_ALWAYS_USE_CMPXCHG16B == 1 && !defined(_WIN64)

#ifdef __cplusplus
_STD_BEGIN
enum _Uninitialized { // tag for suppressing initialization
    _Noinit
};

class _CRTIMP2_PURE_IMPORT _Lockit { // lock while object in existence -- MUST NEST
public:
#ifdef _M_CEE_PURE
    __CLR_OR_THIS_CALL _Lockit() noexcept : _Locktype(0) {
        _Lockit_ctor(this);
    }

    explicit __CLR_OR_THIS_CALL _Lockit(int _Kind) noexcept { // set the lock
        _Lockit_ctor(this, _Kind);
    }

    __CLR_OR_THIS_CALL ~_Lockit() noexcept { // clear the lock
        _Lockit_dtor(this);
    }

#else // _M_CEE_PURE
    __thiscall _Lockit() noexcept;
    explicit __thiscall _Lockit(int) noexcept; // set the lock
    __thiscall ~_Lockit() noexcept; // clear the lock
#endif // _M_CEE_PURE

    static void __cdecl _Lockit_ctor(int) noexcept;
    static void __cdecl _Lockit_dtor(int) noexcept;

private:
    static void __cdecl _Lockit_ctor(_Lockit*) noexcept;
    static void __cdecl _Lockit_ctor(_Lockit*, int) noexcept;
    static void __cdecl _Lockit_dtor(_Lockit*) noexcept;

public:
    __CLR_OR_THIS_CALL _Lockit(const _Lockit&) = delete;
    _Lockit& __CLR_OR_THIS_CALL operator=(const _Lockit&) = delete;

private:
    int _Locktype;
};

#ifdef _M_CEE
class _CRTIMP2_PURE_IMPORT _EmptyLockit { // empty lock class used for bin compat
private:
    int _Locktype;
};

#ifndef _PREPARE_CONSTRAINED_REGIONS
#ifdef _M_CEE_PURE
#define _PREPARE_CONSTRAINED_REGIONS 1
#else // _M_CEE_PURE
#define _PREPARE_CONSTRAINED_REGIONS 0
#endif // _M_CEE_PURE
#endif // _PREPARE_CONSTRAINED_REGIONS

#if _PREPARE_CONSTRAINED_REGIONS
#define _BEGIN_LOCK(_Kind)                                                                  \
    {                                                                                       \
        bool _MustReleaseLock = false;                                                      \
        int _LockKind         = _Kind;                                                      \
        System::Runtime::CompilerServices::RuntimeHelpers::PrepareConstrainedRegions();     \
        try {                                                                               \
            System::Runtime::CompilerServices::RuntimeHelpers::PrepareConstrainedRegions(); \
            try {                                                                           \
            } finally {                                                                     \
                _STD _Lockit::_Lockit_ctor(_LockKind);                                      \
                _MustReleaseLock = true;                                                    \
            }

#define _END_LOCK()                                \
    }                                              \
    finally {                                      \
        if (_MustReleaseLock) {                    \
            _STD _Lockit::_Lockit_dtor(_LockKind); \
        }                                          \
    }                                              \
    }

#else // _PREPARE_CONSTRAINED_REGIONS
#define _BEGIN_LOCK(_Kind) \
    {                      \
        _STD _Lockit _Lock(_Kind);

#define _END_LOCK() }

#endif // _PREPARE_CONSTRAINED_REGIONS

#define _BEGIN_LOCINFO(_VarName) \
    _BEGIN_LOCK(_LOCK_LOCALE)    \
    _Locinfo _VarName;

#define _END_LOCINFO() _END_LOCK()

#define _RELIABILITY_CONTRACT                                                    \
    [System::Runtime::ConstrainedExecution::ReliabilityContract(                 \
        System::Runtime::ConstrainedExecution::Consistency::WillNotCorruptState, \
        System::Runtime::ConstrainedExecution::Cer::Success)]

#else // _M_CEE
#define _BEGIN_LOCK(_Kind) \
    {                      \
        _STD _Lockit _Lock(_Kind);

#define _END_LOCK() }

#define _BEGIN_LOCINFO(_VarName) \
    {                            \
        _Locinfo _VarName;

#define _END_LOCINFO() }

#define _RELIABILITY_CONTRACT
#endif // _M_CEE

class _CRTIMP2_PURE_IMPORT _Init_locks { // initialize mutexes
public:
#ifdef _M_CEE_PURE
    __CLR_OR_THIS_CALL _Init_locks() noexcept {
        _Init_locks_ctor(this);
    }

    __CLR_OR_THIS_CALL ~_Init_locks() noexcept {
        _Init_locks_dtor(this);
    }

#else // _M_CEE_PURE
    __thiscall _Init_locks() noexcept;
    __thiscall ~_Init_locks() noexcept;
#endif // _M_CEE_PURE

private:
    static void __cdecl _Init_locks_ctor(_Init_locks*) noexcept;
    static void __cdecl _Init_locks_dtor(_Init_locks*) noexcept;
};

#if _HAS_EXCEPTIONS
#define _TRY_BEGIN try {
#define _CATCH(x) \
    }             \
    catch (x) {
#define _CATCH_ALL \
    }              \
    catch (...) {
#define _CATCH_END }

#define _RERAISE  throw
#define _THROW(x) throw x

#else // _HAS_EXCEPTIONS
#define _TRY_BEGIN \
    {              \
        if (1) {
#define _CATCH(x) \
    }             \
    else if (0) {
#define _CATCH_ALL \
    }              \
    else if (0) {
#define _CATCH_END \
    }              \
    }

#ifdef _DEBUG
#define _RAISE(...) 
#else // _DEBUG
#define _RAISE(...) 
#endif // _DEBUG

#define _RERAISE
#define _THROW(x) 
#endif // _HAS_EXCEPTIONS
_STD_END
#endif // __cplusplus

#ifndef _RELIABILITY_CONTRACT
#define _RELIABILITY_CONTRACT
#endif // _RELIABILITY_CONTRACT

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

// __declspec(guard(overflow)) enabled by /sdl compiler switch for CRT allocators
#ifdef _GUARDOVERFLOW_CRT_ALLOCATORS
#define _CRT_GUARDOVERFLOW __declspec(guard(overflow))
#else
#define _CRT_GUARDOVERFLOW
#endif

static inline void _Throw_bad_array_new_length() { }

#endif /* _YVALS */

/*
 * Copyright (c) 1996 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 */
// The file \sdpublic\sdk\inc\crt\yvals.h was reviewed by LCA in June 2011 and per license is
//   acceptable for Microsoft use under Dealpoint ID 46582, 201971

#endif	// _STL70_ or _STL100_ or _STL110_
/* 88bf0570-3001-4e78-a5f2-be5765546192 */
