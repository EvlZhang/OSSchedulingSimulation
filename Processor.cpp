#include "Processor.h"
#include <iostream>
#include<string>
#include<sstream>
#include <queue>
using namespace std;
using namespace P;
void processor(int n, std::vector<int> processes, std::map<int, P::Process> processMap,int quantum) {
	
	string taskName ="running";
	
	// this is for setting up the tasks queue in the Process(es);
	for (int i = 0; i < 4; i++) {
		//int len = processes.at(i);
		int current = 1;
		for (int j = i; j < processes.size(); j+=4) {
			int len = processes.at(j);
			//cout << len;

			string currTask = "p" + to_string(current) +taskName + to_string(len);
			processMap.at(current).setTasks(len);
			processMap.at(current).setTaskNames(taskName);
			current++;
		}
		
		taskName = taskName == "running" ? "blocked" : "running";

	}
	
	
	
	int cycle = 1;
	while (quantum==-1) {
		int sum = 0;
		cout << "cycle" << cycle<<"\t";
		for (int i = 1; i <= processMap.size(); i++) {
			
			int res = processMap.at(i).status_update(processMap, n);
			
			if (res == -1) {
				sum += 1;
			}
			else if (res > 0) {
				//this is for determining if a process needs to get running or terminate as there can be cases where it matters
				if(processMap.at(res).getState()!="running"){
					processMap.at(res).setState(processMap.at(res).getTaskName());
					processMap.at(res).setTime(processMap.at(res).getTaskTime());
				}
				if (processMap.at(res).getState() == "running") { Process::running = res; }
				else if (processMap.at(res).getState() == "terminate") {
					while (true) {
						if (Process::fcfsq.size() != 0) {
						
							if (processMap.at(Process::fcfsq.front()).tasks.size() == 0) {
								processMap.at(Process::fcfsq.front()).setState(processMap.at(Process::fcfsq.front()).getTaskName());
								processMap.at(Process::fcfsq.front()).setTime(processMap.at(Process::fcfsq.front()).getTaskTime());
								Process::fcfsq.pop();
							}
							else { break; }
						}
						else { break; }
					}
				}
				//if (res > i) { processMap.at(res).setTime(processMap.at(res).getTime()+1 ); }
			}


		}

		for (int i = 1; i <= processMap.size(); i++) {
			if (processMap.at(i).getState().compare("ready") == 0) {
				cout << "P" << i  << processMap.at(i).getState()<<"\t" << "\t";
			}
			else { cout << "P" << i << processMap.at(i).getState() << "\t"; }
			//if (Process::all_done == true) { processMap.at(i).setState("terminate"); }
		}
		cycle++;
		cout << "\n";
		if (sum >= n) { return; break; }
		
	}
	//this part is the same as above as I have decided to use status_update function to update the processes
	int RR_counter = 0;
	while (quantum != -1) {
		int sum = 0;
		cout << "cycle" << cycle << "\t";
		for (int i = 1; i <= processMap.size(); i++) {
			int res = processMap.at(i).status_update(processMap, n);

			if (res == -1) {
				sum += 1;
			}
			else if (res > 0) {
				if(processMap.at(res).getState()!="running"){
					processMap.at(res).setState(processMap.at(res).getTaskName());
					processMap.at(res).setTime(processMap.at(res).getTaskTime());
					//Process::running_time = 0;
				}
				
				if (processMap.at(res).getState() == "running") { Process::running = res; }
				else if (processMap.at(res).getState() == "terminate") {
					while (true) {
						if (Process::fcfsq.size() != 0) {

							if (processMap.at(Process::fcfsq.front()).tasks.size() == 0) {
								processMap.at(Process::fcfsq.front()).setState(processMap.at(Process::fcfsq.front()).getTaskName());
								processMap.at(Process::fcfsq.front()).setTime(processMap.at(Process::fcfsq.front()).getTaskTime());
								Process::fcfsq.pop();
								//cout << "force terminate";
							}
							else { break; }
						}
						else { break; }
					}
				}
			}


		}

		for (int i = 1; i <= processMap.size(); i++) {
			if (processMap.at(i).getState().compare( "ready")==0) {
				cout << "P" << i <<"is " << processMap.at(i).getState() << "\t";
			}
			else { cout << "P" << i << processMap.at(i).getState() << "\t"; }
		}
		cycle++;
		cout << "\n";
		if (sum >= n) { break; }
		RR_counter++;
	}
	return;
}

