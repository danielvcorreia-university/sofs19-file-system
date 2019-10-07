#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "bin_mksofs.h"

#include <string.h>
#include <inttypes.h>

namespace sofs19
{
    /*
       filling in the contents of the root directory:
       the first 2 entries are filled in with "." and ".." references
       the other entries are empty.
       If rdsize is 2, a second block exists and should be filled as well.
       */
    void grpFillRootDir(uint32_t itotal)
    {
        soProbe(606, "%s(%u)\n", __FUNCTION__, itotal);

        /* change the following line by your code */
        /* return binFillRootDir(itotal); */

        /* DPB number of Dir Struct References to fill a Data Block */
        /* SODirEntry buffer */
        SODirEntry entryBlockDirs[DPB];

        /* Filling all the SODirEntry's with zeros in Data Field "in" 
           and NullReference in Data Field "namespace" */

        /* DÚVIDA SE É "NULL" OU "NullReference" */
        /* DÚVIDA SE SERIA PREFERIVÉL UTILIZAR "memset()" e "memcpy" */

        for (uint32_t i = 2; i < DPB; i++)
        { entryBlockDirs[i] -> in = 0; entryBlockDirs[i] -> namespace = NULL; }

        /* Setting the first SODirEntry to "." and the second one to ".." */

        /* DÚVIDA DE VALOR PARA CAMPO "IN" */
        entryBlockDirs[0] -> in = 0;
        entryBlockDirs[0] -> namespace = ".";
        entryBlockDirs[1] -> in = 0;
        entryBlockDirs[1] -> namespace = "..";

        /* Writing in the block position itotal + 1 (SuperBlock) in the raw disk */
        soWriteRawBlock( itotal+1, entryBlockDirs );
        
        if (rdsize == 2)
        {
            SODirEntry entryBlock2Dirs[DPB];
            for(uint32_t i = 0; i < DPB; i++)
            { entryBlock2Dirs[i] -> in = 0; entryBlock2Dirs[i] -> namespace = NULL; }
            soWriteRawBlock( itotal+2, entryBlock2Dirs );
        }
    }
};

