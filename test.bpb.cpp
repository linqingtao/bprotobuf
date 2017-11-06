#include "test.bpb.h"
namespace a {
namespace b {
namespace c {
Head::Head()
: BMessage()
, start(0)
, afn(0)
, type(0)
, seq(0)
{
}
Head::~Head() {
}

Head::Head(const Head& msg) {
	msg.CopyTo(*this);
}
Head& Head::operator=(const Head& msg) {
	msg.CopyTo(*this);
}
void Head::CopyTo(Head& msg) const {
	msg.set_start(start);
	msg.set_afn(afn);
	msg.set_type(type);
	msg.set_seq(seq);
}
int Head::ByteSize() const {
	int size = 0;
	int i = 0;
	size += 13;
	return size;
}

int Head::ParseFromArray(const char* buffer, int len) {
	int pos = 0;
	int plain_size = 13;
	int diff_size = len - plain_size;
	if (len < plain_size) { return PARSE_BUFFER_SHORT;}
	start = *(char*)(buffer + pos); pos += sizeof(char);
	if (!g_small_endian) {
		afn = bswap_32(*(int*)(buffer + pos));
	} else {
		afn = *(int*)(buffer + pos);
	}
	pos += sizeof(int);
	if (!g_small_endian) {
		type = bswap_32(*(int*)(buffer + pos));
	} else {
		type = *(int*)(buffer + pos);
	}
	pos += sizeof(int);
	if (!g_small_endian) {
		seq = bswap_32(*(unsigned int*)(buffer + pos));
	} else {
		seq = *(unsigned int*)(buffer + pos);
	}
	pos += sizeof(unsigned int);
	return pos;
}
int Head::SerializeToArray(char* buffer, int len) const {
	int pos = 0;
	memcpy(buffer + pos,  &start, sizeof(char)); pos += sizeof(char);
	if (!g_small_endian) {
		int tmp = bswap_32(afn);
		memcpy(buffer + pos, &tmp, sizeof(int)); pos += sizeof(int);
	} else {
		memcpy(buffer + pos,  &afn, sizeof(int)); pos += sizeof(int);
	}
	if (!g_small_endian) {
		int tmp = bswap_32(type);
		memcpy(buffer + pos, &tmp, sizeof(int)); pos += sizeof(int);
	} else {
		memcpy(buffer + pos,  &type, sizeof(int)); pos += sizeof(int);
	}
	if (!g_small_endian) {
		unsigned int tmp = bswap_32(seq);
		memcpy(buffer + pos, &tmp, sizeof(unsigned int)); pos += sizeof(unsigned int);
	} else {
		memcpy(buffer + pos,  &seq, sizeof(unsigned int)); pos += sizeof(unsigned int);
	}
	return pos;
}
Protocol1::Protocol1()
: BMessage()
, a(0)
, b(0)
{
}
Protocol1::~Protocol1() {
}

Protocol1::Protocol1(const Protocol1& msg) {
	msg.CopyTo(*this);
}
Protocol1& Protocol1::operator=(const Protocol1& msg) {
	msg.CopyTo(*this);
}
void Protocol1::CopyTo(Protocol1& msg) const {
	msg.set_a(a);
	msg.set_b(b);
}
int Protocol1::ByteSize() const {
	int size = 0;
	int i = 0;
	size += 12;
	return size;
}

int Protocol1::ParseFromArray(const char* buffer, int len) {
	int pos = 0;
	int plain_size = 12;
	int diff_size = len - plain_size;
	if (len < plain_size) { return PARSE_BUFFER_SHORT;}
	if (!g_small_endian) {
		a = bswap_32(*(int*)(buffer + pos));
	} else {
		a = *(int*)(buffer + pos);
	}
	pos += sizeof(int);
	if (!g_small_endian) {
		b = bswap_64(*(long long*)(buffer + pos));
	} else {
		b = *(long long*)(buffer + pos);
	}
	pos += sizeof(long long);
	return pos;
}
int Protocol1::SerializeToArray(char* buffer, int len) const {
	int pos = 0;
	if (!g_small_endian) {
		int tmp = bswap_32(a);
		memcpy(buffer + pos, &tmp, sizeof(int)); pos += sizeof(int);
	} else {
		memcpy(buffer + pos,  &a, sizeof(int)); pos += sizeof(int);
	}
	if (!g_small_endian) {
		long long tmp = bswap_64(b);
		memcpy(buffer + pos, &tmp, sizeof(long long)); pos += sizeof(long long);
	} else {
		memcpy(buffer + pos,  &b, sizeof(long long)); pos += sizeof(long long);
	}
	return pos;
}
Protocol2::Protocol2()
: BMessage()
, a(0)
, b(0)
{
}
Protocol2::~Protocol2() {
}

Protocol2::Protocol2(const Protocol2& msg) {
	msg.CopyTo(*this);
}
Protocol2& Protocol2::operator=(const Protocol2& msg) {
	msg.CopyTo(*this);
}
void Protocol2::CopyTo(Protocol2& msg) const {
	msg.set_a(a);
	msg.set_b(b);
}
int Protocol2::ByteSize() const {
	int size = 0;
	int i = 0;
	size += 12;
	return size;
}

int Protocol2::ParseFromArray(const char* buffer, int len) {
	int pos = 0;
	int plain_size = 12;
	int diff_size = len - plain_size;
	if (len < plain_size) { return PARSE_BUFFER_SHORT;}
	if (!g_small_endian) {
		a = bswap_32(*(int*)(buffer + pos));
	} else {
		a = *(int*)(buffer + pos);
	}
	pos += sizeof(int);
	if (!g_small_endian) {
		b = bswap_64(*(long long*)(buffer + pos));
	} else {
		b = *(long long*)(buffer + pos);
	}
	pos += sizeof(long long);
	return pos;
}
int Protocol2::SerializeToArray(char* buffer, int len) const {
	int pos = 0;
	if (!g_small_endian) {
		int tmp = bswap_32(a);
		memcpy(buffer + pos, &tmp, sizeof(int)); pos += sizeof(int);
	} else {
		memcpy(buffer + pos,  &a, sizeof(int)); pos += sizeof(int);
	}
	if (!g_small_endian) {
		long long tmp = bswap_64(b);
		memcpy(buffer + pos, &tmp, sizeof(long long)); pos += sizeof(long long);
	} else {
		memcpy(buffer + pos,  &b, sizeof(long long)); pos += sizeof(long long);
	}
	return pos;
}
SendMsg::SendMsg()
: BMessage()
, head()
, type(0)
, l(0)
, u(0)
, f(0)
, bita(0)
, bitb(0)
, bitc(0)
, bitd(0)
, string_len(0)
, info_len(0)
, bytes_len(0)
, uint16_len(0)
, heads_len(0)
, strings_len(0)
, data2(NULL)
, __uint16s_len(0)
, uint16s(NULL)
, child(NULL)
, string_len1(0)
, bytes_len1(0)
, array_len1(0)
{
	str[0] = 0;
	str3[0] = 0;
}
SendMsg::~SendMsg() {
	if (data2 != NULL) {
		delete[] data2; data2 = NULL;
	}
	if (uint16s != NULL) {
		free(uint16s); uint16s = NULL;
	}
	if (child != NULL) {
		delete child; child = NULL;
	}
}

SendMsg::SendMsg(const SendMsg& msg) {
	msg.CopyTo(*this);
}
SendMsg& SendMsg::operator=(const SendMsg& msg) {
	msg.CopyTo(*this);
}
void SendMsg::CopyTo(SendMsg& msg) const {
	msg.set_head(head);
	msg.set_type(type);
	msg.set_l(l);
	msg.set_u(u);
	msg.set_f(f);
	msg.set_bita(bita);
	msg.set_bitb(bitb);
	msg.set_bitc(bitc);
	msg.set_bitd(bitd);
	msg.set_string_len(string_len);
	msg.set_info_len(info_len);
	msg.set_bytes_len(bytes_len);
	msg.set_uint16_len(uint16_len);
	msg.set_heads_len(heads_len);
	msg.set_strings_len(strings_len);
	{
		int i = 0;
		int __len = 2;
		for (; i < __len; ++i) {
			msg.add_heads(i, heads[i]);
		}
	}
	{
		int i = 0;
		int __len = heads_len;
		for (; i < __len; ++i) {
			msg.add_var_heads(var_heads[i]);
		}
	}
	{
		int i = 0;
		int __len = 5;
		for (; i < __len; ++i) {
			msg.add_int16s(i, int16s[i]);
		}
	}
	{
		int i = 0;
		int __len = uint16_len;
		for (; i < __len; ++i) {
			msg.add_uint16s(uint16s[i]);
		}
	}
	if (type == 1) {
		msg.set_child(*(Protocol1*)child);
	} else if (type == 2) {
		msg.set_child(*(Protocol2*)child);
	} 
	msg.set_string_len1(string_len1);
	msg.set_bytes_len1(bytes_len1);
	msg.set_array_len1(array_len1);
	{
		int i = 0;
		int __len = array_len1;
		for (; i < __len; ++i) {
			msg.add_heads3(heads3[i]);
		}
	}
}
int SendMsg::ByteSize() const {
	int size = 0;
	int i = 0;
	size += head.ByteSize();
	size += bytes_len;
	size += string_len;
	for (i = 0; i < 2; ++i) {
		size += heads[i].ByteSize();
	}
	for (i = 0; i < heads_len; ++i) {
		size += var_heads[i].ByteSize();
	}
	size += (sizeof(unsigned int) * uint16_len);
	size += child->ByteSize();
	size += string_len1;
	size += bytes_len1;
	for (i = 0; i < array_len1; ++i) {
		size += heads3[i].ByteSize();
	}
	size += 102;
	return size;
}

int SendMsg::ParseFromArray(const char* buffer, int len) {
	int pos = 0;
	int plain_size = 102;
	int diff_size = len - plain_size;
	if (len < plain_size) { return PARSE_BUFFER_SHORT;}
	{
		int tmp_head_size = head.ParseFromArray(buffer + pos, len - pos);
		if (tmp_head_size <= 0) { return tmp_head_size;}
		diff_size -= tmp_head_size;
		if (diff_size < 0) {
			return PARSE_BUFFER_SHORT;
		}
		pos += tmp_head_size;
	}
	type = *(char*)(buffer + pos); pos += sizeof(char);
	if (!g_small_endian) {
		l = bswap_64(*(long long*)(buffer + pos));
	} else {
		l = *(long long*)(buffer + pos);
	}
	pos += sizeof(long long);
	if (!g_small_endian) {
		u = bswap_32(*(unsigned int*)(buffer + pos));
	} else {
		u = *(unsigned int*)(buffer + pos);
	}
	pos += sizeof(unsigned int);
	if (!g_small_endian) {
		f = bswap_64(*(double*)(buffer + pos));
	} else {
		f = *(double*)(buffer + pos);
	}
	pos += sizeof(double);
	{
		int i = 1;
		for (; i >= 0; --i) {
			bita <<= 1;
			bita |= (buffer[ pos + (0 + i)/8] & (1 << (((0 + i)%8)))) ? 1 : 0;
		}
	}
	{
		int i = 2;
		for (; i >= 0; --i) {
			bitb <<= 1;
			bitb |= (buffer[ pos + (2 + i)/8] & (1 << (((2 + i)%8)))) ? 1 : 0;
		}
	}
	{
		int i = 4;
		for (; i >= 0; --i) {
			bitc <<= 1;
			bitc |= (buffer[ pos + (5 + i)/8] & (1 << (((5 + i)%8)))) ? 1 : 0;
		}
	}
	{
		int i = 6;
		for (; i >= 0; --i) {
			bitd <<= 1;
			bitd |= (buffer[ pos + (10 + i)/8] & (1 << (((10 + i)%8)))) ? 1 : 0;
		}
	}
	pos += 3;
	if (!g_small_endian) {
		string_len = bswap_32(*(int*)(buffer + pos));
	} else {
		string_len = *(int*)(buffer + pos);
	}
	pos += sizeof(int);
	if (!g_small_endian) {
		info_len = bswap_32(*(int*)(buffer + pos));
	} else {
		info_len = *(int*)(buffer + pos);
	}
	pos += sizeof(int);
	if (!g_small_endian) {
		bytes_len = bswap_32(*(int*)(buffer + pos));
	} else {
		bytes_len = *(int*)(buffer + pos);
	}
	pos += sizeof(int);
	if (!g_small_endian) {
		uint16_len = bswap_32(*(int*)(buffer + pos));
	} else {
		uint16_len = *(int*)(buffer + pos);
	}
	pos += sizeof(int);
	if (!g_small_endian) {
		heads_len = bswap_32(*(int*)(buffer + pos));
	} else {
		heads_len = *(int*)(buffer + pos);
	}
	pos += sizeof(int);
	if (!g_small_endian) {
		strings_len = bswap_32(*(int*)(buffer + pos));
	} else {
		strings_len = *(int*)(buffer + pos);
	}
	pos += sizeof(int);
	memcpy(data, buffer + pos, 20); pos += 20;
	diff_size -= bytes_len;
	if (diff_size < 0) {
		return PARSE_BUFFER_SHORT;
	}
	data2 = new(std::nothrow) char[bytes_len];
	memcpy(data2, buffer + pos, bytes_len); pos += bytes_len;
	memcpy(str, buffer + pos, 12); str[12] = 0; pos += 12;
	diff_size -= string_len;
	if (diff_size < 0) {
		return PARSE_BUFFER_SHORT;
	}
	str2 = std::string(buffer + pos, string_len); pos += string_len;
	{
		int i = 0;
		for (; i < 2; ++i) {
			int msg_size = heads[i].ParseFromArray(buffer + pos, len - pos);
			if (msg_size <= 0) { return PARSE_BUFFER_SHORT; }
			diff_size -= msg_size;
			if (diff_size < 0) { return PARSE_BUFFER_SHORT; }
			pos += msg_size;
		}
	}
	{
		int i = 0;
		for (; i < heads_len; ++i) {
			var_heads.push_back(Head());
			int msg_size = var_heads[i].ParseFromArray(buffer + pos, len - pos);
			if (msg_size <= 0) { return PARSE_BUFFER_SHORT; }
			diff_size -= msg_size;
			if (diff_size < 0) { return PARSE_BUFFER_SHORT; }
			pos += msg_size;
		}
	}
	memcpy(int16s, buffer + pos, 5 * sizeof(short));
	pos += (sizeof(short) * 5);
	if (!g_small_endian) {
		int i = 0;
		for (; i < 5; ++i) {
			int16s[i] = bswap_16(int16s[i]);
		}
	}
	diff_size -= (sizeof(unsigned int) * uint16_len);
	if (diff_size < 0) { return PARSE_BUFFER_SHORT; }
	uint16s = (unsigned int*)malloc(uint16_len * sizeof(unsigned int));
	memcpy(uint16s, buffer + pos, uint16_len * sizeof(unsigned int));
	pos += (sizeof(unsigned int) * uint16_len);
	if (!g_small_endian) {
		int i = 0;
		for (; i < uint16_len; ++i) {
			uint16s[i] = bswap_32(uint16s[i]);
		}
	}
	if (type == 1) {
		child = new(std::nothrow) Protocol1();
		int tmp_Protocol1_size = child->ParseFromArray(buffer + pos, len - pos);
		if (tmp_Protocol1_size <= 0) { return PARSE_INVALID;}
		diff_size -= tmp_Protocol1_size;
		if (diff_size < 0) { return PARSE_BUFFER_SHORT; }
		pos += tmp_Protocol1_size;
	} else if (type == 2) {
		child = new(std::nothrow) Protocol2();
		int tmp_Protocol2_size = child->ParseFromArray(buffer + pos, len - pos);
		if (tmp_Protocol2_size <= 0) { return PARSE_INVALID;}
		diff_size -= tmp_Protocol2_size;
		if (diff_size < 0) { return PARSE_BUFFER_SHORT; }
		pos += tmp_Protocol2_size;
	} 
	if (!g_small_endian) {
		string_len1 = bswap_32(*(int*)(buffer + pos));
	} else {
		string_len1 = *(int*)(buffer + pos);
	}
	pos += sizeof(int);
	if (!g_small_endian) {
		bytes_len1 = bswap_32(*(int*)(buffer + pos));
	} else {
		bytes_len1 = *(int*)(buffer + pos);
	}
	pos += sizeof(int);
	if (!g_small_endian) {
		array_len1 = bswap_32(*(int*)(buffer + pos));
	} else {
		array_len1 = *(int*)(buffer + pos);
	}
	pos += sizeof(int);
	diff_size -= string_len1;
	if (diff_size < 0) {
		return PARSE_BUFFER_SHORT;
	}
	if (string_len1 > 6) { return PARSE_INVALID; }
	memcpy(str3, buffer + pos, string_len1); str3[string_len1] = 0; pos += string_len1;
	diff_size -= bytes_len1;
	if (diff_size < 0) {
		return PARSE_BUFFER_SHORT;
	}
	if (bytes_len1 > 4) { return PARSE_INVALID; }
	memcpy(data3, buffer + pos, bytes_len1); pos += bytes_len1;
	if (array_len1 > 3) { return PARSE_INVALID; }
	{
		int i = 0;
		for (; i < array_len1; ++i) {
			int msg_size = heads3[i].ParseFromArray(buffer + pos, len - pos);
			if (msg_size <= 0) { return PARSE_BUFFER_SHORT; }
			diff_size -= msg_size;
			if (diff_size < 0) { return PARSE_BUFFER_SHORT; }
			pos += msg_size;
		}
	}
	return pos;
}
int SendMsg::SerializeToArray(char* buffer, int len) const {
	int pos = 0;
	pos += head.SerializeToArray(buffer+pos, len-pos);
	memcpy(buffer + pos,  &type, sizeof(char)); pos += sizeof(char);
	if (!g_small_endian) {
		long long tmp = bswap_64(l);
		memcpy(buffer + pos, &tmp, sizeof(long long)); pos += sizeof(long long);
	} else {
		memcpy(buffer + pos,  &l, sizeof(long long)); pos += sizeof(long long);
	}
	if (!g_small_endian) {
		unsigned int tmp = bswap_32(u);
		memcpy(buffer + pos, &tmp, sizeof(unsigned int)); pos += sizeof(unsigned int);
	} else {
		memcpy(buffer + pos,  &u, sizeof(unsigned int)); pos += sizeof(unsigned int);
	}
	if (!g_small_endian) {
		double tmp = bswap_64(f);
		memcpy(buffer + pos, &tmp, sizeof(double)); pos += sizeof(double);
	} else {
		memcpy(buffer + pos,  &f, sizeof(double)); pos += sizeof(double);
	}
	buffer[ pos + 0] |= ((bita>>0)<<0);
	buffer[ pos + 0] |= ((bitb>>0)<<2);
	buffer[ pos + 0] |= ((bitc>>0)<<5);
	buffer[ pos + 1] |= ((bitc>>3)<<0);
	buffer[ pos + 1] |= ((bitd>>0)<<2);
	buffer[ pos + 2] |= ((bitd>>6)<<0);
	pos += 3;
	if (!g_small_endian) {
		int tmp = bswap_32(string_len);
		memcpy(buffer + pos, &tmp, sizeof(int)); pos += sizeof(int);
	} else {
		memcpy(buffer + pos,  &string_len, sizeof(int)); pos += sizeof(int);
	}
	if (!g_small_endian) {
		int tmp = bswap_32(info_len);
		memcpy(buffer + pos, &tmp, sizeof(int)); pos += sizeof(int);
	} else {
		memcpy(buffer + pos,  &info_len, sizeof(int)); pos += sizeof(int);
	}
	if (!g_small_endian) {
		int tmp = bswap_32(bytes_len);
		memcpy(buffer + pos, &tmp, sizeof(int)); pos += sizeof(int);
	} else {
		memcpy(buffer + pos,  &bytes_len, sizeof(int)); pos += sizeof(int);
	}
	if (!g_small_endian) {
		int tmp = bswap_32(uint16_len);
		memcpy(buffer + pos, &tmp, sizeof(int)); pos += sizeof(int);
	} else {
		memcpy(buffer + pos,  &uint16_len, sizeof(int)); pos += sizeof(int);
	}
	if (!g_small_endian) {
		int tmp = bswap_32(heads_len);
		memcpy(buffer + pos, &tmp, sizeof(int)); pos += sizeof(int);
	} else {
		memcpy(buffer + pos,  &heads_len, sizeof(int)); pos += sizeof(int);
	}
	if (!g_small_endian) {
		int tmp = bswap_32(strings_len);
		memcpy(buffer + pos, &tmp, sizeof(int)); pos += sizeof(int);
	} else {
		memcpy(buffer + pos,  &strings_len, sizeof(int)); pos += sizeof(int);
	}
	memcpy(buffer + pos, data, 20); pos += 20;
	memcpy(buffer + pos, data2, bytes_len); pos += bytes_len;
	memcpy(buffer + pos, str, 12); pos += 12;
	memcpy(buffer + pos, str2.c_str(), string_len); pos += string_len;
	{
		int i = 0;
		for (; i < 2; ++i) {
			int msg_size = heads[i].SerializeToArray(buffer + pos, len - pos);
			if (msg_size <= 0) { return PARSE_BUFFER_SHORT; }
			pos += msg_size;
		}
	}
	{
		int i = 0;
		for (; i < heads_len; ++i) {
			int msg_size = var_heads[i].SerializeToArray(buffer + pos, len - pos);
			if (msg_size <= 0) { return PARSE_BUFFER_SHORT; }
			pos += msg_size;
		}
	}
	memcpy(buffer + pos, int16s, 5 * sizeof(short)); pos += 10;
	if (!g_small_endian) {
		int i = 0;
		for (; i < 5; ++i) {
			*(short*)(buffer + pos + i) = bswap_16(*(short*)(buffer + pos + i));
		}
	}
	memcpy(buffer + pos, uint16s, 0 * sizeof(unsigned int)); pos += (uint16_len * 4);
	if (!g_small_endian) {
		int i = 0;
		for (; i < 0; i += 2) {
			*(unsigned int*)(buffer + pos + i) = bswap_32(*(unsigned int*)(buffer + pos + i));
		}
	}
	{
		int tmp_len = 0;
		if (child != NULL) {
			tmp_len = child->SerializeToArray(buffer + pos, len - pos);
		}
		pos += tmp_len;
	}
	if (!g_small_endian) {
		int tmp = bswap_32(string_len1);
		memcpy(buffer + pos, &tmp, sizeof(int)); pos += sizeof(int);
	} else {
		memcpy(buffer + pos,  &string_len1, sizeof(int)); pos += sizeof(int);
	}
	if (!g_small_endian) {
		int tmp = bswap_32(bytes_len1);
		memcpy(buffer + pos, &tmp, sizeof(int)); pos += sizeof(int);
	} else {
		memcpy(buffer + pos,  &bytes_len1, sizeof(int)); pos += sizeof(int);
	}
	if (!g_small_endian) {
		int tmp = bswap_32(array_len1);
		memcpy(buffer + pos, &tmp, sizeof(int)); pos += sizeof(int);
	} else {
		memcpy(buffer + pos,  &array_len1, sizeof(int)); pos += sizeof(int);
	}
	memcpy(buffer + pos, str3, string_len1); pos += string_len1;
	memcpy(buffer + pos, data3, bytes_len1); pos += bytes_len1;
	{
		int i = 0;
		for (; i < array_len1; ++i) {
			int msg_size = heads3[i].SerializeToArray(buffer + pos, len - pos);
			if (msg_size <= 0) { return PARSE_BUFFER_SHORT; }
			pos += msg_size;
		}
	}
	return pos;
}
void SendMsg::set_data2(const char* buffer, int len) {
	if (data2 != NULL) { delete data2; }
	data2 = new(std::nothrow) char[len];
	memcpy(data2, buffer, len);
	bytes_len = len;
}
void SendMsg::set_child(const Protocol1& msg) {
	child = new(std::nothrow) Protocol1();
	msg.CopyTo(*(Protocol1*)child);
	type = 1;
}

void SendMsg::set_child(const Protocol2& msg) {
	child = new(std::nothrow) Protocol2();
	msg.CopyTo(*(Protocol2*)child);
	type = 2;
}

void SendMsg::add_var_heads(const Head& msg) {
	var_heads.push_back(msg);
	heads_len++;
}
void SendMsg::add_uint16s(unsigned int val) {
	if (__uint16s_len == uint16_len) {
		void* tmp = realloc(uint16s, (uint16_len * 2 + 1) * sizeof(unsigned int));
		__uint16s_len <<= 1;
		++__uint16s_len;
		if (tmp != uint16s) { free(uint16s); uint16s = (unsigned int*)tmp; }
	}
	uint16s[uint16_len++] = val;
}
void SendMsg::add_heads3(const Head& msg) {
	heads3[array_len1++] = msg;
}
}
}
}
