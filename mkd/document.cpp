#include "document.h"
#include <stdexcept>
#include "flog/flog.h"

namespace mkd {
    Document::Document(const std::string& src) :
        src(src)
    {
        // Parse source
        std::shared_ptr<Block> block;
        while (this->src.available()) {
            // Attempt to parse a thematic break, if it fails, something is very wrong
            block = Block::parse(this->src);
            if (!block) {
                throw std::runtime_error("Failed to parse source file.... what the fuck...");
            }

            // If the block is a blank line, just ignore it
            if (block->type == BLOCK_TYPE_BLANK_LINE) { continue; }

            // Add the new block to the list
            blocks.push_back(block);
        }
    }
}