--------------------------------------------------------------------
--
-- Commande !arnop [Joueur]
--
--------------------------------------------------------------------

-- Est une commande
ScriptType = "command";

-- La commande
CommandString = "arnop";

-- Nombre minium d'arguments
MinArguments = 1;

-- Nombre maximum d'arguments
MaxArguments = 1;

-- Callback
function onCommand(sender, username)

    -- On récupère le joueur qui a envoyé le message
    local player = Player.get(sender);

    -- Si il existe et qu'il est admin
    if player ~=nil and player:getAdmin() then

        -- On récupère le joueur demandé
        local op = Player.get(username);

        -- Si il n'existe pas
        if op == nil then

            -- Non trouvé
            server:sendTwitch("Le joueur " .. username .. " n'a pas été trouvé!");

        -- Sinon si le joueur est déjà admin
        elseif op:getAdmin() then

            -- On envoie un message à twitch
            server:sendTwitch("Le joueur " .. op:getTwitchUsername() .. " est déjà admin!");

        -- Sinon
        else

            -- On le passe admin
            op:setAdmin(true);

            -- On envoie un message à twitch
            server:sendTwitch("Le joueur " .. op:getTwitchUsername() .. " est maintenant admin!");

        end

    end

end
