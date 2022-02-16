#ifndef _PARSE_XML_H_
#define _PARSE_XML_H_

#include <string>
#include "tinyxml.h"
#include "hm_ipc.h"

class ParseXML
{
public:
    ParseXML() = default;
    ~ParseXML() = default;
    void xml_parser(const char *file, SHT_BUFF *buff);
};

#endif
