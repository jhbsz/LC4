/*******************************************************************************************************************************
*
 *	Copyright (c) 2007  QNAP Systems, Inc.  All Rights Reserved.
 *
 *	FILE:
 *		protocols.c
 *
 *	Abstract: 
 *		Common finder protocol utility function implementations.  
 *		Should not be modified unless extremely necessary.
 *
 *	HISTORY:
 *		03/26/07	meiji chang re-create
 *
 ********************************************************************************************************************************/

#define	__INCLUDE_FROM_FINDER_PROTOCOLS_IMPLEMENTATION__	
 
#include <stdio.h>
#include <string.h>
#include "protocols.h"
#include "dbg.h"

// Organize all command data (cmd, seq_num, result, tag_buf) into packet_buf for sending out
// Note: cmd & seq_num & result will be handled with SREV() automatically, do no do SREV in advance
int MakeFinderCommandPacket(char *packet_buf, unsigned int packet_buf_size, unsigned char preamble[8], unsigned short cmd, unsigned short seq_num, unsigned short result, const char *tag_buf, unsigned int tag_buf_len)
{
	// ASSERT( packet_buf_size >= sizeof(FINDER_CMD_HEADER) );
	if (packet_buf_size < sizeof(FINDER_CMD_HEADER)) return 0;

	// ASSERT( tag_buf_len <= (packet_buf_size - sizeof(FINDER_CMD_HEADER)) );
	if (tag_buf_len > (packet_buf_size - sizeof(FINDER_CMD_HEADER))) return 0;

	FINDER_CMD_PACKET *pPacket = (FINDER_CMD_PACKET *) packet_buf;
	int ret = 0;

	// fill in command header...
	// ASSERT(pPacket->Cmd_Hdr.Preamble == 8);
	memcpy(pPacket->Cmd_Hdr.Preamble, preamble, sizeof(pPacket->Cmd_Hdr.Preamble));
	pPacket->Cmd_Hdr.Cmd = SREV(cmd);
	pPacket->Cmd_Hdr.SeqNum = SREV(seq_num);
	pPacket->Cmd_Hdr.Result = SREV(result);

	pPacket->Cmd_Hdr.pad = SREV(PAD_IGNORE_SEQ_NUM);
	ret = sizeof(FINDER_CMD_HEADER);

	// append tag buffer if any...
	if (tag_buf && tag_buf_len) {
		memcpy(pPacket->Tag_List, tag_buf, tag_buf_len);
		ret += tag_buf_len;
	}

	return ret;
}





// return the data length written in the buf
// return 0 if buffer too small...
int TagsToBuffer(DATA_TAG tags[], int tag_cnt, char *buf, int buf_size)
{
	// ASSERT(buf != NULL);

	int len = 0;
	for (int i = 0; i < tag_cnt; i++) {

		if (len + tags[i].len + 2 > buf_size) return 0;		// avoid buffer overflow...

		memcpy(buf, (void *)&(tags[i]), tags[i].len+2);
		len += (tags[i].len + 2);		// tag & len for each tag takes 2 bytes...
		buf += (tags[i].len + 2);		// tag & len for each tag takes 2 bytes...
	}

	return len;
}

// return the data length written in the buf
// return 0 if buffer too small...
int TagListToBuffer(TAG_LIST *tag_list, char *buf, int buf_size)
{
	// ASSERT(tag_list != NULL);
	return TagsToBuffer(tag_list->tags, tag_list->tag_num, buf, buf_size);
}



// return true if add successfully...
bool AddTagToTagList(TAG_LIST *pTagList, int tag_id, void *data, int len)
{
	// ASSERT( pTagList );
	// ASSERT( MAX_SESSION_NODE_TAG_COUNT == (sizeof(pTagList->tags) / sizeof(pTagList->tags[0])) );
	if (pTagList->tag_num >= MAX_SESSION_NODE_TAG_COUNT) {
		DBG("AddTagToTagList()- Error: too many tags (>=MAX_SESSION_NODE_TAG_COUNT).\n");
		return false;
	}

	if (len > sizeof(pTagList->tags[0].data)) return false;

	memcpy(pTagList->tags[pTagList->tag_num].data, data, len);
	pTagList->tags[pTagList->tag_num].tag = tag_id;
	pTagList->tags[pTagList->tag_num].len = len;

	pTagList->tag_num++;

	return true;
}

