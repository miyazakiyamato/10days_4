#include "GameScene.h"
#include <cassert>
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"
#include "MyMtMatrix.h"
#include "Collision.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	for (auto& pair : objectColors_) {
		delete pair.second;
	}
	objectColors_.clear();
	//
	delete mapChipField_;
	//
	delete modelBlock_;
	for (BrokenBlock* brokenBlock : brokenBlocks_) {
		delete brokenBlock;
	}
	brokenBlocks_.clear();
	for (MovingBlock* movingBlock : movingBlocks_) {
		delete movingBlock;
	}
	movingBlocks_.clear();
	for (ButtonBlock* button : buttons_) {
		delete button;
	}
	buttons_.clear();
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	//
	delete debugCamera_;
	//
	delete modelPlayer_;
	delete player_;
	//
	delete modelSkydome_;
	delete skydome_;
	//
	delete modelEnemy_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	for (GoalBlock* goalBlock : goalBlocks_) {
		delete goalBlock;
	}
	//
	delete deathParticles_;
	delete clearParticle_;
	//
	delete fade_;
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	seClear_ = audio_->LoadWave("se_clear.mp3");

	//
	textureHandleBlock_ = TextureManager::Load("./Resources/sibafu.png");
	textureHandlePlayer_ = TextureManager::Load("./Resources/Player/player.png");
	textureHandleEnemy_ = TextureManager::Load("./Resources/PlayerBlock.png");

	for (size_t i = 0; i < 4; i++) {
		uiSprite_.push_back(std::make_unique<Sprite>());
	}
	uiSprite_[0].reset(Sprite::Create(TextureManager::Load("./Resources/clear.png"), {}, {1, 1, 1, 1}, {0, 0}));
	uiSprite_[1].reset(Sprite::Create(TextureManager::Load("./Resources/gameWay.png"), {120.0f,100.0f}, {1, 1, 1, 1}, {0, 0}));
	uiSprite_[2].reset(Sprite::Create(TextureManager::Load("./Resources/moveWay1.png"), {-61.0f,0.0f}, {1, 1, 1, 1}, {0, 0}));
	uiSprite_[3].reset(Sprite::Create(TextureManager::Load("./Resources/moveWay3.png"), {410.0f,100.0f}, {1, 1, 1, 1}, {0, 0}));
	
	//
	modelBlock_ = Model::CreateFromOBJ("cube");
	modelPlayer_ = Model::CreateFromOBJ("Player",true);
	modelEnemy_ = Model::CreateFromOBJ("cube", true);
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	buttonBlock_ = Model::CreateFromOBJ("Crystal", true);
	worldTransform_.Initialize();
	viewProjection_.farZ = 400.0f;
	viewProjection_.Initialize();
	viewProjection_.translation_ = {11,11,-30};
	viewProjection_.UpdateMatrix();
	//
	debugCamera_ = new DebugCamera(1280, 720);
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
	AxisIndicator::GetInstance()->SetVisible(false);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	//スカイドーム
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_,&viewProjection_);
	//マップチップ
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("./Resources/map/map" + std::to_string(stageNum_) + ".csv");
	//ブロック 自キャラ Goal 生成
	GenerateBlocks();

	cameraController = new CameraController;
	cameraController->Initialize(&viewProjection_);
	cameraController->SetTarget(player_);
	cameraController->Reset();
	//
	for (auto& pair : objectColors_) {
		pair.second->TransferMatrix();
	}
	//
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void GameScene::Update() {
	if (input_->TriggerKey(DIK_R)) {
		phase_ = Phase::kFadeOut;
		fade_->Start(Fade::Status::FadeOut, 1.0f);
	}
	if (input_->TriggerKey(DIK_T)) {
		isReturnTitle_ = true;
		phase_ = Phase::kFadeOut;
		fade_->Start(Fade::Status::FadeOut, 1.0f);
	}
	if (input_->TriggerKey(DIK_Y)) {
		isReturnSelect_ = true;
		phase_ = Phase::kFadeOut;
		fade_->Start(Fade::Status::FadeOut, 1.0f);
	}
	if (stop) {
		return;
	}
	// デバッグ用
	mapChipField_->ImGuiUpdate();
	#ifdef _DEBUG
	ImGui::Begin("Camera Settings");
	// translation_ は Vector3 型を想定しています
	// 第3引数の 0.1f はドラッグ時の移動感度です
	ImGui::DragFloat3("Camera Translation", &viewProjection_.translation_.x, 0.1f);
	ImGui::End();
	#endif

	ChangePhase();

	//
	/*ImGui::Begin("Debug2");
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
	ImGui::End();*/
	//
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	
	Model::PreDraw(commandList);
	// スカイドーム
	skydome_->Draw();
	Model::PostDraw();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	if (stageNum_ == 0) {
		uiSprite_[1]->Draw();
		uiSprite_[2]->Draw();
		uiSprite_[3]->Draw();
	}
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion
	
#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	//model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);
	//スカイドーム
	

	//ブロック
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			modelBlock_->Draw(*worldTransformBlock, viewProjection_, textureHandleBlock_);
		}
	}
	for (ButtonBlock* button : buttons_) {
		button->Draw();
	}
	for (MovingBlock* movingBlock : movingBlocks_) {
		movingBlock->Draw();
	}
	for (BrokenBlock* brokenBlock : brokenBlocks_) {
		brokenBlock->Draw();
	}

	//自キャラ描画
	player_->Draw();
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}
	for (GoalBlock* goalBlock : goalBlocks_) {
		goalBlock->Draw();
	}
	//
	float kLineCount = 10;
	if (isDebagCameraActive) {
		for (float i = -kLineCount; i <= kLineCount; i++) {
			PrimitiveDrawer::GetInstance()->DrawLine3d({i, 0, -kLineCount}, {i, 0, 10}, {0.0f, 0.0f, 1.0f, 1.0f});
			PrimitiveDrawer::GetInstance()->DrawLine3d({-kLineCount, 0, i}, {kLineCount, 0, i}, {1.0f, 0.0f, 0.0f, 1.0f});
		}
	}
	if (deathParticles_ != nullptr) {
		deathParticles_->Draw();
	}
	if (clearParticle_ != nullptr) {
		clearParticle_->Draw();
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
	switch (phase_) {
	case GameScene::Phase::kFadeIn:
		fade_->Draw(commandList);
		break;
	case GameScene::Phase::kPlay:
		Sprite::PreDraw(commandList);
		if (stageNum_ == 0) {
			/*uiSprite_[1]->Draw();
			uiSprite_[2]->Draw();*/
			//uiSprite_[3]->Draw();
		}
		Sprite::PostDraw();
		break;
	case GameScene::Phase::kClear:
		Sprite::PreDraw(commandList);
		uiSprite_[0]->Draw();
		Sprite::PostDraw();
		break;
	case GameScene::Phase::kDeath:
		break;
	case GameScene::Phase::kFadeOut:
		fade_->Draw(commandList);
		break;
	default:
		break;
	}
}

