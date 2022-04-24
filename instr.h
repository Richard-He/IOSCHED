//
// Created by heyihan on 4/23/22.
//

#ifndef IO_SCHED_INSTR_H
#define IO_SCHED_INSTR_H

class Instruction{
public:
    int arrive_time, dsk_addr, ins_num, start_time, end_time;
    Instruction(): arrive_time(-1), dsk_addr(-1), ins_num(-1), start_time(-1), end_time(-1){};

};
#endif //IO_SCHED_INSTR_H
