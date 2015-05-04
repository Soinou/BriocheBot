--------------------------------------------------------------------
--
-- Commande !time
--
--------------------------------------------------------------------

-- Est une commande
ScriptType = "command"

-- La commande
CommandString = "time"

-- Nombre minimum d'arguments
MinArguments = 0

-- Nombre maximum d'arguments
MaxArguments = 0

function onCommand(sender)

    -- On récupère le streamer actuel
    local streamer = server:getCurrentStreamer()

    -- Si le streamer n'existe pas
    if streamer == nil then

        -- Erreur
        server:sendTwitch("Aucun streamer actuellement")

    -- Sinon
    else

        -- On récupère le temps de stream depuis le dernier changement
        local hours, minutes, seconds = Utils.getTime(server:streamTime())

        -- On envoie le résultat à Twitch
        server:sendTwitch("Cela fait " .. hours .. " heure(s) " .. minutes .. " minute(s) " .. seconds .. " seconde(s) que " .. streamer:getTwitchUsername() .. " stream")

    end

end
