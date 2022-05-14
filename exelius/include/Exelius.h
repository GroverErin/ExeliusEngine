// For use by Exelius Client Applications only.
#pragma once

// TODO: ALL OF THESE NEED TO BE REWORKED TO REMOVE AS MUCH AS POSSIBLE.
//		 THIS FILE SHOULD BE TOTALLY EMPTY IF POSSIBLE.
#include "source/precompilation/EXEPCH.h"
#include "source/engine/layers/Layer.h"
#include "source/engine/Application.h"
#include "source/utility/math/Rectangle.h"
#include "source/utility/io/File.h"
#include "include/Time.h"

#include "source/resource/ResourceLoader.h"
#include "source/resource/ResourceHandle.h"

#include "source/engine/gameobjects/GameObject.h"
#include "source/engine/gameobjects/components/RigidbodyComponent.h"
#include "source/engine/gameobjects/components/BoxColliderComponent.h"
#include "source/engine/gameobjects/components/TransformComponent.h"
#include "source/engine/gameobjects/components/SpriteRendererComponent.h"
#include "source/engine/gameobjects/components/NameComponent.h"
#include "source/engine/gameobjects/components/GUIDComponent.h"
#include "source/engine/gameobjects/components/CameraComponent.h"
#include "source/engine/gameobjects/components/CircleColliderComponent.h"
#include "source/engine/gameobjects/components/CircleRendererComponent.h"
#include "source/engine/gameobjects/components/LuaScriptComponent.h"

#include "source/engine/resources/resourcetypes/TextureResource.h"
#include "source/engine/resources/resourcetypes/TextFileResource.h"

#include "source/engine/renderer/Renderer2D.h"

#include "source/engine/scenesystem/Scene.h"

#include "source/engine/physics/PhysicsSystem.h"
#include "source/engine/scripting/ScriptingSystem.h"

#include "source/os/events/MouseEvents.h"
#include "source/os/events/KeyEvents.h"

#include "source/render/Texture.h"
#include "source/render/VertexArray.h"
#include "source/render/Shader.h"
#include "source/render/Framebuffer.h"
#include "source/render/Renderer.h"