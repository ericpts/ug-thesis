#include "code.h"
#include <cassert>
#include <optional>

// If `i` is a fixed size instruction, returns how many bytes it takes up.
// Otherwise, returns an empty optional.
std::optional<int> nbytes_skip(Instr i);

// Special handling of the variable-length instructions.
void handle_lookupswitch(BytesParser *bp);
void handle_tableswitch(BytesParser *bp);
void handle_wide(BytesParser *bp);

void skip_instruction(Instr i, BytesParser *bp)
{
    const std::optional<int> maybe_fixed_skip = nbytes_skip(i);
    if (maybe_fixed_skip.has_value()) {
        const int nbytes = maybe_fixed_skip.value();
        // Discard the bytes.
        // TODO(ericpts): Maybe validate the instruction format, instead of just
        // dicarding the bytes.
        bp->next_n(nbytes);
        return;
    }

    // We know that `i` must be a variable length instruction.
    switch (i) {
    case Instr::lookupswitch:
        return handle_lookupswitch(bp);
    case Instr::tableswitch:
        return handle_tableswitch(bp);
    case Instr::wide:
        return handle_wide(bp);
    default:
        // The previous 3 are the only fixed size instructions.
        assert(false);
    }
}

void handle_lookupswitch(BytesParser *bp)
{
    while (bp->bytes_read() % 4 != 0) {
        bp->next_u1();
    }

    bp->next_u4(); // Skip the default byte.
    int npairs = bp->next_u4();
    assert(npairs >= 0);

    for (int i = 0; i < npairs; ++i) {
        bp->next_u4();
        bp->next_u4();
    }
}

void handle_tableswitch(BytesParser *bp)
{
    while (bp->bytes_read() % 4 != 0) {
        bp->next_u1();
    }

    bp->next_u4(); // Skip the default byte.
    const int low = bp->next_u4();
    const int high = bp->next_u4();

    assert(low <= high);
    for (int i = low; i <= high; ++i) {
        bp->next_u4();
    }
}

void handle_wide(BytesParser *bp)
{
    Instr i = static_cast<Instr>(bp->next_u1());

    // Handle format 2:
    // https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.wide
    if (i == Instr::iinc) {
        bp->next_u2();
        bp->next_u2();
        return;
    }

    bp->next_u2();
}

std::optional<int> nbytes_skip(Instr i)
{
    switch (i) {
    case Instr::invokedynamic:
    case Instr::invokeinterface:
    case Instr::invokespecial:
    case Instr::invokestatic:
    case Instr::invokevirtual:
        // Fallthrough.
        assert(false); // Invoke instructions should be handled separately.

    case Instr::aload:
        return 1;
    case Instr::anewarray:
        return 2;
    case Instr::astore:
        return 1;
    case Instr::bipush:
        return 1;
    case Instr::checkcast:
        return 2;
    case Instr::dload:
        return 1;
    case Instr::dstore:
        return 1;
    case Instr::fload:
        return 1;
    case Instr::fstore:
        return 1;
    case Instr::getfield:
        return 2;
    case Instr::getstatic:
        return 2;
    case Instr::goto_:
        return 2;
    case Instr::goto_w:
        return 4;
    case Instr::if_acmpeq:
        return 2;
    case Instr::if_acmpne:
        return 2;
    case Instr::if_icmpeq:
        return 2;
    case Instr::if_icmpne:
        return 2;
    case Instr::if_icmplt:
        return 2;
    case Instr::if_icmpge:
        return 2;
    case Instr::if_icmpgt:
        return 2;
    case Instr::if_icmple:
        return 2;
    case Instr::ifeq:
        return 2;
    case Instr::ifne:
        return 2;
    case Instr::iflt:
        return 2;
    case Instr::ifge:
        return 2;
    case Instr::ifgt:
        return 2;
    case Instr::ifle:
        return 2;
    case Instr::ifnonnull:
        return 2;
    case Instr::ifnull:
        return 2;
    case Instr::iinc:
        return 2;
    case Instr::iload:
        return 1;
    case Instr:: instanceof:
        return 2;
    case Instr::istore:
        return 1;
    case Instr::jsr:
        return 2;
    case Instr::jsr_w:
        return 4;
    case Instr::ldc:
        return 1;
    case Instr::ldc_w:
        return 2;
    case Instr::ldc2_w:
        return 2;
    case Instr::lload:
        return 1;
    case Instr::lstore:
        return 1;
    case Instr::multianewarray:
        return 3;
    case Instr::new_:
        return 2;
    case Instr::newarray:
        return 1;
    case Instr::putfield:
        return 2;
    case Instr::putstatic:
        return 2;
    case Instr::ret:
        return 1;
    case Instr::sipush:
        return 2;
    case Instr::lookupswitch:
        return {};
    case Instr::tableswitch:
        return {};
    case Instr::wide:
        return {};
    default:
        return 0;
    }
}
