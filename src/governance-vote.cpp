// Copyright (c) 2014-2017 The Dash Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <governance-vote.h>
#include <governance-object.h>
#include <masternodeman.h>
#include <masternode-sync.h>
#include <messagesigner.h>
#include <util.h>

#include <boost/lexical_cast.hpp>

std::string CGovernanceVoting::ConvertOutcomeToString(vote_outcome_enum_t nOutcome)
{
    switch(nOutcome)
    {
        case VOTE_OUTCOME_NONE:
            return "NONE"; break;
        case VOTE_OUTCOME_YES:
            return "YES"; break;
        case VOTE_OUTCOME_NO:
            return "NO"; break;
        case VOTE_OUTCOME_ABSTAIN:
            return "ABSTAIN"; break;
    }
    return "error";
}

std::string CGovernanceVoting::ConvertSignalToString(vote_signal_enum_t nSignal)
{
    std::string strReturn = "NONE";
    switch(nSignal)
    {
        case VOTE_SIGNAL_NONE:
            strReturn = "NONE";
            break;
        case VOTE_SIGNAL_FUNDING:
            strReturn = "FUNDING";
            break;
        case VOTE_SIGNAL_VALID:
            strReturn = "VALID";
            break;
        case VOTE_SIGNAL_DELETE:
            strReturn = "DELETE";
            break;
        case VOTE_SIGNAL_ENDORSED:
            strReturn = "ENDORSED";
            break;
        case VOTE_SIGNAL_NOOP1:
            strReturn = "NOOP1";
            break;
        case VOTE_SIGNAL_NOOP2:
            strReturn = "NOOP2";
            break;
        case VOTE_SIGNAL_NOOP3:
            strReturn = "NOOP3";
            break;
        case VOTE_SIGNAL_NOOP4:
            strReturn = "NOOP4";
            break;
        case VOTE_SIGNAL_NOOP5:
            strReturn = "NOOP5";
            break;
        case VOTE_SIGNAL_NOOP6:
            strReturn = "NOOP6";
            break;
        case VOTE_SIGNAL_NOOP7:
            strReturn = "NOOP7";
            break;
        case VOTE_SIGNAL_NOOP8:
            strReturn = "NOOP8";
            break;
        case VOTE_SIGNAL_NOOP9:
            strReturn = "NOOP9";
            break;
        case VOTE_SIGNAL_NOOP10:
            strReturn = "NOOP10";
            break;
        case VOTE_SIGNAL_NOOP11:
            strReturn = "NOOP11";
            break;
        case VOTE_SIGNAL_CUSTOM1:
            strReturn = "CUSTOM1";
            break;
        case VOTE_SIGNAL_CUSTOM2:
            strReturn = "CUSTOM2";
            break;
        case VOTE_SIGNAL_CUSTOM3:
            strReturn = "CUSTOM3";
            break;
        case VOTE_SIGNAL_CUSTOM4:
            strReturn = "CUSTOM4";
            break;
        case VOTE_SIGNAL_CUSTOM5:
            strReturn = "CUSTOM5";
            break;
        case VOTE_SIGNAL_CUSTOM6:
            strReturn = "CUSTOM6";
            break;
        case VOTE_SIGNAL_CUSTOM7:
            strReturn = "CUSTOM7";
            break;
        case VOTE_SIGNAL_CUSTOM8:
            strReturn = "CUSTOM8";
            break;
        case VOTE_SIGNAL_CUSTOM9:
            strReturn = "CUSTOM9";
            break;
        case VOTE_SIGNAL_CUSTOM10:
            strReturn = "CUSTOM10";
            break;
        case VOTE_SIGNAL_CUSTOM11:
            strReturn = "CUSTOM11";
            break;
        case VOTE_SIGNAL_CUSTOM12:
            strReturn = "CUSTOM12";
            break;
        case VOTE_SIGNAL_CUSTOM13:
            strReturn = "CUSTOM13";
            break;
        case VOTE_SIGNAL_CUSTOM14:
            strReturn = "CUSTOM14";
            break;
        case VOTE_SIGNAL_CUSTOM15:
            strReturn = "CUSTOM15";
            break;
        case VOTE_SIGNAL_CUSTOM16:
            strReturn = "CUSTOM16";
            break;
        case VOTE_SIGNAL_CUSTOM17:
            strReturn = "CUSTOM17";
            break;
        case VOTE_SIGNAL_CUSTOM18:
            strReturn = "CUSTOM18";
            break;
        case VOTE_SIGNAL_CUSTOM19:
            strReturn = "CUSTOM19";
            break;
        case VOTE_SIGNAL_CUSTOM20:
            strReturn = "CUSTOM20";
            break;
    }

    return strReturn;
}


