#include "../Include/stdafx.h"
#include "Player_State_Run.h"
#include "Transform.h"
#include "Player.h"
#include "KeyMgr.h"
#include "Texture.h"
#include "Export_Function.h"
#include "Effect_Trail.h"
#include "Pool.h"

CPlayer_State_Run::CPlayer_State_Run(CGameObject* _pOwner)
	:CPlayer_State(_pOwner), m_fAccTime(0.0f), m_fKeyDelayTime(0.05f)
{
	for (_uint i = 0; (_uint)OBJ_DIR::DIR_END > i; ++i)
	{
		m_vecHatPos[i].resize(10, { 0.0f,0.0f,0.0f });
	}
	Set_Hat();
	Set_EffectOffset();
}

CPlayer_State_Run::~CPlayer_State_Run()
{
}

HRESULT CPlayer_State_Run::Ready_State(void)
{
	if (GetAsyncKeyState(VK_UP) & 0x8000 && GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_pOwner->SetObj_Dir(OBJ_DIR::DIR_LU);
	}
	else if (GetAsyncKeyState(VK_UP) & 0x8000 && GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_pOwner->SetObj_Dir(OBJ_DIR::DIR_RU);
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_pOwner->SetObj_Dir(OBJ_DIR::DIR_RD);
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_pOwner->SetObj_Dir(OBJ_DIR::DIR_LD);
	}
	else if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_pOwner->SetObj_Dir(OBJ_DIR::DIR_U);
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_pOwner->SetObj_Dir(OBJ_DIR::DIR_D);
	}
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_pOwner->SetObj_Dir(OBJ_DIR::DIR_L);
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_pOwner->SetObj_Dir(OBJ_DIR::DIR_R);
	}

	m_eRunDir = m_pOwner->GetObj_Dir();
	Change_RunDir();


	Stop_Sound(CHANNELID::SOUND_EFFECT_PLAYER);

	int iSound = rand() % 3;
	if (iSound == 1)
		Play_Sound(L"SFX_7_Run_Ground_1.wav", CHANNELID::SOUND_EFFECT_PLAYER, 0.5f);
	else if (iSound == 2)
		Play_Sound(L"SFX_8_Run_Ground_2.wav", CHANNELID::SOUND_EFFECT_PLAYER, 0.5f);
	else
		Play_Sound(L"SFX_9_Run_Ground_3.wav", CHANNELID::SOUND_EFFECT_PLAYER, 0.5f);

	//Left
	m_pLeftEffect = CPool<CEffect_Trail>::Get_Obj();

	if (!m_pLeftEffect)
	{
		m_pLeftEffect = CEffect_Trail::Create(Get_Device());
		NULL_CHECK_RETURN(m_pLeftEffect, E_FAIL);
		m_pLeftEffect->Ready_Object();
	}
	m_pLeftEffect->Set_Effect(m_pOwner, 0.05f);
	m_pLeftEffect->Set_Color(255, 255, 255, 125);
	Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"Trail", m_pLeftEffect);

	//Right
	m_pRightEffect = CPool<CEffect_Trail>::Get_Obj();

	if (!m_pRightEffect)
	{
		m_pRightEffect = CEffect_Trail::Create(Get_Device());
		NULL_CHECK_RETURN(m_pRightEffect, E_FAIL);
		m_pRightEffect->Ready_Object();
	}
	m_pRightEffect->Set_Effect(m_pOwner, 0.05f);
	m_pRightEffect->Set_Color(255, 255, 255, 125);
	Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"Trail", m_pRightEffect);

	m_iCurrIndex = 8;
	Update_EffectOffset();

	return S_OK;
}

