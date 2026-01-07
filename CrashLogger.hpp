#pragma once

void printRegisters(CONTEXT* context);
void printStackTrace(CONTEXT* context);
LONG WINAPI exceptionHandler(EXCEPTION_POINTERS* exceptionInfo);
void setupExceptionHandler();