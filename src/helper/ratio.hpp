#pragma once

struct Ratio {
public:
	Ratio(unsigned int numerator, unsigned int denominator);
	Ratio(float fRatio);
	operator float();
	void set(float newRatio);
	void set(unsigned int num, unsigned int denom);

	unsigned int numerator();
	unsigned int denominator();

	unsigned int* getNumeratorPtr();
	unsigned int* getDenominatorPtr();

private:
	void recomputeAspectRatio();
	void recomputeNumAndDenom();
private:
	unsigned int m_numerator;
	unsigned int m_denominator;
	float m_ratio;

	bool m_bMustRecomputeAspectRatio;
	bool m_bMustRecomputeNumAndDenom;
};