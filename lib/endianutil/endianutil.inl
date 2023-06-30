/*!
    \file endian.inl
    \brief Big/Little-endian utilities inline implementation
    \author Ivan Shynkarenka
    \date 21.07.2017
    \copyright MIT License
*/

namespace util
{


template <typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr>
std::vector<uint8_t> ConvertVariableSizeToByteArray(T const &data, bool bigEndian, size_t size)
{
    size_t offset = sizeof(data) - size;
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < size; i++)
    {
        uint8_t byte;
        if (Endian::IsLittleEndian())
        {
            byte = data >> (i * 8);
        }
        else
        {
            byte = data >> ((sizeof(data) - 1 - i) * 8);
        }
        if (bigEndian)
        {
            bytes.insert(bytes.begin(), byte);
        }
        else
        {
            bytes.push_back(byte);
        }
    }

    return bytes;
};

template <typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr>
std::vector<uint8_t> ConvertToByteArray(T const &data, bool bigEndian)
{
    std::vector<uint8_t> bytes;
    for (int i = 0; i < sizeof(data); i++)
    {
        uint8_t byte;
        if (Endian::IsLittleEndian())
        {
            byte = data >> (i * 8);
        }
        else
        {
            byte = data >> ((sizeof(data) - 1 - i) * 8);
        }
        if (bigEndian)
        {
            bytes.insert(bytes.begin(), byte);
        }
        else
        {
            bytes.push_back(byte);
        }
    }

    return bytes;
};

template <size_t N>
inline size_t ReadString(const void *buffer, char (&str)[N])
{
    std::memcpy(str, buffer, N);

    return N;
}

inline size_t ReadItchTimestamp(const void *buffer, uint64_t &value)
{
    if (Endian::IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = 0;
        (reinterpret_cast<uint8_t *>(&value))[1] = 0;
        (reinterpret_cast<uint8_t *>(&value))[2] = (reinterpret_cast<const uint8_t *>(buffer))[0];
        (reinterpret_cast<uint8_t *>(&value))[3] = (reinterpret_cast<const uint8_t *>(buffer))[1];
        (reinterpret_cast<uint8_t *>(&value))[4] = (reinterpret_cast<const uint8_t *>(buffer))[2];
        (reinterpret_cast<uint8_t *>(&value))[5] = (reinterpret_cast<const uint8_t *>(buffer))[3];
        (reinterpret_cast<uint8_t *>(&value))[6] = (reinterpret_cast<const uint8_t *>(buffer))[4];
        (reinterpret_cast<uint8_t *>(&value))[7] = (reinterpret_cast<const uint8_t *>(buffer))[5];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[5];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[4];
        (reinterpret_cast<uint8_t *>(&value))[2] = (reinterpret_cast<const uint8_t *>(buffer))[3];
        (reinterpret_cast<uint8_t *>(&value))[3] = (reinterpret_cast<const uint8_t *>(buffer))[2];
        (reinterpret_cast<uint8_t *>(&value))[4] = (reinterpret_cast<const uint8_t *>(buffer))[1];
        (reinterpret_cast<uint8_t *>(&value))[5] = (reinterpret_cast<const uint8_t *>(buffer))[0];
        (reinterpret_cast<uint8_t *>(&value))[6] = 0;
        (reinterpret_cast<uint8_t *>(&value))[7] = 0;
    }

    return 6;
}

