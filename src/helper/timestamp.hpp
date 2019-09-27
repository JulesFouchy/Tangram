#pragma once

class Timestamp {

public:
	Timestamp();
	~Timestamp();
	float timeSince(Timestamp thatDate);
	float getAge();
	static float getTime();
public:
	void reset();

private:
	float m_creationDate;
};