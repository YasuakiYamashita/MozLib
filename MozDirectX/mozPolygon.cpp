//==============================================================================
//
// ポリゴン管理[mozPolygon.cpp]
// Author : Yasuaki Yamashita : 2014/08/22
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "mozPolygon.h"

//==============================================================================
// Polygon Manager
//==============================================================================
namespace moz
{
	namespace DirectX
	{
		//==============================================================================
		// コンストラクタ
		//------------------------------------------------------------------------------
		PolygonManager::PolygonManager(DirectX * dev)
			: m_pDirectX(dev)
			, m_2DusingNum(0u)
			, m_2DLockBuff({ nullptr, nullptr, nullptr })
			, m_2DVtxBuff({ nullptr, nullptr, nullptr })
			, m_3DusingNum(0u)
		{
			const LPDIRECT3DDEVICE9& pDevice =  m_pDirectX->GetDevice();

			// 2D頂点データ初期化
			Init2DVtx();
			// 3D頂点データ初期化
			Init3DVtx();

			// 頂点データ生成
			pDevice->CreateVertexDeclaration(Vtx3dDecl, &m_p3DDec);
			pDevice->CreateVertexDeclaration(Vtx2dDecl, &m_p2DDec);
			pDevice->CreateVertexDeclaration(Vtx3dDecl2, &m_p3DDec2);

			// エラー取得
			LPD3DXBUFFER pError;

			// シェーダコンパイル
			if (FAILED(D3DXCreateEffectFromFile(pDevice, "./data/fx/2d.fx", NULL, NULL, D3DXSHADER_SKIPVALIDATION, NULL, &m_2DEffect, &pError)))
			{
				OutputDebugStringA((char *)pError->GetBufferPointer());
				ASSERT(false, "%s", (char *)pError->GetBufferPointer());
				pError->Release();
				return;
			}

			// シェーダコンパイル
			if (FAILED(D3DXCreateEffectFromFile(pDevice, "./data/fx/3d.fx", NULL, NULL, D3DXSHADER_SKIPVALIDATION, NULL, &m_3DEffect, &pError)))
			{
				OutputDebugStringA((char *)pError->GetBufferPointer());
				ASSERT(false, "%s", (char *)pError->GetBufferPointer());
				pError->Release();
				return;
			}

			// テクニック設定
			m_2DEffect->SetTechnique("basicTechnique");
			m_3DEffect->SetTechnique("basicTechnique");

			// ハンドル取得
			m_2DHandle.proj = m_2DEffect->GetParameterByName(NULL, "mtxProj");
			m_2DHandle.world = m_2DEffect->GetParameterByName(NULL, "mtxWorld");
			m_2DHandle.tex = m_2DEffect->GetParameterByName(NULL, "gDiffuseMap");

			m_3DHandle.matWVP = m_3DEffect->GetParameterByName(NULL, "gMatWVP");
			m_3DHandle.tex = m_3DEffect->GetParameterByName(NULL, "gDiffuseMap");

			// プロジェクションマトリックス
			D3DXMatrixIdentity(&m_mtxProj);
			D3DXMatrixPerspectiveFovLH(&m_mtxProj,		// プロジェクションマトリックスの初期化
				D3DX_PI / 4.0f,				// 視野角
				(float)m_pDirectX->GetWindow()->GetWidth() / (float)m_pDirectX->GetWindow()->GetHeight(),	// アスペクト比
				1.0f,						// rear値
				1500.0f);					// far値

			// ビューマトリックス
			m_posCameraP = D3DXVECTOR3(0.0f, 30, -100.0f);
			m_posCameraR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_vecCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			D3DXMatrixIdentity(&m_mtxView);	// ビューマトリックスの初期化
			D3DXMatrixLookAtLH(&m_mtxView, &m_posCameraP, &m_posCameraR, &m_vecCameraU);

			// 色位置
			m_LighPos = D3DXVECTOR3(-5.0f, 100.0f, -30.0f);

			D3DXMATRIX mProj;

			D3DXMatrixPerspectiveFovLH(&mProj
				, 0.18f*D3DX_PI		// 視野角
				, 1.0f				// アスペクト比
				, 1.0f, 1500.0f);	// near far
			m_mLightVP = m_mtxView * mProj;

			//==============================================================================
			// シャドウマップの生成
			ASSERT(SUCCEEDED(pDevice->CreateDepthStencilSurface(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pShadowMapZ, NULL)) ,"");
			ASSERT(SUCCEEDED(pDevice->CreateTexture(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pShadowMap, NULL)), "");
			ASSERT(SUCCEEDED(m_pShadowMap->GetSurfaceLevel(0, &m_pShadowMapSurf)), "");
			// エッジ
			ASSERT(SUCCEEDED(pDevice->CreateTexture(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pEdgeMap, NULL)), "");
			ASSERT(SUCCEEDED(m_pEdgeMap->GetSurfaceLevel(0, &m_pEdgeMapSurf)), "");
			// エッジをぼかしたマップ
			ASSERT(SUCCEEDED(pDevice->CreateTexture(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pSoftMap[0], NULL)), "");
			ASSERT(SUCCEEDED(m_pSoftMap[0]->GetSurfaceLevel(0, &m_pSoftMapSurf[0])), "");
			ASSERT(SUCCEEDED(pDevice->CreateTexture(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pSoftMap[1], NULL)), "");
			ASSERT(SUCCEEDED(m_pSoftMap[1]->GetSurfaceLevel(0, &m_pSoftMapSurf[1])), "");

			if (FAILED(pDevice->CreateVertexBuffer(sizeof(float) * 5 * 4,
				D3DUSAGE_WRITEONLY,
				NULL,
				D3DPOOL_MANAGED,
				&m_3DVtxBuff2,
				NULL
				)))	{
				ASSERT((m_3DVtxBuff2 != NULL), "頂点バッファが確保できませんでした。");
				return;
			}

			float *vtx = nullptr;

			m_3DVtxBuff2->Lock(0, 0, (void **)&vtx, 0);
			const struct {
				FLOAT       p[3];
				FLOAT       tu, tv;
			}
			Vertex3[4] = {
				//  x      y     z    tu tv
				{ -1.0f, +1.0f, 0.1f, 0, 0, },
				{ +1.0f, +1.0f, 0.1f, 1, 0, },
				{ +1.0f, -1.0f, 0.1f, 1, 1, },
				{ -1.0f, -1.0f, 0.1f, 0, 1, },
			};

			memcpy(vtx, Vertex3, sizeof(float)* 5 * 4);

			m_3DVtxBuff2->Unlock();

			m_3DEffect->OnResetDevice();
		}

