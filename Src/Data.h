#pragma once

#include <string>
#include <regex>
#include <map>

#include "Address.h"
#include "Label.h"
#include "LabelsStor.h"
#include "String.h"

class Data : public Address
{
public:
	enum Type {
		String,
		Byte,
		Word,
		Float,
		Space
	};

protected:
	Label* m_label;
	Type m_type;
	char* m_data;
	unsigned short m_size;

	static bool m_symbols_map_inted;
	static std::map<char, char> m_symbols_map;
	static void init_symbols_map()
	{
		if (m_symbols_map_inted)
			return;
		m_symbols_map_inted = true;

		m_symbols_map[0  ] = 0;    m_symbols_map['I'] = 0111;  m_symbols_map['Ñ'] = 0163;
		m_symbols_map[' '] = 040;  m_symbols_map['J'] = 0112;  m_symbols_map['Ò'] = 0164;
		m_symbols_map['!'] = 041;  m_symbols_map['K'] = 0113;  m_symbols_map['Ó'] = 0165;
		m_symbols_map['"'] = 042;  m_symbols_map['L'] = 0114;  m_symbols_map['Æ'] = 0166;
		m_symbols_map['#'] = 043;  m_symbols_map['M'] = 0115;  m_symbols_map['Â'] = 0167;
		m_symbols_map['$'] = 044;  m_symbols_map['N'] = 0116;  m_symbols_map['Ü'] = 0170;
		m_symbols_map['%'] = 045;  m_symbols_map['O'] = 0117;  m_symbols_map['Û'] = 0171;
		m_symbols_map['&'] = 046;  m_symbols_map['P'] = 0120;  m_symbols_map['Ç'] = 0172;
		m_symbols_map[39 ] = 047;  m_symbols_map['Q'] = 0121;  m_symbols_map['Ø'] = 0173;
		m_symbols_map['('] = 050;  m_symbols_map['R'] = 0122;  m_symbols_map['Ý'] = 0174;
		m_symbols_map[')'] = 051;  m_symbols_map['S'] = 0123;  m_symbols_map['Ù'] = 0175;
		m_symbols_map['*'] = 052;  m_symbols_map['T'] = 0124;  m_symbols_map['×'] = 0176;
		m_symbols_map['+'] = 053;  m_symbols_map['U'] = 0125;  m_symbols_map[8  ] = 0177;
		m_symbols_map[','] = 054;  m_symbols_map['V'] = 0126;  m_symbols_map[1  ] = 001;
		m_symbols_map['-'] = 055;  m_symbols_map['W'] = 0127;  m_symbols_map[2  ] = 002;
		m_symbols_map['.'] = 056;  m_symbols_map['X'] = 0130;  m_symbols_map[3  ] = 003;
		m_symbols_map['/'] = 057;  m_symbols_map['Y'] = 0131;  m_symbols_map[4  ] = 004;
		m_symbols_map['0'] = 060;  m_symbols_map['Z'] = 0132;  m_symbols_map[5  ] = 005;
		m_symbols_map['1'] = 061;  m_symbols_map['['] = 0133;  m_symbols_map[6  ] = 006;
		m_symbols_map['2'] = 062;  m_symbols_map[92 ] = 0134;  m_symbols_map[7  ] = 007;
		m_symbols_map['3'] = 063;  m_symbols_map['['] = 0135;  m_symbols_map[8  ] = 010;
		m_symbols_map['4'] = 064;  m_symbols_map['|'] = 0136;  m_symbols_map[9  ] = 011;
		m_symbols_map['5'] = 065;  m_symbols_map['_'] = 0137;  m_symbols_map[10 ] = 012;
		m_symbols_map['6'] = 066;  m_symbols_map['Þ'] = 0140;  m_symbols_map[11 ] = 013;
		m_symbols_map['7'] = 067;  m_symbols_map['À'] = 0141;  m_symbols_map[12 ] = 014;
		m_symbols_map['8'] = 070;  m_symbols_map['Á'] = 0142;  m_symbols_map[13 ] = 015;
		m_symbols_map['9'] = 071;  m_symbols_map['Ö'] = 0143;  m_symbols_map[14 ] = 016;
		m_symbols_map[':'] = 072;  m_symbols_map['Ä'] = 0144;  m_symbols_map[15 ] = 017;
		m_symbols_map[';'] = 073;  m_symbols_map['Å'] = 0145;  m_symbols_map[16 ] = 020;
		m_symbols_map['<'] = 074;  m_symbols_map['Ô'] = 0146;  m_symbols_map[17 ] = 021;
		m_symbols_map['='] = 075;  m_symbols_map['Ã'] = 0147;  m_symbols_map[18 ] = 022;
		m_symbols_map['>'] = 076;  m_symbols_map['Õ'] = 0150;  m_symbols_map[19 ] = 023;
		m_symbols_map['?'] = 077;  m_symbols_map['È'] = 0151;  m_symbols_map[20 ] = 024;
		m_symbols_map['@'] = 0100; m_symbols_map['É'] = 0152;  m_symbols_map[21 ] = 025;
		m_symbols_map['A'] = 0101; m_symbols_map['Ê'] = 0153;  m_symbols_map[22 ] = 026;
		m_symbols_map['B'] = 0102; m_symbols_map['Ë'] = 0154;  m_symbols_map[23 ] = 027;
		m_symbols_map['C'] = 0103; m_symbols_map['Ì'] = 0155;  m_symbols_map[24 ] = 030;
		m_symbols_map['D'] = 0104; m_symbols_map['Í'] = 0156;  m_symbols_map[25 ] = 031;
		m_symbols_map['E'] = 0105; m_symbols_map['Î'] = 0157;  m_symbols_map[26 ] = 032;
		m_symbols_map['F'] = 0106; m_symbols_map['Ï'] = 0160;  m_symbols_map[27 ] = 033;
		m_symbols_map['G'] = 0107; m_symbols_map['ß'] = 0161;  m_symbols_map[28 ] = 034;
		m_symbols_map['H'] = 0110; m_symbols_map['Ð'] = 0162;  m_symbols_map[29 ] = 035;
		m_symbols_map[30 ] = 036;  m_symbols_map[31 ] = 037;
	}

public:
	Data(std::string data, const LabelsStor& labels_stor)
		: m_data(NULL)
	{
		static std::string error;

		//                           {1}                   {2}             {3}
		static std::regex exp_data("^([_A-Z][_A-Z\\d]*):\\s+(\\.[A-Z]+)\\s+(.*)$");;
		static std::regex exp_str("^\"(.*)\"$");
		static std::regex exp_space("^(0?X?\\d+)\\s*,\\s*(0?X?\\d+)$");
		std::smatch matches;

		String::to_upper(data);
		String::trim(data);

		if (std::regex_match(data, matches, exp_data))
		{
			std::string name = matches[1];
			std::string type = matches[2];
			std::string value = matches[3];

			m_label = labels_stor.get_label(name);
			if (m_label == NULL)
				goto PARSE_ERROR;

			if (type == ".STRING")
				m_type = Type::String;
			else if (type == ".BYTE")
				m_type = Type::Byte;
			else if (type == ".WORD")
				m_type = Type::Word;
			else if (type == ".SPACE")
				m_type = Type::Space;
			else if (type == ".FLOAT")
				m_type = Type::Float;
			else
				goto WRONG_TYPE;

			if (m_type == Type::Space)
			{
				int v;
				char e = 0;
				if (std::regex_match(value, matches, exp_space))
				{
					try {
						e = std::stoi(matches[2], nullptr, 0);
					} catch (...) {
						goto WRONG_ARGS;
					}
					try {
						v = std::stoi(matches[1], nullptr, 0);
					} catch (...) {
						goto WRONG_ARGS;
					}
				}
				else
				{
					try {
						v = std::stoi(value, nullptr, 0);
					} catch (...) {
						goto WRONG_ARGS;
					}
				}
				m_data = new char[v];
				for (int i = 0; i < v; i++)
					m_data[i] = e;
				m_size = v;
			}
			else if (m_type == Type::String)
			{
				std::smatch sm;
				if (std::regex_match(value, sm, exp_str))
				{
					value = sm[1];
					m_size = value.length() + 1;
					m_data = new char[m_size];
					cast_string(value, m_data);
				}
				else
					goto WRONG_ARGS;
			}
			else if (m_type == Type::Byte)
			{
				m_size = 1;
				m_data = new char[1];
				try {
					m_data[0] = (char)std::stoi(value, nullptr, 0);
				} catch (...) {
					goto WRONG_ARGS;
				}
			}
			else if (m_type == Type::Word)
			{
				m_size = 2;
				m_data = new char[2];
				try {
					short* d = (short*)m_data;
					*d = (short)std::stoi(value, nullptr, 0);
				} catch (...) {
					goto WRONG_ARGS;
				}
			}
			else if (m_type == Type::Float)
			{
				m_size = 4;
				m_data = new char[4];
				try {
					float* d = (float*)m_data;
					*d = (float)std::stof(value, nullptr);
				} catch (...) {
					goto WRONG_ARGS;
				}
			}
		}
		if (false)
		{
		PARSE_ERROR:
			error = "Data parse error: " + data;
			throw error;
		WRONG_TYPE:
			error = "Wrong type of data: " + data;
			throw error;
		WRONG_ARGS:
			error = "Wrong arguments: " + data;
			throw error;
		}
	}

	virtual ~Data()
	{
		if (m_data != NULL)
			delete[] m_data;
	}

	virtual void set_addr(unsigned short addr)
	{
		m_addr = addr;
		m_label->set_addr(addr);
	}

	static void cast_string(std::string input, char* output)
	{
		init_symbols_map();
		for (unsigned int i = 0; i < input.length(); i++)
		{
			output[i] = m_symbols_map[input[i]];
		}
		output[input.length()] = 0;
	}

	virtual unsigned short get_size() const
	{
		return m_size;
	}

	virtual void write(Binary& bin) const
	{
		for (int i = 0; i < m_size; i++)
			bin << (unsigned char)m_data[i];
	};
};

bool Data::m_symbols_map_inted = false;
std::map<char, char> Data::m_symbols_map;