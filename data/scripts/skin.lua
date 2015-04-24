--------------------------------------------------------------------
--
-- Commande !skin (Joueur)
--
--------------------------------------------------------------------

-- Est une commande
ScriptType = "command";

-- La commande
CommandString = "skin";

-- Nombre minium d'arguments
MinArguments = 0;

-- Nombre maximum d'arguments
MaxArguments = 1;

-- Callback
function onCommand(sender, twitchUsername)

    -- Variable temporaire pour le joueur
    local player = nil;

    -- Un argument
    if twitchUsername ~= nil then

        -- On récupère le joueur correspondant
        player = Player.get(twitchUsername);

    -- Aucun argument
    else

        -- On récupère le streamer
        player = server:getCurrentStreamer();

    end

    -- Pas de joueur
    if player == nil then

        -- Erreur
        server:sendTwitch("Joueur non trouvé!");

    -- Pas de skin
    elseif player:getOsuSkin() == "null" then

        -- Erreur
        server:sendTwitch("Le joueur " .. player:getTwitchUsername() .. " n'a pas défini de skin!");

    -- Tout est bon
    else

        -- On envoie le skin
        server:sendTwitch("Skin de " .. player:getTwitchUsername() .. ": " .. player:getOsuSkin());

    end

end
