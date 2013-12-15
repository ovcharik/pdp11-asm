#pragma once

#include <fstream>
#include <string>
#include <list>

#include "LabelsStor.h"
#include "Section.h"
#include "Binary.h"

class Compiler
{
	LabelsStor* m_labels;
	std::ifstream* m_ifs;
	Section* m_sections[3];
	Binary m_bin;

public:
	Compiler(std::ifstream& ifs) : m_ifs(&ifs)
	{
		static std::string error;
		m_labels = new LabelsStor(*m_ifs);

		m_sections[0] = new Section(Section::Datas, *m_ifs, *m_labels);
		m_sections[1] = new Section(Section::Code, *m_ifs, *m_labels);
		m_sections[2] = new Section(Section::Stack, *m_ifs, *m_labels);
	};

	virtual ~Compiler()
	{
		delete m_labels;
		delete m_sections[0];
		delete m_sections[1];
		delete m_sections[2];
	}

	void link()
	{
		unsigned short of = m_sections[1]->link(0);
		if (of % 2) of++;
		of += m_sections[0]->link(of);
		if (of % 2) of++;
		of += m_sections[2]->link(of);
	}

	void compile()
	{
		m_bin << *m_sections[1] << *m_sections[0] << *m_sections[2];
	}

	void write(std::ofstream& ofs)
	{
		ofs << m_bin;
	};
};