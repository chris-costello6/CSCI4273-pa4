/*********************/
/* This is the most primitive implementation of the Message library.
*/

/* Written by: Shiv Mishra on October 20, 2014 */
/* Last update: October 20, 2014 */

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <utility>

using namespace std;

class Message
{
	typedef pair<char*, size_t> MsgPair;

public:
   
    Message( );
    Message(char* msg, size_t len);
    ~Message( );
    void msgAddHdr(char *hdr, size_t length);
    char* msgStripHdr(int len);
    int msgSplit(Message& secondMsg, size_t len);
    void msgJoin(Message& secondMsg);
    size_t msgLen( );
    void msgFlat(char *buffer);

private:
    size_t m_MsgLen;
    list<MsgPair> m_MsgContent;
};

Message::Message()
{
	m_MsgLen = 0;
	m_MsgContent.clear();
}

Message::Message(char* msg, size_t len)
{
	MsgPair newMsgPair(msg, len);
	m_MsgLen = len;
	m_MsgContent.push_front(newMsgPair);
}

Message::~Message( )
{
    m_MsgContent.clear();
}

void 
Message::msgAddHdr(char *hdr, size_t length) 
{
	MsgPair hdrMsgPair(hdr, length);
	m_MsgLen += length;
	m_MsgContent.push_front(hdrMsgPair);
}

char* 
Message::msgStripHdr(int len)
{
	char *stripped_content;
	
    if ((m_MsgLen < len) || (len == 0)) 
    	return NULL;

    // if it's a perfect boundary
    if (len == static_cast<int>(m_MsgContent.front().second))
    {
    	stripped_content = m_MsgContent.front().first;
    	m_MsgContent.pop_front();
    	m_MsgLen -= len;
    	return stripped_content;
    }
    // if it wants less than the first header
    else if (len < static_cast<int>(m_MsgContent.front().second))
    {
    	stripped_content = new char[len];

    	memcpy(stripped_content, m_MsgContent.front().first, len);

    	// strip len chars from the front
    	for (int i = 0; i < len; i++)
    	{
    		m_MsgContent.front().first++;
    		m_MsgContent.front().second--;
    	}

    	m_MsgLen -= len;

    	return stripped_content;
    }
    // if it wants more than the first header
    else
    {
    	stripped_content = new char[len];

    	int cumm = 0;

		while (cumm < len)			
		{
			int currLen = static_cast<int>(m_MsgContent.front().second);
			// Add length of the front header
			cumm += currLen;

			// If cumm is now >= to the len, just splice this one
			if(cumm >= len)
		    {
		    	int spliceLen = len-(cumm-currLen);

		    	memcpy(stripped_content+(cumm-currLen), m_MsgContent.front().first, spliceLen);

	    	 	// strip len chars from the front
		    	for (int i = 0; i < spliceLen; i++)
		    	{
		    		m_MsgContent.front().first++;
		    		m_MsgContent.front().second--;
		    	}

		    	break;
		    }
		    // Otherwise, just pull off the whole thing
		    else
		    {
		    	memcpy(stripped_content+(cumm-currLen), m_MsgContent.front().first, m_MsgContent.front().second);
		    }

		    m_MsgContent.pop_front();
		}

		m_MsgLen -= len;

		return stripped_content;	    	
    }

}

int 
Message::msgSplit(Message& secondMsg, size_t len)
{
	list<MsgPair> secondMsgContent;
	list<MsgPair> tempContent;

	if ((len < 0) || (len > m_MsgLen)) 
		return 0;
	
	// if it's a perfect boundary
	if (len == static_cast<int>(m_MsgContent.front().second))
    {
    	tempContent.push_front(m_MsgContent.front());
    	m_MsgContent.pop_front();

    	secondMsg.m_MsgContent = m_MsgContent;
    	secondMsg.m_MsgLen = m_MsgLen-len;

    	m_MsgLen = len;
    	m_MsgContent = tempContent;
    }
    // not a boundary, check where to splice
    else
    {
    	int cumm = 0;

		while (cumm < len)			
		{
			int currLen = static_cast<int>(m_MsgContent.front().second);
			
			// Add length of the front header
			cumm += currLen;

			// If cumm is now >= to the len, just splice this one
			if(cumm >= len)
		    {
		    	int spliceLen = len-(cumm-currLen);
		    	char* newChunk = new char[spliceLen];

		    	memcpy(newChunk, m_MsgContent.front().first, spliceLen);

		    	MsgPair newMsgPair(newChunk, spliceLen);

		    	tempContent.push_back(newMsgPair);

	    	 	// strip len chars from the front
		    	for (int i = 0; i < spliceLen; i++)
		    	{
		    		m_MsgContent.front().first++;
		    		m_MsgContent.front().second--;
		    	}

		    	break;
		    }
		    // Otherwise, just pull off the whole thing
		    else
		    {
		    	tempContent.push_back(m_MsgContent.front());
		    	m_MsgContent.pop_front();

		    	m_MsgLen -= currLen;
		    }

		}

		secondMsg.m_MsgContent = m_MsgContent;
		secondMsg.m_MsgLen = m_MsgLen-len;
		
		m_MsgLen = len;
    	m_MsgContent = tempContent;
    }

	return 1;
}

void 
Message::msgJoin(Message& secondMsg)
{
	list<MsgPair> tempContent = secondMsg.m_MsgContent;

	while (!tempContent.empty())
	{
		m_MsgContent.push_back(tempContent.front());
		tempContent.pop_front();
	}

	m_MsgLen += secondMsg.m_MsgLen;
	
	secondMsg.m_MsgContent.clear();
	secondMsg.m_MsgLen = 0;
}

size_t 
Message::msgLen( )
{
	return m_MsgLen;
}

void 
Message::msgFlat(char *buffer)
{
	//Assume that sufficient memory has been allocated in buffer

	list<MsgPair>::const_iterator iter = m_MsgContent.begin();

	int cumm = 0;

	for (iter; iter != m_MsgContent.end(); ++iter)
	{
		memcpy(buffer+cumm, iter->first, iter->second);

		cumm += iter->second;
	}
	
}