void GameScene::GenerateBlocks() {
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// 同じIDの座標リストを保持するマップ
	std::map<int, std::vector<Vector3>> waypointsMap;

	for (uint32_t y = 0; y < numBlockVirtical; ++y) {
		for (uint32_t x = 0; x < numBlockHorizontal; ++x) {
			MapChipCell cell = mapChipField_->GetMapChipCellByIndex(x, y);
			Vector3 pos = mapChipField_->GetMapChipPositionByIndex(x, y);

			// MovingBlockの移動先やペア位置などをBlank等で置いている場合
			if (cell.type == MapChipType::kBlank && cell.id != -1) {
				waypointsMap[cell.id].push_back(pos);
			} else if (cell.type == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[y][x] = worldTransform;
				worldTransformBlocks_[y][x]->translation_ = pos;
				worldTransformBlocks_[y][x]->UpdateMatrix();
			} else if (cell.type == MapChipType::kPlayer) {
				if (!player_) {
					player_ = new Player();
					player_->Initialize(modelPlayer_, textureHandlePlayer_, &viewProjection_, pos);
					player_->SetMapChipField(mapChipField_);
				}
			} else if (cell.type == MapChipType::kGoal) {
				GoalBlock* newGoalBlocks = new GoalBlock;
				newGoalBlocks->Initialize(modelEnemy_, TextureManager::Load("./Resources/Goal.png"), &viewProjection_, pos);
				goalBlocks_.push_back(newGoalBlocks);
			}

			// IDが存在し、まだカラーが作られていなければ生成してマップに登録
			if (cell.id != -1 && objectColors_.find(cell.id) == objectColors_.end()) {
				ObjectColor* newColor = new ObjectColor();
				newColor->Initialize();
				newColor->SetColor(GetColorById(cell.id));
				objectColors_[cell.id] = newColor;
			}

			if (cell.type == MapChipType::kButton) {
				ButtonBlock* newButton = new ButtonBlock;
				newButton->Initialize(buttonBlock_, TextureManager::Load("./Resources/Button.png"), &viewProjection_, pos);
				newButton->SetId(cell.id);

				buttons_.push_back(newButton);
			} else if (cell.type == MapChipType::kMovingBlock) {
				MovingBlock* newMovingBlock = new MovingBlock;
				// ここで生成された位置が「始まり（5_0など）」の初期位置になります
				newMovingBlock->Initialize(modelBlock_, TextureManager::Load("./Resources/MovingBlock.png"), &viewProjection_, pos);
				newMovingBlock->SetId(cell.id);

				// 同時に、この動くブロック自体の初期位置もwaypointsMapに「最初の位置」として登録しておくと後でペアを組みやすいです
				waypointsMap[cell.id].insert(waypointsMap[cell.id].begin(), pos);

				// カラーのポインタをセットする
				if (cell.id != -1) {
					newMovingBlock->SetObjectColor(objectColors_[cell.id]);
				}
				movingBlocks_.push_back(newMovingBlock);
			} else if (cell.type == MapChipType::kBrokenBlock) {
				BrokenBlock* newBrokenBlock = new BrokenBlock;

				newBrokenBlock->SetId(cell.id);
				newBrokenBlock->Initialize(modelBlock_, TextureManager::Load("./Resources/BrokenBlock.png"), &viewProjection_, pos);
				if (cell.id != -1) {
					newBrokenBlock->SetObjectColor(objectColors_[cell.id]);
				}
				newBrokenBlock->SetMapChipField(mapChipField_);

				brokenBlocks_.push_back(newBrokenBlock);
			}
		}
	}

	// 生成後にIDを基準にして、終わりの位置（0_0など）をMovingBlockに教える
	for (MovingBlock* movingBlock : movingBlocks_) {
		movingBlock->SetPlayer(player_);
		int id = movingBlock->GetId();
		if (waypointsMap.find(id) != waypointsMap.end() && waypointsMap[id].size() >= 2) {
			// waypointsMap[id][0] は自身の初期位置(5_0など)
			// waypointsMap[id][1] は対になる終わりの位置(0_0など)
			Vector3 endPos = waypointsMap[id][1];
			movingBlock->SetEndPosition(endPos);
		}
	}
	for (BrokenBlock* brokenBlock : brokenBlocks_) {
		brokenBlock->SetPlayer(player_);
	}
}

