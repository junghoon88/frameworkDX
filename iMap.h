#pragma once


class iMap //interface 용 가상클래스
{
public:
	virtual bool GetHeight(IN float x, OUT float& y, IN float z) = 0;
};

