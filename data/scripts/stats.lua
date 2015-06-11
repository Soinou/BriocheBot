--------------------------------------------------------------------
--
-- Commande !stats <Pseudo Twitch>
--
--------------------------------------------------------------------

-- Est une commande
ScriptType = "command"

-- La commande
CommandString = "stats"

-- Nombre minimum d'arguments
MinArguments = 0

-- Nombre maximum d'arguments
MaxArguments = 1

-- Callback
function onCommand(sender, username)

    -- Pas de viewer pour l'instant
    local viewer = nil

    -- On a pas de pseudo précisé
    if username == nil then

        -- On récupère le viewer lié à l'envoyeur
        viewer = Viewer.get(sender)

    -- On a un pseudo
    else
        
        -- On récupère le viewer lié à ce pseudo
        viewer = Viewer.get(username)
    
    end

    -- Y'en a pas
    if viewer == nil then
        
        -- On a pas de pseudo
        if username == nil then

            -- Erreur
            server:sendTwitch("Désolé " .. sender .. " mais t'existe pas encore dans ma base de données. Réessaie un peu plus tard !")

        -- On a un pseudo
        else

            -- Erreur
            server:sendTwitch("Désolé mais " .. username .. " existe pas encore pour moi. Faudra réessayer plus tard")
        
        end

    -- Sinon, c'est un viewer
    elseif viewer:getType() == kViewer then

        -- On envoie les infos à twitch
        server:sendTwitch(viewer:getTwitchUsername() .. ". Temps passé connecté: " .. viewer:getOnlineTime() .. " secondes. Nombre de messages: " .. viewer:getMessagesSent())
    
    -- Sinon, c'est un streamer
    elseif viewer:getType() == kStreamer then

        -- On envoie les infos à twitch
        server:sendTwitch(viewer:getTwitchUsername() .. ". Temps passé connecté: " .. viewer:getOnlineTime() .. " secondes. Nombre de messages: " .. viewer:getMessagesSent() .. ". Temps de stream total: " .. viewer:getStreamTime() .. " secondes")

    -- Sinon, c'est un moderateur
    elseif viewer:getType() == kModerator then

        -- On envoie les infos à twitch
        server:sendTwitch(viewer:getTwitchUsername() .. ". Temps passé connecté: " .. viewer:getOnlineTime() .. ". Nombre de messages: " .. viewer:getMessagesSent() .. ". Temps de stream total: " .. viewer:getStreamTime() .. " secondes")
    
    end

end
