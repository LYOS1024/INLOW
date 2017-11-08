#ifndef _INTTYPES_H
#define _INttYPES_H

#include <stdint.h>
#include <sys/cdefs.h>
#define __need_wchar_t
#include <sys/libc-types.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define PRId8 "hhd"
#define PRId16 "hd"
#define PRId32 "d"
#define PRId64 "lld"
#define PRIdLEAST8 "hhd"
#define PRIdLEAST16 "hd"
#define PRIdLEAST32 "d"
#define PRIdLEAST64 "lld"
#define PRIdFAST8 "d"
#define PRIdFAST16 "d"
#define PRIdFAST32 "d"
#define PRIdFAST64 "lld"
#define PRIdMAX "jd"
#define PRIdPTR "ld"

#define PRIi8 "hhi"
#define PRIi16 "hi"
#define PRIi32 "i"
#define PRIi64 "lli"
#define PRIiLEAST8 "hhi"
#define PRIiLEAST16 "hi"
#define PRIiLEAST32 "i"
#define PRIiLEAST64 "lli"
#define PRIiFAST8 "i"
#define PRIiFAST16 "i"
#define PRIiFAST32 "i"
#define PRIiFAST64 "lli"
#define PRIiMAX "ji"
#define PRIiPTR "li"

#define PRIo8 "hho"
#define PRIo16 "ho"
#define PRIo32 "o"
#define PRIo64 "llo"
#define PRIoLEAST8 "hho"
#define PRIoLEAST16 "ho"
#define PRIoLEAST32 "o"
#define PRIoLEAST64 "llo"
#define PRIoFAST8 "o"
#define PRIoFAST16 "o"
#define PRIoFAST32 "o"
#define PRIoFAST64 "llo"
#define PRIoMAX "jo"
#define PRIoPTR "lo"

#define PRIu8 "hhu"
#define PRIu16 "hu"
#define PRIu32 "u"
#define PRIu64 "llu"
#define PRIuLEAST8 "hhu"
#define PRIuLEAST16 "hu"
#define PRIuLEAST32 "u"
#define PRIuLEAST64 "llu"
#define PRIuFAST8 "u"
#define PRIuFAST16 "u"
#define PRIuFAST32 "u"
#define PRIuFAST64 "llu"
#define PRIuMAX "ju"
#define PRIuPTR "lu"

#define PRIx8 "hhx"
#define PRIx16 "hx"
#define PRIx32 "x"
#define PRIx64 "llx"
#define PRIxLEAST8 "hhx"
#define PRIxLEAST16 "hx"
#define PRIxLEAST32 "x"
#define PRIxLEAST64 "llx"
#define PRIxFAST8 "x"
#define PRIxFAST16 "x"
#define PRIxFAST32 "x"
#define PRIxFAST64 "llx"
#define PRIxMAX "jx"
#define PRIxPTR "lx"

#define PRIX8 "hhX"
#define PRIX16 "hX"
#define PRIX32 "X"
#define PRIX64 "llX"
#define PRIXLEAST8 "hhX"
#define PRIXLEAST16 "hX"
#define PRIXLEAST32 "X"
#define PRIXLEAST64 "llX"
#define PRIXFAST8 "X"
#define PRIXFAST16 "X"
#define PRIXFAST32 "X"
#define PRIXFAST64 "llX"
#define PRIXMAX "jX"
#define PRIXPTR "lX"
		
#define SCNd8 PRId8
#define SCNd16 PRId16
#define SCNd32 PRId32
#define SCNd64 PRId64
#define SCNdLEAST8 PRIdLEAST8
#define SCNdLEAST16 PRIdLEAST16
#define SCNdLEAST32 PRIdLEAST32
#define SCNdLEAST64 PRIdLEAST64
#define SCNdFAST8 PRIdFAST8
#define SCNdFAST16 PRIdFAST16
#define SCNdFAST32 PRIdFAST32
#define SCNdFAST64 PRIdFAST64
#define SCNdMAX PRIdMAX
#define SCNdPTR PRIdPTR

#define SCNi8 PRIi8
#define SCNi16 PRIi16
#define SCNi32 PRIi32
#define SCNi64 PRIi64
#define SCNiLEAST8 PRIiLEAST8
#define SCNiLEAST16 PRIiLEAST16
#define SCNiLEAST32 PRIiLEAST32
#define SCNiLEAST64 PRIiLEAST64
#define SCNiFAST8 PRIiFAST8
#define SCNiFAST16 PRIiFAST16
#define SCNiFAST32 PRIiFAST32
#define SCNiFAST64 PRIiFAST64
#define SCNiMAX PRIiMAX
#define SCNiPTR PRIiPTR

#define SCNo8 PRIo8
#define SCNo16 PRIo16
#define SCNo32 PRIo32
#define SCNo64 PRIo64
#define SCNoLEAST8 PRIoLEAST8
#define SCNoLEAST16 PRIoLEAST16
#define SCNoLEAST32 PRIoLEAST32
#define SCNoLEAST64 PRIoLEAST64
#define SCNoFAST8 PRIoFAST8
#define SCNoFAST16 PRIoFAST16
#define SCNoFAST32 PRIoFAST32
#define SCNoFAST64 PRIoFAST64
#define SCNoMAX PRIoMAX
#define SCNoPTR PRIoPTR

#define SCNu8 PRIu8
#define SCNu16 PRIu16
#define SCNu32 PRIu32
#define SCNu64 PRIu64
#define SCNuLEAST8 PRIuLEAST8
#define SCNuLEAST16 PRIuLEAST16
#define SCNuLEAST32 PRIuLEAST32
#define SCNuLEAST64 PRIuLEAST64
#define SCNuFAST8 PRIuFAST8
#define SCNuFAST16 PRIuFAST16
#define SCNuFAST32 PRIuFAST32
#define SCNuFAST64 PRIuFAST64
#define SCNuMAX PRIuMAX
#define SCNuPTR PRIuPTR

#define SCNx8 PRIx8
#define SCNx16 PRIx16
#define SCNx32 PRIx32
#define SCNx64 PRIx64
#define SCNxLEAST8 PRIxLEAST8
#define SCNxLEAST16 PRIxLEAST16
#define SCNxLEAST32 PRIxLEAST32
#define SCNxLEAST64 PRIxLEAST64
#define SCNxFAST8 PRIxFAST8
#define SCNxFAST16 PRIxFAST16
#define SCNxFAST32 PRIxFAST32
#define SCNxFAST64 PRIxFAST64
#define SCNxMAX PRIxMAX
#define SCNxPTR PRIxPTR

#ifdef __cplusplus
}
#endif

#endif
