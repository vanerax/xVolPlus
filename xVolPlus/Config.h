#pragma once

class CConfig
{
public:
	CConfig(void);
	~CConfig(void);
	static UINT GetVolDownKey();
	static UINT GetVolUpKey();
	static UINT GetModifiers();
	static float GetVolGap();
	//static const int f;


private:
	static UINT uVolDownKey;
	static UINT uVolUpKey;
	static float fVolGap;
	static UINT uModifiers;


	void SetDefault();
};