		//==============================================================================
		// デストラクタ
		//------------------------------------------------------------------------------
		PolygonManager::~PolygonManager()
		{
			SAFE_RELEASE(m_p2DDec);
			SAFE_RELEASE(m_2DVtxBuff._vtx);
			SAFE_RELEASE(m_2DVtxBuff._col);
			SAFE_RELEASE(m_2DVtxBuff._tex);
			SAFE_RELEASE(m_2DEffect);
			for (auto it: m_2DPolygonList)
			{
				SAFE_DELETE(it);
			}

			SAFE_RELEASE(m_p3DDec);
			SAFE_RELEASE(m_p3DDec2);
			SAFE_RELEASE(m_3DVtxBuff._vtx);
			SAFE_RELEASE(m_3DVtxBuff._nor);
			SAFE_RELEASE(m_3DVtxBuff._col);
			SAFE_RELEASE(m_3DVtxBuff._tex);
			SAFE_RELEASE(m_3DEffect);
			for (auto it : m_3DPolygonList)
			{
				SAFE_DELETE(it);
			}

			SAFE_RELEASE(m_3DVtxBuff2);

			SAFE_RELEASE(m_pSoftMapSurf[1]);
			SAFE_RELEASE(m_pSoftMap[1]);
			SAFE_RELEASE(m_pSoftMapSurf[0]);
			SAFE_RELEASE(m_pSoftMap[0]);
			SAFE_RELEASE(m_pEdgeMapSurf);
			SAFE_RELEASE(m_pEdgeMap);
			SAFE_RELEASE(m_pShadowMapSurf);
			SAFE_RELEASE(m_pShadowMap);
			SAFE_RELEASE(m_pShadowMapZ);

		}

