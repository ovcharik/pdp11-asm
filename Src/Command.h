#pragma once

#include "Address.h"
#include "Binary.h"
#include "Operand.h"
#include "String.h"

#include <algorithm>
#include <vector>
#include <string>
#include <map>

class Command : public Address
{
public:
	enum Type {
		Unicast      = 0x1,
		Dubcast      = 0x2,
		DubcastArth  = 0x4,
		DubcastArthF = 0x8,
		Jump         = 0x10,
		Mark         = 0x11,
		Sob          = 0x12,
		Full         = 0x14
	};

	enum Code {
		/* unicast   */   /*  dcast w  */   /*   jump    */   /*   other   */
		CLRW  = 0x0A00,   MOVW  = 0x1000,   BR    = 0x0100,   JMP   = 0x0040,
		INCW  = 0x0A80,   CMPW  = 0x2000,   BNE   = 0x0200,   JSR   = 0x0800,
		DECW  = 0x0AC0,   BITW  = 0x3000,   BEQ   = 0x0300,   RST   = 0x0010,
		COMW  = 0x0A40,   BICW  = 0x4000,   BPL   = 0x8000,   MARK  = 0x0D00,
		TSTW  = 0x0BC0,   BISW  = 0x5000,   BMI   = 0x8100,   SOB   = 0x7E00,
		NEGW  = 0x0B00,   ADD   = 0x6000,   BVC   = 0x8400,   EMT   = 0x8800,
		ASRW  = 0x0C80,   SUB   = 0xE000,   BVS   = 0x8500,   TRAP  = 0x8900,
		ASLW  = 0x0CC0,   /* extra arth */  BCC   = 0x8600,   /*  other 2  */
		RORW  = 0x0C00,   MUL   = 0x7000,   BCS   = 0x8700,   IOT   = 0x0004,
		ROLW  = 0x0C40,   DIV   = 0x7200,   BGE   = 0x0400,   BPT   = 0x0003,
		ADCW  = 0x0B40,   ASH   = 0x7400,   BLT   = 0x0500,   RTI   = 0x0002,
		SBCW  = 0x0B80,   ASHC  = 0x7600,   BGT   = 0x0600,   RTT   = 0x0006,
		SWAB  = 0x00C0,   XOR   = 0x7800,   BLE   = 0x0700,   RESET = 0x0005,
		SXT   = 0x0DC0,   /* float arth */  BHI   = 0x8200,   WAIT  = 0x0001,
		/* unicast b */   FADD  = 0x7A00,   BLOS  = 0x8300,   HALT  = 0x0000,
        CLRB  = 0x8A00,   FSUB  = 0x7A08,                     /*  other 3  */
        INCB  = 0x8A80,   FMUL  = 0x7A10,                     CLC   = 0x00A1,
        DECB  = 0x8AC0,   FDIV  = 0x7A18,                     CLV   = 0x00A2,
		COMB  = 0x8A40,   /*  dcast b  */                     CLZ   = 0x00A4,
		TSTB  = 0x8BC0,   MOVB  = 0x9000,                     CLN   = 0x00A8,
		NEGB  = 0x8B00,   CMPB  = 0xA000,                     SEC   = 0x00B1,
		ASRB  = 0x8C80,   BITB  = 0xB000,                     SEV   = 0x00B2,
		ASLB  = 0x8CC0,   BICB  = 0xC000,                     SEZ   = 0x00B4,
		RORB  = 0x8C00,   BISB  = 0xD000,                     SEN   = 0x00B8,
		ROLB  = 0x8C40,                                       CLALL = 0x00AF,
		ADCB  = 0x8B40,                                       SEALL = 0x00BF,
		SBCB  = 0x8B80,                                       NOP   = 0x00A0
	};

	struct SCommand {
		SCommand() {};
		SCommand(Code c, Type t) : code(c), type(t) {};
		Code code;
		Type type;
	};

protected:
	std::vector<Operand*> m_operands;

protected:
	Type m_type;
	Code m_code;
	std::string m_code_str;

