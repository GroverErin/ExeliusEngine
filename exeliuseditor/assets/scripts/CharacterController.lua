-- CharacterController.lua
g_characterStatManager = require("CharacterStatManager.lua") -- A script that is not directly attached to a GameObject in the scene, but exists in the "Assets" directory.

local CharacterController = 
{
    -- "Member" variables that can be changed and viewed in the editor.
    m_moveSpeed = 12;
    m_wallHitThreshold = 5;
}

-- Called when the runtime scene begins.
function CharacterController:OnInitialize(self)
    Log:Log("You can now move the player!");
end

-- Called when the runtime scene is updated.
function CharacterController:OnUpdate(self)
    self:MovePlayer(self);
end

-- Called when the GameObject that owns this script instance is destroyed.
function CharacterController:OnDestroy(self)
    Log:Warn("You shouldn't be able to move the player now!");
end

-- Called if this GameObject has a Collider Component and
-- contacts another GameObject with a Collider Component.
function CharacterController:OnCollisionEnter(self, collisionData)
    local gameObjectA = collisionData.GetObjectA();
    local gameObjectB = collisionData.GetObjectB();

    if(gameObjectA.name == "Wall" or gameObjectB.name == "Wall") then
        Log:Info("Player has hit a wall.");
        g_characterStatManager.m_wallsHit += 1;
        self:StaggerPlayer(self, collisionData.collisionNormal);
    end

    if(g_characterStatManager.m_wallsHit >= self.m_wallHitThreshold) then
        self.gameObject:Destroy();
    end
end

-- Will Only be called by the OnUpdate function.
local function MovePlayer(self)
    local transform = self.gameObject:getTransform();

    if(Input:IsKeyDown(KeyCode.Up)) then
        transform.translation.y += self.m_moveSpeed * Time.deltaTime;
    elseif(Input:IsKeyDown(KeyCode.Down))
        transform.translation.y -= self.m_moveSpeed * Time.deltaTime;
    end

    if(Input:IsKeyDown(KeyCode.Left)) then
        transform.translation.x -= m_moveSpeed * Time.deltaTime;
    elseif(Input:IsKeyDown(KeyCode.Right))
        transform.translation.x += m_moveSpeed * Time.deltaTime;
    end
end

-- Will only be called when a player collides with a "Wall"
local function StaggerPlayer(self)
    local rigidBody = self.gameObject:getRigidBody();
end

return CharacterController;

-- Notes:
--      - self.gameObject is not defined anywhere, but it refers to the GameObject that owns this script instance. It's defined by the engine.
--      - g_characterStatManager acts as a singleton here, though it is meant to display the intention of being able to call non-instanced script functions.
--      - Log acts as a global "class", same as Time and Input.
--      - getTransform() is representative of how all components might be retrieved. This function might be generated automatically via lua bindings?
--      - OnCollisionExit is not defined, so it will not get called, and no crash should occur.