		//==============================================================================
		// 2Dポリゴン初期化
		//------------------------------------------------------------------------------
		void PolygonManager::Init2DVtx(void)
		{
			const LPDIRECT3DDEVICE9& pDevice = m_pDirectX->GetDevice();

			if (FAILED(pDevice->CreateVertexBuffer(sizeof(float)* 3 * k2DMaxBuffer,
				(D3DUSAGE_WRITEONLY),
				NULL,
				D3DPOOL_MANAGED,
				&m_2DVtxBuff._vtx,
				NULL
				)))	{
				ASSERT((m_2DVtxBuff._vtx != NULL), "頂点バッファが確保できませんでした。");
				return;
			}

			if (FAILED(pDevice->CreateVertexBuffer(sizeof(float)* 4 * k2DMaxBuffer,
				(D3DUSAGE_WRITEONLY),
				NULL,
				D3DPOOL_MANAGED,
				&m_2DVtxBuff._col,
				NULL
				)))	{
				ASSERT((m_2DVtxBuff._col != NULL), "頂点バッファが確保できませんでした。");
				return;
			}

			if (FAILED(pDevice->CreateVertexBuffer(sizeof(float)* 2 * k2DMaxBuffer,
				(D3DUSAGE_WRITEONLY),
				NULL,
				D3DPOOL_MANAGED,
				&m_2DVtxBuff._tex,
				NULL
				)))	{
				ASSERT((m_2DVtxBuff._tex != NULL), "頂点バッファが確保できませんでした。");
				return;
			}
		}

		//==============================================================================
		// 3Dポリゴン初期化
		//------------------------------------------------------------------------------
		void PolygonManager::Init3DVtx(void)
		{
			const LPDIRECT3DDEVICE9& pDevice = m_pDirectX->GetDevice();

			if (FAILED(pDevice->CreateVertexBuffer(sizeof(Vector3D)* 4 * k3DMaxBuffer,
				D3DUSAGE_WRITEONLY,
				NULL,
				D3DPOOL_MANAGED,
				&m_3DVtxBuff._vtx,
				NULL
				)))	{
				ASSERT((m_3DVtxBuff._vtx != NULL), "頂点バッファが確保できませんでした。");
				return;
			}

			if (FAILED(pDevice->CreateVertexBuffer(sizeof(Vector3D)* 4 * k3DMaxBuffer,
				D3DUSAGE_WRITEONLY,
				NULL,
				D3DPOOL_MANAGED,
				&m_3DVtxBuff._nor,
				NULL
				)))	{
				ASSERT((m_3DVtxBuff._vtx != NULL), "頂点バッファが確保できませんでした。");
				return;
			}

			if (FAILED(pDevice->CreateVertexBuffer(sizeof(Color)* 4 * k3DMaxBuffer,
				D3DUSAGE_WRITEONLY,
				NULL,
				D3DPOOL_MANAGED,
				&m_3DVtxBuff._col,
				NULL
				)))	{
				ASSERT((m_3DVtxBuff._vtx != NULL), "頂点バッファが確保できませんでした。");
				return;
			}

			if (FAILED(pDevice->CreateVertexBuffer(sizeof(Vector2D)* 4 * k3DMaxBuffer,
				D3DUSAGE_WRITEONLY,
				NULL,
				D3DPOOL_MANAGED,
				&m_3DVtxBuff._tex,
				NULL
				)))	{
				ASSERT((m_3DVtxBuff._vtx != NULL), "頂点バッファが確保できませんでした。");
				return;
			}

		}

		//==============================================================================
		// Update
		//------------------------------------------------------------------------------
		void PolygonManager::Update(void)
		{
			for (auto it : m_3DPolygonList)
			{
				it->Update();
			}

			for (auto it: m_2DPolygonList)
			{
				it->Update();
			}
		}
		