inline uint64_t ReadItchKafkaTimestamp(const uint64_t timestamp, uint64_t &value)
{
    const uint64_t buffer = timestamp;
    if (Endian::IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = 0;
        (reinterpret_cast<uint8_t *>(&value))[1] = 0;
        (reinterpret_cast<uint8_t *>(&value))[2] = (reinterpret_cast<const uint8_t *>(&buffer))[2];
        (reinterpret_cast<uint8_t *>(&value))[3] = (reinterpret_cast<const uint8_t *>(&buffer))[3];
        (reinterpret_cast<uint8_t *>(&value))[4] = (reinterpret_cast<const uint8_t *>(&buffer))[4];
        (reinterpret_cast<uint8_t *>(&value))[5] = (reinterpret_cast<const uint8_t *>(&buffer))[5];
        (reinterpret_cast<uint8_t *>(&value))[6] = (reinterpret_cast<const uint8_t *>(&buffer))[6];
        (reinterpret_cast<uint8_t *>(&value))[7] = (reinterpret_cast<const uint8_t *>(&buffer))[7];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(&buffer))[0];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(&buffer))[1];
        (reinterpret_cast<uint8_t *>(&value))[2] = (reinterpret_cast<const uint8_t *>(&buffer))[2];
        (reinterpret_cast<uint8_t *>(&value))[3] = (reinterpret_cast<const uint8_t *>(&buffer))[3];
        (reinterpret_cast<uint8_t *>(&value))[4] = (reinterpret_cast<const uint8_t *>(&buffer))[4];
        (reinterpret_cast<uint8_t *>(&value))[5] = (reinterpret_cast<const uint8_t *>(&buffer))[5];
        (reinterpret_cast<uint8_t *>(&value))[6] = 0; //(reinterpret_cast<const uint8_t *>(&buffer))[6];
        (reinterpret_cast<uint8_t *>(&value))[7] = 0; //(reinterpret_cast<const uint8_t *>(&buffer))[7];
    }

    return value;
}


inline bool Endian::IsBigEndian()
{
    const unsigned one = 1U;
    return !(reinterpret_cast<const char *>(&one) + sizeof(unsigned) - 1);
    //return false;
}

inline bool Endian::IsLittleEndian()
{
    const unsigned one = 1U;
    return reinterpret_cast<const char *>(&one) + sizeof(unsigned) - 1;
    //return true;
}

inline size_t Endian::ReadBigEndian(const void *buffer, int16_t &value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[0];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[1];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[1];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[0];
    }

    return 2;
}

inline size_t Endian::ReadBigEndian(const void *buffer, uint16_t &value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[0];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[1];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[1];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[0];
    }

    return 2;
}

inline size_t Endian::ReadBigEndian(const void *buffer, int32_t &value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[0];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[1];
        (reinterpret_cast<uint8_t *>(&value))[2] = (reinterpret_cast<const uint8_t *>(buffer))[2];
        (reinterpret_cast<uint8_t *>(&value))[3] = (reinterpret_cast<const uint8_t *>(buffer))[3];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[3];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[2];
        (reinterpret_cast<uint8_t *>(&value))[2] = (reinterpret_cast<const uint8_t *>(buffer))[1];
        (reinterpret_cast<uint8_t *>(&value))[3] = (reinterpret_cast<const uint8_t *>(buffer))[0];
    }

    return 4;
}

inline size_t Endian::ReadBigEndian(const void *buffer, uint32_t &value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[0];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[1];
        (reinterpret_cast<uint8_t *>(&value))[2] = (reinterpret_cast<const uint8_t *>(buffer))[2];
        (reinterpret_cast<uint8_t *>(&value))[3] = (reinterpret_cast<const uint8_t *>(buffer))[3];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[3];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[2];
        (reinterpret_cast<uint8_t *>(&value))[2] = (reinterpret_cast<const uint8_t *>(buffer))[1];
        (reinterpret_cast<uint8_t *>(&value))[3] = (reinterpret_cast<const uint8_t *>(buffer))[0];
    }

    return 4;
}

