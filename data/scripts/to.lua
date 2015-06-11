--------------------------------------------------------------------
--
-- Commande !to <Pseudo> <Temps>
--
--------------------------------------------------------------------

-- Est une commande
ScriptType = "command"

-- La commande
CommandString = "to"

-- Nombre minimum d'arguments
MinArguments = 1

-- Nombre maximum d'arguments
MaxArguments = 2

-- Callback
function onCommand(sender, username, time)

    -- On récupère le streamer qui a envoyé le message
    local streamer = Viewer.get(sender)

    -- Si le joueur existe et n'est pas un viewer
    if streamer ~= nil and streamer:getType() ~= kViewer then

        -- Si le temps n'est pas précisé
        if time == nil then

            -- On timeout sans temps
            server:sendTwitch("/timeout " .. username)

        -- Sinon
        else

            -- On timeout avec le temps
            server:sendTwitch("/timeout " .. username .. " " .. time)

        end

    end

end
