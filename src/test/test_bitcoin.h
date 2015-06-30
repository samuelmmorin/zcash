#ifndef BITCOIN_TEST_TEST_BITCOIN_H
#define BITCOIN_TEST_TEST_BITCOIN_H

#include "chainparamsbase.h"
#include "key.h"
#include "pubkey.h"
#include "txdb.h"

#include <boost/filesystem.hpp>
#include <boost/thread.hpp>

/** Basic testing setup.
 * This just configures logging and chain parameters.
 */
struct BasicTestingSetup {
    ECCVerifyHandle globalVerifyHandle;

    BasicTestingSetup(const std::string& chainName = CBaseChainParams::MAIN);
    ~BasicTestingSetup();
};

// Setup w.r.t. zk-SNARK API
struct JoinSplitTestingSetup: public BasicTestingSetup {
    JoinSplitTestingSetup(const std::string& chainName = CBaseChainParams::MAIN);
    ~JoinSplitTestingSetup();
};

/** Testing setup that configures a complete environment.
 * Included are data directory, coins database, script check threads
 * and wallet (if enabled) setup.
 */
struct TestingSetup: public JoinSplitTestingSetup {
    CCoinsViewDB *pcoinsdbview;
    boost::filesystem::path pathTemp;
    boost::thread_group threadGroup;

    TestingSetup(const std::string& chainName = CBaseChainParams::MAIN);
    ~TestingSetup();
};

class CBlock;
struct CMutableTransaction;
class CScript;

//
// Testing fixture that pre-creates a
// 100-block REGTEST-mode block chain
//
struct TestChain100Setup : public TestingSetup {
    TestChain100Setup();

    // Create a new block with just given transactions, coinbase paying to
    // scriptPubKey, and try to add it to the current chain.
    CBlock CreateAndProcessBlock(const std::vector<CMutableTransaction>& txns,
                                 const CScript& scriptPubKey);

    ~TestChain100Setup();

    std::vector<CTransaction> coinbaseTxns; // For convenience, coinbase transactions
    CKey coinbaseKey; // private/public key needed to spend coinbase transactions
};

#endif
