#include "ratio.hpp"

#include "UI/log.hpp"

Ratio::Ratio(unsigned int numerator, unsigned int denominator)
	: m_numerator(numerator), m_denominator(denominator)
{
	updateRatio();
}

Ratio::operator float() {
	return m_ratio;
}

void Ratio::updateRatio() {
	m_ratio = (float)m_numerator / m_denominator;
}

unsigned int* Ratio::getNumeratorPtr() {
	//spdlog::warn("Don't forget to call updateRatio() once you've changed the numerator !");
	return &m_numerator;
}

unsigned int* Ratio::getDenominatorPtr() {
	//spdlog::warn("Don't forget to call updateRatio() once you've changed the denominator !");
	return &m_denominator;
}