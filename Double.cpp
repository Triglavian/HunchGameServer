#include "Double.h"

Double::Double()
{
	this->_double = 0;
}

Double::Double(const double& _double)
{
	this->_double = _double;
}

Double::Double(const Double& _double)
{
	this->_double = _double._double;
}

Double::~Double()
{
}

std::string Double::ToString()
{
	return std::to_string(_double);
}

double Double::ToDouble() const
{
	return _double;
}

double& Double::ToDouble()
{
	return _double;
}

void Double::operator=(const Double& _double)
{
	this->_double = _double._double;
}

void Double::operator=(const double& _double)
{
	this->_double = _double;
}

Double Double::operator+(const Double& _double)
{
	return Double(this->_double + _double._double);
}

Double Double::operator+(const double& _double)
{
	return Double(this->_double + _double);
}

Double Double::operator-(const Double& _double)
{
	return Double(this->_double - _double._double);
}

Double Double::operator-(const double& _double)
{
	return Double(this->_double - _double);
}

Double Double::operator*(const Double& _double)
{
	return Double(this->_double * _double._double);
}

Double Double::operator*(const double& _double)
{
	return Double(this->_double * _double);
}

Double Double::operator/(const Double& _double)
{
	return Double(this->_double / _double._double);
}

Double Double::operator/(const double& _double)
{
	return Double(this->_double / _double);
}

bool  Double::operator==(const Double& _double)
{
	return this->_double == _double._double;
}

bool Double::operator==(const double& _double)
{
	return this->_double == _double;
}

bool Double::operator!=(const Double& _double)
{
	return this->_double != _double._double;
}

bool Double::operator!=(const double& _double)
{
	return this->_double != _double;
}

std::ostream& operator<<(std::ostream& out, const Double& _double)
{
	out << _double._double;
	return out;
}