_int CPlayer_State_Run::Update_State(const _float& fTimeDelta)
{
	dynamic_cast<CTransform*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_TRANSFORM, ID_STATIC))->Move_Pos(m_pOwner->GetObj_Dir(), dynamic_cast<CPlayer*>(m_pOwner)->Get_Speed() * 2.f, fTimeDelta);
	if (m_fAccTime > m_fKeyDelayTime)
	{
		if (!dynamic_cast<CPlayer*>(m_pOwner)->Is_Stop())
			Key_Input(fTimeDelta);
		m_fAccTime = 0.0f;
	}
	else
		m_fAccTime += fTimeDelta;

	int iSound = rand() % 3;
	if (iSound == 1)
		Play_Sound(L"SFX_7_Run_Ground_1.wav", CHANNELID::SOUND_EFFECT_PLAYER, 0.5f);
	else if (iSound == 2)
		Play_Sound(L"SFX_8_Run_Ground_2.wav", CHANNELID::SOUND_EFFECT_PLAYER, 0.5f);
	else
		Play_Sound(L"SFX_9_Run_Ground_3.wav", CHANNELID::SOUND_EFFECT_PLAYER, 0.5f);


	return 0;
}

void CPlayer_State_Run::LateUpdate_State(void)
{
	if (dynamic_cast<CPlayer*>(m_pOwner)->Is_GetItem())
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::GETTIEM);
	}

	if (dynamic_cast<CPlayer*>(m_pOwner)->Is_BalloonFly())
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::BALLOONFLY);
	}
	
	if (dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat())
		Update_Hat();

	if (m_pLeftEffect && m_pRightEffect)
		Update_EffectOffset();

}

void CPlayer_State_Run::Render_State(void)
{
}

void CPlayer_State_Run::Reset_State(void)
{
	Stop_Sound(CHANNELID::SOUND_EFFECT_PLAYER);

	m_pLeftEffect->Set_End();
	m_pLeftEffect = nullptr;

	m_pRightEffect->Set_End();
	m_pRightEffect = nullptr;
}

void CPlayer_State_Run::Key_Input(const _float& fTimeDelta)
{
	if (KEY_HOLD(KEY::UP_ARROW) && KEY_HOLD(KEY::LEFT_ARROW) && KEY_HOLD(KEY::SHIFT))
	{
		m_eRunDir = OBJ_DIR::DIR_LU;
		if (m_eRunDir != m_pOwner->GetObj_Dir())
		{
			m_pOwner->SetObj_Dir(m_eRunDir);
			Change_RunDir();
			m_iCurrIndex = 8;
			Update_EffectOffset();
		}
	}
	else if (KEY_HOLD(KEY::UP_ARROW) && KEY_HOLD(KEY::RIGHT_ARROW) && KEY_HOLD(KEY::SHIFT))
	{
		m_eRunDir = OBJ_DIR::DIR_RU;
		if (m_eRunDir != m_pOwner->GetObj_Dir())
		{
			m_pOwner->SetObj_Dir(m_eRunDir);
			Change_RunDir();
			m_iCurrIndex = 8;
			Update_EffectOffset();
		}
	}
	else if (KEY_HOLD(KEY::DOWN_ARROW) && KEY_HOLD(KEY::RIGHT_ARROW) && KEY_HOLD(KEY::SHIFT))
	{
		m_eRunDir = OBJ_DIR::DIR_RD;
		if (m_eRunDir != m_pOwner->GetObj_Dir())
		{
			m_pOwner->SetObj_Dir(m_eRunDir);
			Change_RunDir();
			m_iCurrIndex = 8;
			Update_EffectOffset();
		}
	}
	else if (KEY_HOLD(KEY::DOWN_ARROW) && KEY_HOLD(KEY::LEFT_ARROW) && KEY_HOLD(KEY::SHIFT))
	{
		m_eRunDir = OBJ_DIR::DIR_LD;
		if (m_eRunDir != m_pOwner->GetObj_Dir())
		{
			m_pOwner->SetObj_Dir(m_eRunDir);
			Change_RunDir();
			m_iCurrIndex = 8;
			Update_EffectOffset();
		}
	}
	else if (KEY_HOLD(KEY::UP_ARROW) && KEY_HOLD(KEY::SHIFT))
	{
		m_eRunDir = OBJ_DIR::DIR_U;
		if (m_eRunDir != m_pOwner->GetObj_Dir())
		{
			m_pOwner->SetObj_Dir(m_eRunDir);
			Change_RunDir();
			m_iCurrIndex = 8;
			Update_EffectOffset();
		}
	}
	else if (KEY_HOLD(KEY::DOWN_ARROW) && KEY_HOLD(KEY::SHIFT))
	{
		m_eRunDir = OBJ_DIR::DIR_D;
		if (m_eRunDir != m_pOwner->GetObj_Dir())
		{
			m_pOwner->SetObj_Dir(m_eRunDir);
			Change_RunDir();
			m_iCurrIndex = 8;
			Update_EffectOffset();
		}
	}
	else if (KEY_HOLD(KEY::LEFT_ARROW) && KEY_HOLD(KEY::SHIFT))
	{
		m_eRunDir = OBJ_DIR::DIR_L;
		if (m_eRunDir != m_pOwner->GetObj_Dir())
		{
			m_pOwner->SetObj_Dir(m_eRunDir);
			Change_RunDir();
			m_iCurrIndex = 8;
			Update_EffectOffset();
		}
	}
	else if (KEY_HOLD(KEY::RIGHT_ARROW) && KEY_HOLD(KEY::SHIFT))
	{
		m_eRunDir = OBJ_DIR::DIR_R;
		if (m_eRunDir != m_pOwner->GetObj_Dir())
		{
			m_pOwner->SetObj_Dir(m_eRunDir);
			Change_RunDir();
			m_iCurrIndex = 8;
			Update_EffectOffset();
		}
	}
	else
	{
		if (KEY_HOLD(KEY::UP_ARROW) ||
			KEY_HOLD(KEY::RIGHT_ARROW) ||
			KEY_HOLD(KEY::DOWN_ARROW) ||
			KEY_HOLD(KEY::LEFT_ARROW))
			dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::MOVE);
		else
			dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::IDLE);
		
	}


	if (KEY_HOLD(KEY::X))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::ROLL);
	}

	if (KEY_HOLD(KEY::SPACE))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::JUMP);
	}

	if (KEY_HOLD(KEY::A))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::SWING);
	}


}

