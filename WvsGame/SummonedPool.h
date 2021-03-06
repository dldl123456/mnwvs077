#pragma once
#include "FieldPoint.h"

#include <set>
#include <mutex>
#include <atomic>

class Field;
class User;
class Summoned;

class SummonedPool
{
	Field* m_pField;
	std::mutex m_mtxSummonedLock;
	std::atomic<int> m_nSummonedIdCounter;
	std::set<Summoned*> m_sSummoned;

public:
	SummonedPool(Field *pField);
	~SummonedPool();

	std::mutex& GetSummonedPoolLock();
	void OnEnter(User *pUser);
	Summoned* GetSummoned(int nFieldObjID);
	bool CreateSummoned(User* pUser, Summoned* pSummoned, const FieldPoint& pt);
	Summoned* CreateSummoned(User* pUser, int nSkillID, int nSLV, const FieldPoint& pt, unsigned int tEnd, bool bMigrate = false);
	void RemoveSummoned(int nCharacterID, int nSkillID, int nLeaveType);
	void Update(unsigned int tCur);
};

