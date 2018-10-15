#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

const int NumberOfCPU = 4;

struct PCB {
	int PID;
	int arrive_time;
	int length;
	int executed_Length;
	int priority;
};

int current_time;

bool GetProcess(int current_time, int * length, int *priority, bool *end_of_schedule)
{
	static bool opened = false;
	static int number_of_process = 0;
	static bool end_of_process = false;
	static bool have_process = false;
	static int proc_arrive_time = 0;
	static int proc_length = 0;
	static int proc_priority = 0;
	static ifstream fin("data.txt", ios::in);

	if (false == opened) {
		if (!fin) {
			cerr << "Failed to open data.txt!\n";
			exit(1);
		}
	}

	if (true == end_of_process) {
		*length = *priority = 0;
		*end_of_schedule = true;
		return false;
	}

	if (false == have_process) {
		fin >> proc_arrive_time >> proc_length >> proc_priority;
		have_process = true;
	}

	if (proc_arrive_time > current_time) {
		*length = *priority = 0;
		*end_of_schedule = false;
		return false;
	}

	if (-1 == proc_arrive_time) 	{
		end_of_process = true;
		*length = *priority = 0;
		*end_of_schedule = true;
		return false;
	}

	*length = proc_length;
	*priority = proc_priority;
	*end_of_schedule = false;
	have_process = false;
	return true;
}

void FIFO()
{
	int current_time = 0;
	queue <PCB *> ready_queue;
	PCB *current_process = nullptr;
	int total_return_time = 0;
	int total_wait_time = 0;
	int total_process = 0;
	int total_response_time = 0;
	int pid = 1;

	for (;;) {
		int length, priority;
		bool end_of_schedule;

		while (true == GetProcess(current_time, &length, &priority, &end_of_schedule))
		{
			ready_queue.push(new PCB{ pid++, current_time, length, 0, priority });
		}

		if (nullptr != current_process)
		{
			if (current_process->executed_Length >= current_process->length) {
				total_return_time += current_time - current_process->arrive_time;
				total_wait_time += current_time - current_process->arrive_time - current_process->length;
				total_process++;
				delete current_process;
				current_process = nullptr;
			}
		}

		if (nullptr == current_process)
		{
			if (false == ready_queue.empty())
			{
				current_process = ready_queue.front();
				ready_queue.pop();
				if (0 == current_process->executed_Length)
					total_response_time += current_time - current_process->arrive_time;
			}
		}

		if ((true == end_of_schedule) && ready_queue.empty() && (nullptr == current_process))  break;

		 if (nullptr != current_process) {
			current_process->executed_Length++;
//			cout << "Time:" << current_time << "   PID" << current_process->PID << endl;
		 }
//		 else { cout << "PAUSE\n";  }

		current_time++;
	}
	
	cout << "Scheduling Algorithm : FIFO\n";
	cout << "Number of CPU = " << 1 << endl;
	cout << "Total Execution Time = " << current_time << endl;
	cout << "Number of Processes Executed = " << total_process << endl;
	cout << "Average Return Time = " << total_return_time / total_process << endl;
	cout << "Average Wait Time = " << total_wait_time / total_process << endl;
	cout << "Average Response Time = " << total_response_time / total_process << endl;
}

//int main()
//{
//	FIFO();
//	//GenerateMemory();//
//}
