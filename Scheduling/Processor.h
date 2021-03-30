#ifndef FIFO_H
#define FIFO_H
#include "Process.h"
#include <vector>
#include <map>
void processor(int n,std::vector<int> processes,std::map<int,P::Process> processMap,int quantum);
void rr(int n, std::vector<int> processes, std::map<int, P::Process>processMap, int quantum);
#endif