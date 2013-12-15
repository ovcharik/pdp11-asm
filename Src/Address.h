#pragma once

#include "Binary.h"

class Address
{
protected:
	unsigned short m_addr;

public:
	Address() {};
	virtual ~Address() {};

	virtual void set_addr(unsigned short addr) { m_addr = addr; }
	unsigned short get_addr() const { return m_addr; }

	virtual unsigned short get_size() const
	{
		return 0;
	}

	virtual unsigned short get_offset(unsigned short offset) const
	{
		return offset + get_size();
	}

	virtual void write(Binary& bin) const {};

	friend Binary& operator<<(Binary& bin, const Address& addr)
	{
		addr.write(bin);
		return bin;
	}
};