#pragma once
#include <cstddef>
#include <string>
#include <vector>
#include <map>

template <size_t Len>
inline size_t StringCopy(char (&dest)[Len], const char* src)
{
    if (!src || !Len) {
        return 0;
    }
    size_t copied;
    char* out = dest;
    for (copied = 1; *src && copied < Len; ++copied) {
        *out++ = *src++;
    }
    *out = 0;
    return copied - 1;
}

size_t JsonWriteHandshakeObj(char* dest, size_t maxLen, int version, const char* applicationId);

struct DiscordRichPresence;
size_t JsonWriteRichPresenceObj(char* dest, size_t maxLen, int nonce, int pid, const DiscordRichPresence* presence);
size_t JsonWriteSubscribeCommand(char* dest, size_t maxLen, int nonce, const char* evtName);
size_t JsonWriteUnsubscribeCommand(char* dest, size_t maxLen, int nonce, const char* evtName);
size_t JsonWriteJoinReply(char* dest, size_t maxLen, const char* userId, int reply, int nonce);

class JsonValue {
public:
    enum class Type { Null, Int, Bool, String, Array, Object };
    Type type = Type::Null;

    int int_val = 0;
    bool bool_val = false;
    std::string str_val;
    std::vector<JsonValue> array_val;
    std::map<std::string, JsonValue> object_val;

    bool IsObject() const { return type == Type::Object; }
    bool IsInt() const { return type == Type::Int; }
    bool IsString() const { return type == Type::String; }

    int GetInt() const { return int_val; }
    const char* GetString() const { return str_val.c_str(); }

    JsonValue* FindMember(const char* name) {
        if (type == Type::Object) {
            auto it = object_val.find(name);
            if (it != object_val.end()) return &it->second;
        }
        return nullptr;
    }
};

class JsonDocument : public JsonValue {
public:
    void ParseInsitu(char* json);
    bool HasParseError() const { return false; } 
};

inline JsonValue* GetObjMember(JsonValue* obj, const char* name)
{
    if (!obj) return nullptr;
    JsonValue* member = obj->FindMember(name);
    return (member && member->IsObject()) ? member : nullptr;
}

inline int GetIntMember(JsonValue* obj, const char* name, int notFoundDefault = 0)
{
    if (!obj) return notFoundDefault;
    JsonValue* member = obj->FindMember(name);
    return (member && member->IsInt()) ? member->GetInt() : notFoundDefault;
}

inline const char* GetStrMember(JsonValue* obj, const char* name, const char* notFoundDefault = nullptr)
{
    if (!obj) return notFoundDefault;
    JsonValue* member = obj->FindMember(name);
    return (member && member->IsString()) ? member->GetString() : notFoundDefault;
}