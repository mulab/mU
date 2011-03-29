/*
 * UnicodeDevice.h
 *
 *  Created on: 2010-1-7
 *      Author: baritono
 */

#ifndef UNICODEDEVICE_H_
#define UNICODEDEVICE_H_

#include <iosfwd>
#include <boost/shared_ptr.hpp>
#include <boost/iostreams/categories.hpp>
#include "Exceptions.h"
namespace io = boost::iostreams;

namespace mU {

template<typename IOS>
class UnicodeDevice {
private:
	boost::shared_ptr<IOS> iostream;
	enum
	{
		LEN_1_MASK = 0x80,
		LEN_1_CHECK = 0x00,
		LEN_2_MASK = 0xe0,
		LEN_2_CHECK = 0xc0,
		LEN_3_MASK = 0xf0,
		LEN_3_CHECK = 0xe0,
		LEN_4_MASK = 0xf8,
		LEN_4_CHECK = 0xf0
	};
public:
	typedef wchar_t char_type;
	typedef io::bidirectional_device_tag category;

	UnicodeDevice(const boost::shared_ptr<IOS> &ios) : iostream(ios)
	{}

	std::streamsize read(wchar_t *s, std::streamsize n)
	{
		for (std::streamsize i = 0; i < n; ++i)
		{
			char c[4];
			if (!iostream->get(c[0]))
			{
				if (iostream->eof())
				{
					if (i != 0)
					{
						return i;
					}
					else
					{
						return -1;
					}
				}
				else
				{
					throw IOException();
				}
			}
			if ((c[0] & LEN_1_MASK) == LEN_1_CHECK)
			{
				s[i] = static_cast<wchar_t>(c[0]);
				if (c[0] == '\n')
				{
					return i+1;
				}
			}
			else if ((c[0] & LEN_2_MASK) == LEN_2_CHECK)
			{
				if (!iostream->get(c[1]))
				{
					if (iostream->eof())
					{
						throw InvalidUtf8StreamException();
					}
					else
					{
						throw IOException();
					}
				}
				if ((c[1] & 0xc0) != 0x80)
				{
					throw InvalidUtf8StreamException();
				}
				s[i] = (static_cast<wchar_t>(c[0] & 0x1f) << 6)
						| static_cast<wchar_t>(c[1] & 0x3f);
			}
			else if ((c[0] & LEN_3_MASK) == LEN_3_CHECK)
			{
				if (!iostream->read(c+1, 2))
				{
					if (iostream->eof())
					{
						throw InvalidUtf8StreamException();
					}
					else
					{
						throw IOException();
					}
				}
				if ((c[1] & 0xc0) != 0x80 || (c[2] & 0xc0) != 0x80)
				{
					throw InvalidUtf8StreamException();
				}
				s[i] = (static_cast<wchar_t>(c[0] & 0x0f) << 12)
						| (static_cast<wchar_t>(c[1] & 0x3f) << 6)
						| static_cast<wchar_t>(c[2] & 0x3f);
			}
			else if ((c[0] & LEN_4_MASK) == LEN_4_CHECK)
			{
#ifdef _WIN32
				throw UnsupportedCharacterException();
#else
				if (!iostream->read(c+1, 3))
				{
					if (iostream->eof())
					{
						throw InvalidUtf8StreamException();
					}
					else
					{
						throw IOException();
					}
				}
				if ((c[1] & 0xc0) != 0x80 || (c[2] & 0xc0) != 0x80
						|| (c[3] & 0xc0) != 0x80)
				s[i] = (static_cast<wchar_t>(c[0] & 0x07) << 18)
						| (static_cast<wchar_t>(c[1] & 0x3f) << 12)
						| (static_cast<wchar_t>(c[2] & 0x3f) << 6)
						| static_cast<wchar_t>(c[3] & 0x3f);
#endif
			}
			else
			{
				throw InvalidUtf8StreamException();
			}
		}
		return n;
	}

	std::streamsize write(const wchar_t *s, std::streamsize n)
	{
		for (std::streamsize i = 0; i < n; ++i)
		{
			char c[4];
			std::streamsize len;
			wchar_t o = s[i];
			if ((o & 0x7f) == o)
			{
				c[0] = static_cast<char>(o);
				len = 1;
			}
			else if ((o & 0x7ff) == o)
			{
				c[0] = static_cast<char>(((o & 0x7c0) >> 6) | 0xc0);
				c[1] = static_cast<char>((o & 0x3f) | 0x80);
				len = 2;
			}
#ifdef _WIN32
			else
#else
			else if ((o & 0xffff) == o)
#endif
			{
				c[0] = static_cast<char>(((o & 0xf000) >> 12) | 0xe0);
				c[1] = static_cast<char>(((o & 0xfc0) >> 6) | 0x80);
				c[2] = static_cast<char>((o & 0x3f) | 0x80);
				len = 3;
			}
#ifndef _WIN32
			else if ((o & 0xfffff) == o
						|| (o & 0xffff0000) == 0x100000)
			{
				c[0] = static_cast<char>(((o & 0x1c0000) >> 18) | 0xf0);
				c[1] = static_cast<char>(((o & 0x3f000) >> 12) | 0x80);
				c[2] = static_cast<char>(((o & 0xfc0) >> 6) | 0x80);
				c[3] = static_cast<char>((o & 0x3f) | 0x80);
				len = 4;
			}
			else
			{
				throw InvalidUtf8StreamException();
			}
#endif
			if (!iostream->write(c, len))
			{
				throw IOException();
			}
		}
		return n;
	}
};

}

#endif /* UNICODEDEVICE_H_ */
