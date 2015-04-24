--------------------------------------------------------------------
--
-- Commande !status
--
--------------------------------------------------------------------

-- Est une commande
ScriptType = "command";

-- La commande
CommandString = "status";

-- Nombre minium d'arguments
MinArguments = 0;

-- Nombre maximum d'arguments
MaxArguments = 0;

-- Callback
function onCommand(sender)

    -- On récupère le joueur qui a envoyé le message
    local player = Player.get(sender);

    -- Si le joueur existe
    if player ~= nil then

        -- On le définit en tant que streameur
        server:setCurrentStreamer(player);

        -- On envoie un message à twitch
        server:sendTwitch("Streameur actuel: " .. player:getTwitchUsername());

    end

end