vote_outcome_enum_t CGovernanceVoting::ConvertVoteOutcome(const std::string& strVoteOutcome)
{
    vote_outcome_enum_t eVote = VOTE_OUTCOME_NONE;
    if(strVoteOutcome == "yes") {
        eVote = VOTE_OUTCOME_YES;
    }
    else if(strVoteOutcome == "no") {
        eVote = VOTE_OUTCOME_NO;
    }
    else if(strVoteOutcome == "abstain") {
        eVote = VOTE_OUTCOME_ABSTAIN;
    }
    return eVote;
}

vote_signal_enum_t CGovernanceVoting::ConvertVoteSignal(const std::string& strVoteSignal)
{
    vote_signal_enum_t eSignal = VOTE_SIGNAL_NONE;
    if(strVoteSignal == "funding") {
        eSignal = VOTE_SIGNAL_FUNDING;
    }
    else if(strVoteSignal == "valid") {
        eSignal = VOTE_SIGNAL_VALID;
    }
    if(strVoteSignal == "delete") {
        eSignal = VOTE_SIGNAL_DELETE;
    }
    if(strVoteSignal == "endorsed") {
        eSignal = VOTE_SIGNAL_ENDORSED;
    }

    if(eSignal != VOTE_SIGNAL_NONE)  {
        return eSignal;
    }

    // ID FIVE THROUGH CUSTOM_START ARE TO BE USED BY GOVERNANCE ENGINE / TRIGGER SYSTEM

    // convert custom sentinel outcomes to integer and store
    try {
        int i = boost::lexical_cast<int>(strVoteSignal);
        if(i < VOTE_SIGNAL_CUSTOM1 || i > VOTE_SIGNAL_CUSTOM20) {
            eSignal = VOTE_SIGNAL_NONE;
        }
        else  {
            eSignal = vote_signal_enum_t(i);
        }
    }
    catch(std::exception const & e)
    {
        std::ostringstream ostr;
        ostr << "CGovernanceVote::ConvertVoteSignal: error : " << e.what() << std::endl;
        LogPrintf(ostr.str().c_str());
    }

    return eSignal;
}

CGovernanceVote::CGovernanceVote()
    : fValid(true),
      fSynced(false),
      nVoteSignal(int(VOTE_SIGNAL_NONE)),
      masternodeOutpoint(),
      nParentHash(),
      nVoteOutcome(int(VOTE_OUTCOME_NONE)),
      nTime(0),
      vchSig()
{}

CGovernanceVote::CGovernanceVote(const COutPoint& outpointMasternodeIn, const uint256& nParentHashIn, vote_signal_enum_t eVoteSignalIn, vote_outcome_enum_t eVoteOutcomeIn)
    : fValid(true),
      fSynced(false),
      nVoteSignal(eVoteSignalIn),
      masternodeOutpoint(outpointMasternodeIn),
      nParentHash(nParentHashIn),
      nVoteOutcome(eVoteOutcomeIn),
      nTime(GetAdjustedTime()),
      vchSig()
{
    UpdateHash();
}

std::string CGovernanceVote::ToString() const
{
    std::ostringstream ostr;
    ostr << masternodeOutpoint.ToStringShort() << ":"
         << nTime << ":"
         << CGovernanceVoting::ConvertOutcomeToString(GetOutcome()) << ":"
         << CGovernanceVoting::ConvertSignalToString(GetSignal());
    return ostr.str();
}

void CGovernanceVote::Relay(CConnman* connman) const
{
    // Do not relay until fully synced
    if(!masternodeSync.IsSynced()) {
        LogPrint(BCLog::GOV, "CGovernanceVote::Relay -- won't relay until fully synced\n");
        return;
    }

    CInv inv(MSG_GOVERNANCE_OBJECT_VOTE, GetHash());
    connman->RelayInv(inv, MIN_GOVERNANCE_PEER_PROTO_VERSION);
}

void CGovernanceVote::UpdateHash() const
{
    // Note: doesn't match serialization

    CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
    ss << masternodeOutpoint << uint8_t{} << 0xffffffff; // adding dummy values here to match old hashing format
    ss << nParentHash;
    ss << nVoteSignal;
    ss << nVoteOutcome;
    ss << nTime;
    *const_cast<uint256*>(&hash) = ss.GetHash();
}