void GameScene::CheckAllCollisions() {
	//自キャラ敵キャラの当たり判定
	AABB aabb1, aabb2;
	//自キャラの座標
	aabb1 = player_->GetAABB();

	//自キャラと敵当たり判定
	for (Enemy* enemy : enemies_) {
		//敵の座標
		aabb2 = enemy->GetAABB();

		//AABB同士の交差判定
		if (Collision::IsCollision(aabb1, aabb2)) {
			//自キャラコールバック
			player_->OnCollision(enemy);
			//敵キャラコールバック
			enemy->OnCollision(player_);
		}
	}
	// 自キャラごーるの当たり判定
	// 自キャラの座標
	aabb1 = player_->GetAABB();
	
	// 自キャラとゴールの当たり判定
	for (GoalBlock* goal : goalBlocks_) {
		// ゴールの座標
		aabb2 = goal->GetAABB();

		// AABB同士の交差判定
		if (Collision::IsCollision(aabb1, aabb2)) {
			// 自キャラコールバック
			player_->OnCollision(goal);
			// 敵キャラコールバック
			goal->OnCollision(player_);
		}
	}
	// 1. 毎フレームの最初に全ボタンの「今フレームの衝突フラグ」をリセット
	for (ButtonBlock* button : buttons_) {
		button->SetIsCollidingThisFrame(false); // 内部で isCollidingThisFrame_ = false; にする
	}

	// 2. 当たり判定チェック
	AABB playerAABB = player_->GetAABB();
	ButtonBlock* collidedButton = nullptr;

	for (ButtonBlock* button : buttons_) {
		AABB buttonAABB = button->GetAABB();
		if (Collision::IsCollision(playerAABB, buttonAABB)) {
			button->OnCollision(player_);
			collidedButton = button; // 接触したボタンを記録
		}
	}

	// 3. どちらかのボタンに触れた場合の排他制御（アクティブ切り替え）
	if (collidedButton != nullptr) {
		for (ButtonBlock* button : buttons_) {
			if (button == collidedButton) {
				button->SetIsActive(true); // 触れたボタンをアクティブ（半透明）に
			} else {
				button->SetIsActive(false); // それ以外は非アクティブに
			}
		}
	}
	// 自キャラと動くブロックの当たり判定
	aabb1 = player_->GetAABB();
	for (MovingBlock* movingBlock : movingBlocks_) {
		aabb2 = movingBlock->GetAABB();
		if (Collision::IsCollision(aabb1, aabb2)) {
			player_->OnCollision(movingBlock);
		}
	}
	for (BrokenBlock* brokenBlock : brokenBlocks_) {
		aabb2 = brokenBlock->GetAABB();
		if (Collision::IsCollision(aabb1, aabb2)) {
			player_->OnCollision(brokenBlock);
		}
	}
}

