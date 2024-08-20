#include "code_block.h"

namespace mkd::blocks {
    CodeBlock::CodeBlock(const std::string& info, const std::string& code) :
        Block(BLOCK_TYPE_CODE_BLOCK),
        info(info),
        code(code)
    {}

    std::shared_ptr<CodeBlock> CodeBlock::parse(Source& src) {
        // Try to parse an indented code block
        std::shared_ptr<CodeBlock> cb = parseIndented(src);
        if (cb) { return cb; }

        // Try to parse a fenced code block
        return parseFenced(src);
    }

    std::shared_ptr<CodeBlock> CodeBlock::parseIndented(Source& src) {
        // TODO
        return NULL;
    }

    std::shared_ptr<CodeBlock> CodeBlock::parseFenced(Source& src) {
        // Begin parsing session
        src.begin();

        // Expect 0 to 3 spaces
        int indent = 0;
        for (; indent < 3; indent++) {
            if (!src.expect(' ')) { break; }
        }

        // Expect at least three tilde or three backticks
        char bc;
        int openLen;
        if ((openLen = parseCodeFence(src, '`')) >= 3) { bc = '`'; }
        else if ((openLen = parseCodeFence(src, '~')) >= 3) { bc = '~'; }
        else {
            src.abort();
            return NULL;
        }

        // Expect an optional info string
        std::string info;
        while (src.expect([&info](char c){
            // Don't allow backticks
            if (c == '\n') { return false; }

            // Accept and add to the code string
            info += c;
            return true;
        }));

        // The info string can't contain any of the fence character
        if (info.find(bc) != std::string::npos) {
            src.abort();
            return NULL;
        }

        // Expect an optional newline
        src.expect('\n');

        // Trim the info string
        while (info.size() && std::isspace(info[0])) { info = info.substr(1); }
        while (info.size() && std::isspace(info[info.size()-1])) { info.pop_back(); }

        // Read characters until a matching closing string
        std::string code;
        while (true) {
            // Expect at most 3 spaces of indentation
            int closeIndent = 0;
            for (; closeIndent < 3; closeIndent++) {
                if (!src.expect(' ')) { break; }
            }

            // If no more characters are available and the string didn't end with the correct back string, stop
            if (!src.available()) { break; }

            // Look for a backtick string
            int bstr = parseCodeFence(src, bc);

            // If it matches the one in the beginning, stop
            if (bstr >= openLen) { break; }

            // Otherwise add them to the code
            std::string line;
            for (int i = 0; i < closeIndent; i++) { line += ' '; }
            for (int i = 0; i < bstr; i++) { line += bc; }

            // Read rest of the line
            while (src.expect([bc, &line](char c){
                // Stop on newline
                if (c == '\n') { return false; }

                // Accept and add to the code string
                line += c;
                return true;
            }));

            // Skip the newline if there was one
            src.expect('\n');

            // Remove as many indentations as the opening fence if there are any
            for (int i = 0; !line.empty() && std::isspace(line[0]) && i < indent; i++) {
                line = line.substr(1);
            }

            // Add the line to the code
            code += line + '\n';
        }

        // Commit the parsing session and return successfully
        src.commit();
        return std::make_shared<CodeBlock>(info, code);
    }


    int CodeBlock::parseCodeFence(Source& src, char c) {
        // Expect any number of code fence characters
        int count = 0;
        for (; src.expect(c); count++);
        return count;
    }
}