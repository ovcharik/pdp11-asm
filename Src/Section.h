#pragma once

#include <fstream>
#include <string>
#include <regex>
#include <list>

#include "Address.h"
#include "Label.h"
#include "LabelsStor.h"
#include "String.h"
#include "Data.h"
#include "Command.h"
#include "Operand.h"

class Section : public Address
{
public:
	enum Type {
		Datas,
		Stack,
		Code
	};

protected:
	Type m_type;
	Label* m_global;
	std::list<Address*> m_addresses;

public:
	Section(Type type, std::ifstream& ifs, const LabelsStor& labels_stor)
		: m_type(type)
	{
		ifs.clear();
		ifs.seekg(0, std::ios::beg);

		std::string find;
		if (m_type == Datas)
			find = ".DATA";
		else if (m_type == Stack)
			find = ".STACK";
		else
			find = ".CODE";

		std::regex exp_sect(("^.SECTION\\s+" + find + ".*$").c_str());
		static std::regex exp_any("^.SECTION.*$");
		static std::regex exp_global("^.GLOBAL\\s+([_A-Z][_A-Z\\d]*)$");
		static std::regex exp_cmd("^([_A-Z][_A-Z\\d]*:)?([A-Z]+)(\\s+)?([-+_@()A-Z\\d]+)?(\\s*,\\s*)?([-+_@()A-Z\\d]+)?$");
		static std::regex exp_lbl("^([_A-Z][_A-Z\\d]*):$");
		std::smatch matches;

		int line_counter = 0;
		std::string line;
		while (std::getline(ifs, line))
		{
			line_counter++;

			String::to_upper(line);
			String::trim(line);

			if (std::regex_match(line, exp_sect))
			{
				while (std::getline(ifs, line))
				{
					line_counter++;

					String::to_upper(line);
					String::trim(line);

					if (line.length() == 0 || line[0] == '#')
						continue;

					if (std::regex_match(line, exp_any))
						break;

					if (m_type == Code && std::regex_match(line, matches, exp_global))
					{
						m_global = labels_stor.get_label(matches[1]);
						if (m_global == NULL)
						{
							throw_error(line_counter, "Can't find label");
						}
						continue;
					}

					if (m_type == Datas || m_type == Stack)
					{
						Address* data;
						try {
							data = new Data(line, labels_stor);
						} catch (std::string error) {
							throw_error(line_counter, error);
						}
						m_addresses.push_back(data);
					}
					else if (m_type == Code)
					{
						if (std::regex_match(line, matches, exp_cmd))
						{
							if (matches[1].length())
							{
								std::string lab_nam = matches[1];
								lab_nam.erase(lab_nam.end() - 1);
								Label* l = labels_stor.get_label(lab_nam);
								if (l == NULL)
								{
									throw_error(line_counter, "Unknow label");
								}
								m_addresses.push_back(l);
							}

							Command* cmd;
							try {
								cmd = new Command(std::string(matches[2]));
							} catch (std::string error) {
								throw_error(line_counter, error);
							}

							Operand* op;
							if (matches[4].length() != 0)
							{
								try {
									op = new Operand(std::string(matches[4]), labels_stor);
								} catch (std::string error) {
									throw_error(line_counter, error);
								}
								cmd->add_operand(op);
							}
							if (matches[6].length() != 0)
							{
								try {
									op = new Operand(std::string(matches[6]), labels_stor);
								} catch (std::string error) {
									throw_error(line_counter, error);
								}
								cmd->add_operand(op);
							}
							m_addresses.push_back(cmd);
						}
						else if (std::regex_match(line, matches, exp_lbl))
						{
							Label* l = labels_stor.get_label(std::string(matches[1]));
							if (l == NULL)
							{
								throw_error(line_counter, "Unknow label");
							}
							m_addresses.push_back(l);
						}
						else
							throw_error(line_counter, "Unknow construction");
					}

				}
				break;
			}
		}

		ifs.clear();
		ifs.seekg(0, std::ios::beg);
	}

	void throw_error(int line_number, std::string msg)
	{
		static std::string error = "Line: " + std::to_string(line_number) + "\r\n\tMsg: " + msg;
		throw error;
	}

	virtual unsigned short get_size() const
	{
		unsigned short s = 0;
		for (auto a : m_addresses)
		{
			s += a->get_size();
		}
		return s;
	}

	unsigned short link(unsigned short offset)
	{
		unsigned short o = 0;
		unsigned short s = 0;
		for (auto a : m_addresses)
		{
			a->set_addr(offset + o);
			o += a->get_size();
		}
		return o;
	}

	virtual void write(Binary& bin) const
	{
		for (auto a : m_addresses)
			bin << *a;
	};
};