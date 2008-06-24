#include "stdafx.h"
#include <srpc/utility/Unicode.h>
#include <srpc/Types.h>
#include <boost/static_assert.hpp>
#include <limits>
#include <cwchar>

/*
UTF-8과 UCS-4간의 변환 규칙
===========================

  UCS-4                    UTF-8
~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
0x00000000 - 0x0000007F  0xxxxxxx
0x00000080 - 0x000007FF  110xxxxx 10xxxxxx
0x00000800 - 0x0000FFFF  1110xxxx 10xxxxxx 10xxxxxx
0x00010000 - 0x001FFFFF  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
0x00200000 - 0x03FFFFFF  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
0x04000000 - 0x7FFFFFFF  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx

- 0x00000000 ~ 0x010FFFFF 사이의 UCS-4 값만이 유효하다고
  Unicode 3.1와 ISO 10646:2003에 정의되어 있다

- 출처: http://www.jinsuk.pe.kr/Unicode/Unicode_intro-kr.html
*/

// stolen from boost utf8_codecvt_facet.*

namespace srpc {

namespace {

typedef String::value_type Char;
typedef WString::value_type WChar;
typedef String::const_iterator cstringiterator;
typedef WString::const_iterator cwstringiterator;


inline bool invalidLeadingOctet(UInt8 octet)
{
    return (0x7f < octet && octet < 0xc0) || (octet > 0xfd);
}


inline bool invalidContinuingOctet(UInt8 octet)
{
    return (octet < 0x80) || (octet > 0xbf);
}


inline int getOctetCount(UInt8 leadOctet)
{
    //assert(! invalidLeadingOctet(leadOctet));

    // if the 0-bit (MSB) is 0, then 1 character
    if (leadOctet <= 0x7f)
        return 1;

    // Otherwise the count number of consecutive 1 bits starting at MSB
    //assert(0xc0 <= leadOctet && leadOctet <= 0xfd);
    if (0xc0 <= leadOctet && leadOctet <= 0xdf)
        return 2;
    if (0xe0 <= leadOctet && leadOctet <= 0xef)
        return 3;
    if (0xf0 <= leadOctet && leadOctet <= 0xf7)
        return 4;
    if (0xf8 <= leadOctet && leadOctet <= 0xfb)
        return 5;
    return 6;
}


/// continuing octets = octets except for the leading octet
inline int getContOctetCount(UInt8 leadOctet) {
    return getOctetCount(leadOctet) - 1;
}


template<std::size_t s>
inline int getContOctetOutCountImpl(wchar_t word){
    if (word < 0x80) {
        return 0;
    }
    if (word < 0x800) {
        return 1;
    }
    return 2;
}


template<>
inline int getContOctetOutCountImpl<4>(wchar_t word){
    if (word < 0x80) {
        return 0;
    }
    if (word < 0x800) {
        return 1;
    }
    if (word < 0x10000) {
        return 2;
    }
    if (word < 0x200000) {
        return 3;
    }
    if (word < 0x4000000) {
        return 4;
    }
    return 5;
}


inline int getContOctetOutCount(wchar_t word)
{
    return getContOctetOutCountImpl<sizeof(wchar_t)>(word);
}

} // namespace

String toUtf8(const WString& ucs)
{
    const wchar_t octetModifierTable[] =
        { 0x00, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc };

    String utf8;
    utf8.reserve(ucs.length() * sizeof(WString::value_type));

    const cwstringiterator end = ucs.end();
    for(cwstringiterator current = ucs.begin(); current != end; ++current) {
        const wchar_t wc = *current;
        if (wc > (std::numeric_limits<wchar_t>::max)()) {
            //assert(false && "invalid ucs-2(4) character");
            break;
        }

        const int contOctetCount = getContOctetOutCount(wc);
        int shiftExponent = contOctetCount * 6;

        utf8 += static_cast<Char>(octetModifierTable[contOctetCount] +
            (wc >> shiftExponent));

        for (int contOctetIndex = 0; contOctetIndex < contOctetCount;
            ++contOctetIndex) {
            shiftExponent -= 6;
            utf8 += static_cast<Char>(0x80 + ((wc >> shiftExponent) & 0x3f));
        }
    }
    return utf8;
}


WString fromUtf8(const String& utf8)
{
    const wchar_t octetModifierTable[] =
        { 0xff, 0x1f, 0x0f, 0x07, 0x03, 0x01 };
    const int minvalTable[] = { 0, 0x80, 0x800, 0x10000, 0x110000, 0x4000000 };

    WString ucs;
    ucs.reserve(utf8.length());

    const cstringiterator end = utf8.end();
    for (cstringiterator current = utf8.begin(); current != end;) {
        const Char octet = *current;
        if (invalidLeadingOctet(octet)) {
            //assert(false && "invalid leading octet");
            break;
        }
        const int contOctetCount = getContOctetCount(octet);

        if (std::distance(current, end) > contOctetCount) {
            wchar_t wc = octet & octetModifierTable[contOctetCount];
            const cstringiterator contEnd =
                boost::next(current, contOctetCount + 1);
            for (cstringiterator contCurrent = boost::next(current);
                contCurrent != contEnd; ++contCurrent) {
                const Char contOctet = *contCurrent;
                if (invalidContinuingOctet(contOctet)) {
                    //assert(false && "continuing octet must be 10xxxxxx");
                    return ucs;
                }
                wc <<= 6;
                wc |= contOctet & 0x3f;
            }

            if (wc < minvalTable[contOctetCount]) {
                //assert(false && "invalid unicode character");
                break;
            }
            else {
                ucs += wc;
            }
        }
        else {
            //assert(false && "incomplete unicode character");
            break;
        }
        std::advance(current, contOctetCount + 1);
    }
    return ucs;
}

} // namespace srpc
