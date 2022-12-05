#pragma once
#include "IDataType.h"
class Char : public IDataType
{
public:
	Char();
	Char(char _char);
	Char(const Char& _char);
	~Char();
	std::string ToString();	
	char ToChar() const;
	char& ToCharRef();
	void operator=(const Char& _char);
	void operator=(const char& _char);
	bool operator==(const Char& _char);
	bool operator==(const char& _char);
private:
	char _char;
};
