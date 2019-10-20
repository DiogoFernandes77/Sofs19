/*
/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2016-2019
 */

#include "grp_freelists.h"

#include "core.h"
#include "dal.h"
#include "freelists.h"
#include "bin_freelists.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
using namespace std;

namespace sofs19
{
    /* only fill the current block to its end */
    void grpDepleteTailCache(void)
    {
        soProbe(444, "%s()\n", __FUNCTION__);
        soOpenSuperBlock ( );
        SOSuperBlock* sb = soGetSuperBlockPointer ();
        uint32_t nblock = sb->tail_blk; // numero do ref data block
        uint32_t idx_ref = sb->tail_idx; // 1ª pos livre no ref data bloco
        uint32_t ref[RPB];
        uint32_t nfree = RPB-idx_ref;
        soReadDataBlock(nblock, ref); // le do bloco para ref

        if(nfree == 0){ // se tiver cheio cria outro bloco
            uint32_t num = soAllocDataBlock();
            for(uint32_t i = 0; i<RPB; i++){
                ref[i] = NullReference;
            }
            soWriteDataBlock(num,ref);
        }

        else{

            if(nfree >= TAIL_CACHE_SIZE){
                for(uint32_t i=0; i<TAIL_CACHE_SIZE; i++){
                    ref[idx_ref] = sb->tail_cache.ref[i]; // copia para a ref os valores da tail cache
                    idx_ref++;
                    sb->tail_cache.ref[i] = NullReference; // liberta tail cache
                }
                soWriteDataBlock(nblock,ref); // escreve na tail reference data block
            }

            else{
                uint32_t k = TAIL_CACHE_SIZE - 1; 
                for (uint32_t i = 0; i < nfree; i++)
                {
                    ref[idx_ref] = sb->tail_cache.ref[i]; // copia para a ref os valores da tail cache
                    idx_ref++;
                    sb->tail_cache.ref[k--] = NullReference;  //liberta tail cache
                }
                soWriteDataBlock(nblock,ref); // escreve no bloco até á última posiçao   
            }
        }

        soSaveSuperBlock();
        /* change the following line by your code */
        //binDepleteTailCache();
    }
}


