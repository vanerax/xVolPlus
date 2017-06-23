#include "StdAfx.h"
#include "Config.h"

//CConfig::m_fGap = 0.05f;
UINT CConfig::uVolDownKey = VK_LEFT;
UINT CConfig::uVolUpKey = VK_RIGHT;
float CConfig::fVolGap = 0.05f;
UINT CConfig::uModifiers = MOD_CONTROL;


CConfig::CConfig(void)
{
	SetDefault();

}

CConfig::~CConfig(void)
{
}

void CConfig::SetDefault() {
	fVolGap = 0.05f;
	uVolDownKey = VK_LEFT;
	uVolUpKey = VK_RIGHT;
	uModifiers = MOD_CONTROL;

}

UINT CConfig::GetVolDownKey() {
	return CConfig::uVolDownKey;
}


UINT CConfig::GetVolUpKey() {
	return CConfig::uVolUpKey;
}

UINT CConfig::GetModifiers() {
	return uModifiers;
}

float CConfig::GetVolGap(void) {
	return fVolGap;
}