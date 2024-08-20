#include "block.h"

namespace mkd {
    Block::Block(BlockType type) :
        type(type)
    {}

    std::shared_ptr<Block> Block::parse(Source& src) {
        // Attempt to parse a list
        std::shared_ptr<Block> block = blocks::List::parse(src);
        if (block) { return block; }

        // Attempt to parse a thematic break
        block = blocks::ThematicBreak::parse(src);
        if (block) { return block; }

        // Attempt to parse a heading
        block = blocks::Heading::parse(src);
        if (block) { return block; }

        // Attempt to parse a code block
        block = blocks::CodeBlock::parse(src);
        if (block) { return block; }

        // TODO: Other shit

        // Attempt to parse a paragraph
        block = blocks::Paragraph::parse(src);
        if (block) { return block; }

        // Attempt to parse a blank line
        block = blocks::BlankLine::parse(src);
        if (block) { return block; }

        // Should never get here except for an empty document
        return NULL;
    }
    
    Block::~Block() {}
}