inline size_t Endian::ReadBigEndian(const void *buffer, int64_t &value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[0];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[1];
        (reinterpret_cast<uint8_t *>(&value))[2] = (reinterpret_cast<const uint8_t *>(buffer))[2];
        (reinterpret_cast<uint8_t *>(&value))[3] = (reinterpret_cast<const uint8_t *>(buffer))[3];
        (reinterpret_cast<uint8_t *>(&value))[4] = (reinterpret_cast<const uint8_t *>(buffer))[4];
        (reinterpret_cast<uint8_t *>(&value))[5] = (reinterpret_cast<const uint8_t *>(buffer))[5];
        (reinterpret_cast<uint8_t *>(&value))[6] = (reinterpret_cast<const uint8_t *>(buffer))[6];
        (reinterpret_cast<uint8_t *>(&value))[7] = (reinterpret_cast<const uint8_t *>(buffer))[7];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[7];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[6];
        (reinterpret_cast<uint8_t *>(&value))[2] = (reinterpret_cast<const uint8_t *>(buffer))[5];
        (reinterpret_cast<uint8_t *>(&value))[3] = (reinterpret_cast<const uint8_t *>(buffer))[4];
        (reinterpret_cast<uint8_t *>(&value))[4] = (reinterpret_cast<const uint8_t *>(buffer))[3];
        (reinterpret_cast<uint8_t *>(&value))[5] = (reinterpret_cast<const uint8_t *>(buffer))[2];
        (reinterpret_cast<uint8_t *>(&value))[6] = (reinterpret_cast<const uint8_t *>(buffer))[1];
        (reinterpret_cast<uint8_t *>(&value))[7] = (reinterpret_cast<const uint8_t *>(buffer))[0];
    }

    return 8;
}

inline size_t Endian::ReadBigEndian(const void *buffer, uint64_t &value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[0];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[1];
        (reinterpret_cast<uint8_t *>(&value))[2] = (reinterpret_cast<const uint8_t *>(buffer))[2];
        (reinterpret_cast<uint8_t *>(&value))[3] = (reinterpret_cast<const uint8_t *>(buffer))[3];
        (reinterpret_cast<uint8_t *>(&value))[4] = (reinterpret_cast<const uint8_t *>(buffer))[4];
        (reinterpret_cast<uint8_t *>(&value))[5] = (reinterpret_cast<const uint8_t *>(buffer))[5];
        (reinterpret_cast<uint8_t *>(&value))[6] = (reinterpret_cast<const uint8_t *>(buffer))[6];
        (reinterpret_cast<uint8_t *>(&value))[7] = (reinterpret_cast<const uint8_t *>(buffer))[7];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[7];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[6];
        (reinterpret_cast<uint8_t *>(&value))[2] = (reinterpret_cast<const uint8_t *>(buffer))[5];
        (reinterpret_cast<uint8_t *>(&value))[3] = (reinterpret_cast<const uint8_t *>(buffer))[4];
        (reinterpret_cast<uint8_t *>(&value))[4] = (reinterpret_cast<const uint8_t *>(buffer))[3];
        (reinterpret_cast<uint8_t *>(&value))[5] = (reinterpret_cast<const uint8_t *>(buffer))[2];
        (reinterpret_cast<uint8_t *>(&value))[6] = (reinterpret_cast<const uint8_t *>(buffer))[1];
        (reinterpret_cast<uint8_t *>(&value))[7] = (reinterpret_cast<const uint8_t *>(buffer))[0];
    }

    return 8;
}

inline size_t Endian::ReadLittleEndian(const void *buffer, int8_t &value)
{
    if (IsBigEndian())
    {
        ((int8_t *)&value)[0] = ((const int8_t *)buffer)[0];
    }
    else
    {
        ((int8_t *)&value)[0] = ((const int8_t *)buffer)[0];
    }

    return 1;
}

inline size_t Endian::ReadLittleEndian(const void *buffer, uint8_t &value)
{
    (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[0];
    return 1;
}

inline size_t Endian::ReadLittleEndian(const void *buffer, int16_t &value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[1];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[0];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[0];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[1];
    }

    return 2;
}

inline size_t Endian::ReadLittleEndian(const void *buffer, uint16_t &value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[1];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[0];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[0];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[1];
    }

    return 2;
}

