#include "TextureManager.h"
#include "DirectXCommon.h"
#include "SrvUavManager.h"
#include <algorithm>

namespace Engine {

std::unique_ptr<TextureManager> TextureManager::instance = nullptr;

TextureManager* TextureManager::GetInstance()
{
	if (instance == nullptr) {
		instance = std::make_unique<TextureManager>(PrivateToken{});
	}
	return instance.get();
}

void TextureManager::Initialize(DirectXCommon* dxCommon, SrvUavManager* srvUavManager){
	dxCommon_ = dxCommon;
	srvUavManager_ = srvUavManager;
}

void TextureManager::Finalize(){
	instance.reset();
}

void TextureManager::LoadTexture(const std::string& filePath){
	//読み込み済みテクスチャを検索
	if (textureData_.contains(filePath)) {
		//読み込み済みなら早期return
		return;
	}

	//テクスチャ枚数上限チェック
	assert(srvUavManager_->AvailabilityCheck());

	//Textureを読んで転送する
	DirectX::ScratchImage mipImages = dxCommon_->LoadTexture(directoryPath_ + filePath);

	//追加したテクスチャデータの参照を取得
	TextureData& textureData = textureData_[filePath];

	//テクスチャデータの書き込み
	textureData.metadata = mipImages.GetMetadata();
	textureData.resource = dxCommon_->CreateTextureResource(textureData.metadata);
	textureData.intermediateResource = dxCommon_->UploadTextureData(textureData.resource.Get(), mipImages);
	textureData.srvIndex = srvUavManager_->Allocate();
	textureData.srvHandleCPU = srvUavManager_->GetCPUDescriptorHandle(textureData.srvIndex);
	textureData.srvHandleGPU = srvUavManager_->GetGPUDescriptorHandle(textureData.srvIndex);

	srvUavManager_->CreateSRVforTexture2D(textureData.srvIndex, textureData.resource.Get(), textureData.metadata.format, textureData.metadata);
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(const std::string& filePath){
	//テクスチャ枚数上限チェック
	assert(srvUavManager_->AvailabilityCheck());

	TextureData& textureData = textureData_[filePath];
	return textureData.srvHandleGPU;
}

const DirectX::TexMetadata& TextureManager::GetMetaData(const std::string& filePath){
	//範囲外指定違反チェック
	assert(srvUavManager_->AvailabilityCheck());

	TextureData& textureData = textureData_[filePath];
	return textureData.metadata;
}

uint32_t TextureManager::GetSrvIndex(const std::string& filePath){
	//範囲外指定違反チェック
	assert(srvUavManager_->AvailabilityCheck());

	TextureData& textureData = textureData_[filePath];
	return textureData.srvIndex;
}

std::vector<std::string> TextureManager::GetKeys(){
	std::vector<std::string> keys;
	// std::transformを使用してキーを抽出
	std::transform(textureData_.begin(), textureData_.end(), std::back_inserter(keys),
		[](const auto& pair) { return pair.first; });
	return keys;
}

} // namespace Engine