void GameScene::ChangePhase() {
	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->GetStatus() == Fade::Status::None) {
			phase_ = Phase::kPlay;
		}
		break;


	// ゲームプレイフェーズの処理
	case Phase::kPlay:
		// スカイドームの更新
		skydome_->Update();
		// 自キャラ更新
		player_->Update();
		if (player_->GetIsMakeBlock()) {
			Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(mapChipField_->GetMapChipIndexSetByPosition(player_->GetWorldPosition()).xIndex,mapChipField_->GetMapChipIndexSetByPosition(player_->GetWorldPosition()).yIndex);
			if (player_->GetLRDirection() == LRDirection::kRight) {
				enemyPosition.x += 2.0f;
			} else {
				enemyPosition.x -= 2.0f;
			}
			uint32_t mapXIndex = mapChipField_->GetMapChipIndexSetByPosition(enemyPosition).xIndex;
			uint32_t mapYIndex = mapChipField_->GetMapChipIndexSetByPosition(enemyPosition).yIndex;
			if (mapChipField_->GetMapChipData(mapXIndex, mapYIndex) == MapChipType::kBlank) {
				mapChipField_->SetMapChipData(mapXIndex, mapYIndex, MapChipType::kBlock);
				Enemy* newEnemy = new Enemy;
				newEnemy->Initialize(modelEnemy_, textureHandleEnemy_, &viewProjection_, enemyPosition);
				newEnemy->SetMapChipField(mapChipField_);
				newEnemy->SetPlayer(player_);
				enemies_.push_back(newEnemy);
			}
		}
		// 敵の更新
		enemies_.remove_if([](Enemy* enemy) {
			if (!enemy->GetIsAlive()) {
				delete enemy;
				return true;
			}
			return false;
		});
		player_->SetLastEnemy(nullptr);
		for (Enemy* enemy : enemies_) {
			player_->SetLastEnemy(enemy);
		}
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}
		for (GoalBlock* goalBlock : goalBlocks_) {
			goalBlock->Update();
		}
		for (ButtonBlock* button : buttons_) {
			button->Update();
		}
		for (MovingBlock* mBlock : movingBlocks_) {
			bool isAnyActive = false;
			for (ButtonBlock* button : buttons_) {
				if (mBlock->GetId() == button->GetId()) {
					if (button->GetIsActive()) {
						isAnyActive = true;
						break; // 1つでもオンなら確定
					}
				}
			}
			mBlock->SetIsActive(isAnyActive);
			mBlock->Update(brokenBlocks_);
		}
		for (BrokenBlock* bBlock : brokenBlocks_) {
			bool isAnyActive = false;
			for (ButtonBlock* button : buttons_) {
				if (bBlock->GetId() == button->GetId()) {
					if (button->GetIsActive()) {
						isAnyActive = true;
						break; // 1つでもオンなら確定
					}
				}
			}
			bBlock->SetIsActive(isAnyActive);
			bBlock->Update(brokenBlocks_, movingBlocks_);
		}
		player_->SetIsMoveBlock(false);
		for (auto& pair : objectColors_) {
			pair.second->TransferMatrix();
		}
		// カメラコントローラーの更新
		//cameraController->Update();
		// カメラの更新