inline size_t Endian::ReadLittleEndian(const void *buffer, int32_t &value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[3];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[2];
        (reinterpret_cast<uint8_t *>(&value))[2] = (reinterpret_cast<const uint8_t *>(buffer))[1];
        (reinterpret_cast<uint8_t *>(&value))[3] = (reinterpret_cast<const uint8_t *>(buffer))[0];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[0];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[1];
        (reinterpret_cast<uint8_t *>(&value))[2] = (reinterpret_cast<const uint8_t *>(buffer))[2];
        (reinterpret_cast<uint8_t *>(&value))[3] = (reinterpret_cast<const uint8_t *>(buffer))[3];
    }

    return 4;
}

inline size_t Endian::ReadLittleEndian(const void *buffer, uint32_t &value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[3];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[2];
        (reinterpret_cast<uint8_t *>(&value))[2] = (reinterpret_cast<const uint8_t *>(buffer))[1];
        (reinterpret_cast<uint8_t *>(&value))[3] = (reinterpret_cast<const uint8_t *>(buffer))[0];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[0];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[1];
        (reinterpret_cast<uint8_t *>(&value))[2] = (reinterpret_cast<const uint8_t *>(buffer))[2];
        (reinterpret_cast<uint8_t *>(&value))[3] = (reinterpret_cast<const uint8_t *>(buffer))[3];
    }

    return 4;
}

inline size_t Endian::ReadLittleEndian(const void *buffer, int64_t &value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[7];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[6];
        (reinterpret_cast<uint8_t *>(&value))[2] = (reinterpret_cast<const uint8_t *>(buffer))[5];
        (reinterpret_cast<uint8_t *>(&value))[3] = (reinterpret_cast<const uint8_t *>(buffer))[4];
        (reinterpret_cast<uint8_t *>(&value))[4] = (reinterpret_cast<const uint8_t *>(buffer))[3];
        (reinterpret_cast<uint8_t *>(&value))[5] = (reinterpret_cast<const uint8_t *>(buffer))[2];
        (reinterpret_cast<uint8_t *>(&value))[6] = (reinterpret_cast<const uint8_t *>(buffer))[1];
        (reinterpret_cast<uint8_t *>(&value))[7] = (reinterpret_cast<const uint8_t *>(buffer))[0];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[0];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[1];
        (reinterpret_cast<uint8_t *>(&value))[2] = (reinterpret_cast<const uint8_t *>(buffer))[2];
        (reinterpret_cast<uint8_t *>(&value))[3] = (reinterpret_cast<const uint8_t *>(buffer))[3];
        (reinterpret_cast<uint8_t *>(&value))[4] = (reinterpret_cast<const uint8_t *>(buffer))[4];
        (reinterpret_cast<uint8_t *>(&value))[5] = (reinterpret_cast<const uint8_t *>(buffer))[5];
        (reinterpret_cast<uint8_t *>(&value))[6] = (reinterpret_cast<const uint8_t *>(buffer))[6];
        (reinterpret_cast<uint8_t *>(&value))[7] = (reinterpret_cast<const uint8_t *>(buffer))[7];
    }

    return 8;
}

