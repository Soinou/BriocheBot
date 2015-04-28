--------------------------------------------------------------------
--
-- Commande !ban <Pseudo>
--
--------------------------------------------------------------------

-- Est une commande
ScriptType = "command"

-- La commande
CommandString = "ban"

-- Nombre minimum d'arguments
MinArguments = 1

-- Nombre maximum d'arguments
MaxArguments = 1

function onCommand(sender, username)

    -- On récupère le joueur
    local player = Player.get(sender)

    -- Si le joueur existe
    if player ~= nil then

        -- On envoie la commande
        server:sendTwitch("/ban " .. username)

    end

end
