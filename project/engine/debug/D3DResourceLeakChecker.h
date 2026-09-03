#pragma once

namespace Engine {

/// <summary>
/// Direct3Dリソースリークチェッカー
/// プログラム終了時に解放されていないD3Dリソースがあれば警告を出力する
/// 基本的にデバッグビルド時のみ動作させる
/// </summary>
class D3DResourceLeakChecker {
public:
	/// <summary>
	/// デストラクタ
	/// インスタンス破棄時にリークチェックを実行する
	/// </summary>
	~D3DResourceLeakChecker();
};

} // namespace Engine
