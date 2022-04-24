//
// Created by heyihan on 4/23/22.
//
#include "scheduler.h"
#include <cstdlib>


Instruction* FIFO::get_next(int addr) {
    Instruction* instr = ready_queue.front();
    ready_queue.erase(ready_queue.begin());
    return instr;
}

Instruction* SSTF::get_next(int addr) {
    int min_dist = INT_MAX;
    int temp = -1, ind = -1;

    for(int i=0;i<ready_queue.size();i++){
        temp = abs(ready_queue[i]->dsk_addr-addr);
        if(temp < min_dist){
            min_dist = temp;
            ind = i;
        }
    }
    Instruction* inst = ready_queue[ind];
    ready_queue.erase(ready_queue.begin() + ind);
    return inst;
}

Instruction* LOOK::get_next(int addr) {
    int ind = -1, min_dist =INT_MAX;
    Instruction* inst;
    for(int i = 0;i<ready_queue.size();i++){
        if(direction && ready_queue[i]->dsk_addr >= addr && ready_queue[i]->dsk_addr - addr < min_dist){
                min_dist = ready_queue[i]->dsk_addr - addr;
                ind = i;
        }
        else if(!direction && ready_queue[i]->dsk_addr<=addr && addr-ready_queue[i]->dsk_addr < min_dist){
                min_dist = addr - ready_queue[i]->dsk_addr;
                ind = i;
        }
    }
    if(ind !=-1){
        inst = ready_queue[ind];
        ready_queue.erase(ready_queue.begin() + ind);
        return inst;
    }
    else{
        direction = !direction;
        return get_next(addr);
    }
}

Instruction* CLOOK:: get_next(int addr){
    int ind = -1, min_dist = INT_MAX, max_dist = -1;
    Instruction* inst;
    for(int i =0; i<ready_queue.size();i++){
        if(ready_queue[i]->dsk_addr >= addr && abs(ready_queue[i]->dsk_addr - addr) < min_dist){
            min_dist = abs(ready_queue[i]->dsk_addr - addr);
            ind = i;
        }
    }
    if(ind == -1) {
        for (int i = 0; i < ready_queue.size(); i++) {
            if (ready_queue[i]->dsk_addr <= addr && abs(addr - ready_queue[i]->dsk_addr) > max_dist) {
                max_dist = abs(addr - ready_queue[i]->dsk_addr);
                ind = i;
            }
        }
    }
    inst = ready_queue[ind];
    ready_queue.erase(ready_queue.begin() + ind);
    return inst;
}


Instruction* FLOOK:: get_next(int addr){
    int ind = -1, min_dist = INT_MAX;
    Instruction* inst;
    if(swaped&&switch_queue.empty())
        swaped = false;
    else if(!swaped&&ready_queue.empty())
        swaped = true;
    if (!swaped) {
        if (direction) {
            for (int i = 0; i < ready_queue.size(); i++) {
                if (ready_queue[i]->dsk_addr >= addr && abs(ready_queue[i]->dsk_addr - addr) < min_dist) {
                    min_dist = ready_queue[i]->dsk_addr - addr;
                    ind = i;
                }
            }
        }
        else {
            for (int i = 0; i < ready_queue.size(); i++) {
                if (ready_queue[i]->dsk_addr <= addr && abs(ready_queue[i]->dsk_addr - addr) < min_dist) {
                    min_dist = abs(ready_queue[i]->dsk_addr - addr);
                    ind = i;
                }
            }
        }
        if(ind != -1){
            inst = ready_queue[ind];
            ready_queue.erase(ready_queue.begin()+ind);
            return inst;
        }
        else{
            direction = !direction;
            return get_next(addr);
        }
    }
    else {
        if (direction) {
            for (int i = 0; i < switch_queue.size(); i++) {
                if (switch_queue[i]->dsk_addr >= addr && abs(switch_queue[i]->dsk_addr - addr) < min_dist) {
                    min_dist = abs(switch_queue[i]->dsk_addr - addr);
                    ind = i;
                }
            }
        } else {
            for (int i = 0; i < switch_queue.size(); i++) {
                if (switch_queue[i]->dsk_addr <= addr && abs(switch_queue[i]->dsk_addr - addr) < min_dist) {
                    min_dist = abs(switch_queue[i]->dsk_addr - addr);
                    ind = i;
                }
            }
        }
        if (ind != -1) {
            inst = switch_queue[ind];
            switch_queue.erase(switch_queue.begin() + ind);
            return inst;
        }
        else{
            direction = !direction;
            return get_next(addr);
        }
    }
}