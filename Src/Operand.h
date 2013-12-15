#pragma once

#include <string>
#include <iostream>
#include <stdexcept>
#include <regex>

#include "Label.h"
#include "LabelsStor.h"
#include "String.h"

class Operand
{
public:
	enum Type {
		Reg              = 00,
		AddrAddrReg      = 01,
		AddrRegInc       = 02,
		AddrAddrRegInc   = 03,
		AddrRegDec       = 04,
		AddrAddrRegDec   = 05,
		AddrRegIndex     = 06,
		AddrAddrRegIndex = 07
	};

	enum Registr {
		R0 = 00,
		R1 = 01,
		R2 = 02,
		R3 = 03,
		R4 = 04,
		R5 = 05,
		R6 = 06,
		R7 = 07
	};

protected:
	Type m_type;
	Registr  m_registr;
	unsigned int m_value;
	Label* m_label;
	bool m_has_value;
	bool m_has_label;

public:
	Operand(Type type, Registr registr)
		: m_type(type),
		  m_registr(registr),
		  m_has_value(false)
	{};

	Operand(Type type, Label& label)
		: m_has_value(true),
		  m_has_label(true),
		  m_label(&label),
		  m_type(type),
		  m_registr(R7)
	{};

	Operand(std::string str, const LabelsStor& labels_stor)
		: m_has_label(false),
		  m_has_value(false)
	{
		static std::string error;

		String::trim(str);
		String::to_upper(str);
		
		//                          {1} {2} {3}        {4}   {5}         {6}   {7}
		static std::regex exp_all("^(@)?(-)?(0?X?\\d+)?(\\()?([_A-Z\\d]*)(\\))?(\\+)?$");
		static std::regex exp_reg("^R(\\d)$");
		static std::regex exp_num("^(0?X?\\d+)$");
		std::smatch matches;

		if (std::regex_match(str, matches, exp_all))
		{
			if ((matches[4].length() == 0 && matches[6].length() != 0) ||
				(matches[4].length() != 0 && matches[6].length() == 0))
			{
				goto PARSE_ERROR;
			}
			bool addr_addr = (matches[1] == "@");
			bool addr = (matches[4] == "(" && matches[6] == ")");
			bool dec = (matches[2] == "-");
			bool inc = (matches[7] == "+");
			bool has_index = matches[3].length() != 0;
			bool has_value = false;
			bool has_label = false;
			bool has_registr = false;
			std::string label;
			Registr registr;
			unsigned short index;
			unsigned short value;
			if (matches[5].length() != 0)
			{
				std::smatch sm;
				std::string v = matches[5];
				if (std::regex_match(v, sm, exp_reg))
				{
					int r = std::stoi(sm[1]);
					if (r > 7)
						goto WRONG_FORMAT;
					registr = (Registr)r;
					has_registr = true;
				}
				else if (std::regex_match(v, sm, exp_num))
				{
					value = std::stoi(sm[1], nullptr, 0);
					has_value = true;
				}
				else
				{
					label = v;
					has_label = true;
				}
			}
			if (has_index)
			{
				try {
					index = std::stoi(matches[3], nullptr, 0);
				}
				catch (...) {
					goto PARSE_ERROR;
				}
				if (matches[5].length() == 0)
				{
					value = index;
					has_index = false;
					has_value = true;
				}
			}
			if ((!has_registr && has_index) ||
				(has_registr && addr && !addr_addr && !(dec || inc || has_index)) ||
				(has_value + has_registr + has_label > 1) ||
				((inc || dec) && !has_registr) ||
				((inc || dec) && has_index) ||
				(addr_addr && !has_registr) ||
				(inc && dec))
				goto WRONG_FORMAT;

			char type_val = 0;
			if (has_registr)
			{
				if (addr_addr)      type_val |= 1;
				if (inc)            type_val |= 2;
				else if (dec)       type_val |= 4;
				else if (has_index) type_val |= 6;
			}
			else if (has_label || has_value)
			{
				registr = R7;
				if (addr) type_val = 3;
				else      type_val = 2;
			}
			else
				goto WRONG_FORMAT;

			Type type = (Type)type_val;

			m_type = type;
			m_registr = registr;
			if (has_index)
			{
				m_has_value = true;
				m_value = index;
			}
			else if (has_value)
			{
				m_has_value = true;
				m_value = value;
			}
			else if (has_label)
			{
				m_has_label = true;
				m_has_value = true;
				m_label = labels_stor.get_label(label);
				if (m_label == NULL)
				{
					error = "Can't find label: " + label;
					throw error;
				}
			}
			else if (!has_registr)
				goto WRONG_FORMAT;
		}
		else
		{
		PARSE_ERROR:
			error = "Can't parse operand: " + str;
			throw error;
		WRONG_FORMAT:
			error = "Wrong format of operand: " + str;
			throw error;
		}
	};

	virtual ~Operand() {};

	unsigned short get_binary()
	{
		return (m_type << 3) | m_registr;
	};

	Type get_type()
	{
		return m_type;
	}

	Registr get_registr()
	{
		return m_registr;
	}

	bool has_value()
	{
		return m_has_value;
	}

	unsigned short get_value()
	{
		if (m_has_value)
		{
			if (m_has_label)
				return m_label->get_addr();
			else
				return m_value;
		}
		static std::string error = "Operand has't value";
		throw error;
		return 0;
	}
};