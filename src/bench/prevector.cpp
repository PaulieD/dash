// Copyright (c) 2015-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <compat.h>
#include <prevector.h>
#include <serialize.h>
#include <streams.h>

#include <bench/bench.h>

struct nontrivial_t {
    int x;
    nontrivial_t() :x(-1) {}
    ADD_SERIALIZE_METHODS
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {READWRITE(x);}
};
static_assert(!IS_TRIVIALLY_CONSTRUCTIBLE<nontrivial_t>::value,
              "expected nontrivial_t to not be trivially constructible");

typedef unsigned char trivial_t;
static_assert(IS_TRIVIALLY_CONSTRUCTIBLE<trivial_t>::value,
              "expected trivial_t to be trivially constructible");

template <typename T>
static void PrevectorDestructor(benchmark::Bench& bench)
{
    bench.run([&] {
        prevector<28, T> t0;
        prevector<28, T> t1;
        t0.resize(28);
        t1.resize(29);
    });
}

template <typename T>
static void PrevectorClear(benchmark::Bench& bench)
{
    prevector<28, T> t0;
    prevector<28, T> t1;
    bench.run([&] {
        t0.resize(28);
        t0.clear();
        t1.resize(29);
        t1.clear();
    });
}

template <typename T>
void PrevectorResize(benchmark::Bench& bench)
{
    prevector<28, T> t0;
    prevector<28, T> t1;
    bench.run([&] {
        t0.resize(28);
        t0.resize(0);
        t1.resize(29);
        t1.resize(0);
    });
}

template <typename T>
static void PrevectorDeserialize(benchmark::Bench& bench)
{
    CDataStream s0(SER_NETWORK, 0);
    prevector<28, T> t0;
    t0.resize(28);
    for (auto x = 0; x < 900; ++x) {
        s0 << t0;
    }
    t0.resize(100);
    for (auto x = 0; x < 101; ++x) {
        s0 << t0;
    }
    bench.run([&] {
        prevector<28, T> t1;
        for (auto x = 0; x < 1000; ++x) {
            s0 >> t1;
        }
        s0.Init(SER_NETWORK, 0);
    });
}

#define PREVECTOR_TEST(name, nontrivops, trivops)                       \
    static void Prevector ## name ## Nontrivial(benchmark::Bench& bench) { \
        Prevector ## name<nontrivial_t>(bench);                         \
    }                                                                   \
    BENCHMARK(Prevector ## name ## Nontrivial, nontrivops);             \
    static void Prevector ## name ## Trivial(benchmark::Bench& bench) { \
        Prevector ## name<trivial_t>(bench);                            \
    }                                                                   \
    BENCHMARK(Prevector ## name ## Trivial, trivops);

PREVECTOR_TEST(Clear, 80 * 1000 * 1000, 70 * 1000 * 1000)
PREVECTOR_TEST(Destructor, 800 * 1000 * 1000, 800 * 1000 * 1000)
PREVECTOR_TEST(Resize, 80 * 1000 * 1000, 70 * 1000 * 1000)
PREVECTOR_TEST(Deserialize, 6800, 52000)

#include <vector>

typedef prevector<28, unsigned char> prevec;

static void PrevectorAssign(benchmark::Bench& bench)
{
    prevec t;
    t.resize(28);
    std::vector<unsigned char> v;
    bench.run([&] {
        prevec::const_iterator b = t.begin() + 5;
        prevec::const_iterator e = b + 20;
        v.assign(b, e);
    });
}

static void PrevectorAssignTo(benchmark::Bench& bench)
{
    prevec t;
    t.resize(28);
    std::vector<unsigned char> v;
    bench.run([&] {
        prevec::const_iterator b = t.begin() + 5;
        prevec::const_iterator e = b + 20;
        t.assign_to(b, e, v);
    });
}

BENCHMARK(PrevectorAssign)
BENCHMARK(PrevectorAssignTo)
