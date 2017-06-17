#pragma once
#include "stdafx.h"

class WinSocket
{
	WSADATA            wsa_;

public:
	WinSocket()
	{
		static bool init = false;
		if (init) {
			return;
		}

		if (WSAStartup(MAKEWORD(2, 2), &wsa_) != 0) {
			printf("! wsa startup fail\n");
			exit(0);
		}

		init = true;
		printf("### WSA 2.2 set complet.\n");
	}
	~WinSocket()
	{
		WSACleanup();
	}
};

static WinSocket winsocket;