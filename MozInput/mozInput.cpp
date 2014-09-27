//==============================================================================
//
// ���͊Ǘ�[mozInput.cpp]
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
		// �R���X�g���N�^
		//------------------------------------------------------------------------------
		InputManager::InputManager(moz::window::window* p)
			: m_window(p)
		{
			HRESULT hr = DirectInput8Create(p->GetHInstance(), DIRECTINPUT_VERSION,
				IID_IDirectInput8,
				(void **)&m_pDInput,
				NULL);

			ASSERT(SUCCEEDED(hr), "���͗p�f�o�C�X���쐬�o���܂���ł���");

			m_inputkey = new InputKey(this);

		}

		//==============================================================================
		// �f�X�g���N�^
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
		// �R���X�g���N�^
		//------------------------------------------------------------------------------
		InputKey::InputKey(InputManager* p)
			: InputDevice(p)
		{
			LPDIRECTINPUT8 pDev = p->GetDevice();

			// �f�o�C�X�I�u�W�F�N�g���쐬
			HRESULT hr = pDev->CreateDevice(GUID_SysKeyboard, &m_pDev, NULL);
			ASSERT(SUCCEEDED(hr), "�f�o�C�X�I�u�W�F�N�g�����܂���ł���");

			// �f�[�^�t�H�[�}�b�g��ݒ�
			hr = m_pDev->SetDataFormat(&c_dfDIKeyboard);
			ASSERT(SUCCEEDED(hr), "�f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł���");

			// �������[�h��ݒ�i�o�b�N�O���E���h����r�����[�h�j
			hr = m_pDev->SetCooperativeLevel(p->GetWindow()->GetHWnd(), (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
			ASSERT(SUCCEEDED(hr), "�������[�h��ݒ�ł��܂���ł���");

			// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
			m_pDev->Acquire();
		}

		//==============================================================================
		// �f�X�g���N�^
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

					// �g���K�[�쐬
					m_aKeyTrigger[nCntKey] = (aKeyState[nCntKey] ^ m_aKeyState[nCntKey]) & aKeyState[nCntKey];
					// �����[�X�쐬
					m_aKeyRelease[nCntKey] = (aKeyState[nCntKey] ^ m_aKeyState[nCntKey]) & ~aKeyState[nCntKey];
					// �v���X�쐬
					m_aKeyState[nCntKey] = aKeyState[nCntKey];

					// ���s�[�g�쐬
					if (m_aKeyState[nCntKey] & 0x80)
					{
						//0x80�Ŏ~�߂Ă���
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
			// �f�o�C�X���擾����ĂȂ��ꍇ
			else
			{
				m_pDev->Acquire();
			}
		}

		//==============================================================================
		// �L�[�{�[�h�̃v���X��Ԃ��擾
		//------------------------------------------------------------------------------
		bool InputKey::GetPress(int nKey)
		{
			return m_aKeyState[nKey] & 0x80 ? true : false;
		}

		//==============================================================================
		// �L�[�{�[�h�̃g���K�[��Ԃ��擾
		//------------------------------------------------------------------------------
		bool InputKey::GetTrigger(int nKey)
		{
			return m_aKeyTrigger[nKey] & 0x80 ? true : false;
		}

		//==============================================================================
		// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
		//------------------------------------------------------------------------------
		bool InputKey::GetRepeat(int nKey, int interval)
		{
			return ((m_aKeyRepeat[nKey] > interval) || (GetTrigger(nKey))) ? true : false;

		}

		//==============================================================================
		// �L�[�{�[�h�̃����|�X��Ԃ��擾
		//------------------------------------------------------------------------------
		bool InputKey::GetRelease(int nKey)
		{
			return m_aKeyRelease[nKey] & 0x80 ? true : false;
		}

	}
}

//EOF