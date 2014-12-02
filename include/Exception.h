/*
 * m5mpc -- MPD client for Maemo 5
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#if !defined(__EXCEPTION_H__)
#define __EXCEPTION_H__

enum {
	E_TEST,
	E_CONNECTION_FAILED,
	E_STATUS_FAILED,
	E_STATUS_INCORRECT
};

static char *strings[] = {
	"Test exception, used for debugging.",
	"Could not connect.",
	"Could not get status."
};

class Exception
{
private:
	int id;

public:
	Exception(int id);
	~Exception();
	const char *toString(void);
	int getId(void);
};

#endif /* __EXCEPTION_H__ */
