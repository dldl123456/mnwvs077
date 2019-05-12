#pragma once

#include <map>
#include "..\WvsLib\Wz\WzResMan.hpp"

struct RewardInfo;

class MobTemplate
{
	static std::map<int, MobTemplate*>* m_MobTemplates;
	static WZ::Node* m_MobWzProperty;


public:
	//GW_MobReward* m_pReward;

	const std::vector<RewardInfo*> *m_paMobReward;
	unsigned int m_unTotalRewardProb = 0;

	MobTemplate();
	~MobTemplate();
	const std::vector<RewardInfo*>& GetMobReward();

	//cloneNewOne��true�A�N�����ϥΦ@���ݩ� (�Ω�Ƽg�S�w�Ǫ��ݩ�)
	static MobTemplate* GetMobTemplate(int dwTemplateID, bool bCloneNewOne = false);
	static void RegisterMob(int dwTemplateID);
	
	/*
	�����@��public attribute
	*/
	long long int m_lnMaxHP, m_lnMaxMP;

	int 
		m_nLevel, 
		m_nSpeed, 
		m_nPAD, 
		m_nPDD, 
		m_nPDRate,
		m_nMAD, 
		m_nMDD, 
		m_nMDRate,
		m_nACC, 
		m_nEVA, 
		m_nPushed, 
		m_nEXP, 
		m_nCategory, 
		m_nSummonType, 
		m_nMoveAbility;

	bool 
		m_bIsBodyAttack, 
		m_bIsChangeableMob, 
		m_bIsDamagedByMob, 
		m_bIsSmartPhase, 
		m_bIsExplosiveDrop,
		m_bIsTargetFromSrv = true;

	double m_dFs;

	std::string m_strElemAttr, m_strMobType;

	/*void SetMaxHP(long long int maxHp);
	void SetMaxMP(long long int maxMp);

	void SetLevel(int level);
	void SetSpeed(int speed);
	void SetPAD(int PAD);
	void SetPDD(int PDD);
	void SetPDRate(int rate);
	void SetMAD(int MAD);
	void SetMDD(int MDD);
	void SetMDRate(int rate);
	void SetACC(int ACC);
	void SetEVA(int EVA);
	void SetPushed(int pushed);
	void SetEXP(int EXP);
	void SetCategory(int category);

	void SetElementAttribute(const std::string& elemAttr);
	void SetMobType(const std::string& mobType);

	long long int GetMaxHP() const;
	long long int GetMaxMP() const;
		
	int GetLevel() const;
	int GetSpeed() const;
	int GetPAD() const;
	int GetPDD() const;
	int GetPDRate() const;
	int GetMAD() const;
	int GetMDD() const;
	int GetMDRate() const;
	int GetACC() const;
	int GetEVA() const;
	int GetPushed() const;
	int GetEXP() const;
	int GetCategory() const;

	const std::string& GetElementAttribute() const;
	const std::string& GetMobType() const;*/

};
