#include <CLI/CLI.hpp>

#include <cassert>
#include <vector>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

#include "types.h"
#include "classfile.h"

struct ClassParser
{
private:
    // Private data fields.
    const std::vector<uint8_t>& m_data;
    std::vector<uint8_t>::const_iterator m_it;

public:
    // Public methods.

    ClassParser(const std::vector<uint8_t>& data):
        m_data(data),
        m_it(m_data.begin())
    { }

    u1 next_u1()
    {
        assert (m_it != m_data.end());
        return *(m_it++);
    }

    // Returns the next unsigned short, in network order.
    u2 next_u2()
    {
        uint8_t b1 = next_u1();
        uint8_t b2 = next_u1();
        return (b1 << 8u) | b2;
    }

    // Returns the next unsigned int, in network order.
    u4 next_u4()
    {
        uint16_t s1 = next_u2();
        uint16_t s2 = next_u2();
        return (s1 << 16u) | s2;
    }

    std::vector<u1> next_n(int n)
    {
        std::vector<u1> ret(n);
        for (u1 &u : ret) {
            u = next_u1();
        }
        return ret;
    }

    /// Parses a constant from the data buffer, and returns the data
    /// and how many slots it takes up in the constant table.
    std::pair<Constant, int> parse_constant()
    {
        Constant c;

        Constant::Tag& tag = c.tag;
        std::vector<u1>& data = c.data;

        // By default, most types of constants only take up one slot.
        int slots = 1;

        tag = static_cast<Constant::Tag>(next_u1());
        switch (tag) {
            case Constant::Tag::utf8: {
                u2 u = next_u2();
                data = next_n(u);
                break;
            }
            case Constant::Tag::i32: {
                static_assert (sizeof(int32_t) == 4, "Signed 32 bit integer must be 4 bytes.");
                data = next_n(sizeof(int32_t));
                break;
            }
            case Constant::Tag::f32: {
                static_assert (sizeof(float) == 4, "32 bit floating pointer number must be 4 bytes.");
                data = next_n(sizeof(float));
                break;
            }
            case Constant::Tag::i64: {
                static_assert (sizeof(int64_t) == 8, "Signed 64 bit integer must be 8 bytes.");
                data = next_n(sizeof(int64_t));
                slots = 2;
                break;
            }
            case Constant::Tag::f64: {
                static_assert (sizeof(double) == 8, "64 bit floating point number must be 8 bytes.");
                data = next_n(sizeof(double));
                slots = 2;
                break;
            }
            case Constant::Tag::cref: {
                data = next_n(2);
                break;
            }
            case Constant::Tag::sref: {
                data = next_n(2);
                break;
            }
            case Constant::Tag::fref: {
                data = next_n(4);
                break;
            }
            case Constant::Tag::mref: {
                data = next_n(4);
                break;
            }
            case Constant::Tag::iref: {
                data = next_n(4);
                break;
            }
            case Constant::Tag::ntdesc: {
                data = next_n(4);
                break;
            }
            case Constant::Tag::mhandle: {
                data = next_n(3);
                break;
            }
            case Constant::Tag::mtype: {
                data = next_n(2);
                break;
            }
            case Constant::Tag::idynamic: {
                data = next_n(4);
                break;
            }
        }
        return {c, slots};
    }

    ClassFile parse_class()
    {
        const u4 magic = next_u4();
        assert (magic == 0xCAFEBABE);

        const u2 minv = next_u2();
        const u2 majv = next_u2();

        const u2 ncpool = next_u2();
        std::vector<Constant> cpool(ncpool);
        // The constant pool is indexed from  to ncpool - 1.
        for (auto it = cpool.begin() + 1; it != cpool.end(); ) {
            int nslots;
            std::tie(*it, nslots) = parse_constant();
            it += nslots;
        }
        std::cerr << "Done parsing constants.\n";

        const u2 aflags = next_u2();
        const u2 this_class = next_u2();
        const u2 super_class = next_u2();

        const u2 icount = next_u2();
        std::vector<Interface> ipool(icount);
        for (Interface& i : ipool) {
            int idx = next_u2();
            assert (1 <= idx && idx <= ncpool - 1); // It is an index into the constant pool.
            assert (cpool[idx].tag == Constant::Tag::cref);
            i.idx = idx;
        }
        std::cerr << "Done parsing interfaces.\n";

        const u2 fcount = next_u2();
        std::vector<Field> fpool(fcount);
        for (Field& f : fpool) {

        }

        return ClassFile{
            magic,
            minv,
            majv,
            ncpool,
            cpool,
            aflags,
            this_class,
            super_class,
            icount,
            ipool,
            fcount,
            fpool,
        };
    }

};


int main(int argc, char** argv)
{
    std::ifstream fin(argv[1], std::ios::binary);
    std::vector<u1> data(
            (std::istream_iterator<u1>(fin)),
             std::istream_iterator<u1>());

    ClassParser parser(data);
    ClassFile file = parser.parse_class();
    return 0;
}