		//==============================================================================
		// 描画
		//------------------------------------------------------------------------------
		void PolygonManager::Draw(void)
		{
			const LPDIRECT3DDEVICE9& pDevice = m_pDirectX->GetDevice();
			LPDIRECT3DSURFACE9 pOldBackBuffer, pOldZBuffer;
			D3DVIEWPORT9 oldViewport;



			//---------------------------------------------------------
			// 行列の作成
			//---------------------------------------------------------
			// ライト方向から見た射影空間への行列
			D3DXVECTOR3 vLookatPt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			D3DXMATRIX mView;
			D3DXMatrixLookAtLH(&mView, &m_LighPos, &vLookatPt, &vUp);
			D3DXMATRIX oldmtxView = m_mtxView;
			
			//-------------------------------------------------
			// レンダリングターゲットの保存
			//-------------------------------------------------
			pDevice->GetRenderTarget(0, &pOldBackBuffer);
			pDevice->GetDepthStencilSurface(&pOldZBuffer);
			pDevice->GetViewport(&oldViewport);

			//-------------------------------------------------
			// レンダリングターゲットの変更
			//-------------------------------------------------
			pDevice->SetRenderTarget(0, m_pShadowMapSurf);
			pDevice->SetDepthStencilSurface(m_pShadowMapZ);
			// ビューポートの変更
			D3DVIEWPORT9 viewport = { 0, 0      // 左上の座標
				, SHADOW_MAP_SIZE // 幅
				, SHADOW_MAP_SIZE // 高さ
				, 0.0f, 1.0f };     // 前面、後面
			pDevice->SetViewport(&viewport);

			//-------------------------------------------------
			// 1パス目:シャドウマップの作成
			//-------------------------------------------------
			m_3DEffect->SetTechnique("ShadowMapTechnique");
			// シャドウマップのクリア
			pDevice->Clear(0L, NULL
				, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
				, 0xffffffff, 1.0f, 0L);

			m_mtxView = mView;
			Draw3DList();

			//-------------------------------------------------
			// 2パス目:深度のエッジを作る
			//-------------------------------------------------
			pDevice->SetRenderTarget(0, m_pEdgeMapSurf);
			// 頂点データ送信
			pDevice->SetVertexDeclaration(m_p3DDec2);

			m_3DEffect->SetTechnique("EdgeMapTechnique");

			pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
			pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

			m_3DEffect->Begin(nullptr, NULL);
			m_3DEffect->BeginPass(0u);
			m_3DEffect->SetTexture("SrcMap", m_pShadowMap);
			m_3DEffect->CommitChanges();
			pDevice->SetStreamSource(0, m_3DVtxBuff2, 0, sizeof(float) * 5);
			pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
			m_3DEffect->EndPass();
			m_3DEffect->End();

			//-------------------------------------------------
			// 3パス目:エッジをぼかす
			//-------------------------------------------------
			pDevice->SetRenderTarget(0, m_pSoftMapSurf[0]);
			m_3DEffect->SetTechnique("EdgeSmudgeTechnique");

			m_3DEffect->Begin(nullptr, NULL);
			m_3DEffect->BeginPass(0u);
			m_3DEffect->SetTexture("SrcMap", m_pEdgeMap);
			m_3DEffect->CommitChanges();
			pDevice->SetStreamSource(0, m_3DVtxBuff2, 0, sizeof(float)* 5);
			pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
			m_3DEffect->EndPass();
			m_3DEffect->End();
			//-------------------------------------------------
			// 3パス目:エッジをぼかす
			//-------------------------------------------------
			pDevice->SetRenderTarget(0, m_pSoftMapSurf[1]);
			m_3DEffect->SetTechnique("EdgeSmudgeTechnique");

			m_3DEffect->Begin(nullptr, NULL);
			m_3DEffect->BeginPass(0u);
			m_3DEffect->SetTexture("SrcMap", m_pSoftMap[0]);
			m_3DEffect->CommitChanges();
			pDevice->SetStreamSource(0, m_3DVtxBuff2, 0, sizeof(float)* 5);
			pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
			m_3DEffect->EndPass();
			m_3DEffect->End();

			//-------------------------------------------------
			// レンダリングターゲットを元に戻す
			//-------------------------------------------------
			pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			pDevice->SetRenderTarget(0, pOldBackBuffer);
			pDevice->SetDepthStencilSurface(pOldZBuffer);
			pDevice->SetViewport(&oldViewport);
			pOldBackBuffer->Release();
			pOldZBuffer->Release();
			m_mtxView = oldmtxView;

			//-------------------------------------------------
			// 4パス目:シーンの描画
			//-------------------------------------------------
			// テクスチャの設定
			m_3DEffect->SetTechnique("shadowTechnique");
			m_3DEffect->SetTexture("ShadowMap", m_pShadowMap);
			m_3DEffect->SetTexture("SrcMap", m_pSoftMap[1]);

			// モデルの描画
			Draw3DList();			

			//==============================================================================
			// 2D描画
			Draw2DList();
		}

