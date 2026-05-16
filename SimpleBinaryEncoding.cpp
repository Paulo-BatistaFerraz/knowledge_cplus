#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <optional>
#include <vector>
#include <type_traits>

// --- Data Structures ---

struct RepeatingGroup {
    uint8_t Count;
    uint8_t Size;
};

struct FirmAtLevel {
    size_t Id;
    std::array<char, 8> Tag;
};

struct Header {
    uint16_t Identifier;  // 0xCAFE
    uint16_t BodyLength;  // Bytes following header
    uint8_t  MessageType; // Expected: 14
};

struct MessageType14 {
    Header Head;
    uint32_t BidPrice;
    uint16_t BidQuantity;
    RepeatingGroup Group;
    std::vector<FirmAtLevel> Firms;
};

// --- Helper Functions ---

template <typename T>
requires std::is_unsigned_v<T> && std::is_integral_v<T>
static bool ReadLE(const std::vector<std::byte>& s, size_t& off, T& out) {
    if (off + sizeof(T) > s.size()) return false;

    T v = 0;
    for (size_t i = 0; i < sizeof(T); ++i) {
        v |= (static_cast<T>(std::to_integer<uint8_t>(s[off + i])) << (8 * i));
    }
    out = v;
    off += sizeof(T);
    return true;
}

static bool ReadBytes(const std::vector<std::byte>& s, size_t& off, void* dst, size_t n) {
    if (off + n > s.size()) return false;
    std::memcpy(dst, s.data() + off, n);
    off += n;
    return true;
}

// --- Main Parser ---

std::optional<MessageType14> ParseBytes(const std::vector<std::byte>& stream) {
    // Protocol Constants
    constexpr uint16_t kExpectedIdentifier = 0xCAFE;
    constexpr uint8_t  kExpectedMsgType    = 14;
    
    // Wire Sizes (Strictly defined to avoid struct padding issues)
    constexpr size_t kHeaderSize      = 5;  // 2 (ID) + 2 (Len) + 1 (Type)
    constexpr size_t kFixedBodySize   = 8;  // 4 (Price) + 2 (Qty) + 1 (Count) + 1 (Size)
    constexpr size_t kExpectedFirmSize = 16; // 8 (Id) + 8 (Tag)

    size_t off = 0;

    // 1. Parse Header
    Header hdr{};
    if (!ReadLE<uint16_t>(stream, off, hdr.Identifier)) return std::nullopt;
    if (hdr.Identifier != kExpectedIdentifier) return std::nullopt;

    if (!ReadLE<uint16_t>(stream, off, hdr.BodyLength)) return std::nullopt;
    if (!ReadLE<uint8_t>(stream, off, hdr.MessageType)) return std::nullopt;

    // 2. Validate Message Scope
    if (hdr.MessageType != kExpectedMsgType) return std::nullopt;
    
    // Ensure the stream actually contains the amount of data the header claims
    if (stream.size() < (kHeaderSize + hdr.BodyLength)) return std::nullopt;
    
    // Ensure the body is at least large enough for the fixed fields
    if (hdr.BodyLength < kFixedBodySize) return std::nullopt;

    // We constrain reading to the boundaries defined by BodyLength
    const size_t bodyEnd = kHeaderSize + hdr.BodyLength;

    MessageType14 msg{};
    msg.Head = hdr;

    // 3. Parse Fixed Body Fields
    if (!ReadLE<uint32_t>(stream, off, msg.BidPrice)) return std::nullopt;
    if (!ReadLE<uint16_t>(stream, off, msg.BidQuantity)) return std::nullopt;
    if (!ReadLE<uint8_t>(stream, off, msg.Group.Count)) return std::nullopt;
    if (!ReadLE<uint8_t>(stream, off, msg.Group.Size)) return std::nullopt;

    // 4. Validate Repeating Group Metadata
    // This addresses Test #3: If the size in the wire doesn't match our struct, fail.
    if (msg.Group.Size != kExpectedFirmSize) return std::nullopt;

    // 5. Parse Repeating Group
    const size_t count = static_cast<size_t>(msg.Group.Count);
    if (count > 0) {
        // Prevent overflow during total size calculation
        if (count > (std::numeric_limits<size_t>::max() / kExpectedFirmSize)) return std::nullopt;
        
        const size_t totalFirmsBytes = count * kExpectedFirmSize;
        
        // Ensure the repeating group doesn't exceed the BodyLength limit
        if (off + totalFirmsBytes > bodyEnd) return std::nullopt;

        msg.Firms.reserve(count);
        for (size_t i = 0; i < count; ++i) {
            uint64_t wireId = 0;
            if (!ReadLE<uint64_t>(stream, off, wireId)) return std::nullopt;

            FirmAtLevel firm{};
            firm.Id = static_cast<size_t>(wireId);
            if (!ReadBytes(stream, off, firm.Tag.data(), firm.Tag.size())) return std::nullopt;

            msg.Firms.push_back(firm);
        }
    }

    return msg;
}