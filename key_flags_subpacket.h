#pragma once

#include "signature_subpacket_type.h"
#include "decoder.h"
#include "fixed_number.h"
#include "variable_number.h"


namespace pgp {

    /**
     *  Class for holding key flags in a signature subpacket
     */
    class key_flags_subpacket
    {
        public:
            /**
             *  Constructor
             *
             *  @param  parser  The decoder to parse the data
             */
            key_flags_subpacket(decoder &parser) :
                _flags{ parser }
            {
                // all data should be consumed
                if (!parser.empty()) {
                    // this is probably not the correct subpacket type
                    throw std::runtime_error{ "Incorrect subpacket type detected" };
                }
            }

            /**
             *  Constructor
             *
             *  @param  flag    One or more flags to set
             */
            template <typename ...flags>
            explicit key_flags_subpacket(flags... flag) :
                _flags{ static_cast<uint8_t>((flag | ...)) }
            {}

            /**
             *  Comparison operators
             *
             *  @param  other   The object to compare with
             */
            bool operator==(const key_flags_subpacket &other) const noexcept
            {
                return _flags == other._flags;
            }

            /**
             *  Comparison operators
             *
             *  @param  other   The object to compare with
             */
            bool operator!=(const key_flags_subpacket &other) const noexcept
            { return !(*this == other); }

            /**
             *  Determine the size used in encoded format
             *  @return The number of bytes used for encoded storage
             */
            size_t size() const noexcept
            {
                // we need to store the flags plus the type
                uint32_t size = sizeof(_flags) + sizeof(type());

                // and then store this number in a variable number
                return size + variable_number{ size }.size();
            }

            /**
             *  Get the signature subpacket type
             *  @return The type of signature subpacket
             */
            static constexpr signature_subpacket_type type() noexcept
            {
                // we are a key flags subpacket
                return signature_subpacket_type::key_flags;
            }

            /**
             *  Is a specific flag set?
             *
             *  @param  flag    The flag to check
             *  @return If the flag is set
             */
            bool is_set(uint8_t flag) const noexcept
            {
                // check whether the flag is set
                return flag & _flags;
            }

            /**
             *  Write the data to an encoder
             *
             *  @param  writer  The encoder to write to
             *  @throws std::out_of_range, std::range_error
             */
            template <class encoder_t>
            void encode(encoder_t &writer) const
            {
                // first get the size for the data itself
                uint32_t size = sizeof(_flags) + sizeof(type());

                // encode the size, the type, and the number
                variable_number{ size }.encode(writer);
                writer.push(type());
                _flags.encode(writer);
            }
        private:
            uint8   _flags; // the set of binary flags set
    };

}