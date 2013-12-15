#pragma once

#include <fstream>

class Binary
{
protected:
	static const unsigned short BUFFER_SIZE = 0xFFFF;
	char* m_buffer;
	unsigned short m_offset;

public:
	Binary()
	{
		m_buffer = new char[BUFFER_SIZE]();
		m_offset = 0;
	}

	virtual ~Binary()
	{
		delete[] m_buffer;
	}

	Binary& operator<<(unsigned char byte)
	{
		m_buffer[m_offset] = byte;
		m_offset++;
		return *this;
	}

	Binary& operator<<(unsigned short word)
	{
		unsigned short* w = (unsigned short*)(m_buffer + m_offset);
		*w = word;
		m_offset += 2;
		return *this;
	}

	void write(std::ofstream& ofs) const
	{
		ofs.write(m_buffer, BUFFER_SIZE);
	}

	friend std::ofstream& operator<<(std::ofstream& ofs, const Binary& bin)
	{
		bin.write(ofs);
		return ofs;
	}
};