#pragma once

namespace Serialization
{
    static constexpr std::uint32_t SerializationVersion = 1;
    static constexpr std::uint32_t ID                   = 'BBLT';
    static constexpr std::uint32_t SerializationType    = 'BBLR';

    inline void SaveCallback(SKSE::SerializationInterface* a_skse)
    {
        if (!a_skse->OpenRecord(SerializationType, SerializationVersion)) {
            logger::error("Failed to open blade and blunt record");
            return;
        }

    }

    inline void LoadCallback(SKSE::SerializationInterface* a_skse)
    {
        std::uint32_t type;
        std::uint32_t version;
        std::uint32_t length;
        a_skse->GetNextRecordInfo(type, version, length);


        if (type != SerializationType) {
            return;
        }

        if (version != SerializationVersion) {
            logger::error("Unable to load data");
            return;
        }

        float deserializedVal;
        if (!a_skse->ReadRecordData(deserializedVal)) {
            logger::error("Failed to load size");
            return;
        }

    }

} // namespace Serialization
