// Copyright (c) 2017-2019 The Dash Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <evo/assetlocktx.h>
#include <evo/specialtx.h>

#include <consensus/merkle.h>
#include <validation.h>

bool CheckAssetLockTx(const CTransaction& tx, const CBlockIndex* pindexPrev, CValidationState& state)
{
    if (tx.nType != TRANSACTION_ASSET_LOCK) {
        return state.DoS(100, false, REJECT_INVALID, "bad-assetlocktx-type");
    }

    CAssetLockTx assetLockTx;
    if (!GetTxPayload(tx, assetLockTx)) {
        return state.DoS(100, false, REJECT_INVALID, "bad-assetlocktx-payload");
    }

    if (assetLockTx.nVersion == 0 || assetLockTx.nVersion > CAssetLockTx::CURRENT_VERSION) {
        return state.DoS(100, false, REJECT_INVALID, "bad-assetlocktx-version");
    }

    return true;
}

std::string CAssetLockTx::ToString() const
{
    return strprintf("CAssetLockTx(nVersion=%d)", nVersion);
}