		//==============================================================================
		// Draw3D
		//------------------------------------------------------------------------------
		void PolygonManager::Draw3DList(void)
		{
			// デバイス
			const LPDIRECT3DDEVICE9& pDevice = m_pDirectX->GetDevice();
			int startvtx = 0;

			// 射影空間から、テクスチャーの空間に変換する
			const float fOffsetX = 0.5f + (0.5f / (float)SHADOW_MAP_SIZE);
			const float fOffsetY = 0.5f + (0.5f / (float)SHADOW_MAP_SIZE);
			const D3DXMATRIX mScaleBias(0.5f, 0.0f, 0.0f, 0.0f,
				0.0f, -0.5f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				fOffsetX, fOffsetY, 0.0f, 1.0f);

			// 頂点データ送信
			pDevice->SetVertexDeclaration(m_p3DDec);

			//様々なオブジェクトの描画処理を行う
			pDevice->SetStreamSource(0, m_3DVtxBuff._vtx, 0, sizeof(Vector3D));
			pDevice->SetStreamSource(1, m_3DVtxBuff._nor, 0, sizeof(Vector3D));
			pDevice->SetStreamSource(2, m_3DVtxBuff._col, 0, sizeof(Color));
			pDevice->SetStreamSource(3, m_3DVtxBuff._tex, 0, sizeof(Vector2D));

			// シェーダ設定
			m_3DEffect->Begin(nullptr, 0);

			// マトリックス設定
			for (auto it : m_3DPolygonList)
			{
				D3DXMATRIX matWVP, m;
				D3DXVECTOR4 v;

				// 一回ドローを呼ぶ
				it->DrawUpdate();

				matWVP = it->GetWorldMtx() * m_mtxView * m_mtxProj;

				// ライトの空間への射影行列
				m = it->GetWorldMtx() * m_mLightVP;
				m_3DEffect->SetMatrix("mWLP", &m);

				m = m * mScaleBias; // テクスチャ空間への射影行列
				m_3DEffect->SetMatrix("mWLPB", &m);

				// ローカル座標系でのライトベクトル
				D3DXMatrixInverse(&m, NULL, &it->GetWorldMtx());
				D3DXVec3Transform(&v, &m_LighPos, &m);
				D3DXVec4Normalize(&v, &v);
				v.w = 0;
				m_3DEffect->SetVector("vLightDir", &v);

				// 行列設定
				m_3DEffect->SetMatrix(m_3DHandle.matWVP, &matWVP);	// 行列セット

				// テクスチャデーター
				if (it->GetTex())
					m_3DEffect->SetTexture(m_3DHandle.tex, it->GetTex()->GetTex());

				m_3DEffect->CommitChanges();

				// テクスチャ有りとなしでパスを変える
				m_3DEffect->BeginPass(it->GetTex() ? 1u : 0u);

				// ドロー！！
				it->Draw(pDevice, startvtx, it->GetVtxNum());

				// 経過
				startvtx += it->GetVtxNum();

				// 終了
				m_3DEffect->EndPass();
			}

			// シェーダ終了
			m_3DEffect->End();
		}

