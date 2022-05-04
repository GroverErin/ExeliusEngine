-- TileMapGenerator.lua

local TileMapGenerator =
{
    -- "Member" variables that can be changed and viewed in the editor.
    m_tilePrefabPath = ResourceID.new("assets\\prefabs\\tile.exobj");
    m_resourceHandle = nil;
    m_random = nil;
    m_noise = nil;
    m_noiseHeightParams = { noiseDivisor = 2.0, inputRange = 2, octaves = 6, persistance = 0.5, seedOverride = 198491317, heightExponent = 0.1, maxHeight = 8000};
    m_noiseMoistureParams = { noiseDivisor = 2.0, inputRange = 2, octaves = 6, persistance = 0.5, seedOverride = 198491317, maxMoisture = 500.0};
    m_tempuratureParams = { tempuratureNormal = 0.0, equatorNormal = 1.0, poleNormal = 0.0, tempuratureFalloff = 0.7, maxTempC = 50, heightToTempDivisor = 1000, heightToTempRateOfChange = -6.5};
    m_tileMap = { width = 200, height = 200, tileWidth = 0.05, tileHeight = 0.05};
    m_transform = nil;

    m_biomeRanges = { tundra = -5, taiga = 0, temparate = 18, tropical = 30, duneMoisture = 250, savannahMoisture = 375, snowMoisture = 500, grasslandMoisture = 600, reef = 4500, ocean = 4200, mountain = 6200, landHeight = 6500};
    m_biomeColors = {
        error = Color.new(255, 0, 0),
        ocean = Color.new(81, 92, 129),
        reef = Color.new(79, 100, 146),
        glacier = Color.new(189, 198, 206),
        snow = Color.new(160, 183, 168),
        desert = Color.new(191, 176, 132),
        dunes = Color.new(163, 171, 123),
        cliff = Color.new(139, 123, 95),
        savanna = Color.new(161, 154, 110),
        grassland = Color.new(123, 165, 110),
        forest = Color.new(96, 127, 88),
        swamp = Color.new(123, 148, 129),
        rock = Color.new(125, 125, 125),
        blackScorch = Color.new(107, 113, 100)
    };
}

-- Called when the runtime scene begins.
function TileMapGenerator:OnInitialize()
    self.m_resourceHandle = ResourceHandle.new(self.m_tilePrefabPath);
    self.m_resourceHandle:LoadNow();
    self.m_random = Random.new(math.floor(1 * Time.ElapsedGameTime), math.floor(1 * Time.ElapsedGameTime)); -- Not the best. Maybe add a TimeSinceLastEpoch value?
    self.m_noiseHeightParams.seedOverride = self.m_random:Rand();
    self.m_noiseMoistureParams.seedOverride = self.m_random:Rand();
    self.m_noise = PerlinNoise.new();

    print("Generating Tilemap!");
    self:GenerateTileMap();
end

function TileMapGenerator:GenerateTileMap()
    for j = 0, self.m_tileMap.height - 1, 1 do
        for i = 0, self.m_tileMap.width - 1, 1 do
            local index = i + (j * self.m_tileMap.width);

            -- Create the tile
            self.m_tileMap[index] = InstantiateGameObject(self.m_tilePrefabPath:Get());

            -- Add the sprite renderer and set default color (Red for Error Tile).
            local spriteRenderer = self.m_tileMap[index]:GetSpriteRendererComponent();
            spriteRenderer.color.g = 0;
            spriteRenderer.color.b = 0;

            -- Set tile transform
            self.m_transform = self.gameObject:GetTransformComponent();
            local tileTransform = self.m_tileMap[index]:GetTransformComponent();
            tileTransform.translation.x = self.m_transform.translation.x + (i * self.m_tileMap.tileWidth);
            tileTransform.translation.y = self.m_transform.translation.y + (j * self.m_tileMap.tileHeight);
            tileTransform.scale.x = self.m_tileMap.tileWidth;
            tileTransform.scale.y = self.m_tileMap.tileHeight;

            local heightNoise = self:GetHeightNoise(i, j);
            local moistureNoise = self:GetMoistureNoise(i, j);
            local tempuratureNormal = self:GetTempuratureNormal(i, j);

            local heightValue = self:GetHeightValue(heightNoise);
            local tempuratureValue = self:GetTempuratureValue(tempuratureNormal, heightValue);
            local moistureValue = self:GetMoistureValue(moistureNoise, tempuratureValue, heightValue);

            spriteRenderer.color = self:CalculateTileBiome(heightValue, tempuratureValue, moistureValue);
        end
    end