bool AddExtTagToTagList(TAG_LIST *pTagList, int tag_id, void *data, int len)
{
	unsigned char ext_data_len = len;

	if (pTagList->tag_num >= MAX_SESSION_NODE_TAG_COUNT) {
		DBG("AddTagToTagList()- Error: too many tags (>=MAX_SESSION_NODE_TAG_COUNT).\n");
		return false;
	}

	if(ext_data_len > EXT_DATA_TAG_MAX_SIZE) return false;

	EXT_DATA_TAG *pExtTag = (EXT_DATA_TAG *)pTagList->tags[pTagList->tag_num].data;

	pTagList->tags[pTagList->tag_num].tag = TAG_EXTENSION;
	pTagList->tags[pTagList->tag_num].len = sizeof(pExtTag->tag) + sizeof(pExtTag->len) + ext_data_len;
	memcpy(pExtTag->data, data, ext_data_len);
	pExtTag->tag = tag_id;
	pExtTag->len = ext_data_len;

	pTagList->tag_num++;

	return true;
}

// return true if add successfully...
// NOTE: this function will do IREV automatically...
bool AddTagIntegerToTagList(TAG_LIST *pTagList, int tag_id, int value)
{
	// assert( pTagList );
	// assert( MAX_SESSION_NODE_TAG_COUNT == (sizeof(pTagList->tags) / sizeof(pTagList->tags[0])) );
	if (pTagList->tag_num >= MAX_SESSION_NODE_TAG_COUNT) {
		DBG("AddTagIntegerToTagList()- Error: too many tags (>=MAX_SESSION_NODE_TAG_COUNT).\n");
		return false;
	}
	
	int n = IREV(value);
	memcpy(pTagList->tags[pTagList->tag_num].data, &n, sizeof(n));

	pTagList->tags[pTagList->tag_num].tag = tag_id;
	pTagList->tags[pTagList->tag_num].len = sizeof(value);		// because it's char only, no need to do IREV...

	pTagList->tag_num++;

	return true;
}

bool AddExtTagIntegerToTagList(TAG_LIST *pTagList, int tag_id, int value)
{
	int n = IREV(value);
	unsigned char ext_data_len = sizeof(value);

	// assert( pTagList );
	// assert( MAX_SESSION_NODE_TAG_COUNT == (sizeof(pTagList->tags) / sizeof(pTagList->tags[0])) );
	if (pTagList->tag_num >= MAX_SESSION_NODE_TAG_COUNT) {
		DBG("AddTagIntegerToTagList()- Error: too many tags (>=MAX_SESSION_NODE_TAG_COUNT).\n");
		return false;
	}

	if(ext_data_len > EXT_DATA_TAG_MAX_SIZE) return false;

	EXT_DATA_TAG *pExtTag = (EXT_DATA_TAG *)pTagList->tags[pTagList->tag_num].data;

	pTagList->tags[pTagList->tag_num].tag = TAG_EXTENSION;
	pTagList->tags[pTagList->tag_num].len = sizeof(pExtTag->tag) + sizeof(pExtTag->len) + ext_data_len;
	memcpy(pExtTag->data, &n, sizeof(n));
	pExtTag->tag = tag_id;
	pExtTag->len = ext_data_len;

	pTagList->tag_num++;

	return true;
}

// return true if add successfully...
// NOTE: this function will do IREV automatically...
bool AddTagIntegerArrayToTagList(TAG_LIST *pTagList, int tag_id, int *value_array, int count)
{
	// assert( pTagList );
	// assert( MAX_SESSION_NODE_TAG_COUNT == (sizeof(pTagList->tags) / sizeof(pTagList->tags[0])) );
	if (pTagList->tag_num >= MAX_SESSION_NODE_TAG_COUNT) {
		DBG("AddTagIntegerArrayToTagList()- Error: too many tags (>=MAX_SESSION_NODE_TAG_COUNT).\n");
		return false;
	}

	int len = 0;
	for (int i = 0; i < count; i++) {
		if ( len + sizeof(int) > 0xff ) break;		// len is only one byte value, so cannot exceed 255...
		int n = IREV(value_array[i]);
		memcpy(pTagList->tags[pTagList->tag_num].data + len, &n, sizeof(n));
		len += sizeof(n);
	}

	pTagList->tags[pTagList->tag_num].tag = tag_id;
	pTagList->tags[pTagList->tag_num].len = len;		// because it's char only, no need to do IREV...

	// DBG("AddTagIntegerArrayToTagList()- Add %d bytes, %d items.\n", len, i);

	pTagList->tag_num++;

	return true;
}

