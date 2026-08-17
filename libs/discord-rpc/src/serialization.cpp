#include "serialization.h"
#include "discord_rpc.h"
#include <string_view>
#include <cctype>
#include <cstdlib>

class JsonWriter {
    char* dest_;
    size_t maxLen_;
    size_t pos_;
    std::vector<bool> state_;

    void put(char c) { 
        if (pos_ < maxLen_ - 1) dest_[pos_++] = c; 
    }
    void put(std::string_view s) {
        for (char c : s) {
            if (pos_ < maxLen_ - 1) dest_[pos_++] = c;
        }
    }
    void comma() {
        if (!state_.back()) put(',');
        state_.back() = false;
    }

public:
    JsonWriter(char* dest, size_t maxLen) : dest_(dest), maxLen_(maxLen), pos_(0) {
        state_.push_back(true);
    }

    size_t Size() {
        dest_[pos_] = '\0';
        return pos_;
    }

    void StartObject() { comma(); put('{'); state_.push_back(true); }
    void EndObject() { put('}'); state_.pop_back(); }

    void StartArray() { comma(); put('['); state_.push_back(true); }
    void EndArray() { put(']'); state_.pop_back(); }

    void Key(std::string_view k) {
        comma(); put('"'); put(k); put("\":");
        state_.back() = true;
    }

    void String(std::string_view s) {
        comma(); put('"');
        for (char c : s) {
            if (c == '"') put("\\\"");
            else if (c == '\\') put("\\\\");
            else put(c);
        }
        put('"');
    }

    void Int(int i) { comma(); put(std::to_string(i)); }
    void Int64(int64_t i) { comma(); put(std::to_string(i)); }
    void Bool(bool b) { comma(); put(b ? "true" : "false"); }
};

struct WriteObject {
    JsonWriter& w;
    WriteObject(JsonWriter& writer) : w(writer) { w.StartObject(); }
    WriteObject(JsonWriter& writer, std::string_view name) : w(writer) { w.Key(name); w.StartObject(); }
    ~WriteObject() { w.EndObject(); }
};

struct WriteArray {
    JsonWriter& w;
    WriteArray(JsonWriter& writer, std::string_view name) : w(writer) { w.Key(name); w.StartArray(); }
    ~WriteArray() { w.EndArray(); }
};

static void WriteOptionalString(JsonWriter& w, std::string_view k, const char* value) {
    if (value && value[0]) {
        w.Key(k);
        w.String(value);
    }
}

static void JsonWriteNonce(JsonWriter& writer, int nonce) {
    writer.Key("nonce");
    writer.String(std::to_string(nonce));
}

size_t JsonWriteRichPresenceObj(char* dest, size_t maxLen, int nonce, int pid, const DiscordRichPresence* presence) {
    JsonWriter writer(dest, maxLen);
    {
        WriteObject top(writer);
        JsonWriteNonce(writer, nonce);
        writer.Key("cmd");
        writer.String("SET_ACTIVITY");

        {
            WriteObject args(writer, "args");
            writer.Key("pid");
            writer.Int(pid);

            if (presence != nullptr) {
                WriteObject activity(writer, "activity");

                WriteOptionalString(writer, "state", presence->state);
                WriteOptionalString(writer, "details", presence->details);

                if (presence->startTimestamp || presence->endTimestamp) {
                    WriteObject timestamps(writer, "timestamps");
                    if (presence->startTimestamp) {
                        writer.Key("start");
                        writer.Int64(presence->startTimestamp);
                    }
                    if (presence->endTimestamp) {
                        writer.Key("end");
                        writer.Int64(presence->endTimestamp);
                    }
                }

                if ((presence->largeImageKey && presence->largeImageKey[0]) ||
                    (presence->largeImageText && presence->largeImageText[0]) ||
                    (presence->smallImageKey && presence->smallImageKey[0]) ||
                    (presence->smallImageText && presence->smallImageText[0])) {
                    WriteObject assets(writer, "assets");
                    WriteOptionalString(writer, "large_image", presence->largeImageKey);
                    WriteOptionalString(writer, "large_text", presence->largeImageText);
                    WriteOptionalString(writer, "small_image", presence->smallImageKey);
                    WriteOptionalString(writer, "small_text", presence->smallImageText);
                }

                if ((presence->partyId && presence->partyId[0]) || presence->partySize ||
                    presence->partyMax || presence->partyPrivacy) {
                    WriteObject party(writer, "party");
                    WriteOptionalString(writer, "id", presence->partyId);
                    if (presence->partySize && presence->partyMax) {
                        WriteArray size(writer, "size");
                        writer.Int(presence->partySize);
                        writer.Int(presence->partyMax);
                    }
                    if (presence->partyPrivacy) {
                        writer.Key("privacy");
                        writer.Int(presence->partyPrivacy);
                    }
                }

                if ((presence->matchSecret && presence->matchSecret[0]) ||
                    (presence->joinSecret && presence->joinSecret[0]) ||
                    (presence->spectateSecret && presence->spectateSecret[0])) {
                    WriteObject secrets(writer, "secrets");
                    WriteOptionalString(writer, "match", presence->matchSecret);
                    WriteOptionalString(writer, "join", presence->joinSecret);
                    WriteOptionalString(writer, "spectate", presence->spectateSecret);
                }

                writer.Key("instance");
                writer.Bool(presence->instance != 0);
            }
        }
    }
    return writer.Size();
}

