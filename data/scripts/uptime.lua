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

    -- On envoie le résultat à Twitch
    server:sendTwitch("ça fait " .. hours .. " heure(s) " .. minutes .. " minute(s) " .. seconds .. " seconde(s) que je marche ! Espérons que je tienne encore longtemps Kappa")

end
