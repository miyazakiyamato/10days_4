#include "ImGuiManager.h"
#ifdef USE_IMGUI
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
#endif // USE_IMGUI
#include "WinApp.h"
#include "DirectXCommon.h"
#include "SrvUavManager.h"
#include "GlobalVariables.h"
#include "SceneManager.h"

namespace Engine {

void ImGuiManager::Initialize(WinApp* winApp, DirectXCommon* dxCommon, SrvUavManager* srvUavManager){
	winApp_ = winApp;
	dxCommon_ = dxCommon;
	srvUavManager_ = srvUavManager;
#ifdef USE_IMGUI
	//ImGuiのコンテキストを生成
	ImGui::CreateContext();
	//ImGuiのスタイルを設定
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Docking有効化

	//Win32用初期化
	ImGui_ImplWin32_Init(winApp_->GetHwnd());

	// ImGui DX12 Backendの初期化
	// InitInfo構造体を使用して初期化し、RenderHasTexturesフラグが確実に設定されるようにします
	ImGui_ImplDX12_InitInfo initInfo = {};
	initInfo.Device = dxCommon_->GetDevice();
	initInfo.CommandQueue = dxCommon_->GetCommandQueue();
	initInfo.NumFramesInFlight = static_cast<int>(dxCommon_->GetBackBufferCount());
	initInfo.RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	initInfo.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	initInfo.SrvDescriptorHeap = srvUavManager_->GetDescriptorHeapForImGui();
	initInfo.LegacySingleSrvCpuDescriptor = srvUavManager_->GetDescriptorHeapForImGui()->GetCPUDescriptorHandleForHeapStart();
	initInfo.LegacySingleSrvGpuDescriptor = srvUavManager_->GetDescriptorHeapForImGui()->GetGPUDescriptorHandleForHeapStart();
	
	ImGui_ImplDX12_Init(&initInfo);
#endif // USE_IMGUI
}

void ImGuiManager::Finalize(){
#ifdef USE_IMGUI
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif // USE_IMGUI
}

void ImGuiManager::Begin(){
#ifdef USE_IMGUI
	//ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// --- レイアウトの初期化 ---
	ImGuiIO& io = ImGui::GetIO();
	
	// ビューポート全体を覆うDockSpaceを作成
	// これにより、任意のImGuiウィンドウをメインウィンドウにドッキングできるようになります。
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::DockSpaceOverViewport(0U, viewport, ImGuiDockNodeFlags_PassthruCentralNode);
	
	// DockSpaceのIDを取得（OverViewportは"DockSpace"という名前でIDを生成します）
	ImGuiID dockSpaceID = ImGui::GetID("DockSpace");

	// 各デバッグウィンドウの描画設定
	// Dockingシステムが移動やリサイズを管理するため、ここでは最小限の設定のみ行います。
	ImGuiWindowFlags commonFlags = 0; // 閉じるボタンを非表示にする場合は引数で制御

	// メニューバーの描画
	DrawMenuBar();

	if (currentViewMode_ != ViewMode::kGameOnly) {
		if (currentViewMode_ == ViewMode::kEditor) {
			// Scene View
			if (ImGui::Begin("Scene View", nullptr, commonFlags | ImGuiWindowFlags_NoCollapse)) {
				// ゲームシーンの描画結果をテクスチャとして表示
				ImVec2 viewportSize = ImGui::GetContentRegionAvail();
				uint32_t srvIndex = dxCommon_->GetOffScreenSRVIndex();
				D3D12_GPU_DESCRIPTOR_HANDLE handle = srvUavManager_->GetGPUDescriptorHandle(srvIndex);
				ImGui::Image((ImTextureID)handle.ptr, viewportSize);

				// Status Overlay (Inside Scene View)
				ImGui::SetCursorPos(ImVec2(10, 20)); // Window内の相対座標
				DrawStatusOverlayContent();
			}
			ImGui::End();
		}

		// Scene Control
		if (ImGui::Begin("Scene Control", nullptr, commonFlags)) {
			// シーン切り替えボタン
			SceneManager* sceneManager = SceneManager::GetInstance();
			if (ImGui::Button("TITLE")) { sceneManager->ChangeScene("TITLE"); }
			if (ImGui::Button("GAME")) { sceneManager->ChangeScene("GAME"); }
			if (ImGui::Button("CLEAR")) { sceneManager->ChangeScene("CLEAR"); }
			if (ImGui::Button("GAMEOVER")) { sceneManager->ChangeScene("GAMEOVER"); }
			if (ImGui::Button("D_PARTICLE")) { sceneManager->ChangeScene("D_PARTICLE"); }
		}
		ImGui::End();

		// Global Variables
		GlobalVariables::GetInstance()->Update();

		// Watch / Status
		if (ImGui::Begin("Watch / Status", nullptr, commonFlags)) {
			if (ImGui::BeginTabBar("DebugTabs")) {
				if (ImGui::BeginTabItem("Status")) {
					ImGui::Text("FPS: %.1f", io.Framerate);
					SceneManager* sceneManager = SceneManager::GetInstance();
					ImGui::Text("Transition: %s", sceneManager->GetTransition() ? "Active" : "None");
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Logs")) {
					ImGui::Text("Log 1: ...");
					ImGui::Text("Log 2: ...");
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
		ImGui::End();

		
	}
	if (currentViewMode_ != ViewMode::kEditor) {
		// ステータス表示
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
		ImGui::Begin("Status Overlay", nullptr,
			ImGuiWindowFlags_NoTitleBar |         // タイトルバーを非表示
			ImGuiWindowFlags_NoResize |          // リサイズを禁止
			ImGuiWindowFlags_NoScrollbar |       // スクロールバーを非表示
			ImGuiWindowFlags_NoCollapse |        // 折りたたみボタンを非表示
			ImGuiWindowFlags_NoBackground |      // 背景を透明にする
			ImGuiWindowFlags_NoSavedSettings |   // 設定を保存しない
			ImGuiWindowFlags_NoFocusOnAppearing |// アプリケーションがフォーカスされたときにフォーカスしない
			ImGuiWindowFlags_NoNav |             // ナビゲーション入力を受け付けない
			ImGuiWindowFlags_NoDocking           // ドッキング禁止（これ自体はドックされない）
		);
		DrawStatusOverlayContent();
		ImGui::End();
	}
#endif // USE_IMGUI
}

void ImGuiManager::DrawStatusOverlayContent() {
#ifdef USE_IMGUI
	ImGuiIO& io = ImGui::GetIO();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "FPS: %.1f", io.Framerate);
#endif // USE_IMGUI
}

void ImGuiManager::DrawMenuBar() {
#ifdef USE_IMGUI
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Display")) {
			if (ImGui::MenuItem("Editor", nullptr, currentViewMode_ == ViewMode::kEditor)) {
				currentViewMode_ = ViewMode::kEditor;
			}
			if (ImGui::MenuItem("Debug Overlay", nullptr, currentViewMode_ == ViewMode::kDebugOverlay)) {
				currentViewMode_ = ViewMode::kDebugOverlay;
			}
			if (ImGui::MenuItem("Game Only", nullptr, currentViewMode_ == ViewMode::kGameOnly)) {
				currentViewMode_ = ViewMode::kGameOnly;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
#endif // USE_IMGUI
}

void ImGuiManager::End(){
#ifdef USE_IMGUI
	//描画前準備
	ImGui::Render();
#endif // USE_IMGUI
}

void ImGuiManager::Draw(){
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { srvUavManager_->GetDescriptorHeapForImGui() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
#ifdef USE_IMGUI
	//描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
#endif // USE_IMGUI
}

} // namespace Engine
