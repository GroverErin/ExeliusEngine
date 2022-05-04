-- MessageSystem.lua

local MessageSystem =
{
    m_callbacks = {};
}

function MessageSystem:OnUpdate()
    for _,v in ipairs(self.m_callbacks) do v() end
end

function MessageSystem:RegisterForMessage(callback)
    table.insert(self.m_callbacks, callback);
end

return MessageSystem;