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

        //creats the data structure that represents the block
        //the whole data block is formated as an array of DPB directory entries
        SODirEntry root[DPB];

        // '.' directory itself
        root[0].in=0x00000000;
        root[0].name[0]='.';
        // '..' father directory (itself too because it's root)
        root[1].in=0x00000000;
        root[1].name[0]='..';

        // iteration of rest of the blocks
        for(uint32_t i=2; i<DPB; i++) {
            //the other slots must be cleaned:
            //field name filled with zeros and 
            //field inode filled with NullReference
            root[i].in = NullReference;
            for (int j = 0; j <= SOFS19_MAX_NAME; j++){
                root[i].name[j]='0'; 
            }
        }
        // itotal -> the total number of inodes     
        uint32_t usedBlocks = (itotal/IPB)+1;
        // store data structure on disk
        soWriteRawBlock(usedBlocks,root);
        //return binFillRootDir(itotal);
    }
};