void CPlayer_State_Run::Update_Hat()
{
	_vec3 vPos;
	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	vPos.y += 0.3f;
	vPos.z -= 0.005f;
	vPos += m_vecHatPos[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()];
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Reset();
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Scale(m_fScale[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()]);
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Angle(D3DXToRadian(m_fAngle[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()]));
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Pos(vPos);

}

void CPlayer_State_Run::Set_Hat()
{
	//포지션
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][0] = { 0.0f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][1] = { 0.0f, 0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][2] = { 0.0f, 0.09f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][3] = { 0.0f, 0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][4] = { 0.01f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][5] = { 0.01f, 0.013f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][6] = { 0.0f, 0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][7] = { 0.0f, 0.12f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][8] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][9] = { 0.0f, 0.0f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][0] = { 0.0f, 0.08f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][1] = { 0.0f, 0.12f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][2] = { 0.01f, 0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][3] = { 0.01f, 0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][4] = { 0.0f, 0.09f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][5] = { 0.0f, 0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][6] = { 0.0f, 0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][7] = { -0.01f, 0.06f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][8] = { 0.0f, 0.0f, 0.0f};
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][9] = { 0.0f, 0.0f, 0.0f};

	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][0] = { -0.1f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][1] = { -0.08f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][2] = { -0.14f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][3] = { -0.15f, 0.06f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][4] = { -0.15f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][5] = { -0.08f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][6] = { -0.12f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][7] = { -0.15f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][8] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][9] = { 0.0f, 0.0f, 0.0f };


	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][0] = { 0.1f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][1] = { 0.08f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][2] = { 0.14f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][3] = { 0.15f, 0.06f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][4] = { 0.15f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][5] = { 0.08f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][6] = { 0.12f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][7] = { 0.15f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][8] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][9] = { 0.0f, 0.0f, 0.0f };


	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][0] = { -0.03f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][1] = { -0.05f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][2] = { -0.03f, 0.00f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][3] = { -0.04f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][4] = { -0.05f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][5] = { -0.03f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][6] = { -0.03f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][7] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][8] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][9] = { 0.0f, 0.0f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][0] = { -0.04f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][1] = { -0.09f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][2] = { -0.06f, 0.06f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][3] = { -0.09f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][4] = { -0.05f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][5] = { -0.12f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][6] = { -0.16f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][7] = { -0.1f, 0.075f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][8] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][9] = { 0.0f, 0.0f, 0.0f };


	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][0] = { 0.04f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][1] = { 0.09f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][2] = { 0.06f, 0.06f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][3] = { 0.09f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][4] = { 0.05f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][5] = { 0.12f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][6] = { 0.16f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][7] = { 0.1f, 0.075f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][8] = { 0.0f, 0.0f, 0.0f    };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][9] = { 0.0f, 0.0f, 0.0f    };


	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][0] = { 0.03f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][1] = { 0.05f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][2] = { 0.03f, 0.00f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][3] = { 0.04f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][4] = { 0.05f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][5] = { 0.03f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][6] = { 0.03f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][7] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][8] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][9] = { 0.0f, 0.0f, 0.0f };



	//각도
	m_fAngle[(_uint)OBJ_DIR::DIR_U][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][2] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][3] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][4] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][5] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][6] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][7] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][9] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_D][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][2] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][3] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][4] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][5] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][6] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][7] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][9] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_L][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][2] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][3] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][4] = 15.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][5] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][6] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][7] = 15.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][9] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_R][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][2] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][3] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][4] = -15.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][5] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][6] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][7] = -15.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][9] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_LU][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][2] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][3] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][4] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][5] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][6] = 10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][7] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][9] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_LD][0] = 10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][1] = 10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][2] = 5.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][3] = 15.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][4] = 10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][5] = 10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][6] = 10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][7] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][9] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_RU][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][2] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][3] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][4] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][5] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][6] = -10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][7] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][9] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_RD][0] = -10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][1] = -10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][2] = -5.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][3] = -15.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][4] = -10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][5] = -10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][6] = -10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][7] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][9] = 0.0f;


	//스케일
	m_fScale[(_uint)OBJ_DIR::DIR_U][0] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][1] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][2] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][3] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][4] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][5] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][6] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][7] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_D][0] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][1] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][2] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][3] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][4] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][5] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][6] = 0.92f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][7] = 0.92f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_L][0] = 0.65f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][1] = 0.7f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][2] = 0.6f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][3] = 0.6f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][4] = 0.6f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][5] = 0.7f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][6] = 0.6f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][7] = 0.65f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_R][0] = 0.65f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][1] = 0.7f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][2] = 0.6f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][3] = 0.6f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][4] = 0.6f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][5] = 0.7f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][6] = 0.6f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][7] = 0.65f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_LU][0] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][1] = 0.7f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][2] = 0.7f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][3] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][4] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][5] = 0.75f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][6] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][7] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_LD][0] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][1] = 0.70f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][2] = 0.75f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][3] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][4] = 0.70f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][5] = 0.80f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][6] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][7] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_RU][0] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][1] = 0.7f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][2] = 0.7f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][3] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][4] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][5] = 0.75f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][6] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][7] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_RD][0] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][1] = 0.70f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][2] = 0.75f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][3] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][4] = 0.70f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][5] = 0.80f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][6] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][7] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][9] = 0.0f;
}



