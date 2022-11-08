#pragma once
#include "IDataType.h"
class Int : public IDataType
{
public:
	Int();
	Int(const int& _int);
	Int(const Int& _int);
	~Int();
	std::string ToString();
	int ToInt();
	Int operator=(const Int& _int);
	Int operator=(const int& _int);
	Int operator+(const Int& _int);
	Int operator+(const int& _int);
	Int operator-(const Int& _int);
	Int operator-(const int& _int);
	Int operator*(const Int& _int);
	Int operator*(const int& _int);
	Int operator/(const Int& _int);
	Int operator/(const int& _int);
	Int operator%(const Int& _int);
	Int operator%(const int& _int);
	bool operator==(const Int& _int);
	bool operator==(const int& _int);
	bool operator!=(const Int& _int);
	bool operator!=(const int& _int);
	friend std::ostream& operator<<(std::ostream& out, const Int& _int);
	//friend std::istream& operator>>(std::istream& in, const Int& _int);
private:
	int _int;
};

