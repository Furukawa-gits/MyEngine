#include"Scene/GameScene.h"
#include"2D/PostEffect.h"
#include"Base/Fps_Manager.h"
#include"FbxLoder/FbxLoader.h"


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Window_Generate* Win = new Window_Generate;

	Win->Win_Init();

	Win->Win_Create();

	// DirectX初期化処理　ここから

//デバッグレイヤーをオンに
#ifdef _DEBUG
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif

	//directXクラス
	directX directx;

	directx.Init(Win);
	directx.Initializedepth();

	FbxLoader::GetInstance()->Initilize(directx.dev.Get());

	//音初期化
	Audio audio;
	audio.Initialize();

	//入力クラス
	dxinput input;

	//初期化
	input.init(Win->w, Win->hwnd);

	//3dオブジェクト静的初期化
	object3D_obj::StaticInit();

	//ゲームシーン初期化
	GameScene gamescene;
	gamescene.Init(&directx, &input, &audio);

	PostEffect* posteffect = new PostEffect();
	posteffect->Init(directx.dev.Get());

	bool isSetGray = false;

	//FPS処理
	FpsManager fps;

	//FPS調整(一回目)
	fps.init();

	/*-----------------------------------------------　ゲ　ー　ム　ル　ー　プ　--------------------------------------------------------*/
	while (true)
	{
		// メッセージがある？
		if (PeekMessage(&Win->msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Win->msg); // キー入力メッセージの処理
			DispatchMessage(&Win->msg); // プロシージャにメッセージを送る
		}

		// ×ボタンで終了メッセージが来たらゲームループを抜ける
		if (Win->msg.message == WM_QUIT) {
			break;
		}

		fps.Run();

		// DirectX毎フレーム処理　ここから

		//キーボード情報の取得
		input.Update(Win->hwnd);

		//ゲームシーン更新
		gamescene.Update();

		//グレースケール切り替え
		if (input.Triger(DIK_2) && isSetGray == false)
		{
			isSetGray = true;
		}
		else if (input.Triger(DIK_2) && isSetGray == true)
		{
			isSetGray = false;
		}

		//ESCキーで抜ける
		if (input.Triger(DIK_ESCAPE) || gamescene.Isclose == true)
		{
			break;
		}

		// 描画処理
		posteffect->PreDrawScene(directx.cmdList.Get(), directx.dev.Get());
		//背景スプライト、
		gamescene.DrawBack();
		//深度バッファクリア
		posteffect->depthClear(directx.cmdList.Get());
		//3Dオブジェクト
		gamescene.Draw3D();
		posteffect->PostDrawScene(directx.cmdList.Get(), &directx);
		directx.preDraw();
		//ポストエフェクト
		if (isSetGray)
		{
			posteffect->setPipelineGray(directx.cmdList.Get());
		}
		else
		{
			posteffect->reSetPipeline();
		}
		posteffect->Draw(directx.cmdList.Get(), directx.dev.Get());
		//前景スプライト
		gamescene.Draw2D();
		directx.postDraw();

		directx.finishDraw();
	}

	//audio.UnLoadSoundWave(&gamescene.Title_bgm);
	FbxLoader::GetInstance()->Finalize();

	delete posteffect;

	// ウィンドウクラスを登録解除
	Win->Win_Deleate();

	return 0;
}