void CPlayer_State_Run::Change_RunDir()
{
	switch (m_eRunDir)
	{
	case OBJ_DIR::DIR_U:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_Up", true);
		m_deqIndex.clear();
		m_deqIndex.push_back(7);
		m_deqIndex.push_back(0);
		m_deqIndex.push_back(1);
		m_deqIndex.push_back(2);
		m_deqIndex.push_back(3);
		m_deqIndex.push_back(4);
		m_deqIndex.push_back(5);
		m_deqIndex.push_back(6);
		break;
	case OBJ_DIR::DIR_D:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_Down", true);
		m_deqIndex.clear();
		m_deqIndex.push_back(7);
		m_deqIndex.push_back(0);
		m_deqIndex.push_back(1);
		m_deqIndex.push_back(2);
		m_deqIndex.push_back(3);
		m_deqIndex.push_back(4);
		m_deqIndex.push_back(5);
		m_deqIndex.push_back(6);
		break;
	case OBJ_DIR::DIR_L:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_Left", true);
		m_deqIndex.clear();
		m_deqIndex.push_back(7);
		m_deqIndex.push_back(0);
		m_deqIndex.push_back(1);
		m_deqIndex.push_back(2);
		m_deqIndex.push_back(3);
		m_deqIndex.push_back(4);
		m_deqIndex.push_back(5);
		m_deqIndex.push_back(6);
		break;
	case OBJ_DIR::DIR_R:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_Right", true);
		m_deqIndex.clear();
		m_deqIndex.push_back(7);
		m_deqIndex.push_back(0);
		m_deqIndex.push_back(1);
		m_deqIndex.push_back(2);
		m_deqIndex.push_back(3);
		m_deqIndex.push_back(4);
		m_deqIndex.push_back(5);
		m_deqIndex.push_back(6);
		break;
	case OBJ_DIR::DIR_LD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_LeftDown", true);
		m_deqIndex.clear();
		m_deqIndex.push_back(6);
		m_deqIndex.push_back(0);
		m_deqIndex.push_back(1);
		m_deqIndex.push_back(2);
		m_deqIndex.push_back(3);
		m_deqIndex.push_back(4);
		m_deqIndex.push_back(5);
		break;
	case OBJ_DIR::DIR_LU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_LeftUp", true);
		m_deqIndex.clear();
		m_deqIndex.push_back(7);
		m_deqIndex.push_back(0);
		m_deqIndex.push_back(1);
		m_deqIndex.push_back(2);
		m_deqIndex.push_back(3);
		m_deqIndex.push_back(4);
		m_deqIndex.push_back(5);
		m_deqIndex.push_back(6);
		break;
	case OBJ_DIR::DIR_RU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_RightUp", true);
		m_deqIndex.clear();
		m_deqIndex.push_back(7);
		m_deqIndex.push_back(0);
		m_deqIndex.push_back(1);
		m_deqIndex.push_back(2);
		m_deqIndex.push_back(3);
		m_deqIndex.push_back(4);
		m_deqIndex.push_back(5);
		m_deqIndex.push_back(6);
		break;
	case OBJ_DIR::DIR_RD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_RightDown", true);
		m_deqIndex.clear();
		m_deqIndex.push_back(6);
		m_deqIndex.push_back(0);
		m_deqIndex.push_back(1);
		m_deqIndex.push_back(2);
		m_deqIndex.push_back(3);
		m_deqIndex.push_back(4);
		m_deqIndex.push_back(5);
		break;
	}
}

