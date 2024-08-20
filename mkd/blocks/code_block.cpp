#include "code_block.h"

namespace mkd::blocks {
    CodeBlock::CodeBlock(const std::string& code) :
        Block(BLOCK_TYPE_CODE_BLOCK),
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
        for (int i = 0; i < 2; i++) {
            if (!src.expect(' ')) { break; }
        }

        // Expect a backtick or a tilde

        // Expect two more of the same character

        // Commit the parsing session and return successfully
        src.commit();
        return std::make_shared<CodeBlock>();
    }
}