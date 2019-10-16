#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "bin_mksofs.h"

#include <string.h>
#include <inttypes.h>

namespace sofs19
{
    void grpFillSuperBlock(const char *name, uint32_t ntotal, uint32_t itotal, uint32_t nbref)
    {
        soProbe(602, "%s(%s, %u, %u, %u)\n", __FUNCTION__, name, ntotal, itotal, nbref);

        /* change the following line by your code */
        //binFillSuperBlock(name, ntotal, itotal, nbref);
        
        
        SOSuperBlock sb;
        strncpy(sb.name, name,PARTITION_NAME_SIZE); //ou PARTITION_NAME_SIZE + 1??

        sb.magic = MAGIC_NUMBER;
        sb.version = VERSION_NUMBER;

        sb.mntstat = 1; //properly unmounted
        sb.mntcnt = 0;
        sb.ntotal = ntotal;
        sb.itotal = itotal;
       
        sb.it_size = itotal/IPB;
        sb.ifree = itotal - 1;
        
        sb.ihead = 1; //The list of free inodes must start in inode number 1 and go sequentially to the last.
        sb.itail = itotal;//
        
        sb.dz_start = 1 + itotal/IPB; 
        

        

        uint32_t data_zone_size = ntotal -1 -itotal;
        uint32_t free_data_zone = data_zone_size - 1 - nbref;
        
        sb.dz_total = data_zone_size;
        sb.dz_free = free_data_zone;
        


        sb.head_blk = 1;
        sb.head_idx = 0;
        sb.tail_blk = nbref;
        sb.tail_idx = 0;
        
        for(int i = 0;i < TAIL_CACHE_SIZE;i++){
            sb.tail_cache.ref[i] = NullReference;

        }
        int count = sb.tail_blk;
        for(int i = 0; i < HEAD_CACHE_SIZE;i++){
            sb.head_cache.ref[i] = count;
            count++;

        }

        soWriteRawBlock(0,&sb);
        
        
        
        
        
        



    }
};

