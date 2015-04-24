--------------------------------------------------------------------
--
-- Commande !arnadd <Pseudo Twitch> <Pseudo osu!>
--
--------------------------------------------------------------------

-- Est une commande
ScriptType = "command";

-- La commande
CommandString = "arnadd";

-- Nombre minium d'arguments
MinArguments = 2;

-- Nombre maximum d'arguments
MaxArguments = 2;

-- Callback
function onCommand(sender, twitchUsername, osuUsername)

    -- On récupère le joueur qui a envoyé le message
    local player = Player.get(sender);

    -- Si le joueur existe et est admin
    if player ~= nil and player:getAdmin() then

        -- On tente de récupére le joueur qu'on veut ajouter
        local add = Player.get(twitchUsername);

        -- Si il existe
        if add ~= nil then

            -- Erreur
            server:sendTwitch("Le joueur " .. twitchUsername .. " existe déjà!");

        -- Sinon
        else

            -- On ajoute le joueur demandé
            Player.add(twitchUsername, osuUsername);

            -- On confirme à twitch
            server:sendTwitch("Joueur " .. twitchUsername .. " / " .. osuUsername .. " ajouté!");

        end

    end

end
