// For use by Exelius Client Applications only.
#pragma once

// TODO: ALL OF THESE NEED TO BE REWORKED TO REMOVE AS MUCH AS POSSIBLE.
//		 THIS FILE SHOULD BE TOTALLY EMPTY IF POSSIBLE.
#include "source/precompilation/EXEPCH.h"
#include "source/engine/layers/Layer.h"
#include "source/engine/Application.h"
#include "source/engine/gameobjectsystem/GameObjectSystem.h"
#include "source/engine/gameobjectsystem/components/componenttypes/TransformComponent.h"
#include "source/engine/gameobjectsystem/components/componenttypes/SpriteComponent.h"
#include "source/engine/gameobjectsystem/components/componenttypes/UIComponent.h"
#include "source/engine/gameobjectsystem/components/componenttypes/AudioComponent.h"
#include "source/engine/gameobjectsystem/components/componenttypes/TilemapComponent.h"
#include "source/engine/gameobjectsystem/components/componenttypes/AnimationComponent.h"
#include "source/engine/gameobjectsystem/GameObject.h"
#include "source/utility/math/Rectangle.h"
#include "source/utility/generic/Time.h"

#include "source/render/Texture.h"
#include "source/render/VertexArray.h"
#include "source/render/Shader.h"
#include "source/render/Framebuffer.h"
#include "source/render/camera/OrthographicCamera.h"
#include "source/render/camera/OrthographicCameraController.h"
#include "source/render/Renderer2D.h"
#include "source/render/Renderer.h"