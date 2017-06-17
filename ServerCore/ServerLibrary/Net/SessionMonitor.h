#pragma once
#include "stdafx.h"

class SessionMonitor : public Work
{
public :
	SessionMonitor();
	void tick();
};
static SessionMonitor sessionMonitor;