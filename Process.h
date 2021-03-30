#include <queue>
#include <string>
#include <iostream>
#include <map>
#ifndef PROCESS_H
#define PROCESS_H

namespace P {
	class Process {
		
		int time;
		std::string state;
		
	public:
		Process(int number, std::string stat, int pid);
		std::deque<std::string>taskNames;
		std::deque<int> tasks;
		int pid;
		static bool all_done;
		static int running_time;
		static int pquantum;
		static std::map<int, Process> pm;
		static int running;
		static std::queue<int> fcfsq;
		int getTime();
		std::string getState();
		void setTime(int num);
		void setState(std::string stat);

		void setTasks(int task);
		void setTaskNames(std::string taskName);
		int getTaskTime();
		std::string getTaskName();
		int status_update(std::map<int,Process> pm,int id);
		int RRstatus_update(std::map<int, Process>pm, int id);

	};
}
#endif