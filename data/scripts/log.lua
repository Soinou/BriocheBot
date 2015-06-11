--------------------------------------------------------------------
--
-- Chatlog
--
--------------------------------------------------------------------

-- Appelé en cas de messages
ScriptType = "message"

-- Callback
function onMessage(sender, message)

    -- On récupère le viewer lié à l'envoyeur
    local viewer = Viewer.get(sender)

    -- Si il existe
    if viewer ~= nil then
        
        -- On augmente son nombre de messages envoyés
        viewer:setMessagesSent(viewer:getMessagesSent() + 1)

        -- On le mets à jour
        if not viewer:insert() then

            -- On log les éventuelles erreurs
            Meow.error("irc", "Error when trying to update the messages count of " .. sender)
        
        end

    end

    -- Dans tous les cas, on log juste le message
    Meow.info("chat", sender .. ": " .. message)

end
