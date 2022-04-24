#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <getopt.h>
#include <sstream>
#include <vector>
#include "scheduler.h"
#include "instr.h"
#define RIGHT true
#define LEFT false
using namespace std;

bool comp(Instruction* in1, Instruction* in2){
    return in1->ins_num < in2->ins_num;
}
int main(int argc, char* argv[]) {
    int  timestamp =0, cur_track =0, tot_movement =0, tot_turnaround =0, tot_waittime =0, max_waittime =0;
    Scheduler *sched = nullptr;
    vector <Instruction*> iqueue;
    vector <Instruction*> endqueue;
    Instruction* inst;
    bool is_flook = false;
    char* algo;
    int c;
    while((c= getopt(argc, argv,"s:"))!=-1){
        switch(c){
            case 's':
                algo = optarg;
                break;
            default:
                break;
        }
    }
    switch(algo[0]){
        case 'i':
            sched = new FIFO();
            break;
        case 'j':
            sched = new SSTF();
            break;
        case 'c':
            sched = new CLOOK();
            break;
        case 's':
            sched = new LOOK();
            break;
        case 'f':
            sched = new FLOOK();
            is_flook = true;
            break;
    }

    ifstream ifile(argv[2]);
    string token,line;
    int time,address, i = 0;
    while(getline(ifile, line)){
        if(line[0]=='#')
            continue;
        stringstream ss(line);
        ss >> time >> address;
        inst = new Instruction();
        inst->arrive_time = time;
        inst->dsk_addr = address;
        inst->ins_num = i;
        inst->start_time =0;
        inst->end_time = 0;
        iqueue.push_back(inst);
        endqueue.push_back(inst);
        i++;
    }
    int tot = i, cnt =0;
    while(cnt<tot) {
        if (sched->is_empty()) {
            Instruction *temp_inst = iqueue.front();
            iqueue.erase(iqueue.begin());
            sched->ready_queue.push_back(temp_inst);
            timestamp = temp_inst->arrive_time;
        }
        else {
            cnt++;
            inst = sched->get_next(cur_track);
            int waittime = timestamp - inst->arrive_time;
            tot_waittime += waittime;
            if(max_waittime < waittime)
                max_waittime = waittime;
            inst->start_time = timestamp;
            inst->end_time = timestamp +abs(inst->dsk_addr - cur_track);
            if(is_flook&&!sched->swaped) {
                while (!iqueue.empty() && iqueue[0]->arrive_time <= (timestamp + abs(inst->dsk_addr - cur_track))) {
                    sched->switch_queue.push_back(iqueue[0]);
                    iqueue.erase(iqueue.begin());

                }
            }
            else{
                while(!iqueue.empty()&&iqueue[0]->arrive_time<=(timestamp+abs(inst->dsk_addr -cur_track))){
                    sched->ready_queue.push_back( iqueue[0]);
                    iqueue.erase(iqueue.begin());
                }
            }
            timestamp += abs(inst->dsk_addr - cur_track);
            tot_turnaround += timestamp - inst->arrive_time;
            tot_movement += abs(inst->dsk_addr - cur_track);
            cur_track = inst->dsk_addr;
        }
    }
    sort(endqueue.begin(),endqueue.end(), comp);
    for(auto insts:endqueue){
        printf("%5d: %5d %5d %5d\n", insts->ins_num , insts->arrive_time, insts->start_time, insts->end_time);
    }
    double avg_turnaround  = double(tot_turnaround)/tot;
    double avg_waittime = double(tot_waittime)/tot;
    printf("SUM: %d %d %.2lf %.2lf %d\n",
           timestamp, tot_movement, avg_turnaround, avg_waittime, max_waittime);
    return 0;
}