uint256 CGovernanceVote::GetHash() const
{
    return hash;
}

uint256 CGovernanceVote::GetSignatureHash() const
{
    return SerializeHash(*this);
}

bool CGovernanceVote::Sign(const CKey& keyMasternode, const CPubKey& pubKeyMasternode)
{
    std::string strError;

    uint256 hash = GetSignatureHash();

    if(!CHashSigner::SignHash(hash, keyMasternode, vchSig)) {
        LogPrintf("CGovernanceVote::Sign -- SignHash() failed\n");
        return false;
    }

    if (!CHashSigner::VerifyHash(hash, pubKeyMasternode, vchSig, strError)) {
        LogPrintf("CGovernanceVote::Sign -- VerifyHash() failed, error: %s\n", strError);
        return false;
    }

    return true;
}

bool CGovernanceVote::CheckSignature(const CPubKey& pubKeyMasternode) const
{
    std::string strError;

        uint256 hash = GetSignatureHash();

        if (!CHashSigner::VerifyHash(hash, pubKeyMasternode, vchSig, strError)) {
            LogPrintf("CGovernanceVote::Sign -- SignHash() failed\n");
            return false;
        }

    return true;
}

bool CGovernanceVote::IsValid(bool fSignatureCheck) const
{
    if(nTime > GetAdjustedTime() + (60*60)) {
        LogPrint(BCLog::GOV, "CGovernanceVote::IsValid -- vote is too far ahead of current time - %s - nTime %lli - Max Time %lli\n", GetHash().ToString(), nTime, GetAdjustedTime() + (60*60));
        return false;
    }

    // support up to 50 actions (implemented in sentinel)
    if(nVoteSignal > MAX_SUPPORTED_VOTE_SIGNAL)
    {
        LogPrint(BCLog::GOV, "CGovernanceVote::IsValid -- Client attempted to vote on invalid signal(%d) - %s\n", nVoteSignal, GetHash().ToString());
        return false;
    }

    // 0=none, 1=yes, 2=no, 3=abstain. Beyond that reject votes
    if(nVoteOutcome > 3)
    {
        LogPrint(BCLog::GOV, "CGovernanceVote::IsValid -- Client attempted to vote on invalid outcome(%d) - %s\n", nVoteSignal, GetHash().ToString());
        return false;
    }

    masternode_info_t infoMn;
    if(!mnodeman.GetMasternodeInfo(masternodeOutpoint, infoMn)) {
        LogPrint(BCLog::GOV, "CGovernanceVote::IsValid -- Unknown Masternode - %s\n", masternodeOutpoint.ToStringShort());
        return false;
    }

    if(!fSignatureCheck) return true;

    return CheckSignature(infoMn.pubKeyMasternode);
}

bool operator==(const CGovernanceVote& vote1, const CGovernanceVote& vote2)
{
    bool fResult = ((vote1.masternodeOutpoint == vote2.masternodeOutpoint) &&
                    (vote1.nParentHash == vote2.nParentHash) &&
                    (vote1.nVoteOutcome == vote2.nVoteOutcome) &&
                    (vote1.nVoteSignal == vote2.nVoteSignal) &&
                    (vote1.nTime == vote2.nTime));
    return fResult;
}

bool operator<(const CGovernanceVote& vote1, const CGovernanceVote& vote2)
{
    bool fResult = (vote1.masternodeOutpoint < vote2.masternodeOutpoint);
    if(!fResult) {
        return false;
    }
    fResult = (vote1.masternodeOutpoint == vote2.masternodeOutpoint);

    fResult = fResult && (vote1.nParentHash < vote2.nParentHash);
    if(!fResult) {
        return false;
    }
    fResult = fResult && (vote1.nParentHash == vote2.nParentHash);

    fResult = fResult && (vote1.nVoteOutcome < vote2.nVoteOutcome);
    if(!fResult) {
        return false;
    }
    fResult = fResult && (vote1.nVoteOutcome == vote2.nVoteOutcome);

    fResult = fResult && (vote1.nVoteSignal == vote2.nVoteSignal);
    if(!fResult) {
        return false;
    }
    fResult = fResult && (vote1.nVoteSignal == vote2.nVoteSignal);

    fResult = fResult && (vote1.nTime < vote2.nTime);

    return fResult;
}
