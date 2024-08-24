#include "html_tag.h"

#include "flog/flog.h"

namespace mkd::inlines {
    HTMLTag::HTMLTag(const std::string& html) :
        Inline(INLINE_TYPE_HTML_TAG),
        html(html)
    {}

    std::shared_ptr<HTMLTag> HTMLTag::parse(Source& src) {
        // Attempt to parse any of the things it could be
        std::string html;
        if (html = parseOpenTag(src), !html.empty());
        else if (html = parseClosingTag(src), !html.empty());
        else if (html = parseComment(src), !html.empty());
        else if (html = parseProcessingInstruction(src), !html.empty());
        else if (html = parseDeclaration(src), !html.empty());
        else if (html = parseCDATA(src), !html.empty());
        else { return NULL; }

        // Commit the parsing session and return successfully
        return std::make_shared<HTMLTag>(html);
    }

    std::string HTMLTag::parseTagName(Source& src) {
        // Expect an ascii letter
        std::string name;
        if (!src.expect([&name](char c) { return name += c, std::isalpha(c); })) {
            return "";
        }

        // Expect any number of ascii letters, numbers or '-'
        while (src.expect([&name](char c) {
            if (!std::isalpha(c) && !std::isdigit(c) && c != '-') { return false; }
            name += c;
            return true;
        }));

        return name;
    }

    std::string HTMLTag::parseAttribute(Source& src) {
        // Begin parsing session
        src.begin();

        // Expect any number of tabs or space and up to one newline
        std::string attribute;
        bool newline = false;
        while (src.expect([&attribute, &newline](char c) {
            if (c == ' ' || c == '\t' || c == '\r') {
                attribute += c;
            }
            else if (!newline && c == '\n') {
                attribute += c;
                newline = true;
                return true;
            }
            return false;
        }));

        // Expect a letter, '_' or ':'
        if (!src.expect([&attribute](char c) { return attribute += c, std::isalpha(c); })) {
            src.abort();
            return "";
        }

        // Expect any number of ascii letters, numbers or '_', '.', ':' or '-'
        while (src.expect([&attribute](char c) {
            if (!std::isalpha(c) && !std::isdigit(c) && c != '_' && c != '.' && c != ':' && c != '-') {
                return false; 
            }
            attribute += c;
            return true;
        }));

        // Expect an optional value specification
        attribute += parseAttributeValueSpecification(src);

        // Commit and return
        src.commit();
        return attribute;
    }

    std::string HTMLTag::parseAttributeValueSpecification(Source& src) {
        // Begin parsing session
        src.begin();

        // Expect any number of tabs or space and up to one newline
        std::string valueSpec;
        bool newline = false;
        while (src.expect([&valueSpec, &newline](char c) {
            if (c == ' ' || c == '\t' || c == '\r') {
                valueSpec += c;
            }
            else if (!newline && c == '\n') {
                valueSpec += c;
                newline = true;
                return true;
            }
            return false;
        }));

        // Expect a '='
        if (!src.expect('=')) {
            src.abort();
            return "";
        }
        valueSpec += '=';

        // Expect any number of tabs or space and up to one newline
        newline = false;
        while (src.expect([&valueSpec, &newline](char c) {
            if (c == ' ' || c == '\t' || c == '\r') {
                valueSpec += c;
            }
            else if (!newline && c == '\n') {
                valueSpec += c;
                newline = true;
                return true;
            }
            return false;
        }));

        // Expect either an unquoted, single quoted or double quoted value
        std::string value;
        if (value = parseDoubleQuotedAttributeValue(src), !value.empty()) {
            src.commit();
            return valueSpec + value;
        }
        else if (value = parseSingleQuotedAttributeValue(src), !value.empty()) {
            src.commit();
            return valueSpec + value;
        }
        else if (value = parseUnquotedAttributeValue(src), !value.empty()) {
            src.commit();
            return valueSpec + value;
        }

        // Give up
        src.abort();
        return "";
    }

