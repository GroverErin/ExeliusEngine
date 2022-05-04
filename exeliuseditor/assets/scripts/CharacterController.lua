-- CharacterController.lua
g_characterStatManager = require("assets/scripts/CharacterStatManager") -- A script that is not directly attached to a GameObject in the scene, but exists in the "Assets" directory.

local CharacterController =
{
    -- "Member" variables that can be changed and viewed in the editor.
    m_moveSpeed = 12;
    m_wallHitThreshold = 7;
    m_createdObject = nil;
    m_random = nil;
}

-- Called when the runtime scene begins.
function CharacterController:OnInitialize()
    print("You can now move the player!");
    print(string.format("The players move speed is: %s", self.m_moveSpeed));

    --print(self.gameObject:GetName());

    for i,v in pairs(self) do print(i); end

    self.m_random = Random.new(55);

    if (self.gameObject.CharacterController ~= nil) then
        print('We have a CharacterController!');
        print(self.gameObject.CharacterController);
        print(type(self.gameObject.CharacterController));
    else
        print('We do NOT have a CharacterController!');
    end

    if (self.gameObject.CameraController ~= nil) then
        print('We have a CameraController!');
        print(self.gameObject.CameraController);
        print(type(self.gameObject.CameraController));
    else
        print('We do NOT have a CameraController!');
    end

    -- Adding Components to self works too! Though, this one isn't used in this test.
    --self.gameObject:AddCameraComponent();
end

-- Will only be called when a player collides with a "Wall"
local function StaggerPlayer()
    print('Note the Lua "print" function has been overwritten, and is the same as Log.Trace!');
    Log.Trace('Player has been staggered in white!');
    Log.Info('Player has been staggered in green!');
    Log.Warn('Player has been staggered in yellow!');
    Log.Error('Player has been staggered in red with pi concatenated!' .. 3.1415);
    Log.Fatal('Player has been staggered in big red with other arguments: ', 2, 4, 6, 8, "Who do we appreciate?", "LOGS!");
    warn('Lua "warn" is also overwritten, and is the same as Log.Warn');
end

-- Called when the runtime scene is updated.
function CharacterController:OnUpdate()
    --print(Time.DeltaTime);

    -- Slows down time.
    if (Input.IsKeyReleased(KeyCode.A)) then
        Time.SetTimeScale(0.5);
    end

    -- GameObject Creation Test
    if (Input.IsKeyReleased(KeyCode.Q)) then
        self.m_createdObject = CreateGameObject('Instantiated Square');
        local sr = self.m_createdObject:AddSpriteRendererComponent();
        sr.textureResource:SetResourceID("assets/textures/image1.png");
        sr.textureResource:LoadNow();
        --local rb = self.m_createdObject:AddRigidbodyComponent();
        --print(rb.bodyType)
        print(self.m_createdObject:HasTransformComponent());
        local transform = self.m_createdObject:GetTransformComponent();

        transform.translation.x = 3.0;

        sr.color.r = 0;

        print(self.m_createdObject:GetName());
        print(self.m_createdObject:GetGUID());
    end

    if (Input.IsKeyReleased(KeyCode.R)) then
        print(self.m_random:Rand());
        print(self.m_random:FRand());
        print(self.m_random:FRandomRange(3.0, 9.0));
        print(self.m_random:IRandomRange(0, 100));
    end

    -- GameObject Deletion Test
    if (Input.IsKeyReleased(KeyCode.D)) then
        if (self.m_createdObject) then
            self.m_createdObject:Destroy();
            -- These also work.
            --DestroyGameObject(self.m_createdObject);
            --self.m_createdObject.Destroy(self.m_createdObject);

            self.m_createdObject = nil; -- TODO: Make this not required.
        end
    end

    -- Member Function Test
    self:MovePlayer();
end

-- Called when the GameObject that owns this script instance is destroyed.
function CharacterController:OnDestroy()
    if (self.m_createdObject ~= nil) then
        local sr = self.m_createdObject:GetSpriteRendererComponent();
        self.m_createdObject:Destroy();
        self.m_createdObject = nil;
    end
    print("You shouldn't be able to move the player now!");
end

-- Called if this GameObject has a Collider Component and
-- contacts another GameObject with a Collider Component.
function CharacterController:OnCollisionEnter(collisionData)
    local gameObjectA = collisionData.thisGameObject;
    local gameObjectB = collisionData.otherGameObject;

    print(gameObjectA:GetName() .. 'Collided with: ');
    print('\t' .. gameObjectB:GetName());

    if(gameObjectB:GetName() == "Floor") then
        g_characterStatManager.m_wallsHit = g_characterStatManager.m_wallsHit + 1;
        print("Player has hit the floor " .. g_characterStatManager.m_wallsHit .. " times.");
    end

    if(g_characterStatManager.m_wallsHit >= self.m_wallHitThreshold) then
        if (self.m_createdObject ~= nil) then
            self.m_createdObject:Destroy();
            self.m_createdObject = nil;
        end
        -- Local Function Test
        StaggerPlayer();

        self.gameObject:Destroy();
    end
end

-- Will Only be called by the OnUpdate function.
function CharacterController:MovePlayer()
    if (self.m_createdObject) then

        -- Move Transform Component Test
        local transform = self.m_createdObject:GetTransformComponent();
        if(Input.IsKeyDown(KeyCode.Up)) then
            transform.translation.y = transform.translation.y + (self.m_moveSpeed * Time.DeltaTime);
        elseif(Input.IsKeyDown(KeyCode.Down)) then
            transform.translation.y = transform.translation.y - (self.m_moveSpeed * Time.DeltaTime);
        end

        if(Input.IsKeyDown(KeyCode.Left)) then
            transform.translation.x = transform.translation.x - (self.m_moveSpeed * Time.DeltaTime);
        elseif(Input.IsKeyDown(KeyCode.Right)) then
            transform.translation.x = transform.translation.x + (self.m_moveSpeed * Time.DeltaTime);
        end
    end
end

return CharacterController;

-- Notes:
--      - self.gameObject is not defined anywhere, but it refers to the GameObject that owns this script instance. It's defined by the engine.
--      - g_characterStatManager acts as a singleton here, though it is meant to display the intention of being able to call non-instanced script functions.
--      - Time acts as a global "class", same as Input and Log.
--      - OnCollisionExit is not defined, so it will not get called.