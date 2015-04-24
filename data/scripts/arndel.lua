--------------------------------------------------------------------
--
-- Commande !arndel [Joueur]
--
--------------------------------------------------------------------

-- Est une commande
ScriptType = "command";

-- La commande
CommandString = "arndel";

-- Nombre minium d'arguments
MinArguments = 1;

-- Nombre maximum d'arguments
MaxArguments = 1;

-- Callback
function onCommand(sender, twitchUsername)

    -- On récupère le joueur qui a envoyé le message
    local player = Player.get(sender);

    -- Si il existe et qu'il est admin
    if player ~= nil and player:getAdmin() then

        -- On récupère le joueur demandé
        local delete = Player.get(twitchUsername);

        -- Il n'existe pas
        if delete == nil then

            -- Erreur
            server:sendTwitch("Joueur " .. twitchUsername .. " non trouvé!");

        -- C'est l'envoyeur
        elseif player:is(twitchUsername) then

            -- Erreur
            server:sendTwitch("Vous ne pouvez pas vous supprimer vous-même!");

        -- Sinon
        else

            -- On le supprime
            Player.remove(twitchUsername);

             -- On envoie un message à twitch
            server:sendTwitch("Joueur " .. twitchUsername .. " supprimé!");

        end

    end

end
