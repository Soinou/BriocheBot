--------------------------------------------------------------------
--
-- Commande !skin (Alias de !brioche skin)
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
function onCommand(senderNickname, twitchUsername)

    -- Variable temporaire pour le joueur
    local streamer = nil;

    -- Un argument
    if twitchUsername ~= nil then

        -- On récupère le joueur correspondant
        streamer = Viewer.get(twitchUsername)

    -- Aucun argument
    else

        -- On récupère le streamer actuel
        streamer = server:getCurrentStreamer()

    end

    -- Un pseudo twitch précisé mais le joueur n'est pas trouvé
    if twitchUsername ~= nil and streamer == nil then

        -- Erreur
        server:sendTwitch("Nope, " .. twitchUsername .. " existe pas. T'as du te tromper")

    -- Pas de pseudo twitch précisé mais le streamer n'existe pas non plus
    elseif streamer == nil then

        -- Pas de streamer
        server:sendTwitch("Y'a personne qui stream lààà. Attends un peu wesh")

    -- Pas de skin
    elseif streamer:getOsuSkinLink() == "null" then

        -- Erreur
        server:sendTwitch(streamer:getTwitchUsername() .. " il m'a pas filé de skin, donc je peux rien te donner")

    -- Tout est bon
    else

        -- On envoie le skin
        server:sendTwitch("Le skin de " .. streamer:getTwitchUsername() .. " c'est " .. streamer:getOsuSkinLink() .. ". De rien !")

    end

end