		//==============================================================================
		// Draw2D
		//------------------------------------------------------------------------------
		void PolygonManager::Draw2DList(void)
		{
			// デバイス
			const LPDIRECT3DDEVICE9& pDevice = m_pDirectX->GetDevice();
			D3DXMATRIX mtxWorld, mtxTmp;
			int startvtx = 0;

			// 
			const D3DXMATRIX mtxProj(
				2 / (float)m_pDirectX->GetWindow()->GetWidth(), 0.0f, 0.0f, 0.0f,
				0.0f, -2 / (float)m_pDirectX->GetWindow()->GetHeight(), 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				-1.0f, 1.0f, 0.0f, 1.0f
				);

			// 頂点
			pDevice->SetVertexDeclaration(m_p2DDec);

			// 2D描画
			pDevice->SetStreamSource(0, m_2DVtxBuff._vtx, 0, sizeof(Vector3D));
			pDevice->SetStreamSource(1, m_2DVtxBuff._col, 0, sizeof(Color));
			pDevice->SetStreamSource(2, m_2DVtxBuff._tex, 0, sizeof(Vector2D));

			// シェーダ開始
			m_2DEffect->Begin(nullptr, 0);
			m_2DEffect->BeginPass(1u);

			// 一個ずつ描画
			for (auto it: m_2DPolygonList)
			{
				const float sinb = sinf(it->GetRot().v.y);
				const float cosb = cosf(it->GetRot().v.y);

				D3DXMatrixIdentity(&mtxWorld);

				// 移動行列
				D3DXMatrixIdentity(&mtxTmp);
				mtxTmp._41 = it->GetPos().v.x;
				mtxTmp._42 = it->GetPos().v.y;
				mtxWorld = mtxTmp * mtxWorld;

				// 回転行列
				D3DXMatrixIdentity(&mtxTmp);
				mtxTmp._11 = cosb;
				mtxTmp._12 = sinb;
				mtxTmp._21 = -sinb;
				mtxTmp._22 = cosb;
				mtxWorld = mtxTmp * mtxWorld;

				// シェーダ設定
				m_2DEffect->SetMatrix(m_2DHandle.proj, &mtxProj);
				m_2DEffect->SetMatrix(m_2DHandle.world, &mtxWorld);

				// テクスチャ設定
				//if (it->GetTex())
				m_2DEffect->SetTexture(m_2DHandle.tex, m_pShadowMap);

				m_2DEffect->CommitChanges();

				// ドロー
				it->DrawUpdate();

				// テクスチャ有りとなしでパスを変える
				m_2DEffect->BeginPass(it->GetTex() ? 1u : 0u);

				// ドロー
				it->Draw(pDevice, startvtx, it->GetVtxNum());

				startvtx += it->GetVtxNum();

				// シェーダ終了
				m_2DEffect->EndPass();

			}

			// シェーダ終了
			m_2DEffect->End();
		}

		//==============================================================================
		// 2Dポリゴン作成
		//------------------------------------------------------------------------------
		Polygon2D* PolygonManager::Create2D(const Vector2D& size, const Color& col)
		{
			Polygon2D* buf = new Polygon2D(size);

			if (m_2DusingNum + buf->GetVtxNum() > k2DMaxBuffer)
			{
				ASSERT(false, "2Dバッファオーバー");
				SAFE_DELETE(buf);
				return nullptr;
			}

			Lock2D(m_2DusingNum, buf->GetVtxNum());
			buf->SetVtx(&m_2DLockBuff);
			Unlock2D();

			m_2DusingNum += buf->GetVtxNum();
			m_2DPolygonList.push_back(buf);

			return buf;
		}

		//==============================================================================
		// 2D ロック
		//------------------------------------------------------------------------------
		void PolygonManager::Lock2D(unsigned int vtxoffset, unsigned int vtxnum)
		{
			// ロックされていないか確認
			if (m_2DLockBuff.col != nullptr &&
				m_2DLockBuff.tex != nullptr &&
				m_2DLockBuff.vtx != nullptr)
			{
				// 強制的にアンロック
				Unlock2D();
			}

			const int size = sizeof(float);

			// サイズを入れておく
			m_2DLockBuff.num = vtxnum;
			m_2DVtxBuff._vtx->Lock(size * 3 * vtxoffset, size * 3 * vtxnum, (void **)&m_2DLockBuff.vtx, 0);
			m_2DVtxBuff._col->Lock(size * 4 * vtxoffset, size * 4 * vtxnum, (void **)&m_2DLockBuff.col, 0);
			m_2DVtxBuff._tex->Lock(size * 2 * vtxoffset, size * 2 * vtxnum, (void **)&m_2DLockBuff.tex, 0);
		}

		//==============================================================================
		// 2D アンロック
		//------------------------------------------------------------------------------
		void PolygonManager::Unlock2D(void)
		{
			// 色
			if (m_2DLockBuff.col != nullptr)
			{
				m_2DVtxBuff._col->Unlock();
				m_2DLockBuff.col = nullptr;
			}
			// テクスチャ
			if (m_2DLockBuff.tex != nullptr)
			{
				m_2DVtxBuff._tex->Unlock();
				m_2DLockBuff.tex = nullptr;
			}
			// 頂点
			if (m_2DLockBuff.vtx != nullptr)
			{
				m_2DVtxBuff._vtx->Unlock();
				m_2DLockBuff.vtx = nullptr;
			}
		}

