#pragma once

#include <string>

#include "Address.h"

class Label : public Address
{
	std::string m_name;

public:
	Label(std::string name) : m_name(name) {};
	virtual ~Label() {};

	const std::string& get_name() { return m_name; }
	void set_name(const std::string& name) { m_name = name; }
};