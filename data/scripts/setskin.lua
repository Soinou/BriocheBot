--------------------------------------------------------------------
--
-- Commande !setskin [Lien du skin]
--
--------------------------------------------------------------------

-- Est une commande
ScriptType = "command";

-- La commande
CommandString = "setskin";

-- Nombre minium d'arguments
MinArguments = 1;

-- Nombre maximum d'arguments
MaxArguments = 1;

-- Callback
function onCommand(sender, osuSkin)

    -- On récupère le joueur qui a envoyé la commande
    local player = Player.get(sender);

    -- Si le joueur existe
    if player ~= nil then

        -- On change son skin
        player:setOsuSkin(osuSkin);

        -- Et on indique la mise à jour
        server:sendTwitch("Skin de " .. player:getTwitchUsername() .. " mis à jour!");

    end

end
