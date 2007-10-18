// (C) Copyright Aaron W. LaFramboise, Roland Schwarz, Michael Glassford 2004.
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/thread/detail/config.hpp>

#if defined(BOOST_HAS_WINTHREADS) && defined(BOOST_THREAD_BUILD_LIB) && defined(_MSC_VER)

    #include <boost/thread/detail/tss_hooks.hpp>

    #include <stdlib.h>

    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>

    //Definitions required by implementation

    #if (_MSC_VER < 1310) // 1310 == VC++ 7.1
        typedef void (__cdecl *_PVFV)(void);
        #define INIRETSUCCESS
        #define PVAPI void
    #else
        typedef int (__cdecl *_PVFV)(void);
        #define INIRETSUCCESS 0
        #define PVAPI int
    #endif

    typedef void (NTAPI* _TLSCB)(HINSTANCE, DWORD, PVOID);

    //Symbols for connection to the runtime environment

    extern "C"
    {
        extern DWORD _tls_used; //the tls directory (located in .rdata segment)
        extern _TLSCB __xl_a[], __xl_z[];    //tls initializers */
    }

    namespace
    {
        //Forward declarations

        static PVAPI on_tls_prepare(void);
        static PVAPI on_process_init(void);
        static PVAPI on_process_term(void);
        static void NTAPI on_tls_callback(HINSTANCE, DWORD, PVOID);

        //The .CRT$Xxx information is taken from Codeguru:
        //http://www.codeguru.com/Cpp/misc/misc/threadsprocesses/article.php/c6945__2/

        #if (_MSC_VER >= 1310) // 1310 == VC++ 7.1
        #   pragma data_seg(push, old_seg)
        #endif
            //Callback to run tls glue code first.
            //I don't think it is necessary to run it
            //at .CRT$XIB level, since we are only
            //interested in thread detachement. But
            //this could be changed easily if required.

            #pragma data_seg(".CRT$XIU")
            static _PVFV p_tls_prepare = on_tls_prepare;
            #pragma data_seg()

            //Callback after all global ctors.

            #pragma data_seg(".CRT$XCU")
            static _PVFV p_process_init = on_process_init;
            #pragma data_seg()

            //Callback for tls notifications.

            #pragma data_seg(".CRT$XLB")
            _TLSCB p_thread_callback = on_tls_callback;
            #pragma data_seg()

            //Callback for termination.

            #pragma data_seg(".CRT$XTU")
            static _PVFV p_process_term = on_process_term;
            #pragma data_seg()
        #if (_MSC_VER >= 1310) // 1310 == VC++ 7.1
        #   pragma data_seg(pop, old_seg)
        #endif

        PVAPI on_tls_prepare(void)
        {
            //The following line has an important side effect:
            //if the TLS directory is not already there, it will
            //be created by the linker. In other words, it forces a tls
            //directory to be generated by the linker even when static tls
            //(i.e. __declspec(thread)) is not used.
            //The volatile should prevent the optimizer
            //from removing the reference.

            DWORD volatile dw = _tls_used;

            #if (_MSC_VER < 1310) // 1310 == VC++ 7.1
                _TLSCB* pfbegin = __xl_a;
                _TLSCB* pfend = __xl_z;
                _TLSCB* pfdst = pfbegin;
                //pfdst = (_TLSCB*)_tls_used.AddressOfCallBacks;

                //The following loop will merge the address pointers
                //into a contiguous area, since the tlssup code seems
                //to require this (at least on MSVC 6)

                while (pfbegin < pfend)
                {
                    if (*pfbegin != 0)
                    {
                        *pfdst = *pfbegin;
                        ++pfdst;
                    }
                    ++pfbegin;
                }

                *pfdst = 0;
            #endif

            return INIRETSUCCESS;
        }

        PVAPI on_process_init(void)
        {
            //Schedule on_thread_exit() to be called for the main
            //thread before destructors of global objects have been
            //called.

            //It will not be run when 'quick' exiting the
            //library; however, this is the standard behaviour
            //for destructors of global objects, so that
            //shouldn't be a problem.

            atexit(on_thread_exit);

            //Call Boost process entry callback here

            on_process_enter();

            return INIRETSUCCESS;
        }

        PVAPI on_process_term(void)
        {
            on_process_exit();
            return INIRETSUCCESS;
        }

        void NTAPI on_tls_callback(HINSTANCE h, DWORD dwReason, PVOID pv)
        {
            switch (dwReason)
            {
                case DLL_THREAD_DETACH:
                {
                    on_thread_exit();
                    break;
                }
            }
        }
    } //namespace

    extern "C" void tss_cleanup_implemented(void)
    {
        /*
        This function's sole purpose is to cause a link error in cases where
        automatic tss cleanup is not implemented by Boost.Threads as a
        reminder that user code is responsible for calling the necessary
        functions at the appropriate times (and for implementing an a
        tss_cleanup_implemented() function to eliminate the linker's
        missing symbol error).

        If Boost.Threads later implements automatic tss cleanup in cases
        where it currently doesn't (which is the plan), the duplicate
        symbol error will warn the user that their custom solution is no
        longer needed and can be removed.
        */
    }

#endif //defined(BOOST_HAS_WINTHREADS) && defined(BOOST_THREAD_BUILD_LIB)
