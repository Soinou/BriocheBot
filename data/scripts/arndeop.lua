--------------------------------------------------------------------
--
-- Commande !arndeop [Joueur]
--
--------------------------------------------------------------------

-- Est une commande
ScriptType = "command";

-- La commande
CommandString = "arndeop";

-- Nombre minium d'arguments
MinArguments = 1;

-- Nombre maximum d'arguments
MaxArguments = 1;

-- Callback
function onCommand(sender, twitchUsername)

    -- On récupère le joueur qui a envoyé le message
    local player = Player.get(sender);

    -- Si il existe et qu'il est admin
    if player ~=nil and player:getAdmin() then

        -- On récupère le joueur demandé
        local op = Player.get(twitchUsername);

        -- Si il n'existe pas
        if op == nil then

            -- Non trouvé
            server:sendTwitch("Le joueur " .. twitchUsername .. " n'a pas été trouvé!");

        -- Sinon si le joueur est l'envoyeur
        elseif player:is(twitchUsername) then

            -- Pas possible
            server:sendTwitch("Vous ne pouvez pas vous déop vous même!");

        -- Sinon
        else

            -- On lui enlève son statut d'admin
            op:setAdmin(false);

            -- On envoie un message à twitch
            server:sendTwitch("Le joueur " .. op:getTwitchUsername() .. " n'est plus admin!");

        end

    end

end