inline size_t Endian::ReadLittleEndian(const void *buffer, uint64_t &value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[7];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[6];
        (reinterpret_cast<uint8_t *>(&value))[2] = (reinterpret_cast<const uint8_t *>(buffer))[5];
        (reinterpret_cast<uint8_t *>(&value))[3] = (reinterpret_cast<const uint8_t *>(buffer))[4];
        (reinterpret_cast<uint8_t *>(&value))[4] = (reinterpret_cast<const uint8_t *>(buffer))[3];
        (reinterpret_cast<uint8_t *>(&value))[5] = (reinterpret_cast<const uint8_t *>(buffer))[2];
        (reinterpret_cast<uint8_t *>(&value))[6] = (reinterpret_cast<const uint8_t *>(buffer))[1];
        (reinterpret_cast<uint8_t *>(&value))[7] = (reinterpret_cast<const uint8_t *>(buffer))[0];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(&value))[0] = (reinterpret_cast<const uint8_t *>(buffer))[0];
        (reinterpret_cast<uint8_t *>(&value))[1] = (reinterpret_cast<const uint8_t *>(buffer))[1];
        (reinterpret_cast<uint8_t *>(&value))[2] = (reinterpret_cast<const uint8_t *>(buffer))[2];
        (reinterpret_cast<uint8_t *>(&value))[3] = (reinterpret_cast<const uint8_t *>(buffer))[3];
        (reinterpret_cast<uint8_t *>(&value))[4] = (reinterpret_cast<const uint8_t *>(buffer))[4];
        (reinterpret_cast<uint8_t *>(&value))[5] = (reinterpret_cast<const uint8_t *>(buffer))[5];
        (reinterpret_cast<uint8_t *>(&value))[6] = (reinterpret_cast<const uint8_t *>(buffer))[6];
        (reinterpret_cast<uint8_t *>(&value))[7] = (reinterpret_cast<const uint8_t *>(buffer))[7];
    }

    return 8;
}

inline size_t Endian::WriteBigEndian(void *buffer, int16_t value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[0];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[1];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[1];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[0];
    }

    return 2;
}

inline size_t Endian::WriteBigEndian(void *buffer, uint16_t value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[0];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[1];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[1];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[0];
    }

    return 2;
}

inline size_t Endian::WriteBigEndian(void *buffer, int32_t value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[0];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[1];
        (reinterpret_cast<uint8_t *>(buffer))[2] = (reinterpret_cast<const uint8_t *>(&value))[2];
        (reinterpret_cast<uint8_t *>(buffer))[3] = (reinterpret_cast<const uint8_t *>(&value))[3];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[3];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[2];
        (reinterpret_cast<uint8_t *>(buffer))[2] = (reinterpret_cast<const uint8_t *>(&value))[1];
        (reinterpret_cast<uint8_t *>(buffer))[3] = (reinterpret_cast<const uint8_t *>(&value))[0];
    }

    return 4;
}

inline size_t Endian::WriteBigEndian(void *buffer, uint32_t value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[0];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[1];
        (reinterpret_cast<uint8_t *>(buffer))[2] = (reinterpret_cast<const uint8_t *>(&value))[2];
        (reinterpret_cast<uint8_t *>(buffer))[3] = (reinterpret_cast<const uint8_t *>(&value))[3];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[3];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[2];
        (reinterpret_cast<uint8_t *>(buffer))[2] = (reinterpret_cast<const uint8_t *>(&value))[1];
        (reinterpret_cast<uint8_t *>(buffer))[3] = (reinterpret_cast<const uint8_t *>(&value))[0];
    }

    return 4;
}

inline size_t Endian::WriteBigEndian(void *buffer, int64_t value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[0];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[1];
        (reinterpret_cast<uint8_t *>(buffer))[2] = (reinterpret_cast<const uint8_t *>(&value))[2];
        (reinterpret_cast<uint8_t *>(buffer))[3] = (reinterpret_cast<const uint8_t *>(&value))[3];
        (reinterpret_cast<uint8_t *>(buffer))[4] = (reinterpret_cast<const uint8_t *>(&value))[4];
        (reinterpret_cast<uint8_t *>(buffer))[5] = (reinterpret_cast<const uint8_t *>(&value))[5];
        (reinterpret_cast<uint8_t *>(buffer))[6] = (reinterpret_cast<const uint8_t *>(&value))[6];
        (reinterpret_cast<uint8_t *>(buffer))[7] = (reinterpret_cast<const uint8_t *>(&value))[7];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[7];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[6];
        (reinterpret_cast<uint8_t *>(buffer))[2] = (reinterpret_cast<const uint8_t *>(&value))[5];
        (reinterpret_cast<uint8_t *>(buffer))[3] = (reinterpret_cast<const uint8_t *>(&value))[4];
        (reinterpret_cast<uint8_t *>(buffer))[4] = (reinterpret_cast<const uint8_t *>(&value))[3];
        (reinterpret_cast<uint8_t *>(buffer))[5] = (reinterpret_cast<const uint8_t *>(&value))[2];
        (reinterpret_cast<uint8_t *>(buffer))[6] = (reinterpret_cast<const uint8_t *>(&value))[1];
        (reinterpret_cast<uint8_t *>(buffer))[7] = (reinterpret_cast<const uint8_t *>(&value))[0];
    }

    return 8;
}

