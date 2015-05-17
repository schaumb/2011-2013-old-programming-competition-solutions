#ifndef PROTOCOLBUF_UTIL_HH_
#define PROTOCOLBUF_UTIL_HH_

#include <iosfwd>
#include <string>
#include <sstream>
#include <array>
#include <memory>
#include <vector>
#include <climits>

namespace protoutil {

typedef std::array<unsigned char, 4> ProtoSize;
inline ProtoSize encodeSize(uint32_t size) {
	// instead of htonl:
	ProtoSize sizeBytes = { { static_cast<unsigned char>((size >> 24) & 0xff),
			static_cast<unsigned char>((size >> 16) & 0xff),
			static_cast<unsigned char>((size >> 8) & 0xff),
			static_cast<unsigned char>(size & 0xff) } };
	return sizeBytes;
}

template<class T>
size_t decodeSize(const T& sizeBytes) {
	uint32_t size = 0;
	// instead of ntohl:
	for (size_t i = 0; i < sizeof(size); ++i) {
		size <<= CHAR_BIT;
		size |= (uint32_t) sizeBytes[i];
	}
	return size;
}

template<typename Proto>
void writeProtoOnStream(const Proto& proto, std::ostream& stream) {
	std::string buffer;
	proto.SerializeToString(&buffer);

	uint32_t size = buffer.size();
	ProtoSize sizeBytes = encodeSize(size);

	stream.write(reinterpret_cast<char*>(sizeBytes.data()), sizeBytes.size());
	stream.write(buffer.data(), size);
	stream.flush();
}

template<typename Proto>
void readProtoFromStream(Proto& proto, std::istream& stream) {
	ProtoSize sizeBytes;
	stream.read(reinterpret_cast<char*>(sizeBytes.data()), sizeBytes.size());
	uint32_t size = decodeSize(sizeBytes);

	std::vector<char> rawBuffer(size);
	stream.read(&rawBuffer[0], size);
	std::string buffer(rawBuffer.begin(), rawBuffer.end());

	proto.ParseFromString(buffer);
}

}

#endif

