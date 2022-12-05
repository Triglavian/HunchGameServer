#include "Int.h"

Int::Int()
{
	this->_int = 0;
}

Int::Int(const int& _int)
{
	this->_int = _int;
}

Int::Int(const Int& _int)
{
	this->_int = _int._int;
}

Int::~Int()
{
}

std::string Int::ToString()
{	
	return std::to_string(this->_int);
}

int Int::ToInt() const
{
	return _int;
}

int& Int::ToIntRef()
{
	return _int;
}
/*
int& Int::operator&()
{
	return _int;
}
*/
 void Int::operator=(const Int& _int)
{
	this->_int = _int._int;
}	

 void Int::operator=(const int& _int)
{
	this->_int = _int;
}

 Int Int::operator+(const Int& _int)
{
	return Int(this->_int + _int._int);
}

 Int Int::operator+(const int& _int)
{
	return Int(this->_int + _int);
}

 Int Int::operator-(const Int& _int)
{
	return Int(this->_int - _int._int);
}

 Int Int::operator-(const int& _int)
{
	return Int(this->_int - _int);
}

 Int Int::operator*(const Int& _int)
{
	return Int(this->_int * _int._int);
}

 Int Int::operator*(const int& _int)
{
	return Int(this->_int * _int);
}

 Int Int::operator/(const Int& _int)
{
	return Int(this->_int / _int._int);
}

 Int Int::operator/(const int& _int)
{
	return Int(this->_int / _int);
}

 Int Int::operator%(const Int& _int)
{
	return Int(this->_int % _int._int);
}

 Int Int::operator%(const int& _int)
{
	return Int(this->_int % _int);
}

 bool  Int::operator==(const Int& _int)
 {
	 return this->_int == _int._int;
 }

 bool Int::operator==(const int& _int)
 {
	 return this->_int == _int;
 }

 bool Int::operator!=(const Int& _int)
 {
	 return this->_int != _int._int;
 }

 bool Int::operator!=(const int& _int)
 {
	 return this->_int != _int;
 }

 bool Int::operator<(const Int& _int)
 {
	 return this->_int < _int._int;
 }

 bool Int::operator<(const int& _int)
 {
	 return this->_int < _int;
 }

 bool Int::operator>(const Int& _int)
 {
	 return this->_int > _int._int;
 }

 bool Int::operator>(const int& _int)
 {
	 return this->_int > _int;
 }

 std::ostream& operator<<(std::ostream& out, const Int& _int)
 {
	 out << _int._int;
	 return out;
 }

 /*
 std::istream& operator>>(std::istream& in, const Int& _int)
 {
	 in >> _int;
	 return in;
 }
 */