inline size_t Endian::WriteBigEndian(void *buffer, uint64_t value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[0];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[1];
        (reinterpret_cast<uint8_t *>(buffer))[2] = (reinterpret_cast<const uint8_t *>(&value))[2];
        (reinterpret_cast<uint8_t *>(buffer))[3] = (reinterpret_cast<const uint8_t *>(&value))[3];
        (reinterpret_cast<uint8_t *>(buffer))[4] = (reinterpret_cast<const uint8_t *>(&value))[4];
        (reinterpret_cast<uint8_t *>(buffer))[5] = (reinterpret_cast<const uint8_t *>(&value))[5];
        (reinterpret_cast<uint8_t *>(buffer))[6] = (reinterpret_cast<const uint8_t *>(&value))[6];
        (reinterpret_cast<uint8_t *>(buffer))[7] = (reinterpret_cast<const uint8_t *>(&value))[7];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[7];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[6];
        (reinterpret_cast<uint8_t *>(buffer))[2] = (reinterpret_cast<const uint8_t *>(&value))[5];
        (reinterpret_cast<uint8_t *>(buffer))[3] = (reinterpret_cast<const uint8_t *>(&value))[4];
        (reinterpret_cast<uint8_t *>(buffer))[4] = (reinterpret_cast<const uint8_t *>(&value))[3];
        (reinterpret_cast<uint8_t *>(buffer))[5] = (reinterpret_cast<const uint8_t *>(&value))[2];
        (reinterpret_cast<uint8_t *>(buffer))[6] = (reinterpret_cast<const uint8_t *>(&value))[1];
        (reinterpret_cast<uint8_t *>(buffer))[7] = (reinterpret_cast<const uint8_t *>(&value))[0];
    }

    return 8;
}

inline size_t Endian::WriteLittleEndian(void *buffer, int16_t value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[1];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[0];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[0];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[1];
    }

    return 2;
}

inline size_t Endian::WriteLittleEndian(void *buffer, uint16_t value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[1];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[0];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[0];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[1];
    }

    return 2;
}

inline size_t Endian::WriteLittleEndian(void *buffer, int32_t value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[3];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[2];
        (reinterpret_cast<uint8_t *>(buffer))[2] = (reinterpret_cast<const uint8_t *>(&value))[1];
        (reinterpret_cast<uint8_t *>(buffer))[3] = (reinterpret_cast<const uint8_t *>(&value))[0];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[0];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[1];
        (reinterpret_cast<uint8_t *>(buffer))[2] = (reinterpret_cast<const uint8_t *>(&value))[2];
        (reinterpret_cast<uint8_t *>(buffer))[3] = (reinterpret_cast<const uint8_t *>(&value))[3];
    }

    return 4;
}

inline size_t Endian::WriteLittleEndian(void *buffer, uint32_t value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[3];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[2];
        (reinterpret_cast<uint8_t *>(buffer))[2] = (reinterpret_cast<const uint8_t *>(&value))[1];
        (reinterpret_cast<uint8_t *>(buffer))[3] = (reinterpret_cast<const uint8_t *>(&value))[0];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[0];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[1];
        (reinterpret_cast<uint8_t *>(buffer))[2] = (reinterpret_cast<const uint8_t *>(&value))[2];
        (reinterpret_cast<uint8_t *>(buffer))[3] = (reinterpret_cast<const uint8_t *>(&value))[3];
    }

    return 4;
}

