#include "Float.h"

Float::Float()
{
	this->_float = 0;
}

Float::Float(const float& _float)
{
	this->_float = _float;
}

Float::Float(const Float& _float)
{
	this->_float = _float._float;
}

Float::~Float()
{

}

std::string Float::ToString()
{
	return std::to_string(_float);
}

float Float::ToFloat() const
{
	return _float;
}

float& Float::ToFloatRef()
{
	return _float;
}

void Float::operator=(const Float& _float)
{
	this->_float = _float._float;
}

void Float::operator=(const float& _float)
{
	this->_float = _float;
}

Float Float::operator+(const Float& _float)
{
	return Float(this->_float + _float._float);
}

Float Float::operator+(const float& _float)
{
	return Float(this->_float + _float);
}

Float Float::operator-(const Float& _float)
{
	return Float(this->_float - _float._float);
}

Float Float::operator-(const float& _float)
{
	return Float(this->_float - _float);
}

Float Float::operator*(const Float& _float)
{
	return Float(this->_float * _float._float);
}

Float Float::operator*(const float& _float)
{
	return Float(this->_float * _float);
}

Float Float::operator/(const Float& _float)
{
	return Float(this->_float / _float._float);
}

Float Float::operator/(const float& _float)
{
	return Float(this->_float / _float);
}

bool  Float::operator==(const Float& _float)
{
	return this->_float == _float._float;
}

bool Float::operator==(const float& _float)
{
	return this->_float == _float;
}

bool Float::operator!=(const Float& _float)
{
	return this->_float != _float._float;
}

bool Float::operator!=(const float& _float)
{
	return this->_float != _float;
}

std::ostream& operator<<(std::ostream& out, const Float& _float)
{
	out << _float._float;
	return out;
}