#pragma once
#include "../libs/KittyMemory/MemoryPatch.h"
#include "../libs/KittyMemory/MemoryBackup.h"
#include "../libs/KittyMemory/KittyMemory.h"
#include "../libs/KittyMemory/KittyUtils.h"

class PatchManager {
private:
    std::vector<MemoryPatch> patches;
public:
    void addPatch(const char *libraryName, uintptr_t address,std::string hex){
        patches.push_back(MemoryPatch::createWithHex(libraryName,address,hex));
    }

    void Modify(){
        for(int k = 0; k < patches.size(); k++){
            patches[k].Modify();
        }
    }

    void Restore(){
        for(int k = 0; k < patches.size(); k++){
            patches[k].Restore();
        }
    }

};