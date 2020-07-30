// Copyright (c) 2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bench/bench.h>

#include <crypto/sha256.h>
#include <key.h>
#include <stacktraces.h>
#include <validation.h>
#include <util.h>
#include <random.h>

#include <boost/lexical_cast.hpp>

#include <memory>

#include <bls/bls.h>

static const char* DEFAULT_BENCH_FILTER = ".*";

void InitBLSTests();
void CleanupBLSTests();
void CleanupBLSDkgTests();

static fs::path SetDataDir()
{
    fs::path ret = fs::temp_directory_path() / "bench_dash" / fs::unique_path();
    fs::create_directories(ret);
    gArgs.ForceSetArg("-datadir", ret.string());
    return ret;
}

static void SetupBenchArgs()
{
    gArgs.AddArg("-?", "Print this help message and exit", false, OptionsCategory::OPTIONS);
    gArgs.AddArg("-list", "List benchmarks without executing them. Can be combined with -scaling and -filter", false, OptionsCategory::OPTIONS);
    gArgs.AddArg("-filter=<regex>", strprintf("Regular expression filter to select benchmark by name (default: %s)", DEFAULT_BENCH_FILTER), false, OptionsCategory::OPTIONS);
}

// parses a comma separated list like "10,20,30,50"
static std::vector<double> parseAsymptote(const std::string& str) {
    std::stringstream ss(str);
    std::vector<double> numbers;
    double d;
    char c;
    while (ss >> d) {
        numbers.push_back(d);
        ss >> c;
    }
    return numbers;
}

int main(int argc, char** argv)
{
    SetupBenchArgs();
    gArgs.ParseParameters(argc, argv);

    if (gArgs.IsArgSet("-?") || gArgs.IsArgSet("-h") || gArgs.IsArgSet("-help")) {
        std::cout << gArgs.GetHelpMessage();

        return 0;
    }

    // Set the datadir after parsing the bench options
    const fs::path bench_datadir{SetDataDir()};

    SHA256AutoDetect();

    RegisterPrettySignalHandlers();
    RegisterPrettyTerminateHander();

    RandomInit();
    ECC_Start();
    ECCVerifyHandle verifyHandle;

    BLSInit();
    InitBLSTests();
    SetupEnvironment();
    fPrintToDebugLog = false; // don't want to write to debug.log file

    benchmark::Args args;
    args.regex_filter = gArgs.GetArg("-filter", DEFAULT_BENCH_FILTER);
    args.is_list_only = gArgs.GetBoolArg("-list", false);
    args.asymptote = parseAsymptote(gArgs.GetArg("-asymptote", ""));
    args.output_csv = gArgs.GetArg("-output_csv", "");
    args.output_json = gArgs.GetArg("-output_json", "");

    fs::remove_all(bench_datadir);

    // need to be called before global destructors kick in (PoolAllocator is needed due to many BLSSecretKeys)
    CleanupBLSDkgTests();
    CleanupBLSTests();

    ECC_Stop();
}
