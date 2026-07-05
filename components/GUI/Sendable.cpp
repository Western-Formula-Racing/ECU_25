#include "Sendable.h"

template <class T>
void TelemetryPacket::pack(const char* key, T new_val) 
{
    memset(id, 0, sizeof(id));
    strncpy(id, key, sizeof(id) - 1); // Ensure null-termination

    if constexpr (std::is_same_v<T, int>) {
        type = static_cast<uint8_t>(SendableDataType::INT);
        value.i = new_val;
    } else if constexpr (std::is_same_v<T, bool>) {
        type = static_cast<uint8_t>(SendableDataType::BOOL);
        value.b = new_val ? 1 : 0;
    } else if constexpr (std::is_same_v<T, uint32_t>) {
        type = static_cast<uint8_t>(SendableDataType::UINT);
        value.ui = new_val;
    } else if constexpr (std::is_same_v<T, double>) {
        type = static_cast<uint8_t>(SendableDataType::DOUBLE);
        value.d = new_val;
    } else if constexpr (std::is_same_v<T, float>) {
        type = static_cast<uint8_t>(SendableDataType::FLOAT);
        value.f = new_val;
    } else if constexpr (std::is_same_v<T, const char*> || std::is_same_v<T, char*>) {
        type = static_cast<uint8_t>(SendableDataType::STRING);
        memset(value.s, 0, sizeof(value.s));
        strncpy(value.s, new_val, sizeof(value.s) - 1); // Ensure null-termination
    }
}

template <class T>
const uint8_t* Sendable<T>::get_serialized_data()
{
    return reinterpret_cast<const uint8_t*>(&_packet);
}

// Explicit template instantiations for the types used by the application
template class Sendable<int>;
template class Sendable<bool>;
template class Sendable<uint32_t>;
template class Sendable<float>;
template class Sendable<double>;
template class Sendable<char*>;
template void TelemetryPacket::pack<int>(const char* key, int new_val);
template void TelemetryPacket::pack<bool>(const char* key, bool new_val);
template void TelemetryPacket::pack<uint32_t>(const char* key, uint32_t new_val);
template void TelemetryPacket::pack<float>(const char* key, float new_val);
template void TelemetryPacket::pack<double>(const char* key, double new_val);
template void TelemetryPacket::pack<char*>(const char* key, char* new_val);
template void TelemetryPacket::pack<const char*>(const char* key, const char* new_val);