size_t JsonWriteHandshakeObj(char* dest, size_t maxLen, int version, const char* applicationId) {
    JsonWriter writer(dest, maxLen);
    {
        WriteObject obj(writer);
        writer.Key("v");
        writer.Int(version);
        writer.Key("client_id");
        writer.String(applicationId);
    }
    return writer.Size();
}

size_t JsonWriteSubscribeCommand(char* dest, size_t maxLen, int nonce, const char* evtName) {
    JsonWriter writer(dest, maxLen);
    {
        WriteObject obj(writer);
        JsonWriteNonce(writer, nonce);
        writer.Key("cmd");
        writer.String("SUBSCRIBE");
        writer.Key("evt");
        writer.String(evtName);
    }
    return writer.Size();
}

size_t JsonWriteUnsubscribeCommand(char* dest, size_t maxLen, int nonce, const char* evtName) {
    JsonWriter writer(dest, maxLen);
    {
        WriteObject obj(writer);
        JsonWriteNonce(writer, nonce);
        writer.Key("cmd");
        writer.String("UNSUBSCRIBE");
        writer.Key("evt");
        writer.String(evtName);
    }
    return writer.Size();
}

size_t JsonWriteJoinReply(char* dest, size_t maxLen, const char* userId, int reply, int nonce) {
    JsonWriter writer(dest, maxLen);
    {
        WriteObject obj(writer);
        writer.Key("cmd");
        if (reply == DISCORD_REPLY_YES) {
            writer.String("SEND_ACTIVITY_JOIN_INVITE");
        } else {
            writer.String("CLOSE_ACTIVITY_JOIN_REQUEST");
        }

        writer.Key("args");
        {
            WriteObject args(writer);
            writer.Key("user_id");
            writer.String(userId);
        }
        JsonWriteNonce(writer, nonce);
    }
    return writer.Size();
}

static const char* SkipWhitespace(const char* p) {
    while (*p && std::isspace(static_cast<unsigned char>(*p))) p++;
    return p;
}

static const char* ParseValue(const char* p, JsonValue& val) {
    p = SkipWhitespace(p);
    if (!*p) return p;

    if (*p == '{') {
        val.type = JsonValue::Type::Object;
        p++;
        while (*p) {
            p = SkipWhitespace(p);
            if (*p == '}') { p++; break; }
            if (*p == ',') { p++; p = SkipWhitespace(p); continue; }

            if (*p == '"') {
                std::string key;
                p++;
                while (*p && *p != '"') key += *p++;
                if (*p == '"') p++;

                p = SkipWhitespace(p);
                if (*p == ':') p++;

                JsonValue member;
                p = ParseValue(p, member);
                val.object_val[key] = std::move(member);
            } else {
                p++;
            }
        }
    } else if (*p == '[') {
        val.type = JsonValue::Type::Array;
        p++;
        while (*p) {
            p = SkipWhitespace(p);
            if (*p == ']') { p++; break; }
            if (*p == ',') { p++; p = SkipWhitespace(p); continue; }

            JsonValue item;
            p = ParseValue(p, item);
            val.array_val.push_back(std::move(item));
        }
    } else if (*p == '"') {
        val.type = JsonValue::Type::String;
        p++;
        while (*p && *p != '"') {
            if (*p == '\\' && *(p + 1) == '"') { val.str_val += '"'; p += 2; }
            else if (*p == '\\' && *(p + 1) == '\\') { val.str_val += '\\'; p += 2; }
            else { val.str_val += *p++; }
        }
        if (*p == '"') p++;
    } else if (std::isdigit(static_cast<unsigned char>(*p)) || *p == '-') {
        val.type = JsonValue::Type::Int;
        val.int_val = std::atoi(p);
        if (*p == '-') p++;
        while (std::isdigit(static_cast<unsigned char>(*p))) p++;
    } else if (std::strncmp(p, "true", 4) == 0) {
        val.type = JsonValue::Type::Bool; val.bool_val = true; p += 4;
    } else if (std::strncmp(p, "false", 5) == 0) {
        val.type = JsonValue::Type::Bool; val.bool_val = false; p += 5;
    } else if (std::strncmp(p, "null", 4) == 0) {
        val.type = JsonValue::Type::Null; p += 4;
    } else {
        p++;
    }
    return p;
}

void JsonDocument::ParseInsitu(char* json) {
    ParseValue(json, *this);
}