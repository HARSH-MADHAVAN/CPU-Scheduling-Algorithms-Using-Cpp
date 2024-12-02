#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <climits>  // For INT_MAX

using namespace std;

// Structure for Process
struct Process {
    int pid;        // Process ID
    int arrival;    // Arrival Time
    int burst;      // Burst Time
    int remaining;  // Remaining Time for SRT
    int completion; // Completion Time
    int waiting;    // Waiting Time
    int turnAround; // Turnaround Time

    Process(int id, int a, int b)
        : pid(id), arrival(a), burst(b), remaining(b), completion(0), waiting(0), turnAround(0) {}
};

// Function to implement FCFS Scheduling
void FCFS(vector<Process>& processes) {
    int n = processes.size();
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrival < b.arrival;
    });

    int time = 0;
    for (int i = 0; i < n; i++) {
        Process& p = processes[i];
        if (time < p.arrival) {
            time = p.arrival;
        }
        p.completion = time + p.burst;
        p.turnAround = p.completion - p.arrival;
        p.waiting = p.turnAround - p.burst;
        time = p.completion;
    }

    cout << "FCFS Scheduling:" << endl;
    for (const auto& p : processes) {
        cout << "Process " << p.pid << ": Waiting Time = " << p.waiting
             << ", Turnaround Time = " << p.turnAround << endl;
    }
}

// Function to implement Round Robin Scheduling
void RoundRobin(vector<Process>& processes, int quantum) {
    int n = processes.size();
    queue<Process*> q;
    int time = 0;
    int completed = 0;

    for (auto& p : processes) {
        p.remaining = p.burst;
    }

    while (completed < n) {
        // Add new processes to the queue
        for (auto& p : processes) {
            if (p.arrival <= time && p.remaining > 0 && find(q.front(), q.back(), &p) == q.back()) {
                q.push(&p);
            }
        }

        if (!q.empty()) {
            Process* p = q.front();
            q.pop();

            int execTime = min(quantum, p->remaining);
            p->remaining -= execTime;
            time += execTime;

            if (p->remaining == 0) {
                p->completion = time;
                p->turnAround = p->completion - p->arrival;
                p->waiting = p->turnAround - p->burst;
                completed++;
            } else {
                q.push(p);
            }
        } else {
            time++;
        }
    }

    cout << "Round Robin Scheduling (Quantum = " << quantum << "):" << endl;
    for (const auto& p : processes) {
        cout << "Process " << p.pid << ": Waiting Time = " << p.waiting
             << ", Turnaround Time = " << p.turnAround << endl;
    }
}

// Function to implement Shortest Process Next (SPN) Scheduling
void SPN(vector<Process>& processes) {
    int n = processes.size();
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrival < b.arrival;
    });

    vector<bool> finished(n, false);
    int time = 0;
    int completed = 0;

    while (completed < n) {
        int idx = -1;
        int minBurst = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!finished[i] && processes[i].arrival <= time && processes[i].burst < minBurst) {
                minBurst = processes[i].burst;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
        } else {
            Process& p = processes[idx];
            p.completion = time + p.burst;
            p.turnAround = p.completion - p.arrival;
            p.waiting = p.turnAround - p.burst;
            finished[idx] = true;
            time = p.completion;
            completed++;
        }
    }

    cout << "Shortest Process Next (SPN) Scheduling:" << endl;
    for (const auto& p : processes) {
        cout << "Process " << p.pid << ": Waiting Time = " << p.waiting
             << ", Turnaround Time = " << p.turnAround << endl;
    }
}

// Function to implement Shortest Remaining Time (SRT) Scheduling
void SRT(vector<Process>& processes) {
    int n = processes.size();
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrival < b.arrival;
    });

    vector<bool> finished(n, false);
    int time = 0;
    int completed = 0;

    while (completed < n) {
        int idx = -1;
        int minRemaining = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (!finished[i] && processes[i].arrival <= time && processes[i].remaining < minRemaining) {
                minRemaining = processes[i].remaining;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
        } else {
            Process& p = processes[idx];
            p.remaining--;
            time++;

            if (p.remaining == 0) {
                p.completion = time;
                p.turnAround = p.completion - p.arrival;
                p.waiting = p.turnAround - p.burst;
                finished[idx] = true;
                completed++;
            }
        }
    }

    cout << "Shortest Remaining Time (SRT) Scheduling:" << endl;
    for (const auto& p : processes) {
        cout << "Process " << p.pid << ": Waiting Time = " << p.waiting
             << ", Turnaround Time = " << p.turnAround << endl;
    }
}

int main() {
    vector<Process> processes = {
        Process(1, 0, 8),
        Process(2, 1, 4),
        Process(3, 2, 9),
        Process(4, 3, 5)
    };

    vector<Process> fcfs = processes;
    vector<Process> rr = processes;
    vector<Process> spn = processes;
    vector<Process> srt = processes;

    FCFS(fcfs);
    RoundRobin(rr, 4);
    SPN(spn);
    SRT(srt);

    return 0;
}