end

function TileMapGenerator:GetHeightNoise(x, y)
    local noise = self.m_noise:GetAverageNoise(x, y, (self.m_tileMap.width / self.m_noiseHeightParams.noiseDivisor), (self.m_tileMap.height / self.m_noiseHeightParams.noiseDivisor), self.m_noiseHeightParams.inputRange, self.m_noiseHeightParams.octaves, self.m_noiseHeightParams.persistance, self.m_noiseHeightParams.seedOverride);
    local sinX = math.sin(math.pi * (x / self.m_tileMap.width));
    local sinY = math.sin(math.pi * (y / self.m_tileMap.height));
    local powNoise = (sinY * sinX) ^ self.m_noiseHeightParams.heightExponent;
    return (noise * powNoise);
end

function TileMapGenerator:GetMoistureNoise(x, y)
    return self.m_noise:GetAverageNoise(x, y, self.m_tileMap.width, self.m_tileMap.height, self.m_noiseMoistureParams.inputRange, self.m_noiseMoistureParams.octaves, self.m_noiseMoistureParams.persistance, self.m_noiseMoistureParams.seedOverride);
end

function TileMapGenerator:GetTempuratureNormal(x, y)
    return ((self.m_tempuratureParams.tempuratureNormal + (self.m_tempuratureParams.equatorNormal - self.m_tempuratureParams.poleNormal)) * (math.sin(math.pi * (y / self.m_tileMap.height)) ^ self.m_tempuratureParams.tempuratureFalloff));
end

function TileMapGenerator:GetHeightValue(heightNoise)
    return self.m_noiseHeightParams.maxHeight * heightNoise;
end

function TileMapGenerator:GetTempuratureValue(tempMapVal, heightValue)
    return (tempMapVal * self.m_tempuratureParams.maxTempC) + (heightValue / self.m_tempuratureParams.heightToTempDivisor) * self.m_tempuratureParams.heightToTempRateOfChange;
end

function TileMapGenerator:GetMoistureValue(moistureNoise, tempValue, heightValue)
    local moistureValue = 0;
    if (heightValue > self.m_biomeRanges.reef) then
        local lerpWeight = 1.0 - ((tempValue - self.m_biomeRanges.ocean) / self.m_biomeRanges.landHeight);
        local maxMoistureForHeight = ExeliusMath.Lerp(0.0, self.m_noiseMoistureParams.maxMoisture, lerpWeight);
        moistureValue = (1.0 - moistureNoise) * maxMoistureForHeight;
    end

    return moistureValue;
end

function TileMapGenerator:CalculateTileBiome(heightValue, tempuratureValue, moistureValue)
    if (heightValue <= self.m_biomeRanges.ocean) then
		return self.m_biomeColors.ocean;
    end

	if (heightValue <= self.m_biomeRanges.reef) then
		return self.m_biomeColors.reef;
    end

	if (heightValue >= self.m_biomeRanges.mountain) then
		return self.m_biomeColors.rock;
    end

	-- The tempurature range is under -5 C
	if (tempuratureValue <= self.m_biomeRanges.tundra) then
		return self.m_biomeColors.glacier;

	-- The tempurature range is under 0 C
	elseif (tempuratureValue <= self.m_biomeRanges.taiga) then
		if (moistureValue <= self.m_biomeRanges.snowMoisture) then
            return self.m_biomeColors.snow;
		else
            return self.m_biomeColors.grassland;
        end

	-- The tempurature range is under 18 C
	elseif (tempuratureValue <= self.m_biomeRanges.temparate) then
		if (moistureValue <= self.m_biomeRanges.savannahMoisture) then
            return self.m_biomeColors.savanna
		elseif (moistureValue <= self.m_biomeRanges.grasslandMoisture) then
            return self.m_biomeColors.grassland;
		else
            return self.m_biomeColors.swamp;
        end

	-- The tempurature range is under 30 C
	elseif (tempuratureValue <= self.m_biomeRanges.tropical) then
		if (moistureValue <= self.m_biomeRanges.duneMoisture) then
            return self.m_biomeColors.dunes;
		else
            return self.m_biomeColors.desert;
        end
    end
    return self.m_biomeColors.error;
end

return TileMapGenerator;