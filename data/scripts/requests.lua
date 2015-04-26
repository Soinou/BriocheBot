--------------------------------------------------------------------
--
-- Gestion des requests
--
--------------------------------------------------------------------

-- Appelé en cas de messages
ScriptType = "message"

-- Callback
function onMessage(sender, message)

    -- On tente de parser le message
    local request = Request.parse(sender, message)

    -- Si le message est une requête
    if request ~= nil then

        -- On envoie la request sur twitch et osu!
        server:sendTwitch(request:toTwitchString())
        server:sendOsu(request:toOsuString())

    end

end
