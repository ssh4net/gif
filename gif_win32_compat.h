#ifndef WIN32_COMPAT_H
#define WIN32_COMPAT_H

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#include <process.h>

#define open    _open
#define close   _close
#define fdopen  _fdopen
#define unlink  _unlink
#define strdup  _strdup
#define strtok_r strtok_s

/* Windows compatibility for open() flags */
#ifndef O_BINARY
#define O_BINARY _O_BINARY
#endif

#ifndef O_CREAT
#define O_CREAT _O_CREAT
#endif

#ifndef O_RDWR
#define O_RDWR _O_RDWR
#endif

#ifndef O_WRONLY
#define O_WRONLY _O_WRONLY
#endif

#ifndef O_RDONLY
#define O_RDONLY _O_RDONLY
#endif

#endif /* _WIN32 */

#endif /* WIN32_COMPAT_H */ 