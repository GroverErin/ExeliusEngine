#pragma once

#include "Layer.h"
#include "source/utility/generic/Macros.h"
#include "source/utility/containers/Vector2.h"
#include "source/utility/generic/Color.h"

#include <EASTL/string.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    /*!
    \brief Image layers contain a single image which make up that
    layer. The parser contains the fully resolved path to the image
    relative to the working directory.
    */
    class ImageLayer final : public Layer
    {
    public:
        explicit ImageLayer(const eastl::string&);

        Type GetType() const override { return Layer::Type::Image; }
        void Parse(const pugi::xml_node&, TilemapResource*) override;

        /*!
        \brief Returns the path, relative to the working directory,
        of the image used by the image layer.
        */
        const eastl::string& GetImagePath() const { return m_filePath; }

        /*!
        \brief Returns the colour used by the image to represent transparent
        pixels. By default this is (0, 0, 0, 0)
        */
        const Color& GetTransparencyColor() const { return m_transparencyColor; }

        /*!
        \brief Returns true if the image used by this layer specifically states a
        colour to use as transparency
        */
        bool HasTransparency() const { return m_hasTransparency; }

        /*!
        \brief Returns the size of the image of the image layer in pixels.
        */
        const Vector2u& HetImageSize() const { return m_imageSize; }

    private:
        eastl::string m_workingDir;
        eastl::string m_filePath;
        Color m_transparencyColor;
        bool m_hasTransparency;
        Vector2u m_imageSize;
    };

    template <>
    inline ImageLayer& Layer::GetLayerAs<ImageLayer>()
    {
        EXE_ASSERT(GetType() == Type::Image);
        return *static_cast<ImageLayer*>(this);
    }
}