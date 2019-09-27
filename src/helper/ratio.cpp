#include "ratio.hpp"

#include "UI/log.hpp"

Ratio::Ratio(unsigned int numerator, unsigned int denominator)
	: m_numerator(numerator), m_denominator(denominator),
	  m_bMustRecomputeAspectRatio(false), m_bMustRecomputeNumAndDenom(false)
{
	recomputeAspectRatio();
}

Ratio::Ratio(float fRatio)
	: m_ratio(fRatio),
	  m_bMustRecomputeAspectRatio(false), m_bMustRecomputeNumAndDenom(false)
{
	recomputeNumAndDenom();
}

Ratio::operator float() {
	if (m_bMustRecomputeAspectRatio)
		recomputeAspectRatio();
	return m_ratio;
}

void Ratio::set(float newRatio) {
	m_ratio = newRatio;
	m_bMustRecomputeNumAndDenom = true;
}

void Ratio::set(unsigned int num, unsigned int denom) {
	m_numerator = num;
	m_denominator = denom;
	m_bMustRecomputeAspectRatio = true;
}

void Ratio::recomputeAspectRatio() {
	m_ratio = (float)m_numerator / m_denominator;
	m_bMustRecomputeAspectRatio = false;
}

void Ratio::recomputeNumAndDenom() {
	// following this article : https://begriffs.com/pdf/dec2frac.pdf
	float precision = 0.00001f;
	float n = 1;
	float Z_n = m_ratio;
	int D_n_minus_one = 0;
	int D_n = 1;
	int N_n = round(m_ratio * D_n);
	while (n < 5 && abs(m_ratio - (float)D_n / N_n) > precision) {
		n++;
		float tmp = Z_n - floor(Z_n);
		if (abs(tmp) > precision) {
			Z_n = 1.0f / (tmp);
			int tmpD_n = D_n;
			D_n = D_n * floor(Z_n) + D_n_minus_one;
			D_n_minus_one = tmpD_n;
			N_n = round(m_ratio * D_n);
		}
		else {
			break;
		}
	}
	m_numerator = N_n;
	m_denominator = D_n;
	m_bMustRecomputeNumAndDenom = false;
	spdlog::info("{} is approx {} / {}", m_ratio, m_numerator, m_denominator);
}

unsigned int Ratio::numerator() {
	if (m_bMustRecomputeNumAndDenom)
		recomputeNumAndDenom();
	return m_numerator;
}
unsigned int Ratio::denominator() {
	if (m_bMustRecomputeNumAndDenom)
		recomputeNumAndDenom();
	return m_denominator;
}

unsigned int* Ratio::getNumeratorPtr() {
	m_bMustRecomputeAspectRatio = true;
	return &m_numerator;
}

unsigned int* Ratio::getDenominatorPtr() {
	m_bMustRecomputeAspectRatio = true;
	return &m_denominator;
}