--------------------------------------------------------------------
--
-- Commande !arnrestart
--
--------------------------------------------------------------------

-- Est une commande
ScriptType = "command";

-- La commande
CommandString = "arnrestart";

-- Nombre minium d'arguments
MinArguments = 0;

-- Nombre maximum d'arguments
MaxArguments = 0;

-- Callback
function onCommand(sender)

    -- On récupère le joueur qui a envoyé le message
    local player = Player.get(sender);

    -- Si le joueur existe et est admin
    if player ~= nil and player:getAdmin() then

        -- On stoppe le serveur (Il sera relancé automatiquement)
        server:stop();

    end

end
