#pragma once

#include "Layer.h"
#include "source/utility/generic/Macros.h"
#include "source/utility/containers/Vector2.h"

#include <EASTL/string.h>
#include <EASTL/vector.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    /*!
    \brief Layer groups are used to organize the layers of
    the map in a hierarchy. They can contain all other layer
    types including more layer groups to further nest layers.
    */
    class LayerGroup final : public Layer
    {
    public:

        LayerGroup(const eastl::string & workDir, const Vector2u & tileCount);
        ~LayerGroup() = default;
        LayerGroup(const LayerGroup&) = delete;
        const LayerGroup& operator = (const LayerGroup&) = delete;
        LayerGroup(LayerGroup&&) = default;
        LayerGroup& operator = (LayerGroup&&) = default;


        Type GetType() const override { return Layer::Type::Group; }
        void Parse(const pugi::xml_node&, TilemapResource*) override;

        /*!
        \brief Returns a reference to the vector containing the layer data.
        Layers are pointer-to-baseclass, the concrete type of which can be
        found via Layer::getType()
        \see Layer
        */
        const eastl::vector<Layer::Ptr>& GetLayers() const { return m_layers; }

    private:

        eastl::vector<Layer::Ptr> m_layers;

        eastl::string m_workingDir;
        Vector2u m_tileCount;
    };

    template <>
    inline LayerGroup& Layer::GetLayerAs<LayerGroup>()
    {
        EXE_ASSERT(GetType() == Type::Group);
        return *static_cast<LayerGroup*>(this);
    }
}