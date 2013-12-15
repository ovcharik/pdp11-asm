#pragma once

#include <fstream>
#include <string>
#include <regex>
#include <map>

#include "Label.h"
#include "String.h"

class LabelsStor
{
	std::map<std::string, Label*> m_labels;

public:
	LabelsStor(std::ifstream& ifs)
	{
		ifs.clear();
		ifs.seekg(0, std::ios::beg);

		static std::string error;

		std::string line;
		static std::regex exp_label("^([_A-Z][_A-Z\\d]*):.*");
		std::smatch matches;

		while (std::getline(ifs, line))
		{
			String::to_upper(line);
			String::trim(line);

			if (std::regex_match(line, matches, exp_label))
			{
				std::string name = matches[1];
				if (m_labels.find(name) != m_labels.end())
				{
					error = "Dublicate label: " + name;
					throw error;
				}
				m_labels[name] = new Label(name);
			}
		}
		
		ifs.clear();
		ifs.seekg(0, std::ios::beg);
	}

	virtual ~LabelsStor()
	{
		for (auto l : m_labels)
		{
			delete l.second;
		}
	}

	Label* get_label(std::string label) const
	{
		String::to_upper(label);
		String::trim(label);

		auto i = m_labels.find(label);

		if (i == m_labels.end())
			return NULL;
		return i->second;
	}

	void trace()
	{
		std::cout << "Labels Storage:" << std::endl;
		for (auto l : m_labels)
		{
			std::cout << l.second->get_name() << std::endl;
		}
	}
};