bool AddExtTagIntegerArrayToTagList(TAG_LIST *pTagList, int tag_id, int *value_array, int count)
{
	if (pTagList->tag_num >= MAX_SESSION_NODE_TAG_COUNT) {
		DBG("AddTagIntegerArrayToTagList()- Error: too many tags (>=MAX_SESSION_NODE_TAG_COUNT).\n");
		return false;
	}

	unsigned char ext_data_len = 0;
	EXT_DATA_TAG *pExtTag = (EXT_DATA_TAG *)pTagList->tags[pTagList->tag_num].data;

	pTagList->tags[pTagList->tag_num].tag = TAG_EXTENSION;
	pExtTag->tag = tag_id;
	for (int i = 0; i < count; i++)
	{
		if ( ext_data_len > EXT_DATA_TAG_MAX_SIZE ) break;		// len is only one byte value, so cannot exceed EXT_DATA_TAG_MAX_SIZE (123)...
		int n = IREV(value_array[i]);
		memcpy(pExtTag->data + ext_data_len, &n, sizeof(n));
		ext_data_len += sizeof(n);
	}
	pExtTag->len = ext_data_len;
	pTagList->tags[pTagList->tag_num].len = sizeof(pExtTag->tag) + sizeof(pExtTag->len) + ext_data_len;

	pTagList->tag_num++;

	return true;
}

// return true if add successfully...
bool AddTagStringToTagList(TAG_LIST *pTagList, int tag_id, const char *szData)
{
	// ASSERT( pTagList );
	// ASSERT( MAX_SESSION_NODE_TAG_COUNT == (sizeof(pTagList->tags) / sizeof(pTagList->tags[0])) );
	if (pTagList->tag_num >= MAX_SESSION_NODE_TAG_COUNT) {
		DBG("AddTagStringToTagList()- Error: too many tags (>=MAX_SESSION_NODE_TAG_COUNT).\n");
		return false;		// too many tags...
	}

	int len = strlen(szData) + 1;		// include the null terminated char.
	if (len > sizeof(pTagList->tags[0].data)) return false;

	memcpy(pTagList->tags[pTagList->tag_num].data, szData, len);
	pTagList->tags[pTagList->tag_num].tag = tag_id;
	pTagList->tags[pTagList->tag_num].len = len;

	pTagList->tag_num++;

	return true;
}

bool AddExtTagStringToTagList(TAG_LIST *pTagList, int tag_id, const char *szData)
{
	// ASSERT( pTagList );
	// ASSERT( MAX_SESSION_NODE_TAG_COUNT == (sizeof(pTagList->tags) / sizeof(pTagList->tags[0])) );
	if (pTagList->tag_num >= MAX_SESSION_NODE_TAG_COUNT) {
		DBG("AddTagStringToTagList()- Error: too many tags (>=MAX_SESSION_NODE_TAG_COUNT).\n");
		return false;		// too many tags...
	}

	int len = strlen(szData) + 1;		// include the null terminated char.
	if (len > EXT_DATA_TAG_MAX_SIZE) return false;

	unsigned char ext_data_len = len;
	EXT_DATA_TAG *pExtTag = (EXT_DATA_TAG *)pTagList->tags[pTagList->tag_num].data;

	pTagList->tags[pTagList->tag_num].tag = TAG_EXTENSION;
	pTagList->tags[pTagList->tag_num].len = sizeof(pExtTag->tag) + sizeof(pExtTag->len) + ext_data_len;
	pExtTag->tag = tag_id;
	pExtTag->len = ext_data_len;
	memcpy(pExtTag->data, szData, ext_data_len);

	pTagList->tag_num++;

	return true;
}


