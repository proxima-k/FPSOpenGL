#pragma once

#include "steam/steam_api.h"

class SteamManager
{
public:
	int getAppId()
	{
		return APP_ID;
	};

private:
	const int APP_ID = 480;
};