    std::string HTMLTag::parseUnquotedAttributeValue(Source& src) {
        // Expect any number of ascii letters, numbers or '_', '.', ':' or '-'
        std::string value;
        while (src.expect([&value](char c) {
            if (c == '"' || c == '\'' ||  c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '=' || c == '<' || c == '>' || c == '`') {
                return false; 
            }
            value += c;
            return true;
        }));

        // Return the value
        return value;
    }

    std::string HTMLTag::parseSingleQuotedAttributeValue(Source& src) {
        // Begin parsing session
        src.begin();

        // Expect a '\''
        std::string value;
        if (!src.expect('\'')) {
            src.abort();
            return "";
        }
        value += '\'';

        // Expect any number of characters except '\''
        while (src.expect([&value](char c) {
            if (c == '\'') { return false; }
            value += c;
            return true;
        }));

        // Expect a '\''
        if (!src.expect('\'')) {
            src.abort();
            return "";
        }
        value += '\'';

        // Commit and return value
        src.commit();
        return value;
    }

    std::string HTMLTag::parseDoubleQuotedAttributeValue(Source& src) {
        // Begin parsing session
        src.begin();

        // Expect a '"'
        std::string value;
        if (!src.expect('"')) {
            src.abort();
            return "";
        }
        value += '"';

        // Expect any number of characters except '"'
        while (src.expect([&value](char c) {
            if (c == '"') { return false; }
            value += c;
            return true;
        }));

        // Expect a '\''
        if (!src.expect('"')) {
            src.abort();
            return "";
        }
        value += '"';

        // Commit and return value
        src.commit();
        return value;
    }

    std::string HTMLTag::parseOpenTag(Source& src) {
        // Begin parsing session
        src.begin();

        // Expect a '<'
        std::string html = "<";
        if (!src.expect('<')) {
            flog::debug("A");
            src.abort();
            return "";
        }

        // Expect a tag name
        std::string name = parseTagName(src);
        if (name.empty()) {
            flog::debug("B");
            src.abort();
            return "";
        }
        html += name;

        // Expect any number of attributes
        std::string attrib;
        while (attrib = parseAttribute(src), !attrib.empty()) {
            html += attrib;
        }

        // Expect any number of tabs or space and up to one newline
        bool newline = false;
        while (src.expect([&html, &newline](char c) {
            if (c == ' ' || c == '\t' || c == '\r') {
                html += c;
            }
            else if (!newline && c == '\n') {
                html += c;
                newline = true;
                return true;
            }
            return false;
        }));

        // Expect an optional '/'
        if (src.expect('/')) {
            html += '/';
        }

        // Expect a '>'
        if (!src.expect('>')) {
            flog::debug("C");
            src.abort();
            return "";
        }
        html += '>';

        // Commit and return successfully
        src.commit();
        return html;
    }

    std::string HTMLTag::parseClosingTag(Source& src) {
        // Begin parsing session
        src.begin();

        // Expect a '<'
        std::string html = "<";
        if (!src.expect('<')) {
            src.abort();
            return "";
        }

        // Expect a tag name
        std::string name = parseTagName(src);
        if (name.empty()) {
            src.abort();
            return "";
        }
        html += name;

        // Expect any number of tabs or space and up to one newline
        bool newline = false;
        while (src.expect([&html, &newline](char c) {
            if (c == ' ' || c == '\t' || c == '\r') {
                html += c;
            }
            else if (!newline && c == '\n') {
                html += c;
                newline = true;
                return true;
            }
            return false;
        }));

        // Expect a '/'
        if (!src.expect('/')) {
            src.abort();
            return "";
        }
        html += '/';

        // Expect a '>'
        if (!src.expect('>')) {
            src.abort();
            return "";
        }
        html += '>';

        // Commit and return successfully
        src.commit();
        return html;
    }

    std::string HTMLTag::parseComment(Source& src) {
        // TODO
        return  "";
    }

    std::string HTMLTag::parseProcessingInstruction(Source& src) {
        // TODO
        return  "";
    }

    std::string HTMLTag::parseDeclaration(Source& src) {
        // TODO
        return  "";
    }

    std::string HTMLTag::parseCDATA(Source& src) {
        // TODO
        return  "";
    }
}