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

    -- On récupère le joueur qui a envoyé le message
    local player = Player.get(sender)

    -- Si le joueur existe
    if player ~= nil then

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
