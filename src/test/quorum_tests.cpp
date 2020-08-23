// Copyright (c) 2020 The Dash Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test/test_dash.h>
#include <bls/bls.h>
#include <llmq/quorums.h>
#include <consensus/params.h>
#include <bls/bls_worker.h>

#include <iostream>

#include <boost/test/unit_test.hpp>

//extern CBLSWorker blsWorker;

struct Member {
    CBLSId id;

    BLSVerificationVectorPtr vvec;
    BLSSecretKeyVector skShares;
};

BOOST_FIXTURE_TEST_SUITE(quorums_tests, BasicTestingSetup)

    BOOST_AUTO_TEST_CASE(quorum_contributions_tests)
    {

        std::vector<CBLSSecretKey> sks;

        std::vector<Member> members;
        std::vector<CBLSId> ids;

        std::vector<BLSVerificationVectorPtr> receivedVvecs;
        BLSSecretKeyVector receivedSkShares;

        BLSVerificationVectorPtr quorumVvec;
        CBLSWorker blsWorker;

        blsWorker.Start();

        int quorum_size = 10;
        for (int i = 0; i < quorum_size; i++) {
            auto sk = CBLSSecretKey();
            sk.MakeNewKey();
            sks.emplace_back(sk);
        }

        members.resize(quorum_size);
        ids.resize(quorum_size);

        for (int i = 0; i < quorum_size; i++) {
            members[i].id.SetInt(i + 1);
            ids[i] = members[i].id;
        }

        for (int i = 0; i < quorum_size; i++) {
            blsWorker.GenerateContributions(quorum_size / 2 + 1, ids, members[i].vvec, members[i].skShares);
        }

        for (const auto& member : members) {
            printf("Member %s\n", member.id.ToString().c_str());
            for (auto skShare : member.skShares) {
                printf("skShare %s\n", skShare.ToString().c_str());
            }
            for (auto verif : *member.vvec) {
                printf("verification %s\n", verif.ToString().c_str());
            }

        }
//        skShare = blsWorker.AggregateSecretKeys(skContributions);
        blsWorker.Stop();
        BOOST_CHECK(false);
    }


/*
    BOOST_AUTO_TEST_CASE(quorum_contributions_tests2)
    {

        std::vector<CBLSSecretKey> sks;

        int quorum_size = 10;
        for (int i = 0; i < quorum_size; i++) {
            auto sk = CBLSSecretKey();
            sk.MakeNewKey();
            sks.emplace_back(sk);
        }

        CBLSWorker blsWorker;

        auto& params = Params().GetConsensus().llmqs.at(Consensus::LLMQ_DEVNET);

        auto quorumPtr = std::make_shared<llmq::CQuorum>(params, blsWorker);
        quorumPtr->Init()


*/
/*
        llmq::CQuorum* quorum = llmq::CQuorum(Consensus::LLMQ_DEVNET, blsworker);
        CQuorumCPtr quorum = quorumManager->GetQuorum(llmqType, recSig.quorumHash);

*//*

//        skShare = blsWorker.AggregateSecretKeys(skContributions);

    }

*/

BOOST_AUTO_TEST_SUITE_END()
