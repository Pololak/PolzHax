#pragma once
#include <stdlib.h>

int GetProcessId();

struct BaseConnection {
    static BaseConnection* Create();
    static void Destroy(BaseConnection*&);
    bool isOpen{false};
    bool Open();
    bool Close();
    bool Write(const void* data, size_t length);
    bool Read(void* data, size_t length);
};
