#pragma once
#include "pch.h"
class AccountManager
{
	USE_LOCK;

public : 
	void AccountThenPlayer();
	void Lock();
};

extern AccountManager GAccountManager;
