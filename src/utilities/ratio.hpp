#pragma once

struct Ratio {
public:
	Ratio(unsigned int numerator, unsigned int denominator);
	Ratio(float fRatio);
	operator float();
	//operator =(float ratio);
	void set(float newRatio);

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