		//==============================================================================
		// 3D ロック
		//------------------------------------------------------------------------------
		void PolygonManager::Lock3D(unsigned int vtxoffset, unsigned int vtxnum)
		{
			// ロックされていないか確認
			if (m_3DLockBuff.col != nullptr &&
				m_3DLockBuff.tex != nullptr &&
				m_3DLockBuff.vtx != nullptr &&
				m_3DLockBuff.nor != nullptr)
			{
				// 強制的にアンロック
				Unlock3D();
			}

			const int size = sizeof(float);

			// サイズを入れておく
			m_3DLockBuff.num = vtxnum;
			m_3DVtxBuff._vtx->Lock(size * 3 * vtxoffset, size * 3 * vtxnum, (void **)&m_3DLockBuff.vtx, 0);
			m_3DVtxBuff._col->Lock(size * 4 * vtxoffset, size * 4 * vtxnum, (void **)&m_3DLockBuff.col, 0);
			m_3DVtxBuff._tex->Lock(size * 2 * vtxoffset, size * 2 * vtxnum, (void **)&m_3DLockBuff.tex, 0);
			m_3DVtxBuff._nor->Lock(size * 2 * vtxoffset, size * 2 * vtxnum, (void **)&m_3DLockBuff.nor, 0);
		}

		//==============================================================================
		// インデックス
		//------------------------------------------------------------------------------
		LPDIRECT3DINDEXBUFFER9 PolygonManager::CreateIndex(unsigned int IndexNum,const WORD* Index)
		{
			LPDIRECT3DINDEXBUFFER9 indexBuff;
			const LPDIRECT3DDEVICE9& pDevice = m_pDirectX->GetDevice();
			WORD * pIndex = nullptr;
			
			// インデックスバッファ取得
			if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD)* IndexNum,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&indexBuff,
				NULL)))
			{
				ASSERT(false, "インデックスバッファが取得出来ませんでした");
				return nullptr;
			}

			// Index 格納
			if (Index != nullptr)
			{
				indexBuff->Lock(0, 0, (void **)&pIndex, 0);
				for (unsigned int i = 0; i < IndexNum; ++i)
				{
					pIndex[i] = Index[i];
				}
				indexBuff->Unlock();
			}

			return indexBuff;
		}

		//==============================================================================
		// 3D アンロック
		//------------------------------------------------------------------------------
		void PolygonManager::Unlock3D(void)
		{
			// 色
			if (m_3DLockBuff.col != nullptr)
			{
				m_3DVtxBuff._col->Unlock();
				m_3DLockBuff.col = nullptr;
			}
			// テクスチャ
			if (m_3DLockBuff.tex != nullptr)
			{
				m_3DVtxBuff._tex->Unlock();
				m_3DLockBuff.tex = nullptr;
			}
			// 頂点
			if (m_3DLockBuff.vtx != nullptr)
			{
				m_3DVtxBuff._vtx->Unlock();
				m_3DLockBuff.vtx = nullptr;
			}
			// 法線
			if (m_3DLockBuff.nor != nullptr)
			{
				m_3DVtxBuff._nor->Unlock();
				m_3DLockBuff.nor = nullptr;
			}
		}

		//==============================================================================
		// ポリゴンコンテナ
		//------------------------------------------------------------------------------
		void PolygonContainer::Draw(const LPDIRECT3DDEVICE9& device, unsigned int startvtx, unsigned int vtxnum)
		{
			device->DrawPrimitive(D3DPT_TRIANGLESTRIP, startvtx, (vtxnum + 1) / 2);
		}

		//==============================================================================
		// デストラクタ
		//------------------------------------------------------------------------------
		PolygonContainer::~PolygonContainer()
		{
			SAFE_RELEASE(m_indexBuff);
		}
	}
}

//==============================================================================
// 2Dマネージャー
//==============================================================================
namespace moz
{
	namespace DirectX
	{
		//==============================================================================
		// コンストラクタ
		//------------------------------------------------------------------------------
		Polygon2D::Polygon2D(const Vector2D& size)
			: m_size(size)
		{
			SetVtxNum(4);
		}

		//==============================================================================
		// デストラクタ
		//------------------------------------------------------------------------------
		Polygon2D::~Polygon2D()
		{

		}

