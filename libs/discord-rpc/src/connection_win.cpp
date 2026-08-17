#include "connection.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int GetProcessId()
{
    return static_cast<int>(::GetCurrentProcessId());
}

struct BaseConnectionWin : public BaseConnection {
    HANDLE pipe{INVALID_HANDLE_VALUE};
};

static BaseConnectionWin Connection;

BaseConnection* BaseConnection::Create()
{
    return &Connection;
}

void BaseConnection::Destroy(BaseConnection*& c)
{
    if (c) {
        c->Close();
        c = nullptr;
    }
}

bool BaseConnection::Open()
{
    wchar_t pipeName[] = L"\\\\?\\pipe\\discord-ipc-0";
    constexpr size_t pipeDigit = (sizeof(pipeName) / sizeof(wchar_t)) - 2;
    auto* self = reinterpret_cast<BaseConnectionWin*>(this);

    for (wchar_t digit = L'0'; digit <= L'9';) {
        pipeName[pipeDigit] = digit;
        
        self->pipe = ::CreateFileW(pipeName, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
        if (self->pipe != INVALID_HANDLE_VALUE) {
            self->isOpen = true;
            return true;
        }

        DWORD err = ::GetLastError();
        if (err == ERROR_FILE_NOT_FOUND) {
            digit++;
        } 
        else if (err == ERROR_PIPE_BUSY) {
            if (!::WaitNamedPipeW(pipeName, 10000)) {
                return false;
            }
        } 
        else {
            return false;
        }
    }
    return false;
}

bool BaseConnection::Close()
{
    auto* self = reinterpret_cast<BaseConnectionWin*>(this);
    if (self->pipe != INVALID_HANDLE_VALUE) {
        ::CloseHandle(self->pipe);
        self->pipe = INVALID_HANDLE_VALUE;
    }
    self->isOpen = false;
    return true;
}

bool BaseConnection::Write(const void* data, size_t length)
{
    if (length == 0) return true;
    if (!data) return false;

    auto* self = reinterpret_cast<BaseConnectionWin*>(this);
    if (self->pipe == INVALID_HANDLE_VALUE) return false;

    DWORD bytesWritten = 0;
    const DWORD bytesLength = static_cast<DWORD>(length);
    
    return ::WriteFile(self->pipe, data, bytesLength, &bytesWritten, nullptr) && bytesWritten == bytesLength;
}

bool BaseConnection::Read(void* data, size_t length)
{
    if (!data) return false;

    auto* self = reinterpret_cast<BaseConnectionWin*>(this);
    if (self->pipe == INVALID_HANDLE_VALUE) return false;

    DWORD bytesAvailable = 0;
    if (!::PeekNamedPipe(self->pipe, nullptr, 0, nullptr, &bytesAvailable, nullptr)) {
        Close();
        return false;
    }

    if (bytesAvailable < length) {
        return false;
    }

    DWORD bytesRead = 0;
    if (::ReadFile(self->pipe, data, static_cast<DWORD>(length), &bytesRead, nullptr)) {
        return bytesRead == static_cast<DWORD>(length);
    }

    Close();
    return false;
}