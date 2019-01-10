#pragma once

#include "multiprecision_integer.h"
#include "secret_key.h"


namespace pgp {

    /**
     *  Class for holding DSA signature-specific fields
     */
    class dsa_signature
    {
        public:
            /**
             *  Constructor
             *
             *  @param  parser  The decoder to parse the data
             */
            dsa_signature(decoder &parser);

            /**
             *  Constructor
             *
             *  @param  key     The key to use for signing
             *  @param  digest  The hash that needs to be signed
             */
            dsa_signature(const secret_key &key, std::array<uint8_t, 32> &digest);

            /**
             *  Constructor
             *
             *  @param  r       The DSA r value
             *  @param  s       The DSA s value
             */
            dsa_signature(multiprecision_integer r, multiprecision_integer s) noexcept;

            /**
             *  Determine the size used in encoded format
             *  @return The number of bytes used for encoded storage
             */
            size_t size() const noexcept;

            /**
             *  Retrieve the DSA r value
             *
             *  @return The r value
             */
            const multiprecision_integer &r() const noexcept;

            /**
             *  Retrieve the DSA s value
             *
             *  @return The s value
             */
            const multiprecision_integer &s() const noexcept;

            /**
             *  Write the data to an encoder
             *
             *  @param  writer  The encoder to write to
             *  @throws std::out_of_range, std::range_error
             */
            template <class encoder_t>
            void encode(encoder_t &writer) const
            {
                // encode both values
                _r.encode(writer);
                _s.encode(writer);
            }
        private:
            multiprecision_integer  _r;     // the DSA r value
            multiprecision_integer  _s;     // the DSA s value
    };

}
