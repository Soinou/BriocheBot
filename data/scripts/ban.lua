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

function onCommand(sender, twitchUsername)

    -- On récupère le viewer
    local viewer = Viewer.get(sender)

    -- Si le viewer existe
    if viewer ~= nil then

        -- C'est un streamer ou un modo
        if viewer:getType() == kStreamer or viewer:getType() == kModerator then

            -- Ok
            server:sendTwitch("/ban " .. twitchUsername)

        -- C'est un viewer
        else

            -- Ahah nope
            server:sendTwitch("Y'a que les streamers et les modos qui peuvent ban. Retente ta chance!")

        end

    end

end
