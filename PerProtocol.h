/*********************/
/* This is the most primitive implementation of the Message library.
*/

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <utility>

#include "Message.h"
#include "ThreadPool.h"

using namespace std;

class PerProtocol
{

public:
	
	PerProtocol( );

private:

	int inUDPSock;
	int outUDPSock;

};