#ifdef _DEBUG
		if (input_->TriggerKey(DIK_P)) {
			isDebagCameraActive = !isDebagCameraActive;
			AxisIndicator::GetInstance()->SetVisible(isDebagCameraActive);
		}
#endif // DEBUG
		if (isDebagCameraActive) {
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
			// ビュープロジェクション
			viewProjection_.TransferMatrix();
		} else {
			viewProjection_.UpdateMatrix();
		}
		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}
				worldTransformBlock->UpdateMatrix();
			}
		}
		// すべての当たり判定
		CheckAllCollisions();

		if (!player_->GetIsAlive() && clearParticle_ == nullptr && GetIsClear()) {
			// 死亡演出フェーズに切り替え
			phase_ = Phase::kDeath;
			if (player_->GetIsClear()) {
				phase_ = Phase::kClear;
				audio_->PlayWave(seClear_, false);
			}
			// 自キャラの座標を取得
			clearParticle_ = new ClearParticle;
			clearParticle_->Initialize(&viewProjection_, player_->GetWorldPosition());
			break;
		}
		if (!player_->GetIsAlive() && deathParticles_ == nullptr) {
			// 死亡演出フェーズに切り替え
			phase_ = Phase::kDeath;
			/*if (player_->GetIsClear()) {
				phase_ = Phase::kClear;
			}*/
			// 自キャラの座標を取得
			deathParticles_ = new DeathParticles;
			deathParticles_->Initialize(modelPlayer_, textureHandlePlayer_, &viewProjection_, player_->GetWorldPosition());
			break;
		}
		break;
		/////////////////////////////////////////////////////////////////////
		// クリア演出フェーズの処理
	case Phase::kClear:
		// 天球の更新
		skydome_->Update();
		player_->SetIsAlive(true);
		// 自キャラ更新
		player_->Update();
		for (GoalBlock* goalBlock : goalBlocks_) {
			goalBlock->Update();
		}
		// デスパーティクル
		if (clearParticle_ != nullptr) {
			clearParticle_->Update();
		}
		
		// カメラの更新
#ifdef _DEBUG
		if (input_->TriggerKey(DIK_P)) {
			isDebagCameraActive = !isDebagCameraActive;
			AxisIndicator::GetInstance()->SetVisible(isDebagCameraActive);
		}
#endif // DEBUG
		if (isDebagCameraActive) {
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
			// ビュープロジェクション
			viewProjection_.TransferMatrix();
		} else {
			viewProjection_.UpdateMatrix();
		}
		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}
				worldTransformBlock->UpdateMatrix();
			}
		}
		if (clearParticle_ && clearParticle_->IsFinished()) {
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}
		break;


	case Phase::kDeath:
		// 天球の更新
		skydome_->Update();

		for (GoalBlock* goalBlock : goalBlocks_) {
			goalBlock->Update();
		}
		// デスパーティクル
		if (deathParticles_ != nullptr) {
			deathParticles_->Update();
		}
		// カメラの更新
#ifdef _DEBUG
		if (input_->TriggerKey(DIK_P)) {
			isDebagCameraActive = !isDebagCameraActive;
			AxisIndicator::GetInstance()->SetVisible(isDebagCameraActive);
		}
#endif // DEBUG
		if (isDebagCameraActive) {
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
			// ビュープロジェクション
			viewProjection_.TransferMatrix();
		} else {
			viewProjection_.UpdateMatrix();
		}
		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}
				worldTransformBlock->UpdateMatrix();
			}
		}
		if (deathParticles_ && deathParticles_->IsFinished()) {
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}
		break;


	case Phase::kFadeOut:
		fade_->Update();
		finished_ = fade_->IsFinished();
		break;
	}
}