	static std::map<std::string, SCommand> m_code_map; 
	static bool m_code_map_inited;
	static void init_code_map()
	{
		if (m_code_map_inited)
		{
			return;
		}

		m_code_map["CLRW" ] = SCommand(Code::CLRW,  Type::Unicast);
		m_code_map["INCW" ] = SCommand(Code::INCW,  Type::Unicast);
		m_code_map["DECW" ] = SCommand(Code::DECW,  Type::Unicast);
		m_code_map["COMW" ] = SCommand(Code::COMW,  Type::Unicast);
		m_code_map["TSTW" ] = SCommand(Code::TSTW,  Type::Unicast);
		m_code_map["NEGW" ] = SCommand(Code::NEGW,  Type::Unicast);
		m_code_map["ASRW" ] = SCommand(Code::ASRW,  Type::Unicast);
		m_code_map["ASLW" ] = SCommand(Code::ASLW,  Type::Unicast);
		m_code_map["RORW" ] = SCommand(Code::RORW,  Type::Unicast);
		m_code_map["ROLW" ] = SCommand(Code::ROLW,  Type::Unicast);
		m_code_map["ADCW" ] = SCommand(Code::ADCW,  Type::Unicast);
		m_code_map["SBCW" ] = SCommand(Code::SBCW,  Type::Unicast);
		m_code_map["SWAB" ] = SCommand(Code::SWAB,  Type::Unicast);
		m_code_map["SXT"  ] = SCommand(Code::SXT,   Type::Unicast);
		
		m_code_map["CLRB" ] = SCommand(Code::CLRB,  Type::Unicast);
		m_code_map["INCB" ] = SCommand(Code::INCB,  Type::Unicast);
		m_code_map["DECB" ] = SCommand(Code::DECB,  Type::Unicast);
		m_code_map["COMB" ] = SCommand(Code::COMB,  Type::Unicast);
		m_code_map["TSTB" ] = SCommand(Code::TSTB,  Type::Unicast);
		m_code_map["NEGB" ] = SCommand(Code::NEGB,  Type::Unicast);
		m_code_map["ASRB" ] = SCommand(Code::ASRB,  Type::Unicast);
		m_code_map["ASLB" ] = SCommand(Code::ASLB,  Type::Unicast);
		m_code_map["RORB" ] = SCommand(Code::RORB,  Type::Unicast);
		m_code_map["ROLB" ] = SCommand(Code::ROLB,  Type::Unicast);
		m_code_map["ADCB" ] = SCommand(Code::ADCB,  Type::Unicast);
		m_code_map["SBCB" ] = SCommand(Code::SBCB,  Type::Unicast);
		
		m_code_map["MOVB" ] = SCommand(Code::MOVB,  Type::Dubcast);
		m_code_map["CMPB" ] = SCommand(Code::CMPB,  Type::Dubcast);
		m_code_map["BITB" ] = SCommand(Code::BITB,  Type::Dubcast);
		m_code_map["BICB" ] = SCommand(Code::BICB,  Type::Dubcast);
		m_code_map["BISB" ] = SCommand(Code::BISB,  Type::Dubcast);
		
		m_code_map["MOVW" ] = SCommand(Code::MOVW,  Type::Dubcast);
		m_code_map["CMPW" ] = SCommand(Code::CMPW,  Type::Dubcast);
		m_code_map["BITW" ] = SCommand(Code::BITW,  Type::Dubcast);
		m_code_map["BICW" ] = SCommand(Code::BICW,  Type::Dubcast);
		m_code_map["BISW" ] = SCommand(Code::BISW,  Type::Dubcast);

		m_code_map["ADD"  ] = SCommand(Code::ADD,   Type::Dubcast);
		m_code_map["SUB"  ] = SCommand(Code::SUB,   Type::Dubcast);
		m_code_map["MUL"  ] = SCommand(Code::MUL,   Type::DubcastArth);
		m_code_map["DIV"  ] = SCommand(Code::DIV,   Type::DubcastArth);
		m_code_map["ASH"  ] = SCommand(Code::ASH,   Type::DubcastArth);
		m_code_map["ASHC" ] = SCommand(Code::ASHC,  Type::DubcastArth);
		m_code_map["XOR"  ] = SCommand(Code::XOR,   Type::DubcastArth);
		
		m_code_map["FADD" ] = SCommand(Code::FADD,  Type::DubcastArthF);
		m_code_map["FSUB" ] = SCommand(Code::FSUB,  Type::DubcastArthF);
		m_code_map["FMUL" ] = SCommand(Code::FMUL,  Type::DubcastArthF);
		m_code_map["FDIV" ] = SCommand(Code::FDIV,  Type::DubcastArthF);
		
		m_code_map["BR"   ] = SCommand(Code::BR,    Type::Jump);
		m_code_map["BNE"  ] = SCommand(Code::BNE,   Type::Jump);
		m_code_map["BEQ"  ] = SCommand(Code::BEQ,   Type::Jump);
		m_code_map["BPL"  ] = SCommand(Code::BPL,   Type::Jump);
		m_code_map["BMI"  ] = SCommand(Code::BMI,   Type::Jump);
		m_code_map["BVC"  ] = SCommand(Code::BVC,   Type::Jump);
		m_code_map["BVS"  ] = SCommand(Code::BVS,   Type::Jump);
		m_code_map["BCC"  ] = SCommand(Code::BCC,   Type::Jump);
		m_code_map["BCS"  ] = SCommand(Code::BCS,   Type::Jump);
		m_code_map["BGE"  ] = SCommand(Code::BGE,   Type::Jump);
		m_code_map["BLT"  ] = SCommand(Code::BLT,   Type::Jump);
		m_code_map["BGT"  ] = SCommand(Code::BGT,   Type::Jump);
		m_code_map["BLE"  ] = SCommand(Code::BLE,   Type::Jump);
		m_code_map["BHI"  ] = SCommand(Code::BHI,   Type::Jump);
		m_code_map["BLOS" ] = SCommand(Code::BLOS,  Type::Jump);

		m_code_map["JMP"  ] = SCommand(Code::JMP,   Type::Unicast);
		m_code_map["JSR"  ] = SCommand(Code::JSR,   Type::DubcastArth);
		m_code_map["RST"  ] = SCommand(Code::RST,   Type::DubcastArthF);
		m_code_map["MARK" ] = SCommand(Code::MARK,  Type::Mark);
		m_code_map["SOB"  ] = SCommand(Code::SOB,   Type::Sob);
		m_code_map["EMT"  ] = SCommand(Code::EMT,   Type::Jump);
		m_code_map["TRAP" ] = SCommand(Code::TRAP,  Type::Jump);
		
		m_code_map["IOT"  ] = SCommand(Code::IOT,   Type::Full);
		m_code_map["BPT"  ] = SCommand(Code::BPT,   Type::Full);
		m_code_map["RTI"  ] = SCommand(Code::RTI,   Type::Full);
		m_code_map["RTT"  ] = SCommand(Code::RTT,   Type::Full);
		m_code_map["RESET"] = SCommand(Code::RESET, Type::Full);
		m_code_map["WAIT" ] = SCommand(Code::WAIT,  Type::Full);
		m_code_map["HALT" ] = SCommand(Code::HALT,  Type::Full);
		
		m_code_map["CLC"  ] = SCommand(Code::CLC,   Type::Full);
		m_code_map["CLV"  ] = SCommand(Code::CLV,   Type::Full);
		m_code_map["CLZ"  ] = SCommand(Code::CLZ,   Type::Full);
		m_code_map["CLN"  ] = SCommand(Code::CLN,   Type::Full);
		m_code_map["SEC"  ] = SCommand(Code::SEC,   Type::Full);
		m_code_map["SEV"  ] = SCommand(Code::SEV,   Type::Full);
		m_code_map["SEZ"  ] = SCommand(Code::SEZ,   Type::Full);
		m_code_map["SEN"  ] = SCommand(Code::SEN,   Type::Full);
		m_code_map["CLALL"] = SCommand(Code::CLALL, Type::Full);
		m_code_map["SEALL"] = SCommand(Code::SEALL, Type::Full);
		m_code_map["NOP"  ] = SCommand(Code::NOP,   Type::Full);

		m_code_map_inited = true;
	}

public:
	Command()
	{
		init_code_map();
	};
	Command(std::string& str)
	{
		init_code_map();
		str = String::to_upper(String::trim(str));
		auto f = m_code_map.find(str);
		if (f == m_code_map.end())
		{
			std::string error = "Unknow command: " + str;
			throw error;
		}
		SCommand& c = m_code_map[str];
		m_code = c.code;
		m_type = c.type;
		m_code_str = str;
	}

