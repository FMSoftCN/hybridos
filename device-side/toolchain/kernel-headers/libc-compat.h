#ifndef _LINUX_LIBC_COMPAT_H
#define _LINUX_LIBC_COMPAT_H

/* We have included glibc headers... */
#if defined(__GLIBC__) || defined(__MUSL__)

/* Coordinate with glibc net/if.h header. */
#if defined(_NET_IF_H)

/* GLIBC headers included first so don't define anything
 * that would already be defined. */

#define __DEF_IF_IFCONF 0
#define __DEF_IF_IFMAP 0
#define __DEF_IF_IFNAMSIZ 0
#define __DEF_IF_IFREQ 0
/* Everything up to IFF_DYNAMIC, matches net/if.h until glibc 2.23 */
#define __DEF_IF_NET_DEVICE_FLAGS 0
/* For the future if glibc adds IFF_LOWER_UP, IFF_DORMANT and IFF_ECHO */
#ifndef __DEF_IF_NET_DEVICE_FLAGS_LOWER_UP_DORMANT_ECHO
#define __DEF_IF_NET_DEVICE_FLAGS_LOWER_UP_DORMANT_ECHO 1
#endif /* __DEF_IF_NET_DEVICE_FLAGS_LOWER_UP_DORMANT_ECHO */

#else /* _NET_IF_H */

/* Linux headers included first, and we must define everything
 * we need. The expectation is that glibc will check the
 * __DEF_* defines and adjust appropriately. */

#define __DEF_IF_IFCONF 1
#define __DEF_IF_IFMAP 1
#define __DEF_IF_IFNAMSIZ 1
#define __DEF_IF_IFREQ 1
/* Everything up to IFF_DYNAMIC, matches net/if.h until glibc 2.23 */
#define __DEF_IF_NET_DEVICE_FLAGS 1
/* For the future if glibc adds IFF_LOWER_UP, IFF_DORMANT and IFF_ECHO */
#define __DEF_IF_NET_DEVICE_FLAGS_LOWER_UP_DORMANT_ECHO 1

#endif /* _NET_IF_H */

#else /* !defined(__GLIBC__) */

/* Definitions for if.h */
#define __DEF_IF_IFCONF 1
#define __DEF_IF_IFMAP 1
#define __DEF_IF_IFNAMSIZ 1
#define __DEF_IF_IFREQ 1
/* Everything up to IFF_DYNAMIC, matches net/if.h until glibc 2.23 */
#define __DEF_IF_NET_DEVICE_FLAGS 1
/* For the future if glibc adds IFF_LOWER_UP, IFF_DORMANT and IFF_ECHO */
#define __DEF_IF_NET_DEVICE_FLAGS_LOWER_UP_DORMANT_ECHO 1

#endif /*  __GLIBC__ */

#endif /* _LINUX_LIBC_COMPAT_H */
