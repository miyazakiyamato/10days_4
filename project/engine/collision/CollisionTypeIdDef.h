#pragma once
#include <cstdint>

namespace Engine {

/// <summary>
/// コリジョン種別ID定義
/// オブジェクトの種類を識別し、衝突判定のフィルタリング等に使用する
/// </summary>
enum class CollisionTypeIdDef : uint32_t {
	kDefault,   //!< デフォルト
	kFather,    //!< 親プレイヤー
	kChild,     //!< 子プレイヤー
	kEnemy,     //!< 敵キャラクター
	kTorch,     //!< 松明
	kTree,      //!< 木
	kOasis,     //!< オアシス
};

} // namespace Engine
