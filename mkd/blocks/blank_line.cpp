#include "blank_line.h"

namespace mkd::blocks {
    BlankLine::BlankLine() :
        Block(BLOCK_TYPE_BLANK_LINE)
    {}

    std::shared_ptr<BlankLine> BlankLine::parse(Source& src) {
        // If no newline, do nothing
        if (!src.expect('\n')) { return NULL; }
        return std::make_shared<BlankLine>();
    }
}