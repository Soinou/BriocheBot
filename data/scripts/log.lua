--------------------------------------------------------------------
--
-- Chatlog
--
--------------------------------------------------------------------

-- Appelé en cas de messages
ScriptType = "message"

-- Callback
function onMessage(sender, message)

    -- On log juste le message
    Meow.info("chat", sender .. ": " .. message)

end
