--------------------------------------------------------------------
--
-- Commande !uptime
--
--------------------------------------------------------------------

-- Est une commande
ScriptType = "command"

-- La commande
CommandString = "uptime"

-- Nombre minimum d'arguments
MinArguments = 0

-- Nombre maximum d'arguments
MaxArguments = 0

function onCommand(sender)

    -- On récupère le temps de stream depuis le dernier changement
    local hours, minutes, seconds = Utils.getTime(server:upTime())

    -- On récupère le streamer actuel
    local streamer = server:getCurrentStreamer()

    -- On envoie le résultat à Twitch
    server:sendTwitch("Cela fait " .. hours .. " heure(s) " .. minutes .. " minute(s) " .. seconds .. " seconde(s) que je suis connecté!")

end
