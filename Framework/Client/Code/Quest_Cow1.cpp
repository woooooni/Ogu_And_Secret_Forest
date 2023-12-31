#include "Export_Function.h"
#include "Quest_Cow1.h"
#include "GameMgr.h"
#include "DefaultItem.h"
#include "InventoryMgr.h"
#include "QuestMgr.h"
#include "Quest_Cow2.h"
#include "UIMgr.h"

CQuest_Cow1::CQuest_Cow1()
	: CQuest_Hunting(NPC_CODE::NPC_COW, nullptr, OBJ_ID::TRASH_SLIME, 5)
{

	m_strQuestTitle = L"쓰레기 청소";
	m_strQuestSub = L"쓰레기 슬라임 5마리 잡기";
	m_strQuestDesc = L"쓰레기를 무단 투기하는 악당들을 혼내주자!";

	// TODO :: 소아저씨 대사
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"오구 안녕? 엄마 심부름 왔니?");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"아니라고? 그럼 어쩐 일이니?");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"허허... 내가 그날 무엇을 본 건지 궁금하다고?");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"저~기 숲 안쪽에서 이상한 녀석들이 무리 지어 나오더니");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"숲 이곳저곳에 고약한 냄새가 나는 봉지들을\n잔뜩 버리고는 사라졌단다.");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"내가 왕년에~\n저 놀이터에서 17 대 1로 싸워서 이긴 적이 있어요!");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"그래서 혼내주려고 막 달려갔는데...");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"하필 쓰레기를 버리는 녀석들은 18명이지 뭐냐.");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"한 명만 없었어도 내가 혼쭐을 내주는 건데 말이야.");

	m_vecNpcDescList[_uint(QUEST_PROGRESS::CONTINUE)].push_back(L"뭐라고? 열일곱명이라고?");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::CONTINUE)].push_back(L"아니란다. 분명 열여덟명이었어. 다시 가서 세어보렴.\n아저씨는 바빠서 이만");

	m_vecNpcDescList[_uint(QUEST_PROGRESS::COMPLETE)].push_back(L"다섯 놈이었다니...");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::COMPLETE)].push_back(L"오늘 어디 놀러 나간다고 했지?\n용돈 안 필요하니?");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::COMPLETE)].push_back(L"이거 받고! 오늘 들은 이야기는 못 들은 걸로 해주렴.");

	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();

	if (pPlayer != nullptr)
		m_pRewardItem = CDefaultItem::Create(Engine::Get_Device(), OBJ_ID::ITEM, ITEM_CODE::HAT_TURTLE);
	
	m_iRewardCoin = 100;
	
}

CQuest_Cow1::~CQuest_Cow1()
{
}

void CQuest_Cow1::Update_Quest(_float& fTimeDelta)
{
	if (CUIMgr::GetInstance()->Get_Dialog()->Is_Active())
		CUIMgr::GetInstance()->Get_QuestList()->Set_Shown(false);

	if (!CUIMgr::GetInstance()->Get_NewQuestUI()->Is_Active() ||
		!CUIMgr::GetInstance()->Get_Dialog()->Is_Active())
	{
		CUIMgr::GetInstance()->Get_QuestList()->Set_Title(m_strQuestSub);
		CUIMgr::GetInstance()->Get_QuestList()->Set_Shown(true);
	}

	if (m_eQuestProgress == QUEST_PROGRESS::COMPLETE)
		CUIMgr::GetInstance()->Get_QuestList()->Set_Shown(false);

	__super::Update_Quest(fTimeDelta);
}

void CQuest_Cow1::Accept_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::CONTINUE;
	CQuestMgr::GetInstance()->Add_PlayerQuest(this);

	CUIMgr::GetInstance()->Get_NewQuestUI()->Set_Title(m_strQuestTitle);
	CUIMgr::GetInstance()->Get_NewQuestUI()->Set_Desc(m_strQuestDesc);

	CUIMgr::GetInstance()->Get_NewQuestUI()->Reserve_Active(true);
}

void CQuest_Cow1::Clear_Quest()
{
	CQuest_Cow2* pNextQuest = new CQuest_Cow2();
	CQuestMgr::GetInstance()->Add_Quest(pNextQuest);
	m_eQuestProgress = QUEST_PROGRESS::PROGRESS_END;

	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();

	if (pPlayer == nullptr || m_pRewardItem == nullptr)
		return;

	CInventoryMgr::GetInstance()->Add_Item(m_pRewardItem);
	pPlayer->Add_Money(m_iRewardCoin);

	CUIMgr::GetInstance()->Get_ShortcutKey()->Set_Active(false);
	CUIMgr::GetInstance()->Get_QuestList()->Set_Active(false);
}
