#include "PipelineFactory.h"
#include "ModelPipeline.h"
#include "ModelComputePipeline.h"
#include "SkyboxPipeline.h"
#include "SpritePipeline.h"
#include "ParticlePipeline.h"
#include "InitializeParticlePipeline.h"
#include "UpdateParticlePipeline.h"
#include "EmitParticlePipeline.h"
#include "Line3DPipeline.h"
#include "OffScreenPipeline.h"

namespace Engine {

std::unique_ptr<BasePipeline> PipelineFactory::ChangePipeline(const std::string& pipelineName) {
	if (pipelineName == "Object3d") {
		return std::make_unique<ModelPipeline>();
	}if (pipelineName == "SkinningObject3d") {
		return std::make_unique<ModelComputePipeline>();
	} else if (pipelineName == "Skybox") {
		return std::make_unique<SkyboxPipeline>();
	}else if (pipelineName == "Sprite") {
		return std::make_unique<SpritePipeline>();
	} else if (pipelineName == "Particle") {
		return std::make_unique<ParticlePipeline>();
	} else if (pipelineName == "InitializeParticle") {
		return std::make_unique<InitializeParticlePipeline>();
	}else if (pipelineName == "UpdateParticle") {
		return std::make_unique<UpdateParticlePipeline>();
	} else if (pipelineName == "EmitParticle") {
		return std::make_unique<EmitParticlePipeline>();
	} else if (pipelineName == "Line3D") {
		return std::make_unique<Line3DPipeline>();
	} else if (pipelineName == "OffScreen/Fullscreen"||
		pipelineName == "OffScreen/Grayscale" ||
		pipelineName == "OffScreen/Vignette" ||
		pipelineName == "OffScreen/BoxFilter" ||
		pipelineName == "OffScreen/GaussianFilter" ||
		pipelineName == "OffScreen/LuminanceBasedOutline" ||
		pipelineName == "OffScreen/DepthBasedOutline" ||
		pipelineName == "OffScreen/RadialBlur"||
		pipelineName == "OffScreen/Dissolve" ||
		pipelineName == "OffScreen/Random") {
		return std::make_unique<OffScreenPipeline>();
	}
	assert(0);
	return nullptr;
}

} // namespace Engine
