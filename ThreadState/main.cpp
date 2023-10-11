#include "ThreadState.h"
#include <iostream>

void TestThread() {
	Sleep(250);
	while (true) {
		_mm_pause();
	}
}

void PrintThreadState(HANDLE hThread) {
	static int CallCount = 0;
	std::cout << "State " << CallCount << " -- Thread sleeping : " << IsThreadSleeping(hThread) << "\n";
	std::cout << "State " << CallCount << " -- Thread suspended: " << IsThreadSuspended(hThread) << "\n\n";
	CallCount++;
}

int main() {
	// create thread in suspended state and print its state
	HANDLE hThread = CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)TestThread, nullptr, CREATE_SUSPENDED, nullptr);
	if (!hThread || hThread == INVALID_HANDLE_VALUE) {
		std::cout << "Failed to create thread for testing!";
		getc(stdin);
		return 1;
	}
	else {
		PrintThreadState(hThread);
	}

	// resume it and wait for the thread to start, then print state again
	ResumeThread(hThread);
	Sleep(1);
	PrintThreadState(hThread);

	// wait for it to finish sleeping, then print state one last time
	Sleep(500);
	PrintThreadState(hThread);
	std::cout << "Tests complete. Press \"Enter\" to exit the console.";
	getc(stdin);
	return 0;
}