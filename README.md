# ThreadState
Get the state of a running thread by using the undocumented ThreadSystemThreadInformation enum of NtQueryInformationThread on Windows via a handle to the thread.

Why?
The only other way to retrieve this information is through NtQuerySystemInformation with SystemProcessInformation. This call takes a very long time to complete, an average of about ~2ms on my system, whereas the one call to NtQueryInformationThread was <0.001ms.

How do I use this?
You can freely use and modify the header file provided to suit your own needs. The code was tested using Microsoft Visual Studio on a Windows 10 x64 machine, and it should work for (most) versions of Windows. As this function is undocumented, it may lose functionality at any time.