void CPlayer_State_Run::Update_EffectOffset()
{
	if (m_iCurrIndex == m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx())
		return;

	m_iCurrIndex = m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx();

	_uint iIndex[4];
	iIndex[0] = m_deqIndex[0];
	iIndex[1] = m_deqIndex[1];
	iIndex[2] = m_deqIndex[2];
	iIndex[3] = m_deqIndex[3];
	
	
	m_deqIndex.pop_front();
	m_deqIndex.push_back(iIndex[0]);

	
	m_pLeftEffect->Set_Offset(m_vecLeftOffset[(_uint)m_pOwner->GetObj_Dir()][iIndex[0]],
							  m_vecLeftOffset[(_uint)m_pOwner->GetObj_Dir()][iIndex[1]],
							  m_vecLeftOffset[(_uint)m_pOwner->GetObj_Dir()][iIndex[2]],
							  m_vecLeftOffset[(_uint)m_pOwner->GetObj_Dir()][iIndex[3]]);


	m_pRightEffect->Set_Offset(m_vecRightOffset[(_uint)m_pOwner->GetObj_Dir()][iIndex[0]],
							   m_vecRightOffset[(_uint)m_pOwner->GetObj_Dir()][iIndex[1]],
							   m_vecRightOffset[(_uint)m_pOwner->GetObj_Dir()][iIndex[2]],
							   m_vecRightOffset[(_uint)m_pOwner->GetObj_Dir()][iIndex[3]]);
}

