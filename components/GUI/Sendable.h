#ifndef _SENDABLE_
#define _SENDABLE_
#include <cstring>
#include <type_traits>

enum class SendableDataType : uint8_t {
    INT = 0,
    BOOL = 1,
    UINT = 2,
    FLOAT = 3,
    DOUBLE = 4,
    STRING = 5,
};

#pragma pack(push, 1)
struct TelemetryPacket {
    protected:
        char id[32]; // 32 bytes for ID (null-terminated string)
        uint8_t type; // 1 byte for data type
        union 
        {
            int i; // 4 bytes
            uint8_t b; // 1 byte
            uint32_t ui; // 4 bytes
            float f; // 4 bytes
            double d; // 8 bytes
            char s[32]; // 32 bytes (max string length of 31 + null terminator)
        } value;
    public:
        template <typename T>
        void pack(const char* key, T new_val);
};
#pragma pack(pop)
// Total Size: 32 (id) + 1 (type) + 32 (value) = 65 bytes

class BaseSendable {
public:
    virtual ~BaseSendable() = default;
    virtual void refresh() = 0;
    virtual const uint8_t* get_serialized_data() = 0;
};

template <class T>
class Sendable : public BaseSendable
{
private:
    char* key = new char[32];  // 32 bytes for key (null-terminated string)
    TelemetryPacket _packet;
    T (*value_fn)();
public:
    // Constructor
    Sendable(const char* key, T (*fn)()) : value_fn(fn)
    {
        strncpy(this->key, key, 31); // Ensure null-termination
        this->key[31] = '\0'; // Ensure null termination
    }

    void refresh() override
    {
        T value = value_fn();
        _packet.pack(key, value); // Use the provided key
    }

    inline char* get_key() {
        return key;
    }

    // Serializes data to byte array
    const uint8_t* get_serialized_data() override;
};

#endif