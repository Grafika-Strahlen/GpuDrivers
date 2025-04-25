#pragma once

// C4339: '__type_info_node': use of undefined type detected in CLR meta-data (/Wall)
#ifndef _VCRUNTIME_DISABLED_WARNING_4339
    #ifdef _M_CEE_PURE
        #define _VCRUNTIME_DISABLED_WARNING_4339 4339
    #else
        #define _VCRUNTIME_DISABLED_WARNING_4339
    #endif
#endif

// C4412: function signature contains type '<typename>';
//        C++ objects are unsafe to pass between pure code and mixed or native. (/Wall)
#ifndef _VCRUNTIME_DISABLED_WARNING_4412
    #ifdef _M_CEE_PURE
        #define _VCRUNTIME_DISABLED_WARNING_4412 4412
    #else
        #define _VCRUNTIME_DISABLED_WARNING_4412
    #endif
#endif

// Use _VCRUNTIME_EXTRA_DISABLED_WARNINGS to add additional warning suppressions to VCRuntime headers.
#ifndef _VCRUNTIME_EXTRA_DISABLED_WARNINGS
    #define _VCRUNTIME_EXTRA_DISABLED_WARNINGS
#endif

// C4514: unreferenced inline function has been removed (/Wall)
// C4820: '<typename>' : 'N' bytes padding added after data member (/Wall)
#ifndef _VCRUNTIME_DISABLED_WARNINGS
    #define _VCRUNTIME_DISABLED_WARNINGS _VCRUNTIME_DISABLED_WARNING_4339 _VCRUNTIME_DISABLED_WARNING_4412 4514 4820 _VCRUNTIME_EXTRA_DISABLED_WARNINGS
#endif
