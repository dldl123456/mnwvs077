#include "GW_ItemSlotEquip.h"
#include "WvsUnified.h"
#include "..\WvsLib\Logger\WvsLogger.h"
#include "..\WvsLib\DateTime\GameDateTime.h"

#include "Poco\Data\MySQL\MySQLException.h"
#include "..\WvsLib\Memory\MemoryPoolMan.hpp"

#define ADD_EQUIP_FLAG(name, container)\
if(n##name != 0) {\
	nFlag |= EQP_##name;\
	VALUE_HOLDER vh;\
	vh.type = sizeof(n##name);\
	if((vh.type) == 1)vh.cValue = n##name;\
	if ((vh.type) == 2)vh.sValue = n##name;\
	if ((vh.type) == 4)vh.iValue = n##name;\
	if ((vh.type) == 8)vh.liValue = n##name;\
	container.push_back(vh);\
}

#define DECODE_EQUIP_FLAG(name)\
if (nFlag & EQP_##name) {\
	if (sizeof(n##name) == 1) n##name = iPacket->Decode1();\
	if (sizeof(n##name) == 2) n##name = iPacket->Decode2();\
	if (sizeof(n##name) == 4) n##name = iPacket->Decode4();\
	if (sizeof(n##name) == 8) n##name = iPacket->Decode8();\
}

GW_ItemSlotEquip::GW_ItemSlotEquip()
{
	nInstanceType = GW_ItemSlotInstanceType::GW_ItemSlotEquip_Type;
}


GW_ItemSlotEquip::~GW_ItemSlotEquip()
{
}

void GW_ItemSlotEquip::Load(ATOMIC_COUNT_TYPE SN)
{
	std::string sColumnName = "ItemSN";
	if (bIsCash)
		sColumnName = "CashItemSN";
	Poco::Data::Statement queryStatement(GET_DB_SESSION);
	queryStatement << "SELECT * FROM ItemSlot_EQP Where " + sColumnName + " = " << SN;
	queryStatement.execute();

	Poco::Data::RecordSet recordSet(queryStatement);
	nCharacterID = recordSet["CharacterID"];
	liItemSN = recordSet["ItemSN"];
	liCashItemSN = recordSet["CashItemSN"];
	nItemID = recordSet["ItemID"];
	liExpireDate = recordSet["ExpireDate"];
	nAttribute = recordSet["Attribute"];
	nPOS = recordSet["POS"];
	nRUC = (unsigned char)(unsigned short)recordSet["RUC"];
	nCUC = (unsigned char)(unsigned short)recordSet["CUC"];
	nCuttable = recordSet["Cuttable"];
	nSTR = recordSet["I_STR"];
	nDEX = recordSet["I_DEX"];
	nINT = recordSet["I_INT"];
	nLUK = recordSet["I_LUK"];
	nMaxHP = recordSet["I_MaxHP"];
	nMaxMP = recordSet["I_MaxMP"];
	nPAD = recordSet["I_PAD"];
	nMAD = recordSet["I_MAD"];
	nPDD = recordSet["I_PDD"];
	nMDD = recordSet["I_MDD"];
	nACC = recordSet["I_ACC"];
	nEVA = recordSet["I_EVA"];
	nSpeed = recordSet["I_Speed"];
	nCraft = recordSet["I_Craft"];
	nJump = recordSet["I_Jump"];
	nType = GW_ItemSlotType::EQUIP;

	bIsCash = (liCashItemSN != -1);
}

void GW_ItemSlotEquip::Save(int nCharacterID)
{
	if (nType != GW_ItemSlotType::EQUIP)
		throw std::runtime_error("Invalid Equip Type.");
	Poco::Data::Statement queryStatement(GET_DB_SESSION);
	try 
	{
		if (liItemSN < -1 /*nStatus == GW_ItemSlotStatus::DROPPED*/) //DROPPED or DELETED
		{
			liItemSN *= -1;
			queryStatement << "UPDATE ItemSlot_EQP Set CharacterID = -1 Where CharacterID = " << nCharacterID << " and ItemSN = " << liItemSN;

			//WvsLogger::LogFormat(WvsLogger::LEVEL_ERROR, "Del SQL = %s\n", queryStatement.toString().c_str());
			queryStatement.execute();
			return;
		}
		if (liItemSN == -1)
		{
			liItemSN = IncItemSN(GW_ItemSlotType::EQUIP);
			if (bIsCash && liCashItemSN == -1)
				liCashItemSN = IncItemSN(GW_ItemSlotType::CASH);
			queryStatement << "INSERT INTO ItemSlot_EQP (ItemSN, CashItemSN, ItemID, CharacterID, ExpireDate, Attribute, POS, RUC, CUC, Cuttable, I_STR, I_DEX, I_INT, I_LUK, I_MaxHP, I_MaxMP, I_PAD, I_MAD, I_PDD, I_MDD, I_ACC, I_EVA, I_Speed, I_Craft, I_Jump) VALUES("
				<< liItemSN << ", "
				<< liCashItemSN << ", "
				<< nItemID << ", "
				<< nCharacterID << ", "
				<< liExpireDate << ", "
				<< nAttribute << ", "
				<< nPOS << ", "
				<< (unsigned short)nRUC << ", "
				<< (unsigned short)nCUC << ", "
				<< (unsigned short)nCuttable << ", "
				<< nSTR << ", "
				<< nDEX << ", "
				<< nINT << ", "
				<< nLUK << ", "
				<< nMaxHP << ", "
				<< nMaxMP << ", "
				<< nPAD << ", "
				<< nMAD << ", "
				<< nPDD << ", "
				<< nMDD << ", "
				<< nACC << ", "
				<< nEVA << ", "
				<< nSpeed << ", "
				<< nCraft << ", "
				<< nJump << ")";
			//WvsLogger::LogFormat(WvsLogger::LEVEL_ERROR, "Insert SQL = %s\n", queryStatement.toString().c_str());
		}
		else
		{
			queryStatement << "UPDATE ItemSlot_EQP Set "
				<< "CashItemSN = '" << liCashItemSN << "', "
				<< "ItemID = '" << nItemID << "', "
				<< "CharacterID = '" << nCharacterID << "', "
				<< "ExpireDate = '" << liExpireDate << "', "
				<< "Attribute = '" << nAttribute << "', "
				<< "POS ='" << nPOS << "', "
				<< "RUC ='" << (unsigned short)nRUC << "', "
				<< "CUC ='" << (unsigned short)nCUC << "', "
				<< "Cuttable = '" << (unsigned short)nCuttable << "', "
				<< "I_STR = '" << nSTR << "', "
				<< "I_DEX = '" << nDEX << "', "
				<< "I_INT = '" << nINT << "', "
				<< "I_LUK = '" << nLUK << "', "
				<< "I_MaxHP = '" << nMaxHP << "', "
				<< "I_MaxMP = '" << nMaxMP << "', "
				<< "I_PAD = '" << nPAD << "', "
				<< "I_MAD = '" << nMAD << "', "
				<< "I_PDD = '" << nPDD << "', "
				<< "I_MDD = '" << nMDD << "', "
				<< "I_ACC = '" << nACC << "', "
				<< "I_EVA = '" << nEVA << "', "
				<< "I_Speed = '" << nSpeed << "', "
				<< "I_Craft = '" << nCraft << "', "
				<< "I_Jump = '" << nJump << "' WHERE ItemSN = " << liItemSN;
			//WvsLogger::LogFormat(WvsLogger::LEVEL_ERROR, "Update SQL = %s\n", queryStatement.toString().c_str());
		}
		queryStatement.execute();
	}
	catch (Poco::Data::MySQL::StatementException &) {
		printf("SQL Exception : %s\n", queryStatement.toString().c_str());
	}
}

/*
Equip Encoder Entry (Then Encoding Position Info And Equip Attributes Info.)
*/
void GW_ItemSlotEquip::Encode(OutPacket *oPacket, bool bForInternal) const
{
	EncodeInventoryPosition(oPacket);
	if (bForInternal)
		oPacket->Encode8(liItemSN);
	RawEncode(oPacket);
}

/*
Encode Equip Information
*/
void GW_ItemSlotEquip::RawEncode(OutPacket *oPacket) const
{
	GW_ItemSlotBase::RawEncode(oPacket);
	EncodeEquipBase(oPacket);
	EncodeEquipAdvanced(oPacket);
}

/*
Encode Equip Basic Information
*/
//ADD_EQUIP_FLAG cause compiler think it will convert short to char, but it actually wont.
#pragma warning(disable:4244)  
void GW_ItemSlotEquip::EncodeEquipBase(OutPacket *oPacket) const
{
	oPacket->Encode1(nRUC);
	oPacket->Encode1(nCUC);
	oPacket->Encode2(nSTR);
	oPacket->Encode2(nDEX);
	oPacket->Encode2(nINT);
	oPacket->Encode2(nLUK);
	oPacket->Encode2(nMaxHP);
	oPacket->Encode2(nMaxHP);
	oPacket->Encode2(nPAD);
	oPacket->Encode2(nMAD);
	oPacket->Encode2(nPDD);
	oPacket->Encode2(nMDD);
	oPacket->Encode2(nACC);
	oPacket->Encode2(nEVA);
	oPacket->Encode2(nCraft);
	oPacket->Encode2(nSpeed);
	oPacket->Encode2(nJump);
}
#pragma warning(default:4244)  

/*
Encode Equip Advanced Information, Including Potential, Sockets, And Etc.
*/
void GW_ItemSlotEquip::EncodeEquipAdvanced(OutPacket *oPacket) const
{
	oPacket->EncodeStr("Owner");
	oPacket->Encode2(nAttribute);

	if (liCashItemSN == -1)
		oPacket->Encode8(liItemSN);
}

void GW_ItemSlotEquip::Decode(InPacket *iPacket, bool bForInternal)
{
	if (bForInternal)
		liItemSN = iPacket->Decode8();
	RawDecode(iPacket);
}

void GW_ItemSlotEquip::RawDecode(InPacket *iPacket)
{
	GW_ItemSlotBase::RawDecode(iPacket);
	DecodeEquipBase(iPacket);
	DecodeEquipAdvanced(iPacket);
}

#pragma warning(disable:4244)  
void GW_ItemSlotEquip::DecodeEquipBase(InPacket *iPacket)
{
	nRUC = iPacket->Decode1();
	nCUC = iPacket->Decode1();
	nSTR = iPacket->Decode2();
	nDEX = iPacket->Decode2();
	nINT = iPacket->Decode2();
	nLUK = iPacket->Decode2();
	nMaxHP = iPacket->Decode2();
	nMaxHP = iPacket->Decode2();
	nPAD = iPacket->Decode2();
	nMAD = iPacket->Decode2();
	nPDD = iPacket->Decode2();
	nMDD = iPacket->Decode2();
	nACC = iPacket->Decode2();
	nEVA = iPacket->Decode2();
	nSpeed = iPacket->Decode2();
	nCraft = iPacket->Decode2();
	nJump = iPacket->Decode2();
}
#pragma warning(default:4244) 

void GW_ItemSlotEquip::DecodeEquipAdvanced(InPacket *iPacket)
{
	std::string strTitle = iPacket->DecodeStr();
	nAttribute = iPacket->Decode2();

	if (liCashItemSN == -1)
		liItemSN = iPacket->Decode8();
}

void GW_ItemSlotEquip::Release()
{
	FreeObj(this);
}

GW_ItemSlotBase * GW_ItemSlotEquip::MakeClone()
{
	GW_ItemSlotEquip* ret = AllocObj(GW_ItemSlotEquip);
	*ret = *this;
	ret->liItemSN = -1;
	return ret;
}