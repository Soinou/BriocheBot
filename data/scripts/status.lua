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
function onCommand(sender)

    -- On récupère le streamer qui a envoyé le message
    local streamer = Viewer.get(sender);

    -- Si le streamer existe et n'est pas un viewer
    if streamer ~= nil and streamer:getType() ~= kViewer then

        -- On le définit en tant que streameur
        server:setCurrentStreamer(streamer);

        -- On envoie un message à twitch
        server:sendTwitch("Nouveau streamer: " .. streamer:getTwitchUsername() .. ". Bon courage !");

    end

end
