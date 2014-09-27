//==============================================================================
//
// 入力管理[mozInput.cpp]
// Author : Yasuaki Yamashita : 2014/09/25
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "mozInput.h"

//==============================================================================
// InputManager
//------------------------------------------------------------------------------
namespace moz
{
	namespace input
	{

		//==============================================================================
		// コンストラクタ
		//------------------------------------------------------------------------------
		InputManager::InputManager(moz::window::window* p)
			: m_window(p)
		{
			HRESULT hr = DirectInput8Create(p->GetHInstance(), DIRECTINPUT_VERSION,
				IID_IDirectInput8,
				(void **)&m_pDInput,
				NULL);

			ASSERT(SUCCEEDED(hr), "入力用デバイスが作成出来ませんでした");

			m_inputkey = new InputKey(this);

		}

		//==============================================================================
		// デストラクタ
		//------------------------------------------------------------------------------
		InputManager::~InputManager()
		{
			SAFE_RELEASE(m_pDInput);
			SAFE_DELETE(m_inputkey);
		}

		//==============================================================================
		// Update
		//------------------------------------------------------------------------------
		void InputManager::Update(void)
		{
			m_inputkey->Update();
		}
	}
}

//==============================================================================
// InputKey
//------------------------------------------------------------------------------
namespace moz
{
	namespace input
	{
		//==============================================================================
		// コンストラクタ
		//------------------------------------------------------------------------------
		InputKey::InputKey(InputManager* p)
			: InputDevice(p)
		{
			LPDIRECTINPUT8 pDev = p->GetDevice();

			// デバイスオブジェクトを作成
			HRESULT hr = pDev->CreateDevice(GUID_SysKeyboard, &m_pDev, NULL);
			ASSERT(SUCCEEDED(hr), "デバイスオブジェクトが作れませんでした");

			// データフォーマットを設定
			hr = m_pDev->SetDataFormat(&c_dfDIKeyboard);
			ASSERT(SUCCEEDED(hr), "データフォーマットを設定できませんでした");

			// 協調モードを設定（バックグラウンド＆非排他モード）
			hr = m_pDev->SetCooperativeLevel(p->GetWindow()->GetHWnd(), (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
			ASSERT(SUCCEEDED(hr), "強調モードを設定できませんでした");

			// キーボードへのアクセス権を獲得(入力制御開始)
			m_pDev->Acquire();
		}

		//==============================================================================
		// デストラクタ
		//------------------------------------------------------------------------------
		InputKey::~InputKey()
		{
			SAFE_RELEASE(m_pDev);
		}

		//------------------------------------------------------------------------------
		// Update
		//------------------------------------------------------------------------------
		void InputKey::Update(void)
		{
			BYTE aKeyState[256];

			if (SUCCEEDED(m_pDev->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
			{
				for (int nCntKey = 0; nCntKey < 256; nCntKey++)
				{

					// トリガー作成
					m_aKeyTrigger[nCntKey] = (aKeyState[nCntKey] ^ m_aKeyState[nCntKey]) & aKeyState[nCntKey];
					// リリース作成
					m_aKeyRelease[nCntKey] = (aKeyState[nCntKey] ^ m_aKeyState[nCntKey]) & ~aKeyState[nCntKey];
					// プレス作成
					m_aKeyState[nCntKey] = aKeyState[nCntKey];

					// リピート作成
					if (m_aKeyState[nCntKey] & 0x80)
					{
						//0x80で止めておく
						if (m_aKeyRepeat[nCntKey] < 0x80)
						{
							m_aKeyRepeat[nCntKey]++;
						}
					}
					else
					{
						m_aKeyRepeat[nCntKey] = 0;
					}
				}
			}
			// デバイスが取得されてない場合
			else
			{
				m_pDev->Acquire();
			}
		}

		//==============================================================================
		// キーボードのプレス状態を取得
		//------------------------------------------------------------------------------
		bool InputKey::GetPress(int nKey)
		{
			return m_aKeyState[nKey] & 0x80 ? true : false;
		}

		//==============================================================================
		// キーボードのトリガー状態を取得
		//------------------------------------------------------------------------------
		bool InputKey::GetTrigger(int nKey)
		{
			return m_aKeyTrigger[nKey] & 0x80 ? true : false;
		}

		//==============================================================================
		// キーボードのリピート状態を取得
		//------------------------------------------------------------------------------
		bool InputKey::GetRepeat(int nKey, int interval)
		{
			return ((m_aKeyRepeat[nKey] > interval) || (GetTrigger(nKey))) ? true : false;

		}

		//==============================================================================
		// キーボードのリリ－ス状態を取得
		//------------------------------------------------------------------------------
		bool InputKey::GetRelease(int nKey)
		{
			return m_aKeyRelease[nKey] & 0x80 ? true : false;
		}

	}
}

//EOF