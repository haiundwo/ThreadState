#pragma once

#include <Windows.h>
#include <ntstatus.h>
#include <winternl.h>
#pragma comment(lib, "ntdll.lib")

typedef struct SYSTEM_THREAD_INFORMATION_FULL {
    LARGE_INTEGER KernelTime; // 0x0
    LARGE_INTEGER UserTime; // 0x8
    LARGE_INTEGER CreateTime; // 0x10
    ULONG WaitTime; // 0x18
    PVOID StartAddress; // 0x20
    CLIENT_ID ClientId; // 0x28
    LONG Priority; // 0x38
    LONG BasePriority; // 0x3C
    ULONG ContextSwitches; // 0x40
    ULONG ThreadState; // 0x44
    ULONG WaitReason; // 0x48
} SYSTEM_THREAD_INFORMATION_FULL, * PSYSTEM_THREAD_INFORMATION_FULL;

typedef enum _KWAIT_REASON {
    Executive = 0,
    FreePage = 1,
    PageIn = 2,
    PoolAllocation = 3,
    DelayExecution = 4,
    Suspended = 5,
    UserRequest = 6,
    WrExecutive = 7,
    WrFreePage = 8,
    WrPageIn = 9,
    WrPoolAllocation = 10,
    WrDelayExecution = 11,
    WrSuspended = 12,
    WrUserRequest = 13,
    WrEventPair = 14,
    WrQueue = 15,
    WrLpcReceive = 16,
    WrLpcReply = 17,
    WrVirtualMemory = 18,
    WrPageOut = 19,
    WrRendezvous = 20,
    Spare2 = 21,
    Spare3 = 22,
    Spare4 = 23,
    Spare5 = 24,
    WrCalloutStack = 25,
    WrKernel = 26,
    WrResource = 27,
    WrPushLock = 28,
    WrMutex = 29,
    WrQuantumEnd = 30,
    WrDispatchInt = 31,
    WrPreempted = 32,
    WrYieldExecution = 33,
    WrFastMutex = 34,
    WrGuardedMutex = 35,
    WrRundown = 36,
    MaximumWaitReason = 37
} KWAIT_REASON;

bool QueryThreadInformation(HANDLE hThread, PSYSTEM_THREAD_INFORMATION_FULL pInformation) {
	DWORD Size = sizeof(SYSTEM_THREAD_INFORMATION_FULL);
	NTSTATUS Status = NtQueryInformationThread(hThread, (THREADINFOCLASS)0x28 /*ThreadSystemThreadInformation*/, pInformation, Size, &Size);
	return Status == STATUS_SUCCESS && Size == sizeof(SYSTEM_THREAD_INFORMATION_FULL);
}

bool IsThreadSuspended(HANDLE hThread) {
    SYSTEM_THREAD_INFORMATION_FULL TI;
	if (!QueryThreadInformation(hThread, &TI)) return false;
    return TI.WaitReason == KWAIT_REASON::Suspended || TI.WaitReason == KWAIT_REASON::WrSuspended;
}

bool IsThreadSleeping(HANDLE hThread) {
    SYSTEM_THREAD_INFORMATION_FULL TI;
    if (!QueryThreadInformation(hThread, &TI)) return false;
    return TI.WaitReason == KWAIT_REASON::DelayExecution || TI.WaitReason == KWAIT_REASON::WrDelayExecution;
}

PVOID GetThreadStartAddress(HANDLE hThread) {
    SYSTEM_THREAD_INFORMATION_FULL TI;
    if (!QueryThreadInformation(hThread, &TI)) return nullptr;
    return TI.StartAddress;
}