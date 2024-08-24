#pragma once
#include "../inline.h"

// namespace mkd::blocks {
//     class HTML;
// }

namespace mkd::inlines {
    class HTMLTag : public Inline {
    public:
        HTMLTag(const std::string& html);

        static std::shared_ptr<HTMLTag> parse(Source& src);

        const std::string html;
    
    private:
        static std::string parseTagName(Source& src);
        static std::string parseAttribute(Source& src);
        static std::string parseAttributeValueSpecification(Source& src);
        static std::string parseUnquotedAttributeValue(Source& src);
        static std::string parseSingleQuotedAttributeValue(Source& src);
        static std::string parseDoubleQuotedAttributeValue(Source& src);
        static std::string parseOpenTag(Source& src);
        static std::string parseClosingTag(Source& src);
        static std::string parseComment(Source& src);
        static std::string parseProcessingInstruction(Source& src);
        static std::string parseDeclaration(Source& src);
        static std::string parseCDATA(Source& src);
    };
}