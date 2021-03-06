#include "file_cache.h"

namespace IO {
	void FileReaderByteStreamer::SetFileReader( IO::FileReader::Ptr file ) {
		m_reader = file;
		auto pos = file->Position();
		auto size = file->Size();
		if(pos > size)
		{
			DO_THROW(Err::CriticalError, u8"File position was beyond EOF, can not deal with that");
		}
		m_bytesLeft = size - pos;
		m_position = m_cacheSize = 0;
	}

	uint8_t& FileReaderByteStreamer::ReadByte() {
		if (m_bytesLeft == 0) {
			DO_THROW(Err::CriticalError, "No bytes left to fetch.");
		}

		if (m_position == m_cacheSize) {
			m_position = 0;
			m_cacheSize = m_reader->Read(m_cache, 1, CacheSize);
		}

		m_bytesLeft--;
		return m_cache[m_position++];
	}

	FileInt FileReaderByteStreamer::BytesLeft() {
		return m_bytesLeft;
	}

	void FileReaderByteStreamer::SkipBytes( int numToSkip ) {
		for (int i = 0; i < numToSkip; ++i)
			ReadByte();
	}

	FileInt FileReaderByteStreamer::VirtualFilePosition() {
		return m_reader->Position() - (m_cacheSize - m_position);
	}
}
