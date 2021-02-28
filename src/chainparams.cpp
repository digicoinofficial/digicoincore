// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>
#include <consensus/consensus.h>

#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>

#include <assert.h>

#include <chainparamsseeds.h>

///////////////////////////////////////////// // digicoin
#include <libdevcore/SHA3.h>
#include <libdevcore/RLP.h>
#include "arith_uint256.h"
/////////////////////////////////////////////

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 00 << 488804799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    genesis.hashStateRoot = uint256(h256Touint(dev::h256("e965ffd002cd6ad0e2dc402b8044de833e06b23127ea8c3d80aec91410771495"))); // digicoin
    genesis.hashUTXORoot = uint256(h256Touint(dev::sha3(dev::rlp("")))); // digicoin
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
 static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
 {
     const char* pszTimestamp = "Feb 17, 2021 digicoin is bringing crypto onto its network";
     const CScript genesisOutputScript = CScript() << ParseHex("040d61d8653448c98731ee5fffd303c15e71ec2057b77f11ab3601979728cdaff2d68afbba14e4fa0bc44f2072b0b23ef63717f8cdfbe58dcd33f32b6afe98741a") << OP_CHECKSIG;
     return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
 }

 void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
 {
     consensus.vDeployments[d].nStartTime = nStartTime;
     consensus.vDeployments[d].nTimeout = nTimeout;
 }

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

