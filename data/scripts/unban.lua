--------------------------------------------------------------------
--
-- Commande !unban <Pseudo>
--
--------------------------------------------------------------------

-- Est une commande
ScriptType = "command"

-- La commande
CommandString = "unban"

-- Nombre minimum d'arguments
MinArguments = 1

-- Nombre maximum d'arguments
MaxArguments = 1

function onCommand(sender, username)

    -- On récupère le streamer qui a envoyé la commande
    local streamer = Viewer.get(sender)

    -- Si le streamer existe
    if streamer ~= nil and streamer:getType() ~= kViewer then

        -- On envoie la commande
        server:sendTwitch("/unban " .. username)

    end

end
