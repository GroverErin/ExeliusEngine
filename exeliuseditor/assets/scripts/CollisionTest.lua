-- CollisionTest.lua

local CollisionTest =
{
    -- "Member" variables that can be changed and viewed in the editor.
    m_spawnCooldown = 1.0;
    m_spawnCooldownOriginal = 1.0;
}

local function Boink()
    print("Boink");
end

-- Called if this GameObject has a Collider Component and
-- contacts another GameObject with a Collider Component.
function CollisionTest:OnCollisionEnter(collisionData)
    local gameObjectA = collisionData.thisGameObject;
    local gameObjectB = collisionData.otherGameObject;

    --print(self.gameObject:GetName() .. " received an OnCollision call!");
    --print(gameObjectA:GetName() .. ' Collided with: ');
    --print('\t' .. gameObjectB:GetName());

    local otherScript = gameObjectB:GetLuaScriptComponent();
    local possiblyBadComponent = gameObjectB:GetSpriteRendererComponent();
    if (possiblyBadComponent ~= nil) then
        -- print('The other object has a SpriteRendererComponent!');
    end

    if (otherScript ~= nil) then
        -- otherScript:Bonk();
        -- otherScript:Boink(); Error because its a local function.
    end
end

function CollisionTest:Bonk()
    --print(self.gameObject:GetName() .. " was Bonked.");
end

return CollisionTest;