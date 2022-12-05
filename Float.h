#pragma once
#include "IDataType.h"
class Float : public IDataType
{
public:
	Float();
	Float(const float& _float);
	Float(const Float& _float);
	~Float();
	std::string ToString();
	float ToFloat() const;
	float& ToFloatRef();
	void operator=(const Float& _float);
	void operator=(const float& _float);
	Float operator+(const Float& _float);
	Float operator+(const float& _float);
	Float operator-(const Float& _float);
	Float operator-(const float& _float);
	Float operator*(const Float& _float);
	Float operator*(const float& _float);
	Float operator/(const Float& _float);
	Float operator/(const float& _float);
	bool operator==(const Float& _float);
	bool operator==(const float& _float);
	bool operator!=(const Float& _float);
	bool operator!=(const float& _float);
	friend std::ostream& operator<<(std::ostream& out, const Float& _float);
private:
	float _float;
};

