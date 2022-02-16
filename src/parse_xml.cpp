#include <iostream>
#include "parse_xml.h"

void ParseXML::xml_parser(const char *file, SHT_BUFF *buff) {
    memset(buff, 0, sizeof(SHT_BUFF));
    TiXmlDocument doc;
    if (!doc.LoadFile(file)) {
        std::cout << "Load XML file failed. " << doc.ErrorDesc() << std::endl;
        exit(-1);
    }
    TiXmlElement *root = doc.FirstChildElement();
    if (root->NoChildren()) {
        std::cout << "XML file format error, please check it" << std::endl;
        exit(-1);
    }
    TiXmlElement *msg = root->FirstChildElement();
    while(msg) {
        if(!msg->NoChildren()) {
            TiXmlElement *next = msg->FirstChildElement();
            while(next) {
                if(!strcmp("topic", next->Value())) {
                    strcpy(buff->sht_head[buff->used_num].name,  next->GetText());
                    //std::cout << std::stoi(next->Attribute("size")) << std::stoi(next->Attribute("dt_len")) << std::endl;
                    buff->sht_head[buff->used_num].buff_info.dt_len = static_cast<uint32_t>(std::stoi(next->Attribute("dt_len")));
                    buff->sht_head[buff->used_num].buff_info.size = static_cast<uint32_t>(std::stoi(next->Attribute("size")));
                    buff->sht_head[buff->used_num].total_len = (buff->sht_head[buff->used_num].buff_info.dt_len*buff->sht_head[buff->used_num].buff_info.size);
                    buff->data_len += buff->sht_head[buff->used_num].total_len;
                    //std::cout << "topic_name: " << buff->sht_head[buff->used_num].name << std::endl;
                    //std::cout << "dt_len: " << buff->sht_head[buff->used_num].buff_info.dt_len << std::endl;
                    //std::cout << "size: " << buff->sht_head[buff->used_num].buff_info.size << std::endl;
                    //std::cout << "sht data len: " << buff->sht_head[buff->used_num].total_len << "\n" << std::endl;
                    buff->used_num++;
                }else {
                    std::cout << "Program doesn't use " << next->GetText() << " now" << std::endl;
                }
                next = next->NextSiblingElement();
            }
        }
        if(strcmp("topic_info", msg->Value())) {
            std::cout << msg->Value() << " : " << msg->GetText() << std::endl;
        }
        msg = msg->NextSiblingElement();
    }
}
