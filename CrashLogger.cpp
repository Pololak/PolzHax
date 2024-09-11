#include <windows.h>
#include <dbghelp.h>
#include <iostream>
#include <sstream>
#include <ctime>
#include "CrashLogger.hpp"

std::ostringstream crashInfo;

void printRegisters(CONTEXT* context) {
    crashInfo << "=== Registers ===\n";
    crashInfo << "EIP: " << std::hex << context->Eip << "\n";
    crashInfo << "ESP: " << std::hex << context->Esp << "\n";
    crashInfo << "EBP: " << std::hex << context->Ebp << "\n";
    crashInfo << "EAX: " << std::hex << context->Eax << "\n";
    crashInfo << "EBX: " << std::hex << context->Ebx << "\n";
    crashInfo << "ECX: " << std::hex << context->Ecx << "\n";
    crashInfo << "EDX: " << std::hex << context->Edx << "\n";
    crashInfo << "ESI: " << std::hex << context->Esi << "\n";
    crashInfo << "EDI: " << std::hex << context->Edi << "\n\n";
}

void printStackTrace(CONTEXT* context) {
    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();

    SymInitialize(process, NULL, TRUE);

    STACKFRAME64 stackFrame;
    memset(&stackFrame, 0, sizeof(STACKFRAME64));

    DWORD machineType;
#ifdef _M_X64
    machineType = IMAGE_FILE_MACHINE_AMD64;
    stackFrame.AddrPC.Offset = context->Rip;
    stackFrame.AddrPC.Mode = AddrModeFlat;
    stackFrame.AddrFrame.Offset = context->Rsp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;
    stackFrame.AddrStack.Offset = context->Rsp;
    stackFrame.AddrStack.Mode = AddrModeFlat;
#elif _M_IX86
    machineType = IMAGE_FILE_MACHINE_I386;
    stackFrame.AddrPC.Offset = context->Eip;
    stackFrame.AddrPC.Mode = AddrModeFlat;
    stackFrame.AddrFrame.Offset = context->Ebp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;
    stackFrame.AddrStack.Offset = context->Esp;
    stackFrame.AddrStack.Mode = AddrModeFlat;
#endif

    crashInfo << "=== Stack trace ===\n";
    while (StackWalk64(machineType, process, thread, &stackFrame, context, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL)) {
        DWORD64 address = stackFrame.AddrPC.Offset;
        DWORD64 moduleBase = SymGetModuleBase64(process, address);

        crashInfo << "Address: " << std::hex << address;

        // Получение информации о символе
        char symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
        SYMBOL_INFO* symbol = reinterpret_cast<SYMBOL_INFO*>(symbolBuffer);
        symbol->MaxNameLen = MAX_SYM_NAME;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

        DWORD64 displacement = 0;
        if (SymFromAddr(process, address, &displacement, symbol)) {
            crashInfo << " (" << symbol->Name << "+" << std::hex << displacement << ")";
        }
        else {
            crashInfo << " (No symbol)";
        }

        // Получение информации о модуле
        if (moduleBase) {
            IMAGEHLP_MODULE64 moduleInfo;
            memset(&moduleInfo, 0, sizeof(IMAGEHLP_MODULE64));
            moduleInfo.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
            if (SymGetModuleInfo64(process, moduleBase, &moduleInfo)) {
                crashInfo << " [" << moduleInfo.ModuleName << "+" << std::hex << (address - moduleBase) << "]";
            }
        }

        crashInfo << "\n";
    }

    SymCleanup(process);
}

LONG WINAPI exceptionHandler(EXCEPTION_POINTERS* exceptionInfo) {
    crashInfo << "Exception code: " << std::hex << exceptionInfo->ExceptionRecord->ExceptionCode << "\n";
    crashInfo << "Exception address: " << exceptionInfo->ExceptionRecord->ExceptionAddress << "\n\n";

    printRegisters(exceptionInfo->ContextRecord);
    printStackTrace(exceptionInfo->ContextRecord);

    MessageBoxA(NULL, crashInfo.str().c_str(), "Crash Info", MB_OK | MB_ICONERROR);

    return EXCEPTION_EXECUTE_HANDLER;
}

void setupExceptionHandler() {
    SetUnhandledExceptionFilter(exceptionHandler);
}