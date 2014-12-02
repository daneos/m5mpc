/*
 * m5mpc -- MPD client for Maemo 5
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#include "Exception.h"

//-----------------------------------------------------------------------------
Exception::Exception(int id)
{
	this->id = id;
}

//-----------------------------------------------------------------------------
Exception::~Exception()
{

}

//-----------------------------------------------------------------------------
const char *Exception::toString(void)
{
	return strings[this->id];
}

//-----------------------------------------------------------------------------
int Exception::getId(void)
{
	return this->id;
}