void CPlayer_State_Run::Set_EffectOffset()
{
	//Left

	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_D][0] = { -0.41f, 0.0f, 0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_D][1] = { -0.42f, 0.12f, 0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_D][2] = { -0.42f, 0.22f, 0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_D][3] = { -0.41f, 0.22f, 0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_D][4] = { -0.41f, 0.0f, 0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_D][5] = { -0.42f, 0.1f, 0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_D][6] = { -0.41f, 0.23f, 0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_D][7] = { -0.4f, 0.23f, 0.05f };

	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_U][0] = { -0.4f, 0.0f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_U][1] = { -0.4f, 0.13f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_U][2] = { -0.4f, 0.21f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_U][3] = { -0.4f, 0.21f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_U][4] = { -0.4f, 0.0f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_U][5] = { -0.41f, 0.13f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_U][6] = { -0.4f, 0.26f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_U][7] = { -0.4f, 0.26f, -0.05f };

	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_L][0] = { 0.03f, 0.35f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_L][1] = { -0.03f, -0.1f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_L][2] = { 0.12f, 0.14f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_L][3] = { 0.07f, 0.25f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_L][4] = { 0.04f, 0.35f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_L][5] = { -0.03f, -0.1f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_L][6] = { 0.12f, 0.15f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_L][7] = { 0.07f, 0.2f, -0.05f };

	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_R][0] = { -0.03f, 0.37f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_R][1] = { 0.03f, -0.12f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_R][2] = { -0.12f, 0.16f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_R][3] = { -0.07f, 0.27f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_R][4] = { -0.04f, 0.37f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_R][5] = { 0.03f, -0.12f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_R][6] = { -0.12f, 0.17f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_R][7] = { -0.07f, 0.22f, -0.05f };

	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_LU][0] = { -0.23f, -0.1f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_LU][1] = { -0.12f, 0.1f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_LU][2] = { -0.12f, 0.1f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_LU][3] = { -0.2f, 0.1f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_LU][4] = { -0.24f, -0.12f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_LU][5] = { -0.17f, 0.12f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_LU][6] = { -0.15f, 0.12f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_LU][7] = { -0.22f, 0.16f, -0.05f };

	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_RU][0] = { -0.23f, 0.3f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_RU][1] = { -0.18f, 0.2f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_RU][2] = { -0.25f, 0.19f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_RU][3] = { -0.24f, 0.25f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_RU][4] = { -0.24f, 0.29f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_RU][5] = { -0.22f, 0.19f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_RU][6] = { -0.23f, 0.27f, -0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_RU][7] = { -0.24f, 0.27f, -0.05f };

	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_LD][0] = { -0.4f, 0.1f, 0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_LD][1] = { -0.41f, 0.21f, 0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_LD][2] = { -0.4f, 0.19f, 0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_LD][3] = { -0.42f, -0.05f, 0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_LD][4] = { -0.4f, 0.19f, 0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_LD][5] = { -0.4f, 0.19f, 0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_LD][6] = { -0.4f, -0.05f, 0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_LD][7] = { 0.0f, 0.0f, 0.0f };

	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_RD][0] = { -0.29f, 0.1f, 0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_RD][1] = { -0.18f, 0.22f, 0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_RD][2] = { -0.22f, 0.22f, 0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_RD][3] = { -0.3f, 0.1f, 0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_RD][4] = { -0.18f, 0.2f, 0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_RD][5] = { -0.22f, 0.22f, 0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_RD][6] = { -0.29f, 0.1f, 0.05f };
	m_vecLeftOffset[(_uint)OBJ_DIR::DIR_RD][7] = { 0.0f, 0.0f, 0.0f };


	//Right

	m_vecRightOffset[(_uint)OBJ_DIR::DIR_D][0] = { 0.41f, 0.0f, 0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_D][1] = { 0.41f, 0.13f, 0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_D][2] = { 0.39f, 0.26f, 0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_D][3] = { 0.39f, 0.26f, 0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_D][4] = { 0.41f, 0.0f, 0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_D][5] = { 0.41f, 0.1f, 0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_D][6] = { 0.4f, 0.19f, 0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_D][7] = { 0.39f, 0.22f, 0.05f };

	m_vecRightOffset[(_uint)OBJ_DIR::DIR_U][0] = { 0.4f, 0.0f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_U][1] = { 0.4f, 0.14f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_U][2] = { 0.4f, 0.26f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_U][3] = { 0.4f, 0.26f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_U][4] = { 0.4f, 0.0f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_U][5] = { 0.41f, 0.13f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_U][6] = { 0.39f, 0.22f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_U][7] = { 0.39f, 0.22f, -0.05f };

	m_vecRightOffset[(_uint)OBJ_DIR::DIR_L][0] = { 0.03f, 0.37f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_L][1] = { -0.03f, -0.12f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_L][2] = { 0.12f, 0.16f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_L][3] = { 0.07f, 0.27f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_L][4] = { 0.04f, 0.37f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_L][5] = { -0.03f, -0.12f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_L][6] = { 0.12f, 0.17f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_L][7] = { 0.07f, 0.22f, -0.05f };

	m_vecRightOffset[(_uint)OBJ_DIR::DIR_R][0] = { -0.03f, 0.35f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_R][1] = { 0.03f, -0.1f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_R][2] = { -0.12f, 0.14f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_R][3] = { -0.07f, 0.25f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_R][4] = { -0.04f, 0.35f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_R][5] = { 0.03f, -0.1f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_R][6] = { -0.12f, 0.15f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_R][7] = { -0.07f, 0.2f, -0.05f };

	m_vecRightOffset[(_uint)OBJ_DIR::DIR_LU][0] = { 0.23f, 0.3f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_LU][1] = { 0.18f, 0.2f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_LU][2] = { 0.25f, 0.19f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_LU][3] = { 0.24f, 0.25f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_LU][4] = { 0.24f, 0.29f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_LU][5] = { 0.22f, 0.19f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_LU][6] = { 0.23f, 0.27f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_LU][7] = { 0.24f, 0.27f, -0.05f };

	m_vecRightOffset[(_uint)OBJ_DIR::DIR_RU][0] = { 0.23f, -0.1f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_RU][1] = { 0.12f, 0.1f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_RU][2] = { 0.12f, 0.1f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_RU][3] = { 0.2f, 0.1f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_RU][4] = { 0.24f, -0.12f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_RU][5] = { 0.17f, 0.12f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_RU][6] = { 0.15f, 0.12f, -0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_RU][7] = { 0.22f, 0.16f, -0.05f };

	m_vecRightOffset[(_uint)OBJ_DIR::DIR_LD][0] = { 0.28f, 0.1f, 0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_LD][1] = { 0.18f, 0.22f, 0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_LD][2] = { 0.22f, 0.22f, 0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_LD][3] = { 0.3f, 0.1f, 0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_LD][4] = { 0.18f, 0.2f, 0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_LD][5] = { 0.22f, 0.22f, 0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_LD][6] = { 0.29f, 0.1f, 0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_LD][7] = { 0.0f, 0.0f, 0.0f };

	m_vecRightOffset[(_uint)OBJ_DIR::DIR_RD][0] = { 0.4f, 0.1f, 0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_RD][1] = { 0.41f, 0.21f, 0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_RD][2] = { 0.4f, 0.19f, 0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_RD][3] = { 0.42f, -0.05f, 0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_RD][4] = { 0.4f, 0.19f, 0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_RD][5] = { 0.4f, 0.19f, 0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_RD][6] = { 0.4f, -0.05f, 0.05f };
	m_vecRightOffset[(_uint)OBJ_DIR::DIR_RD][7] = { 0.0f, 0.0f, 0.0f };

}
