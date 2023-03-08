#pragma once
#include <Windows.h>
#include "lazy.h"
#include "..\xor.hpp"
typedef NTSTATUS(__stdcall* _NtQueryInformationProcess)(_In_ HANDLE, _In_  unsigned int, _Out_ PVOID, _In_ ULONG, _Out_ PULONG);
typedef NTSTATUS(__stdcall* _SetInformationThread)(_In_ HANDLE, _In_ THREAD_INFORMATION_CLASS, _In_ PVOID, _In_ ULONG);
typedef NTSTATUS(__stdcall* _NtSetInformationThread)(_In_ HANDLE, _In_ THREAD_INFORMATION_CLASS, _In_ PVOID, _In_ ULONG);
typedef NTSTATUS(NTAPI* pdef_NtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask OPTIONAL, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response);
typedef NTSTATUS(NTAPI* pdef_RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);

void BSOD() {
	BOOLEAN bEnabled;
	ULONG uResp;
	LPVOID lpFuncAddress = GetProcAddress(LoadLibraryA(_("ntdll.dll").c_str()), _("RtlAdjustPrivilege").c_str());
	LPVOID lpFuncAddress2 = GetProcAddress(GetModuleHandle(_("ntdll.dll").c_str()), _("NtRaiseHardError").c_str());
	pdef_RtlAdjustPrivilege NtCall = (pdef_RtlAdjustPrivilege)lpFuncAddress;
	pdef_NtRaiseHardError NtCall2 = (pdef_NtRaiseHardError)lpFuncAddress2;
	NTSTATUS NtRet = NtCall(19, TRUE, FALSE, &bEnabled);
	NtCall2(STATUS_FLOAT_MULTIPLE_FAULTS, 0, 0, 0, 6, &uResp);
}
void searchDbg() {
	if (IsDebuggerPresent())
		BSOD();
	if (CheckRemoteDebuggerPresent(GetCurrentProcess(), FALSE))
		BSOD();
	if (FindWindowA(NULL, (_("The Wireshark Network Analyzer").c_str()))) { BSOD(); }
	if (FindWindowA(NULL, (_("Progress Telerik Fiddler Web Debugger").c_str()))) { BSOD(); }
	if (FindWindowA(NULL, (_("Fiddler").c_str()))) { BSOD(); }
	if (FindWindowA(NULL, (_("HTTP Debugger").c_str()))) { BSOD(); }
	if (FindWindowA(NULL, (_("x64dbg").c_str()))) { BSOD(); }
	if (FindWindowA(NULL, (_("dnSpy").c_str()))) { BSOD(); }
	if (FindWindowA(NULL, (_("FolderChangesView").c_str()))) { BSOD(); }
	if (FindWindowA(NULL, (_("BinaryNinja").c_str()))) { BSOD(); }
	if (FindWindowA(NULL, (_("HxD").c_str()))) { BSOD(); }
	if (FindWindowA(NULL, (_("Cheat Engine 7.4").c_str()))) { BSOD(); }
	if (FindWindowA(NULL, (_("Cheat Engine 7.2").c_str()))) { BSOD(); }
	if (FindWindowA(NULL, (_("Cheat Engine 7.1").c_str()))) { BSOD(); }
	if (FindWindowA(NULL, (_("Cheat Engine 7.0").c_str()))) { BSOD(); }
	if (FindWindowA(NULL, (_("Cheat Engine 6.9").c_str()))) { BSOD(); }
	if (FindWindowA(NULL, (_("Cheat Engine 6.8").c_str()))) { BSOD(); }
	if (FindWindowA(NULL, (_("Ida").c_str()))) { BSOD(); }
	if (FindWindowA(NULL, (_("Ida Pro").c_str()))) { BSOD(); }
	if (FindWindowA(NULL, (_("Ida Freeware").c_str()))) { BSOD(); }
	if (FindWindowA(NULL, (_("HTTP Debugger Pro").c_str()))) { BSOD(); }
	if (FindWindowA(NULL, (_("Process Hacker").c_str()))) { BSOD(); }
	if (FindWindowA(NULL, (_("Process Hacker 2").c_str()))) { BSOD(); }
	if (FindWindowA(NULL, (_("OllyDbg").c_str()))) { BSOD(); }
}


int debugstring()
{
	LI_FN(SetLastError).safe()(0);
	const auto last_error = LI_FN(GetLastError).safe()();

	return last_error != 0;
}

int hidethread()
{
	unsigned long thread_hide_from_debugger = 0x11;

	const auto ntdll = LI_FN(LoadLibraryA).safe()(_("ntdll.dll").c_str());

	if (ntdll == INVALID_HANDLE_VALUE || ntdll == NULL) { return false; }

	_NtQueryInformationProcess NtQueryInformationProcess = NULL;
	NtQueryInformationProcess = (_NtQueryInformationProcess)LI_FN(GetProcAddress).safe()(ntdll, _("NtQueryInformationProcess").c_str());

	if (NtQueryInformationProcess == NULL) { return false; }

	(_NtSetInformationThread)(GetCurrentThread(), thread_hide_from_debugger, 0, 0, 0);

	return true;
}

int remotepresent()
{
	int debugger_present = false;

	LI_FN(CheckRemoteDebuggerPresent).safe()(LI_FN(GetCurrentProcess).safe()(), &debugger_present);

	return debugger_present;
}

int contextthread()
{
	int found = false;
	CONTEXT ctx = { 0 };
	void* h_thread = LI_FN(GetCurrentThread).safe()();

	ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	if (LI_FN(GetThreadContext).safe()(h_thread, &ctx))
	{
		if ((ctx.Dr0 != 0x00) || (ctx.Dr1 != 0x00) || (ctx.Dr2 != 0x00) || (ctx.Dr3 != 0x00) || (ctx.Dr6 != 0x00) || (ctx.Dr7 != 0x00))
		{
			found = true;
		}
	}

	return found;
}