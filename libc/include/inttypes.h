#ifndef _INTTYPES_H
#define _INTTYPES_H

#include <stdint.h>
#include <stdlib.h>


#define PRIdMAX "d"
#define PRIiMAX "i"
#define PRIuMAX "u"
#define PRIxMAX "x"
#define PRIXMAX "X"


#define PRId8 "d"
#define PRIi8 "i"
#define PRIu8 "u"
#define PRIx8 "x"
#define PRIX8 "X"

#define PRId16 "d"
#define PRIi16 "i"
#define PRIu16 "u"
#define PRIx16 "x"
#define PRIX16 "X"

#define PRId32 "d"
#define PRIi32 "i"
#define PRIu32 "u"
#define PRIx32 "x"
#define PRIX32 "X"

#define PRId64 "d"
#define PRIi64 "i"
#define PRIu64 "u"
#define PRIx64 "x"
#define PRIX64 "X"

#define PRIdLEAST8 "d"
#define PRIiLEAST8 "i"
#define PRIuLEAST8 "u"
#define PRIxLEAST8 "x"
#define PRIXLEAST8 "X"

#define PRIdLEAST16 "d"
#define PRIiLEAST16 "i"
#define PRIuLEAST16 "u"
#define PRIxLEAST16 "x"
#define PRIXLEAST16 "X"

#define PRIdLEAST32 "d"
#define PRIiLEAST32 "i"
#define PRIuLEAST32 "u"
#define PRIxLEAST32 "x"
#define PRIXLEAST32 "X"

#define PRIdLEAST64 "d"
#define PRIiLEAST64 "i"
#define PRIuLEAST64 "u"
#define PRIxLEAST64 "x"
#define PRIXLEAST64 "X"


#define PRIdFAST8 "d"
#define PRIiFAST8 "i"
#define PRIuFAST8 "u"
#define PRIxFAST8 "x"
#define PRIXFAST8 "X"

#define PRIdFAST16 "d"
#define PRIiFAST16 "i"
#define PRIuFAST16 "u"
#define PRIxFAST16 "x"
#define PRIXFAST16 "X"

#define PRIdFAST32 "d"
#define PRIiFAST32 "i"
#define PRIuFAST32 "u"
#define PRIxFAST32 "x"
#define PRIXFAST32 "X"

#define PRIdFAST64 "d"
#define PRIiFAST64 "i"
#define PRIuFAST64 "u"
#define PRIxFAST64 "x"
#define PRIXFAST64 "X"


#define PRId8PTR "d"
#define PRIi8PTR "i"
#define PRIu8PTR "u"
#define PRIx8PTR "x"
#define PRIX8PTR "X"

#define PRId16PTR "d"
#define PRIi16PTR "i"
#define PRIu16PTR "u"
#define PRIx16PTR "x"
#define PRIX16PTR "X"

#define PRId32PTR "d"
#define PRIi32PTR "i"
#define PRIu32PTR "u"
#define PRIx32PTR "x"
#define PRIX32PTR "X"

#define PRId64PTR "d"
#define PRIi64PTR "i"
#define PRIu64PTR "u"
#define PRIx64PTR "x"
#define PRIX64PTR "X"



#define SCNdMAX "d"
#define SCNiMAX "i"
#define SCNuMAX "u"
#define SCNxMAX "x"
#define SCNXMAX "X"


#define SCNd8 "d"
#define SCNi8 "i"
#define SCNu8 "u"
#define SCNx8 "x"
#define SCNX8 "X"

#define SCNd16 "d"
#define SCNi16 "i"
#define SCNu16 "u"
#define SCNx16 "x"
#define SCNX16 "X"

#define SCNd32 "d"
#define SCNi32 "i"
#define SCNu32 "u"
#define SCNx32 "x"
#define SCNX32 "X"

#define SCNd64 "d"
#define SCNi64 "i"
#define SCNu64 "u"
#define SCNx64 "x"
#define SCNX64 "X"

#define SCNdLEAST8 "d"
#define SCNiLEAST8 "i"
#define SCNuLEAST8 "u"
#define SCNxLEAST8 "x"
#define SCNXLEAST8 "X"

#define SCNdLEAST16 "d"
#define SCNiLEAST16 "i"
#define SCNuLEAST16 "u"
#define SCNxLEAST16 "x"
#define SCNXLEAST16 "X"

#define SCNdLEAST32 "d"
#define SCNiLEAST32 "i"
#define SCNuLEAST32 "u"
#define SCNxLEAST32 "x"
#define SCNXLEAST32 "X"

#define SCNdLEAST64 "d"
#define SCNiLEAST64 "i"
#define SCNuLEAST64 "u"
#define SCNxLEAST64 "x"
#define SCNXLEAST64 "X"


#define SCNdFAST8 "d"
#define SCNiFAST8 "i"
#define SCNuFAST8 "u"
#define SCNxFAST8 "x"
#define SCNXFAST8 "X"

#define SCNdFAST16 "d"
#define SCNiFAST16 "i"
#define SCNuFAST16 "u"
#define SCNxFAST16 "x"
#define SCNXFAST16 "X"

#define SCNdFAST32 "d"
#define SCNiFAST32 "i"
#define SCNuFAST32 "u"
#define SCNxFAST32 "x"
#define SCNXFAST32 "X"

#define SCNdFAST64 "d"
#define SCNiFAST64 "i"
#define SCNuFAST64 "u"
#define SCNxFAST64 "x"
#define SCNXFAST64 "X"


#define SCNd8PTR "d"
#define SCNi8PTR "i"
#define SCNu8PTR "u"
#define SCNx8PTR "x"
#define SCNX8PTR "X"

#define SCNd16PTR "d"
#define SCNi16PTR "i"
#define SCNu16PTR "u"
#define SCNx16PTR "x"
#define SCNX16PTR "X"

#define SCNd32PTR "d"
#define SCNi32PTR "i"
#define SCNu32PTR "u"
#define SCNx32PTR "x"
#define SCNX32PTR "X"

#define SCNd64PTR "d"
#define SCNi64PTR "i"
#define SCNu64PTR "u"
#define SCNx64PTR "x"
#define SCNX64PTR "X"


#define imaxabs abs
#define imaxdiv div
#define strtoimax strtol
#define strtoumax strtoul
#define wcstoimax wcstol
#define wcstoumax wcstoul

typedef struct imaxdiv {
    intmax_t quot;
    intmax_t rem;    
} imaxdiv_t;


#endif
