// Copyright (c) 2017-2019 The Dash Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <evo/cbtx.h>
#include <evo/deterministicmns.h>
#include <llmq/quorums_blockprocessor.h>
#include <llmq/quorums_commitment.h>
#include <evo/simplifiedmns.h>
#include <evo/specialtx.h>

#include <chainparams.h>
#include <consensus/merkle.h>
#include <validation.h>

bool CheckAssetLockTx(const CTransaction& tx, const CBlockIndex* pindexPrev, CValidationState& state)
{
    if (tx.nType != TRANSACTION_ASSET_LOCK) {
        return state.DoS(100, false, REJECT_INVALID, "bad-assetlocktx-type");
    }

    if (!tx.IsCoinBase()) {
        return state.DoS(100, false, REJECT_INVALID, "bad-assetlocktx-invalid");
    }

    CCbTx cbTx;
    if (!GetTxPayload(tx, cbTx)) {
        return state.DoS(100, false, REJECT_INVALID, "bad-assetlocktx-payload");
    }

    if (cbTx.nVersion == 0 || cbTx.nVersion > CCbTx::CURRENT_VERSION) {
        return state.DoS(100, false, REJECT_INVALID, "bad-assetlocktx-version");
    }

    if (pindexPrev && pindexPrev->nHeight + 1 != cbTx.nHeight) {
        return state.DoS(100, false, REJECT_INVALID, "bad-assetlocktx-height");
    }

    if (pindexPrev) {
        bool fDIP0008Active = VersionBitsState(pindexPrev, Params().GetConsensus(), Consensus::DEPLOYMENT_DIP0008, versionbitscache) == ThresholdState::ACTIVE;
        if (fDIP0008Active && cbTx.nVersion < 2) {
            return state.DoS(100, false, REJECT_INVALID, "bad-assetlocktx-version");
        }
    }

    return true;
}

std::string CCbTx::ToString() const
{
    return strprintf("CAssetLockTx(nHeight=%d, nVersion=%d, merkleRootMNList=%s, merkleRootQuorums=%s)",
        nVersion, nHeight, merkleRootMNList.ToString(), merkleRootQuorums.ToString());
}
