//
// Created by heyihan on 4/23/22.
//

#ifndef IO_SCHED_SCHEDULER_H
#define IO_SCHED_SCHEDULER_H
#ifndef INT_MAX
#define INT_MAX 2147483647
#endif
#include "instr.h"
#include <vector>
using namespace std;

class Scheduler{
public:
    Scheduler():direction(false), swaped(false){}
    virtual Instruction* get_next(int addr)=0;
    vector<Instruction*> ready_queue;
    vector<Instruction*> switch_queue;
    virtual bool is_empty()=0;
    bool direction, swaped;
};


class FIFO: public Scheduler{
public:
    Instruction* get_next(int addr) override;
    bool is_empty() override{return ready_queue.empty();}
};


class SSTF: public Scheduler{
public:
    Instruction* get_next(int addr) override;
    bool is_empty() override{return ready_queue.empty();}
};


class LOOK: public Scheduler{
public:
    Instruction* get_next(int addr) override;
    bool is_empty() override{return ready_queue.empty();}
};


class FLOOK:public Scheduler{
public:
    Instruction* get_next(int addr) override;
    bool is_empty() override{return ready_queue.empty() && switch_queue.empty(); }
};


class CLOOK:public Scheduler{
public:
    Instruction* get_next(int addr) override;
    bool is_empty() override{return ready_queue.empty();}
};
#endif //IO_SCHED_SCHEDULER_H
