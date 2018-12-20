#include "signature.h"


namespace pgp {

    /**
     *  Constructor
     *
     *  @param  parser  The decoder to parse the data
     */
    signature::signature(decoder &parser) :
        _version{ parser },
        _type{ parser.extract_number<uint8_t>() },
        _key_algorithm{ parser.extract_number<uint8_t>() },
        _hash_algorithm{ parser.extract_number<uint8_t>() },
        _hashed_subpackets{ parser },
        _unhashed_subpackets{ parser }
    {}

    /**
     *  Determine the size used in encoded format
     *  @return The number of bytes used for encoded storage
     *  @throws std::runtime_error for unknown signature types
     */
    size_t signature::size() const
    {
        // we need the size of the version
        return _version.size() + sizeof(_type) + sizeof(_key_algorithm) + sizeof(_hash_algorithm) + _hashed_subpackets.size() + _unhashed_subpackets.size();
    }

    /**
     *  Get the signature type
     *  @return The type of signature
     */
    signature_type signature::type() const noexcept
    {
        // return the stored type
        return _type;
    }

    /**
     *  Get the used key algorithm
     *
     *  @return The public key algorithm
     */
    key_algorithm signature::public_key_algorithm() const noexcept
    {
        // return the stored key algorithm
        return _key_algorithm;
    }

    /**
     *  Get the used hashing algorithm
     *
     *  @return The hashing algorithm
     */
    hash_algorithm signature::hashing_algorithm() const noexcept
    {
        // return the stored hashing algorithm
        return _hash_algorithm;
    }

    /**
     *  Retrieve the hashed subpackets
     *
     *  @return The hashed subpackets
     */
    const signature_subpacket_set &signature::hashed_subpackets() const noexcept
    {
        // return the stored hashed subpackets
        return _hashed_subpackets;
    }

    /**
     *  Retrieve the unhashed subpackets
     *
     *  @return The unhashed subpackets
     */
    const signature_subpacket_set &signature::unhashed_subpackets() const noexcept
    {
        // return the stored unhashed subpackets
        return _unhashed_subpackets;
    }

    /**
     *  Write the data to an encoder
     *
     *  @param  writer  The encoder to write to
     *  @throws std::out_of_range, std::range_error
     */
    void signature::encode(encoder &writer) const
    {
        // encode all the fields of the signature
        _version.encode(writer);
        writer.insert_enum(_type);
        writer.insert_enum(_key_algorithm);
        writer.insert_enum(_hash_algorithm);
        _hashed_subpackets.encode(writer);
        _unhashed_subpackets.encode(writer);
    }

}
