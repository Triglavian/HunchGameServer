#include "Char.h"

Char::Char()
{
	_char = '\0';
}

Char::Char(char _char)
{
	this->_char = _char;
}

Char::Char(const Char& _char)
{
	this->_char = _char._char;
}

Char::~Char()
{

}

std::string Char::ToString()
{
	return std::to_string(_char);
}

char Char::ToChar() const
{
	return _char;
}

char& Char::ToCharRef()
{
	return _char;
}

void Char::operator=(const Char& _char)
{
	this->_char = _char._char;
}

void Char::operator=(const char& _char)
{
	this->_char = _char;
}

bool Char::operator==(const Char& _char)
{
	return this->_char == _char._char;
}

bool Char::operator==(const char& _char)
{
	return this->_char == _char;
}