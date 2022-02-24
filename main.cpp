#include"Scene/GameScene.h"
#include"Base/Fps_Manager.h"


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

	//音初期化
	Audio audio;
	audio.Initialize();

	//入力クラス
	dxinput input;

	//初期化
	input.init(Win->w, Win->hwnd);

	//3dオブジェクト静的初期化
	object3D::StaticInit();

	//FPS処理
	FpsManager fps;

	//ゲームシーン初期化
	GameScene gamescene;
	gamescene.Init(&directx, &input, &audio);

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

		//ESCキーで抜ける
		if (input.Triger(DIK_ESCAPE) || gamescene.Isclose == true)
		{
			break;
		}

		// 描画処理前
		directx.Begin_Draw();

		//ゲームシーン描画
		gamescene.Draw();

		// 描画処理後
		directx.Finish_Draw();
	}

	//audio.UnLoadSoundWave(&gamescene.Title_bgm);

	// ウィンドウクラスを登録解除
	Win->Win_Deleate();

	return 0;
}
