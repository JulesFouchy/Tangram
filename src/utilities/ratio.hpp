#pragma once

struct Ratio {
public:
	Ratio(unsigned int numerator, unsigned int denominator);
	operator float();
	void updateRatio();

	unsigned int* getNumeratorPtr();
	unsigned int* getDenominatorPtr();
private:
	unsigned int m_numerator;
	unsigned int m_denominator;
	float m_ratio;
};