static void MineGenesis(CBlockHeader& genesisBlock, const uint256& powLimit, bool noProduction)
{
 printf("NOTE: Genesis nTime = %u \n", genesisBlock.nTime);
 printf("WARN: Genesis nNonce (BLANK!) = %u \n", genesisBlock.nNonce);
 genesisBlock.nNonce = 0;

 arith_uint256 besthash;
 memset(&besthash,0xFF,32);
 arith_uint256 hashTarget = UintToArith256(powLimit);
 printf("Target: %s\n", hashTarget.GetHex().c_str());
 arith_uint256 newhash = UintToArith256(genesisBlock.GetHash());
 while (newhash > hashTarget) {
     genesisBlock.nNonce++;
     if (genesisBlock.nNonce == 0) {
         printf("NONCE WRAPPED, incrementing time\n");
         ++genesisBlock.nTime;
     }
     if ((genesisBlock.nNonce & 0xfff) == 0)
         printf("nonce %08X: hash = %s (target = %s)\n",
                genesisBlock.nNonce, newhash.ToString().c_str(),
                hashTarget.ToString().c_str());

     if(newhash < besthash) {
         besthash = newhash;
         printf("New best: %s\n", newhash.GetHex().c_str());
     }
     newhash = UintToArith256(genesisBlock.GetHash());
 }
 printf("Genesis nTime = %u \n", genesisBlock.nTime);
 printf("Genesis nNonce = %u \n", genesisBlock.nNonce);
 printf("Genesis nBits: %08x\n", genesisBlock.nBits);
 printf("Genesis Hash = %s\n", newhash.ToString().c_str());
 printf("Genesis hashStateRoot = %s\n", genesisBlock.hashStateRoot.ToString().c_str());
 printf("Genesis Hash Merkle Root = %s\n", genesisBlock.hashMerkleRoot.ToString().c_str());
}

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 985500; // digicoin halving every 4 years
        consensus.BIP16Exception = uint256S("0x00001c8b18755416e4e9b340e5032f2d057f24d4767dd1bcfedc2ef7d2df3f03");
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x00001c8b18755416e4e9b340e5032f2d057f24d4767dd1bcfedc2ef7d2df3f03");
        consensus.BIP65Height = 0; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.BIP66Height = 0; // 00000000000000000379eaa19dce8c9b722d46ae6a57c2f1a988119488b50931
        consensus.powLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 6 * 60; // 6 minutes
        consensus.nPowTargetSpacing = 3 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = true;
        consensus.fPoSNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00"); // digicoin

        // By default assume that the signatures in ancestors of this block are valid.
       // consensus.defaultAssumeValid = uint256S("0x0000f286807cd4035cadfe039e4691f4960794344640c234abeae3544310fb9c"); // block height

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
         pchMessageStart[0] = 0x1F;  //D
         pchMessageStart[1] = 0x26;  //G
         pchMessageStart[2] = 0x17;  //A
         pchMessageStart[3] = 0x3F;  //S
         nDefaultPort = 3108;
         nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1614384200, 33004, 0x1f00ffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x00001c8b18755416e4e9b340e5032f2d057f24d4767dd1bcfedc2ef7d2df3f03"));
        assert(genesis.hashMerkleRoot == uint256S("0xead7dd9db90aac6f1a1b57de5b520c8831483f13de22cda1199cc4c57dbba8cd"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.

        vSeeds.emplace_back("seeds.digiasset.network", false);
        vSeeds.emplace_back("seeds1.digiasset.network", false);
        vSeeds.emplace_back("seeds2.digiasset.network", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,31);  //  D
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,38);  //  G
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,66);  //  T
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));
        bech32_hrp = "diga";


        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = {
            {
                { 0, uint256S("0x00001c8b18755416e4e9b340e5032f2d057f24d4767dd1bcfedc2ef7d2df3f03")}
            }
        };

        chainTxData = ChainTxData{
        };

        /* disable fallback fee on mainnet */
        m_fallback_fee_enabled = false;

        consensus.nLastPOWBlock = 1000;
        consensus.nMPoSRewardRecipients = 10;
        consensus.nFirstMPoSBlock = consensus.nLastPOWBlock +
                                    consensus.nMPoSRewardRecipients +
                                    COINBASE_MATURITY;

        //consensus.nFixUTXOCacheHFHeight=100000; //disabled but fixed elsewhere
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 2880; // digicoin halving every 4 years
        consensus.BIP16Exception = uint256S("0x00001c8b18755416e4e9b340e5032f2d057f24d4767dd1bcfedc2ef7d2df3f03");
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x00001c8b18755416e4e9b340e5032f2d057f24d4767dd1bcfedc2ef7d2df3f03");
        consensus.BIP65Height = 0; // 00000000007f6655f22f98e72ed80d8b06dc761d5da09df0fa1dc4be4f861eb6
        consensus.BIP66Height = 0; // 000000002104c8c45e99a8853285a3b592602a3ccde2b832481da85e9e4ba182
        consensus.powLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 6 * 60; // 6 minutes
        consensus.nPowTargetSpacing = 3 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = true;
        consensus.fPoSNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000d6e528bcf5a3a556f"); // digicoin

        // By default assume that the signatures in ancestors of this block are valid.
        //consensus.defaultAssumeValid = uint256S("0x00"); // 320000

        pchMessageStart[0] = 0x12;
        pchMessageStart[1] = 0x1E;
        pchMessageStart[2] = 0x22;
        pchMessageStart[3] = 0x57;
        nDefaultPort = 13108;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1573312470, 124684, 0x1f00ffff, 1, 50000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x00007d7449879ca49e10a204a2e59dade4386a39217137571a36f4812e80fde1"));
        assert(genesis.hashMerkleRoot == uint256S("0x2c51b867e835c5cdc737fb19c1887543f14cc459a59080a6a4855d723c9e75f1"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.emplace_back("testnet.digicoin.network", false); // digicoin testnet

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,23);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,50);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        bech32_hrp = "tf";

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;


        checkpointData = {
            {
                { 0, uint256S("0x0")}
            }
        };

        chainTxData = ChainTxData{
        };

        /* enable fallback fee on testnet */
        m_fallback_fee_enabled = true;

        consensus.nLastPOWBlock = 2000;
        consensus.nMPoSRewardRecipients = 2;
        consensus.nFirstMPoSBlock = consensus.nLastPOWBlock +
                                    consensus.nMPoSRewardRecipients +
                                    COINBASE_MATURITY;

        //consensus.nFixUTXOCacheHFHeight=84500;
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP16Exception = uint256S("0x665ed5b402ac0b44efc37d8926332994363e8a7278b7ee9a58fb972efadae943");
        consensus.BIP34Height = 0; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests) // activate for digicoin
        consensus.BIP34Hash = uint256S("0x665ed5b402ac0b44efc37d8926332994363e8a7278b7ee9a58fb972efadae943");
        consensus.BIP65Height = 0; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 0; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 6 * 60; // 16 minutes
        consensus.nPowTargetSpacing = 3 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.fPoSNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0x12;
        pchMessageStart[1] = 0x1E;
        pchMessageStart[2] = 0x22;
        pchMessageStart[3] = 0x57;
        nDefaultPort = 23108;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1573223858 , 137772, 0x1f00ffff, 1, 5000000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        MineGenesis(genesis, consensus.powLimit, true);
        assert(consensus.hashGenesisBlock == uint256S("0x00001c8b18755416e4e9b340e5032f2d057f24d4767dd1bcfedc2ef7d2df3f03"));
        assert(genesis.hashMerkleRoot == uint256S("0xead7dd9db90aac6f1a1b57de5b520c8831483f13de22cda1199cc4c57dbba8cd"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = {
            {
                { 0, uint256S("0x0")}
            }
        };

        chainTxData = ChainTxData{
        };

        consensus.nLastPOWBlock = 0x7fffffff;
        consensus.nMPoSRewardRecipients = 10;
        consensus.nFirstMPoSBlock = 5000;


        consensus.nEnableHeaderSignatureHeight = 8801;

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,17);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,50);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "digat";

        /* enable fallback fee on regtest */
        m_fallback_fee_enabled = true;
    }
};

/**
 * Regression network parameters overwrites for unit testing
 */
class CUnitTestParams : public CRegTestParams
{
public:
    CUnitTestParams()
    {
        // Activate the the BIPs for regtest as in Bitcoin
        consensus.BIP16Exception = uint256();
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)

        //  have 500 blocks of maturity, increased values for regtest in unit tests in order to correspond with it
        consensus.nSubsidyHalvingInterval = 750;
        consensus.nRuleChangeActivationThreshold = 558; // 75% for testchains
        consensus.nMinerConfirmationWindow = 744; // Faster than normal for regtest (744 instead of 2016)
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    else if (chain == CBaseChainParams::UNITTEST)
        return std::unique_ptr<CChainParams>(new CUnitTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
