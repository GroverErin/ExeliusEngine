-- CameraController.lua

local CameraController =
{
    -- "Member" variables that can be changed and viewed in the editor.
    m_moveSpeed = 12;
}

-- Called when the runtime scene begins.
function CameraController:OnInitialize()
    -- print(self.gameObject:GetName());

    -- if (self.gameObject.CharacterController ~= nil) then
    --     print('We have a CharacterController!');
    --     print(self.gameObject.CharacterController);
    --     print(type(self.gameObject.CharacterController));
    -- else
    --     print('We do NOT have a CharacterController!');
    -- end

    -- if (self.gameObject.CameraController ~= nil) then
    --     print('We have a CameraController!');
    --     print(self.gameObject.CameraController);
    --     print(type(self.gameObject.CameraController));
    -- else
    --     print('We do NOT have a CameraController!');
    -- end
end

-- Called when the runtime scene is updated.
function CameraController:OnUpdate()
    -- Member Function Test
    self:MoveCamera();
end

-- Will Only be called by the OnUpdate function.
function CameraController:MoveCamera()

     local transform = self.gameObject:GetTransformComponent();
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

return CameraController;