#pragma once


class iMap //interface �� ����Ŭ����
{
public:
	virtual bool GetHeight(IN float x, OUT float& y, IN float z) = 0;
};