inline size_t Endian::WriteLittleEndian(void *buffer, int64_t value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[7];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[6];
        (reinterpret_cast<uint8_t *>(buffer))[2] = (reinterpret_cast<const uint8_t *>(&value))[5];
        (reinterpret_cast<uint8_t *>(buffer))[3] = (reinterpret_cast<const uint8_t *>(&value))[4];
        (reinterpret_cast<uint8_t *>(buffer))[4] = (reinterpret_cast<const uint8_t *>(&value))[3];
        (reinterpret_cast<uint8_t *>(buffer))[5] = (reinterpret_cast<const uint8_t *>(&value))[2];
        (reinterpret_cast<uint8_t *>(buffer))[6] = (reinterpret_cast<const uint8_t *>(&value))[1];
        (reinterpret_cast<uint8_t *>(buffer))[7] = (reinterpret_cast<const uint8_t *>(&value))[0];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[0];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[1];
        (reinterpret_cast<uint8_t *>(buffer))[2] = (reinterpret_cast<const uint8_t *>(&value))[2];
        (reinterpret_cast<uint8_t *>(buffer))[3] = (reinterpret_cast<const uint8_t *>(&value))[3];
        (reinterpret_cast<uint8_t *>(buffer))[4] = (reinterpret_cast<const uint8_t *>(&value))[4];
        (reinterpret_cast<uint8_t *>(buffer))[5] = (reinterpret_cast<const uint8_t *>(&value))[5];
        (reinterpret_cast<uint8_t *>(buffer))[6] = (reinterpret_cast<const uint8_t *>(&value))[6];
        (reinterpret_cast<uint8_t *>(buffer))[7] = (reinterpret_cast<const uint8_t *>(&value))[7];
    }

    return 8;
}

inline size_t Endian::WriteLittleEndian(void *buffer, uint64_t value)
{
    if (IsBigEndian())
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[7];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[6];
        (reinterpret_cast<uint8_t *>(buffer))[2] = (reinterpret_cast<const uint8_t *>(&value))[5];
        (reinterpret_cast<uint8_t *>(buffer))[3] = (reinterpret_cast<const uint8_t *>(&value))[4];
        (reinterpret_cast<uint8_t *>(buffer))[4] = (reinterpret_cast<const uint8_t *>(&value))[3];
        (reinterpret_cast<uint8_t *>(buffer))[5] = (reinterpret_cast<const uint8_t *>(&value))[2];
        (reinterpret_cast<uint8_t *>(buffer))[6] = (reinterpret_cast<const uint8_t *>(&value))[1];
        (reinterpret_cast<uint8_t *>(buffer))[7] = (reinterpret_cast<const uint8_t *>(&value))[0];
    }
    else
    {
        (reinterpret_cast<uint8_t *>(buffer))[0] = (reinterpret_cast<const uint8_t *>(&value))[0];
        (reinterpret_cast<uint8_t *>(buffer))[1] = (reinterpret_cast<const uint8_t *>(&value))[1];
        (reinterpret_cast<uint8_t *>(buffer))[2] = (reinterpret_cast<const uint8_t *>(&value))[2];
        (reinterpret_cast<uint8_t *>(buffer))[3] = (reinterpret_cast<const uint8_t *>(&value))[3];
        (reinterpret_cast<uint8_t *>(buffer))[4] = (reinterpret_cast<const uint8_t *>(&value))[4];
        (reinterpret_cast<uint8_t *>(buffer))[5] = (reinterpret_cast<const uint8_t *>(&value))[5];
        (reinterpret_cast<uint8_t *>(buffer))[6] = (reinterpret_cast<const uint8_t *>(&value))[6];
        (reinterpret_cast<uint8_t *>(buffer))[7] = (reinterpret_cast<const uint8_t *>(&value))[7];
    }

    return 8;
}

} // namespace util