	virtual ~Command() {};

	std::vector<unsigned short>& get_binary() const
	{
		static std::vector<unsigned short> result;
		result.clear();

		switch (m_type)
		{
		case Type::Unicast:
			{
				if (m_operands.size() != 1)
				{
					std::string error = "Wrong format of command: " + m_code_str;
					throw error;
				}
				unsigned short c = m_code;
				c |= m_operands[0]->get_binary();
				result.push_back(c);
				if (m_operands[0]->has_value())
				{
					result.push_back(m_operands[0]->get_value());
				}
			}
			break;
		case Type::Dubcast:
			{
				if (m_operands.size() != 2)
				{
					std::string error = "Wrong format of command: " + m_code_str;
					throw error;
				}
				unsigned short c = m_code;
				c |= (m_operands[0]->get_binary() << 6);
				c |= m_operands[1]->get_binary();
				result.push_back(c);
				if (m_operands[0]->has_value())
				{
					result.push_back(m_operands[0]->get_value());
				}
				if (m_operands[1]->has_value())
				{
					result.push_back(m_operands[1]->get_value());
				}
			}
			break;
		case Type::DubcastArth:
			{
				if (m_operands.size() != 2)
				{
					std::string error = "Wrong format of command: " + m_code_str;
					throw error;
				}
				if (m_operands[0]->has_value() ||
					m_operands[0]->get_type() != Operand::Type::Reg)
				{
					std::string error = "Wrong operand(s) for command: " + m_code_str;
					throw error;
				}
				unsigned short c = m_code;
				c |= (m_operands[0]->get_registr() << 6);
				c |= m_operands[1]->get_binary();
				result.push_back(c);
				if (m_operands[1]->has_value())
				{
					result.push_back(m_operands[1]->get_value());
				}
			}
			break;
		case Type::DubcastArthF:
			{
				if (m_operands.size() != 1)
				{
					std::string error = "Wrong format of command: " + m_code_str;
					throw error;
				}
				if (m_operands[0]->get_type() != Operand::Type::Reg)
				{
					std::string error = "Wrong type of addressing for: " + m_code_str;
					throw error;
				}
				if (m_operands[0]->has_value())
				{
					std::string error = "Wrong operand for command: " + m_code_str;
					throw error;
				}
				unsigned short c = m_code;
				c |= m_operands[0]->get_registr();
				result.push_back(c);
			}
			break;
		case Type::Jump:
			{
				if (m_operands.size() != 1)
				{
					std::string error = "Wrong format of command: " + m_code_str;
					throw error;
				}
				if (!m_operands[0]->has_value())
				{
					std::string error = "Wrong operand for command: " + m_code_str;
					throw error;
				}
				char d = (m_operands[0]->get_value() - get_addr()) / 2 - 1;
				/*if ((d & 0xFFFFFF00) != 0)
				{
					std::string error = "Can't addressing jump command: " + m_code_str;
					error += "\n\tTry use R7 for jump";
					throw error;
				}*/
				unsigned short c = m_code;
				c |= (d & 0xFF);
				result.push_back(c);
			}
			break;
		case Type::Mark:
			{
				if (m_operands.size() != 1)
				{
					std::string error = "Wrong format of command: " + m_code_str;
					throw error;
				}
				if (!m_operands[0]->has_value())
				{
					std::string error = "Wrong operand for command: " + m_code_str;
					throw error;
				}
				unsigned short c = m_code;
				unsigned short v = m_operands[0]->get_value();
				if ((v & 0177700) != 0)
				{
					std::string error = "Value is large than allowed [0 .. 077(0x3F)]: " + m_code_str;
					throw error;
				}
				c |= ((v / 2) & 077);
				result.push_back(c);
			}
			break;
		case Type::Sob:
			{
				if (m_operands.size() != 2)
				{
					std::string error = "Wrong format of command: " + m_code_str;
					throw error;
				}
				if (!m_operands[1]->has_value() ||
					m_operands[0]->has_value() ||
					m_operands[0]->get_type() != Operand::Type::Reg)
				{
					std::string error = "Wrong operand(s) for command: " + m_code_str;
					throw error;
				}
				unsigned short v = m_operands[1]->get_value();
				unsigned short a = get_addr();
				if (v > a)
				{
					std::string error = "Value must be less than current address: " + m_code_str;
					throw error;
				}
				unsigned short d = (a - v) / 2 + 1;
				if ((d & 0177700) != 0)
				{
					std::string error = "Can't addressing SOB command: value large than allowed";
					throw error;
				}
				unsigned short c = m_code;
				c |= (m_operands[0]->get_registr() << 6);
				c |= (d & 077);
				result.push_back(c);
			}
			break;
		case Type::Full:
			{
				if (m_operands.size() != 0)
				{
					std::string error = "Wrong format of command: " + m_code_str;
					throw error;
				}
				result.push_back(m_code);
			}
			break;
		default:
			{
				std::string error = "Wrong type of command: " + m_code_str;
				throw error;
			}
		}
		return result;
	};

	virtual unsigned short get_size() const
	{
		unsigned short size = 1;
		switch (m_type)
		{
		case Command::DubcastArth:
		case Command::Dubcast:
			if (m_operands.size() == 2 && m_operands[1]->has_value())
				size++;
			if (m_type == Command::DubcastArth)
				break;
		case Command::Unicast:
			if (m_operands.size() >= 1 && m_operands[0]->has_value())
				size++;
			break;
		default:
			break;
		}
		return size * 2;
	}

	void add_operand(Operand* op)
	{
		m_operands.push_back(op);
	}

	virtual void write(Binary& bin) const
	{
		auto words = get_binary();
		for (unsigned short w : words)
			bin << w;
	};
};

bool Command::m_code_map_inited = false;
std::map<std::string, Command::SCommand> Command::m_code_map;

typedef Command::Code CommandCode;