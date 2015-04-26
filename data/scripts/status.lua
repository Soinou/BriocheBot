--------------------------------------------------------------------
--
-- Commande !status (Alias de !brioche stream)
--
--------------------------------------------------------------------

-- Est une commande
ScriptType = "command";

-- La commande
CommandString = "status";

-- Nombre minium d'arguments
MinArguments = 0;

-- Nombre maximum d'arguments (Pas de maximum)
MaxArguments = -1;

-- Callback
function onCommand(senderNickname)

    -- On récupère le joueur qui a envoyé le message
    local player = Player.get(senderNickname);

    -- Si le joueur existe
    if player ~= nil then

        -- On le définit en tant que streameur
        server:setCurrentStreamer(player);

        -- On envoie un message à twitch
        server:sendTwitch("Streameur actuel: " .. player:getTwitchUsername());

    end

end