// search packet (cmd + tag_buf) and get specific tag data
// the data buffer is provided with length, will check for buffer overflow...
// return written length if found, o/w 0
int GetTagData(FINDER_CMD_PACKET *packet_buf, int packet_buf_len, int tag_id, void *tag_buf, int tag_buf_size)
{
	// assert(packet_buf);
	// assert(tag_buf);
	
	unsigned char *buf = (unsigned char *) packet_buf;

	for (int i = sizeof(packet_buf->Cmd_Hdr); i < packet_buf_len;) 
	{
		DATA_TAG *tag = (DATA_TAG *) &(buf[i]);
		if (tag->tag == tag_id) 
		{
			int l = tag->len;
			if (tag->len > tag_buf_size) l = tag_buf_size;
			memcpy(tag_buf, tag->data, l);
			return l;
		}
		else if(tag->tag == TAG_EXTENSION)
		{
			EXT_DATA_TAG *ext_tag = (EXT_DATA_TAG *) tag->data;
			if(ext_tag->tag == tag_id)
			{
				int m = ext_tag->len;
				if (ext_tag->len > tag_buf_size) m = tag_buf_size;
				memcpy(tag_buf, ext_tag->data, m);
				return m;
			}
		}

		i += (tag->len + 2);
	}
	return 0;
}


// search packet (cmd + tag_buf) and get specific tag data
// the data buffer is provided with length, will check for buffer overflow...
// return written length if found, o/w 0
char *GetTagDataString(FINDER_CMD_PACKET *packet_buf, int packet_buf_len, int tag_id, void *tag_buf, int tag_buf_size)
{
	// assert(tag_buf);
	int len = GetTagData(packet_buf, packet_buf_len, tag_id, tag_buf, tag_buf_size);
	
//	if ( tag_buf && ( len < tag_buf_size ) ) ((unsigned char *)tag_buf)[len] = 0;
	if ( len < tag_buf_size ) ((unsigned char *)tag_buf)[len] = 0;
	else ((unsigned char *)tag_buf)[tag_buf_size-1] = 0;	// one byte was overwritten for EOS
	
	return (char *) tag_buf;
}




/*
// return true if add successfully...
bool AddTagToSessionNode(SESSION_NODE *pSN, int tag_id, void *data, int len)
{
	// ASSERT( MAX_SESSION_NODE_TAG_COUNT == (sizeof(pSN->dt) / sizeof(pSN->dt[0])) );
	if (pSN->dtIdx >= MAX_SESSION_NODE_TAG_COUNT) return false;

	if (len > sizeof(pSN->dt[0].data)) return false;

	memcpy(pSN->dt[pSN->dtIdx].data, data, len);
	pSN->dt[pSN->dtIdx].tag = tag_id;
	pSN->dt[pSN->dtIdx].len = len;

	pSN->dtIdx++;

	return true;
}

bool AddExtTagToSessionNode(SESSION_NODE *pSN, int tag_id, void *data, int len)
{
	unsigned char ext_data_len = len;

	// ASSERT( MAX_SESSION_NODE_TAG_COUNT == (sizeof(pSN->dt) / sizeof(pSN->dt[0])) );
	if (pSN->dtIdx >= MAX_SESSION_NODE_TAG_COUNT) return false;

	if (ext_data_len > EXT_DATA_TAG_MAX_SIZE) return false;

	EXT_DATA_TAG *ext_tag = (EXT_DATA_TAG *) pSN->dt[pSN->dtIdx].data;

	pSN->dt[pSN->dtIdx].tag = TAG_EXTENSION;
	pSN->dt[pSN->dtIdx].len = sizeof(ext_tag->tag) + sizeof(ext_tag->len) + ext_data_len;
	ext_tag->tag = tag_id;
	ext_tag->len = ext_data_len;
	memcpy(ext_tag->data, data, ext_data_len);

	pSN->dtIdx++;

	return true;
}

// value will be handled with IREV() automatically.
bool AddTagIntegerToSessionNode(SESSION_NODE *pSN, int tag_id, int value)
{
	int n = IREV(value);
	return AddTagToSessionNode(pSN, tag_id, &n, sizeof(n));
}

bool AddExtTagIntegerToSessionNode(SESSION_NODE *pSN, int tag_id, int value)
{
	int n = IREV(value);
	return AddExtTagToSessionNode(pSN, tag_id, &n, sizeof(n));
}
*/