		//==============================================================================
		// SetVtx
		//------------------------------------------------------------------------------
		void Polygon2D::SetVtx(const _2DLOCKBUFF* buff)
		{
			buff->vtx[0] = Vector3D(-m_size.v.x / 2.f, m_size.v.y / 2.f, 1.0f);
			buff->vtx[1] = Vector3D(-m_size.v.x / 2.f, -m_size.v.y / 2.f, 1.0f);
			buff->vtx[2] = Vector3D(m_size.v.x / 2.f, m_size.v.y / 2.f, 1.0f);
			buff->vtx[3] = Vector3D(m_size.v.x / 2.f, -m_size.v.y / 2.f, 1.0f);

			buff->col[0] = m_col;
			buff->col[1] = m_col;
			buff->col[2] = m_col;
			buff->col[3] = m_col;

			buff->tex[0] = Vector2D(0, 1);
			buff->tex[1] = Vector2D(0, 0);
			buff->tex[2] = Vector2D(1, 1);
			buff->tex[3] = Vector2D(1, 0);
		}

		//==============================================================================
		// Draw
		//------------------------------------------------------------------------------
		void Polygon2D::DrawUpdate(void)
		{
		
		}

		//==============================================================================
		// Update
		//------------------------------------------------------------------------------
		void Polygon2D::Update(void)
		{

		}
	}
}

//==============================================================================
// 3Dマネージャー
//==============================================================================
namespace moz
{
	namespace DirectX
	{
		//==============================================================================
		// コンストラクタ
		//------------------------------------------------------------------------------
		Polygon3D::Polygon3D(const Vector2D& size)
			: m_size(size)
		{
			SetVtxNum(4);
		}

		//==============================================================================
		// デストラクタ
		//------------------------------------------------------------------------------
		Polygon3D::~Polygon3D()
		{

		}

		//==============================================================================
		// SetVtx
		//------------------------------------------------------------------------------
		void Polygon3D::SetVtx(const _3DLOCKBUFF* buff)
		{
			buff->vtx[0] = Vector3D(-m_size.v.x / 2.f, -m_size.v.y / 2.f, 0.0f);
			buff->vtx[1] = Vector3D(-m_size.v.x / 2.f, m_size.v.y / 2.f, 0.0f);
			buff->vtx[2] = Vector3D(m_size.v.x / 2.f, -m_size.v.y / 2.f, 0.0f);
			buff->vtx[3] = Vector3D(m_size.v.x / 2.f, m_size.v.y / 2.f, 0.0f);

			buff->nor[0] = Vector3D(0, 1, 0);
			buff->nor[1] = Vector3D(0, 1, 0);
			buff->nor[2] = Vector3D(0, 1, 0);
			buff->nor[3] = Vector3D(0, 1, 0);

			buff->col[0] = m_col;
			buff->col[1] = m_col;
			buff->col[2] = m_col;
			buff->col[3] = m_col;

			buff->tex[0] = Vector2D(0, 1);
			buff->tex[1] = Vector2D(0, 0);
			buff->tex[2] = Vector2D(1, 1);
			buff->tex[3] = Vector2D(1, 0);
		}

		//==============================================================================
		// Draw
		//------------------------------------------------------------------------------
		void Polygon3D::DrawUpdate(void)
		{
			D3DXMATRIX mtxRot;
			D3DXMatrixIdentity(&mtxRot);
			D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.v.y, m_rot.v.x, m_rot.v.z);

			m_mtxWorld._11 = m_scl.v.x * mtxRot._11;
			m_mtxWorld._12 = m_scl.v.x * mtxRot._12;
			m_mtxWorld._13 = m_scl.v.x * mtxRot._13;
			m_mtxWorld._14 = 0.0f;

			m_mtxWorld._21 = m_scl.v.y * mtxRot._21;
			m_mtxWorld._22 = m_scl.v.y * mtxRot._22;
			m_mtxWorld._23 = m_scl.v.y * mtxRot._23;
			m_mtxWorld._24 = 0.0f;

			m_mtxWorld._31 = m_scl.v.z * mtxRot._31;
			m_mtxWorld._32 = m_scl.v.z * mtxRot._32;
			m_mtxWorld._33 = m_scl.v.z * mtxRot._33;
			m_mtxWorld._34 = 0.0f;

			m_mtxWorld._41 = m_pos.v.x;
			m_mtxWorld._42 = m_pos.v.y;
			m_mtxWorld._43 = m_pos.v.z;
			m_mtxWorld._44 = 1.0f;
		}

		//==============================================================================
		// Update
		//------------------------------------------------------------------------------
		void Polygon3D::Update(void)
		{

		}
	}
}

//EOF