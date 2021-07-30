/*
* Copyright: JessMA Open Source (ldcsaa@gmail.com)
*
* Author	: Bruce Liang
* Website	: https://github.com/ldcsaa
* Project	: https://github.com/ldcsaa/HP-Socket
* Blog		: http://www.cnblogs.com/ldcsaa
* Wiki		: http://www.oschina.net/p/hp-socket
* QQ Group	: 44636872, 75375912
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#pragma once

#include "GlobalDef.h"

#include <unistd.h>
#include <sched.h>
#include <pthread.h>

#include <sys/syscall.h>
#include <sys/sysinfo.h>

using namespace std;


#define MAX_WORKER_THREAD_COUNT			512

#define DEFAULT_OBJECT_CACHE_LOCK_TIME	(30 * 1000)

#define DEFAULT_OBJECT_CACHE_POOL_SIZE	600

#define DEFAULT_OBJECT_CACHE_POOL_HOLD	600

#define DEFAULT_BUFFER_CACHE_CAPACITY	4096

#define DEFAULT_BUFFER_CACHE_POOL_SIZE	1024

#define DEFAULT_BUFFER_CACHE_POOL_HOLD	1024


#define SysGetSystemConfig				sysconf
#define SysGetSystemInfo				sysinfo

#if !defined(__ANDROID__)
#define SysGetPageSize				getpagesize
#define SysGetNumberOfProcessors	get_nprocs
#else
#define SysGetPageSize()			sysconf(_SC_PAGESIZE)
#define SysGetNumberOfProcessors()	sysconf(_SC_NPROCESSORS_ONLN)
#endif

#define SYS_PAGE_SIZE					GetSysPageSize()

#define PROCESSOR_COUNT					(::SysGetNumberOfProcessors())
#define GetCurrentProcessId				getpid
#define SELF_PROCESS_ID					(::GetCurrentProcessId())
#define gettid()						syscall(__NR_gettid)
#define GetCurrentNativeThreadId()		gettid()
#define SELF_NATIVE_THREAD_ID			(::GetCurrentNativeThreadId())
#define GetCurrentThreadId				pthread_self
#define SELF_THREAD_ID					(::GetCurrentThreadId())
#define IsSameThread(tid1, tid2)		pthread_equal((tid1), (tid2))
#define IsSelfThread(tid)				IsSameThread((tid), SELF_THREAD_ID)
inline BOOL IsSameNativeThread(pid_t pid1, pid_t pid2)
{
	return (pid1 == pid2);
}
#define IsSelfNativeThread(pid)			IsSameNativeThread((pid), SELF_PROCESS_ID)
#define DEFAULT_WORKER_THREAD_COUNT		GetDefaultWorkerThreadCount()

// Yield
#if defined(__cplusplus)
#include <thread>
static inline void __atomic_yield()
{
	std::this_thread::yield();
}
#elif defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
static inline void __atomic_yield()
{
	YieldProcessor();
}
#elif defined(__SSE2__)
#include <emmintrin.h>
static inline void __atomic_yield()
{
	_mm_pause();
}
#elif   (defined(__GNUC__) || defined(__clang__)) && \
		(defined(__x86_64__) || defined(__i386__) || defined(__arm__) || defined(__armel__) || defined(__ARMEL__) || \
		defined(__aarch64__) || defined(__powerpc__) || defined(__ppc__) || defined(__PPC__))
#if defined(__x86_64__) || defined(__i386__)
static inline void __atomic_yield()
{
	__asm__ volatile ("pause" ::: "memory");
}
#elif defined(__aarch64__)
static inline void __atomic_yield()
{
	__asm__ volatile("wfe");
}
#elif (defined(__arm__) && __ARM_ARCH__ >= 7)
static inline void __atomic_yield()
{
	__asm__ volatile("yield" ::: "memory");
}
#elif defined(__powerpc__) || defined(__ppc__) || defined(__PPC__)
static inline void __atomic_yield()
{
	__asm__ __volatile__("or 27,27,27" ::: "memory");
}
#elif defined(__armel__) || defined(__ARMEL__)
static inline void __atomic_yield()
{
	__asm__ volatile ("nop" ::: "memory");
}
#endif
#elif defined(__sun)
// Fallback for other archs
#include <synch.h>
static inline void __atomic_yield()
{
	smt_pause();
}
#elif defined(__wasi__)
#include <sched.h>
static inline void __atomic_yield()
{
	sched_yield();
}
#else
#include <unistd.h>
static inline void __atomic_yield()
{
	sleep(0);
}
#endif	// Yield

#define YieldProcessor					__atomic_yield
#define SwitchToThread					sched_yield

inline void __asm_nop() { __asm__ __volatile__("nop" : : : "memory"); }
inline void __asm_rep_nop() { __asm__ __volatile__("rep; nop" : : : "memory"); }

DWORD GetSysPageSize();
DWORD GetKernelVersion();
BOOL IsKernelVersionAbove(BYTE major, BYTE minor, BYTE revise);
DWORD GetDefaultWorkerThreadCount();


#if defined(__ANDROID__)

#include<android/api-level.h>

#define pthread_cancel(t)

#if defined(__ANDROID_API__)
#if (__ANDROID_API__ < 21)

#define ppoll(fd, nfds, ptmspec, sig)							poll((fd), (nfds), ((ptmspec) == nullptr) ? -1 : ((ptmspec)->tv_sec * 1000 + (ptmspec)->tv_nsec / 1000000))
#define epoll_create1(flag)										epoll_create(32)
#define epoll_pwait(epfd, events, maxevents, timeout, sigmask)	epoll_wait((epfd), (events), (maxevents), (timeout))

#endif
#endif

#endif
