#include "GlobalClipboard.h"

char* g_clipboard;

void saveClipboard(EditorUI* self) {
    auto addr = reinterpret_cast<uintptr_t>(self) + 0x264;
    auto str_len = *reinterpret_cast<size_t*>(addr + 16);
    if (str_len) {
        char* str_buf;
        if (str_len < 16) {
            str_buf = reinterpret_cast<char*>(addr);
        }
        else {
            str_buf = *reinterpret_cast<char**>(addr);
        }
        g_clipboard = reinterpret_cast<char*>(realloc(g_clipboard, str_len + 1));
        memcpy(g_clipboard, str_buf, str_len + 1);
    }
}

void loadClipboard(EditorUI* self) {
	if (g_clipboard && g_clipboard[0]) {
		auto clipboard = reinterpret_cast<uintptr_t>(self) + 0x264;
		auto len = strlen(g_clipboard);
		*reinterpret_cast<size_t*>(clipboard + 16) = len; // size
		*reinterpret_cast<size_t*>(clipboard + 20) = (std::max)(static_cast<int>(len), 15); // capacity
		if (len <= 15) {
			memcpy(reinterpret_cast<char*>(clipboard), g_clipboard, len + 1);
		}
		else {
			void* newb = malloc(len + 1);
			memcpy(newb, g_clipboard, len + 1);
			*reinterpret_cast<void**>(clipboard) = newb;
		}
		self->updateButtons();
	}
}