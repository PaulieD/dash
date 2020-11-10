// Copyright (c) 2017-2019 The Dash Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_EVO_ASSETLOCKTX_H
#define BITCOIN_EVO_ASSETLOCKTX_H

#include <consensus/validation.h>
#include <primitives/transaction.h>
#include <univalue.h>

// coinbase transaction
class CAssetLockTx
{
public:
    static const uint16_t CURRENT_VERSION = 1;

public:
    uint16_t nVersion{CURRENT_VERSION};
    // TODO I don't know if I need this
    uint256 inputsHash; // replay protection

public:
    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action)
    {
        READWRITE(nVersion);
        READWRITE(inputsHash);
    }

    std::string ToString() const;

    void ToJson(UniValue& obj) const
    {
        obj.clear();
        obj.setObject();
        obj.pushKV("version", (int)nVersion);
        obj.pushKV("inputsHash", inputsHash.ToString());
    }
};

bool CheckAssetLockTx(const CTransaction& tx, const CBlockIndex* pindexPrev, CValidationState& state);

#endif // BITCOIN_EVO_ASSETLOCKTX_H
