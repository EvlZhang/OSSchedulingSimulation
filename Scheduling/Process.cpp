
#include "Process.h"

using namespace std;
using namespace P;


//all of these are static variables, some are unnecessary and are kept for debugging
int Process::running = 0;
queue<int> Process::fcfsq;
map<int, Process> Process::pm;
int Process::running_time = 0;
int Process::pquantum;
bool Process::all_done = false;
Process::Process(int number, string stat,int pid) {
	time = number;
	state = stat;
	this->pid = pid;
}

//getter and setters
int Process::getTime() {
	return time;
}

string Process::getState() {
	return state;
}
void Process::setTime(int num) {
	this->time = num;
}
void Process::setState(string stat) {
	this->state = stat;
}

void Process::setTasks(int task) {
	tasks.push_back(task);
}
void Process::setTaskNames(string taskName) {
	taskNames.push_back(taskName);
}
int Process::getTaskTime() {
	if (tasks.size() > 0) {
		int i = tasks.front();
		tasks.pop_front();
		return i;
	}
	else {
		return 0;
	}
	
}
string Process::getTaskName() {
	
	if (taskNames.size() > 0) {
		string s = taskNames.front();
		taskNames.pop_front();
		return s;
	}
	else {
		return "terminate";
	}
	
}


// this is the method used for determining the state of a process as well as sending a signal to the process if needed
int Process::status_update(map<int, Process> processMap,int id){

	
	int next=0;

	//cout<< pid<<"\t";
	/*if (state == "ready from quantum suspension") {
		running = fcfsq.front();
		fcfsq.pop();
		
		next = running;
	}*/
	//cout << "running P" << running;
	if (state == "ready"||state=="preempted") {
		//if (this->tasks.empty() && fcfsq.front() == pid) { this->setState("terminate"); fcfsq.pop(); }
		//if (all_done == true) { this->setState("terminate"); }
		if (running == 0) {
			
			this->setState(this->getTaskName());
			this->setTime(this->getTaskTime());
			if (this->getState() == "running") {
				running = pid;
			}
			if (fcfsq.size() > 0 && fcfsq.front() == pid) {
				fcfsq.pop();
				//cout << "poped" <<pid <<"\n";
				
			}
			
			
		}
		else {
			bool duplicate = false;
			queue<int> copy = fcfsq;
			//cout << "queue content: \n";
			int len = copy.size();
			for (int i = 0; i < len; i++) {
				int temp = copy.front();
				//cout << temp ;
				if (temp == pid) { duplicate = true; }
				copy.pop();

			}
			if (fcfsq.size()==0||duplicate==false) {
				
					fcfsq.push(this->pid);
					//cout << "pushed " << pid <<"\t";
				
			}

			
		}
	}
	else if (state == "running") {
		time--;
		running_time++;
		//cout << pid << "--";
		
		if (time == 0) {
			this->setState(this->getTaskName());
			this->setTime(this->getTaskTime());
			running = 0;
			running_time = 0;
			//cout << "current " << pid << "front " << fcfsq.front();
			 {
					queue<int> copy = fcfsq;
					//cout << "queue content: \n";
					int len = copy.size();
					for (int i = 0; i < len; i++) {
						//cout << copy.front() << " ";
							copy.pop();

					}
				if (fcfsq.size() > 0&&fcfsq.front()<pid) {
					
					next = fcfsq.front();
					fcfsq.pop();
					//cout << "poped" << next << "\n";
					
				}
			}

			if (this->tasks.size()==0 && pid == id) {
				all_done = true;
			}
		}
	}
	else if (state == "blocked") {
		time--;

		if (time == 0) {
			this->setState("ready");
			bool all_blocked = false;
			if (this->tasks.size() != 0) { 
				bool duplicate = false;
				queue<int> copy = fcfsq;
				//cout << "queue content: \n";
				int len = copy.size();
				for (int i = 0; i < len; i++) {
					int temp = copy.front();
					//cout << temp << "\n";
					if (temp == pid) { duplicate = true; }
					
					copy.pop();

				}
				if (duplicate == false) {

					fcfsq.push(this->pid);
				}
				
				
			}
			if (pquantum != -1) {
				if (fcfsq.size() != 0 && fcfsq.front() == pid && this->tasks.size() != 0) {  next = pid; fcfsq.pop(); }
				//else if (this->tasks.size() == 0) { next = pid; }
			}
			

			if (running == 0 && this->tasks.size() == 0 &&fcfsq.size()==0) {this->setState("terminate"); }

			//cout << "push";
		}
	}
	/*cout << "P" << id << state << "\t";*/
	
	if (state == "terminate") {
		return -1;

	}
	if (running_time == pquantum && time != 0 && this->state == "running") {
		this->taskNames.push_front(this->state);
		this->tasks.push_front(this->time);
		this->setState("ready");
		this->setTime(0);
		
		
		if (fcfsq.size() > 0 && fcfsq.front() < this->pid) {
			next = fcfsq.front();
			fcfsq.pop();
			//cout << "triggered";
		}
		fcfsq.push(this->pid);
		bool all_blocked = true;
		/*for (int i = 1; i <= processMap.size(); i++) {
			if (i != pid && processMap.at(i).getState() != "blocked") {
				all_blocked = false;
			}
		}*/
		//if (all_blocked&&fcfsq.front()==pid) { next = fcfsq.front(); fcfsq.pop(); }
		//if (fcfsq.size() == 1) { next = pid; }
		

		running = 0;
		running_time = 0;
	}
	
	return next;
}

int Process::RRstatus_update(map<int, Process>pm, int id) {

	int next = 0;

	//cout<< pid<<"\t";

	if (state == "ready") {
		if (this->tasks.size() == 0) { this->setState("terminate"); }
		if (running == false) {
			this->setState(this->getTaskName());
			this->setTime(this->getTaskTime());
			if (this->getState() == "running") {
				running = true;
			}
			if (fcfsq.size() > 0 && fcfsq.front() == pid) {
				fcfsq.pop();

			}

		}
		else {
			if (fcfsq.size() == 0 || (fcfsq.back() != pid && fcfsq.front() != pid)) {
				fcfsq.push(this->pid);
			}

		}
	}
	else if (state == "running") {
		time--;
		running_time++;
		if (time == 0) {
			this->setState(this->getTaskName());
			this->setTime(this->getTaskTime());
			running = false;
			//cout << "current " << pid << "front " << fcfsq.front();
			if (fcfsq.size() > 0 && fcfsq.front() < this->pid) {
				next = fcfsq.front();
				fcfsq.pop();
				//cout << "triggered";

			}


		}
	}
	else if (state == "blocked") {
		time--;
		if (time == 0) {
			this->setState("ready");
			fcfsq.push(this->pid);
			//cout << "push";
		}
	}
	/*cout << "P" << id << state << "\t";*/

	if (state == "terminate") {
		return -1;

	}
	if (running_time == pquantum) {
		
	}
	return next;
}
