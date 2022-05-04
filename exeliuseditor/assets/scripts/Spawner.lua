-- Spawner.lua
-- local g_messageSystem = require("assets/scripts/MessageSystem");

local Spawner =
{
    -- "Member" variables that can be changed and viewed in the editor.
    m_spawnCooldown = 0.1;
    m_spawnCooldownOriginal = 0.1;
    m_gameObjectPrefabPath = ResourceID.new("assets\\prefabs\\smallball.exobj");
    m_resourceHandle = nil;
}

function Spawner:OnInitialize()
    self.m_resourceHandle = ResourceHandle.new(self.m_gameObjectPrefabPath);
    self.m_resourceHandle:LoadNow();
end

function Spawner:OnUpdate()
    self.m_spawnCooldown = self.m_spawnCooldown - Time.DeltaTime;
    if (self.m_spawnCooldown < 0) then
        local obj = InstantiateGameObject(self.m_gameObjectPrefabPath:Get());
        local transform = self.gameObject:GetTransformComponent();
        local rigidbody = obj:GetRigidbodyComponent();

        local vec2 = Vec2.new();
        vec2.x = transform.translation.x;
        vec2.y = transform.translation.y;
        rigidbody:SetPosition(vec2);
        self.m_spawnCooldown = self.m_spawnCooldownOriginal;
    end
end

return Spawner;