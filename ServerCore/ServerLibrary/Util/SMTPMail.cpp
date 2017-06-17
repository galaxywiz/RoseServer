#include "stdafx.h"
#include "SMTPMail.h"

void smtpWriteLine(SOCKET sock, const char *str, const char *arg)
{
    array<char, SIZE_4096> buf;
    if (arg != NULL) {
        snprintf(buf, str, arg);
    }
    else {
        snprintf(buf, str);
    }

    ::send(sock, buf.data(), (int) buf.size(), 0);
}

bool connectSMTP(SOCKET *sock)
{
	*sock = ::socket(AF_INET, SOCK_STREAM, 0);
	if (*sock == SOCKET_ERROR) {
		SLog(L"! mail socket make fail");
		return false;
	}

	const int SMTP_PORT = 25;
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons((u_short)SMTP_PORT);
	inet_pton(AF_INET, "127.0.0.1", &(serverAddr.sin_addr));

	if (::connect(*sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		return false;
	}
	return true;
}

bool sendMail(const char *from, const char *to, const char *subject, const char *body)
{
	SOCKET sock;
	if (!connectSMTP(&sock)) {
		SLog(L"! smtp mail port connect fail, mail :[%S] body => %S", subject, body);
		return false;
	}
   
    smtpWriteLine(sock, "HELO %s", from);
    smtpWriteLine(sock, "MAIL FROM: %s", from);
    smtpWriteLine(sock, "RCPT TO: %s", to);
    smtpWriteLine(sock, "DATA", NULL);

    smtpWriteLine(sock, "From: %s", from);
    smtpWriteLine(sock, "To: %s", to);
    smtpWriteLine(sock, "Subject: %s", subject);

    smtpWriteLine(sock, "\n", NULL);

    smtpWriteLine(sock, "%s", body);

    smtpWriteLine(sock, ".", NULL);
    smtpWriteLine(sock, "QUIT", NULL);

    closesocket(sock);
	SLog(L"* send mail [%S]", subject);

    return true;
}