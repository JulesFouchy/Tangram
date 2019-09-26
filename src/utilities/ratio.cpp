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

void Ratio::recomputeAspectRatio() {
	m_ratio = (float)m_numerator / m_denominator;
	m_bMustRecomputeAspectRatio = false;
}

void Ratio::recomputeNumAndDenom() {
	spdlog::error("Ratio::recomputeNumAndDenom not iplemented yet !");
}

unsigned int* Ratio::getNumeratorPtr() {
	m_bMustRecomputeAspectRatio = true;
	return &m_numerator;
}

unsigned int* Ratio::getDenominatorPtr() {
	m_bMustRecomputeAspectRatio = true;
	return &m_denominator;
}