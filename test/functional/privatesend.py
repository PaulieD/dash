#!/usr/bin/env python3
# Copyright (c) 2019 The Dash Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

from test_framework.test_framework import BitcoinTestFramework, DashTestFramework
from test_framework.util import assert_equal, assert_greater_than

'''
privatesend.py

Tests PrivateSend basic RPC
'''

class PrivateSendBasicTest(BitcoinTestFramework):
    def set_test_params(self):
        self.num_nodes = 1

    def run_test(self):
        self.test_privatesend_start_stop()
        self.test_privatesend_setamount()
        self.test_privatesend_setrounds()

    def test_privatesend_start_stop(self):
        # Start Mixing
        self.nodes[0].privatesend("start")
        # Get PrivateSend info
        ps_info = self.nodes[0].getprivatesendinfo()
        # Ensure that it started properly
        assert_equal(ps_info['enabled'], True)
        assert_equal(ps_info['running'], True)

        # Stop mixing
        self.nodes[0].privatesend("stop")
        # Get PrivateSend info
        ps_info = self.nodes[0].getprivatesendinfo()
        # Ensure that it stopped properly
        assert_equal(ps_info['enabled'], True)
        assert_equal(ps_info['running'], False)

    def test_privatesend_setamount(self):
        # Try normal values
        self.nodes[0].setprivatesendamount(50)
        ps_info = self.nodes[0].getprivatesendinfo()
        assert_equal(ps_info['max_amount'], 50)

        # Try large values
        self.nodes[0].setprivatesendamount(1200000)
        ps_info = self.nodes[0].getprivatesendinfo()
        assert_equal(ps_info['max_amount'], 1200000)

    def test_privatesend_setrounds(self):
        # Try normal values
        self.nodes[0].setprivatesendrounds(5)
        ps_info = self.nodes[0].getprivatesendinfo()
        assert_equal(ps_info['max_rounds'], 5)

class PrivateSendMixingTest(DashTestFramework):
    def set_test_params(self):
        self.set_dash_test_params(4, 1, fast_dip3_enforcement=True)

    def run_test(self):

        self.log.info("Give a balance to each node")
        self.nodes[0].sendtoaddress(self.nodes[1].getnewaddress(), 10)
        self.nodes[0].sendtoaddress(self.nodes[2].getnewaddress(), 10)
        self.nodes[0].generate(1)
        self.sync_all()

        self.log.info("Set up settings and start mixing on each node")
        self.nodes[0].setprivatesendrounds(2)
        self.nodes[1].setprivatesendrounds(2)
        self.nodes[2].setprivatesendrounds(2)
        self.nodes[0].privatesend("start")
        self.nodes[1].privatesend("start")
        self.nodes[2].privatesend("start")

        self.log.info("Wait for first round of mixing to happen")
        self.bump_mocktime(120)
        # Generate a block allowing a second round to happen
        self.log.info("Generate a block allowing a second round to happen")
        self.nodes[0].generate(1)
        self.sync_all()
        self.log.info("Wait for second round of mixing to happen")
        self.bump_mocktime(120)
        self.nodes[0].generate(1)
        self.sync_all()

        self.log.info("Verify that all nodes have a non-zero PrivateSend balance")
        assert_greater_than(self.nodes[0].getwalletinfo()["privatesend_balance"], 0)
        assert_greater_than(self.nodes[1].getwalletinfo()["privatesend_balance"], 0)
        assert_greater_than(self.nodes[2].getwalletinfo()["privatesend_balance"], 0)

if __name__ == '__main__':
    PrivateSendMixingTest().main()
    PrivateSendBasicTest().main()
