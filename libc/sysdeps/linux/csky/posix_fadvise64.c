/* vi: set sw=4 ts=4: */
/*
 * posix_fadvise64() for CSKY uClibc
 * http://www.opengroup.org/onlinepubs/009695399/functions/posix_fadvise.html
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#include <features.h>
#include <unistd.h>
#include <errno.h>
#include <endian.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <fcntl.h>

#ifdef __UCLIBC_HAS_LFS__

#if defined __NR_fadvise64_64
/* This is for the CSKY version of fadvise64_64 which swaps the params
 * about to avoid having ABI compat issues
 */
#define __NR___syscall_fadvise64_64 __NR_fadvise64_64
int __libc_posix_fadvise64(int fd, __off64_t offset, __off64_t len, int advise)
{
  INTERNAL_SYSCALL_DECL (err);
  int ret = INTERNAL_SYSCALL (fadvise64_64, err, 6, fd, advise,
                              __LONG_LONG_PAIR ((long)(offset >> 32), (long)offset),
                              __LONG_LONG_PAIR ((long)(len >> 32), (long)len));
  if (!INTERNAL_SYSCALL_ERROR_P (ret, err))
    return 0;
  if (INTERNAL_SYSCALL_ERRNO (ret, err) != ENOSYS)
   return INTERNAL_SYSCALL_ERRNO (ret, err);
  return 0;
}
weak_alias(__libc_posix_fadvise64, posix_fadvise64);
#else
int posix_fadvise64(int fd, __off64_t offset, __off64_t len, int advise)
{
	return ENOSYS;
}
#endif
#endif
