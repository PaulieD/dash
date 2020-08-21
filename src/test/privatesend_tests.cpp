// Copyright (c) 2020 The Dash Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test/test_dash.h>

#include <amount.h>
#include <privatesend/privatesend.h>
#include <privatesend/privatesend-client.h>

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(privatesend_tests, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(ps_collatoral_tests)
{
    // Good collateral values
    BOOST_CHECK(CPrivateSend::IsCollateralAmount(0.00010000 * COIN));
    BOOST_CHECK(CPrivateSend::IsCollateralAmount(0.00012345 * COIN));
    BOOST_CHECK(CPrivateSend::IsCollateralAmount(0.00032123 * COIN));
    BOOST_CHECK(CPrivateSend::IsCollateralAmount(0.00019000 * COIN));

    // Bad collateral values
    BOOST_CHECK(!CPrivateSend::IsCollateralAmount(0.00009999 * COIN));
    BOOST_CHECK(!CPrivateSend::IsCollateralAmount(0.00040001 * COIN));
    BOOST_CHECK(!CPrivateSend::IsCollateralAmount(0.00100000 * COIN));
    BOOST_CHECK(!CPrivateSend::IsCollateralAmount(0.00100001 * COIN));
}

BOOST_AUTO_TEST_CASE(ps_options_set_tests)
{
    // Test SetRounds
    BOOST_CHECK_THROW(CPrivateSendClientOptions::SetRounds(-1), std::runtime_error);
    BOOST_CHECK_THROW(CPrivateSendClientOptions::SetRounds(MIN_PRIVATESEND_ROUNDS-1), std::runtime_error);
    BOOST_CHECK_THROW(CPrivateSendClientOptions::SetRounds(MAX_PRIVATESEND_ROUNDS+1), std::runtime_error);
    for (int i = MIN_PRIVATESEND_ROUNDS; i <= MAX_PRIVATESEND_ROUNDS; i++) {
        CPrivateSendClientOptions::SetRounds(i);
        BOOST_CHECK_EQUAL(CPrivateSendClientOptions::GetRounds(), i);
    }
    // test SetAmount
    BOOST_CHECK_THROW(CPrivateSendClientOptions::SetAmount(-1), std::runtime_error);
    BOOST_CHECK_THROW(CPrivateSendClientOptions::SetAmount(MIN_PRIVATESEND_AMOUNT-1), std::runtime_error);
    BOOST_CHECK_THROW(CPrivateSendClientOptions::SetAmount(MAX_PRIVATESEND_AMOUNT+1), std::runtime_error);

    CAmount checkAmounts[] = {MIN_PRIVATESEND_AMOUNT, MAX_PRIVATESEND_AMOUNT, 50, 250, 1000, 2000, 8000};
    for (const auto& amount : checkAmounts) {
        CPrivateSendClientOptions::SetAmount(amount);
        BOOST_CHECK_EQUAL(CPrivateSendClientOptions::GetAmount(), amount);
    }

    // test SetEnabled
    CPrivateSendClientOptions::SetEnabled(true);
    BOOST_CHECK(CPrivateSendClientOptions::IsEnabled());
    CPrivateSendClientOptions::SetEnabled(false);
    BOOST_CHECK(!CPrivateSendClientOptions::IsEnabled());

    // test SetMultiSessionEnabled
    CPrivateSendClientOptions::SetMultiSessionEnabled(true);
    BOOST_CHECK(CPrivateSendClientOptions::IsMultiSessionEnabled());
    CPrivateSendClientOptions::SetMultiSessionEnabled(false);
    BOOST_CHECK(!CPrivateSendClientOptions::IsMultiSessionEnabled());
}

BOOST_AUTO_TEST_SUITE_END()
