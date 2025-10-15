#include "pch.h"
#include "PlayerManager.h"
#include "AccountManager.h"

PlayerManager GPlayerManager;

void PlayerManager::PlayerThenAccount()
{
	WRITE_LOCK;
	GAccountManager.AccountThenPlayer();
}

void PlayerManager::Lock()
{
	WRITE_LOCK;
}


