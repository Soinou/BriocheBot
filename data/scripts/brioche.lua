--------------------------------------------------------------------
--
-- Commande !brioche <commande> <arguments> <Pseudo Twitch> <Pseudo osu!>
--
--------------------------------------------------------------------

-- Est une commande
ScriptType = "command"

-- La commande
CommandString = "brioche"

-- Nombre minimum d'arguments
MinArguments = 1

-- Nombre maximum d'arguments
MaxArguments = 4

--------------------------------------------------------------------
--
-- Commande !brioche add <Pseudo Twitch> <Pseudo osu!>
--
-- senderNickname: Le pseudo de l'auteur de la commande
-- senderPlayer: L'objet Player associé à cet auteur (Ou nil)
-- twitchUsername: Le pseudo twitch du joueur à ajouter
-- osuUsername: Le pseudo osu! du joueur à ajouter
--
--------------------------------------------------------------------

local function briocheAdd(senderNickname, senderPlayer, twitchUsername, osuUsername)

    -- On tente de récupére le joueur qu'on veut ajouter
    local player = Player.get(twitchUsername)

    -- Si il existe
    if player ~= nil then

        -- Erreur
        server:sendTwitch("Le joueur " .. twitchUsername .. " existe déjà!")

    -- Sinon si on a les deux pseudos remplis
    elseif twitchUsername ~= nil and osuUsername ~= nil then

        -- On ajoute le joueur demandé
        Player.add(twitchUsername, osuUsername)

        -- On confirme à twitch
        server:sendTwitch("Joueur " .. twitchUsername .. " / " .. osuUsername .. " ajouté!")

    end

end

--------------------------------------------------------------------
--
-- Commande !brioche edit <Pseudo Twitch> <Pseudo Twitch> <Pseudo osu!>
--
-- senderNickname: Le pseudo de l'auteur de la commande
-- senderPlayer: L'objet Player associé à cet auteur (Ou nil)
-- twitchUsername: Le pseudo twitch du joueur à supprimer
-- newTwitchUsername: Le nouveau pseudo twitch du joueur
-- newOsuUsername: Le nouveau pseudo osu! du joueur
--
--------------------------------------------------------------------

local function briocheEdit(senderNickname, senderPlayer, twitchUsername, newTwitchUsername, newOsuUsername)

    -- On tente de récupére le joueur qu'on veut modifier
    local player = Player.get(twitchUsername)

    -- Si il n'existe pas
    if player == nil then

        -- Erreur
        server:sendTwitch("Le joueur " .. twitchUsername .. " n'existe pas!")

    -- Sinon si on a les deux pseudos remplis
    elseif newTwitchUsername ~= nil and newOsuUsername ~= nil then

        -- On modifie le joueur demandé
        player:edit(newTwitchUsername, newOsuUsername)

        -- On confirme à twitch
        server:sendTwitch("Joueur " .. twitchUsername .. " modifié en " .. newTwitchUsername .. " / " .. newOsuUsername)

    end

end

--------------------------------------------------------------------
--
-- Commande !brioche del <Pseudo Twitch>
--
-- senderNickname: Le pseudo de l'auteur de la commande
-- senderPlayer: L'objet Player associé à cet auteur (Ou nil)
-- twitchUsername: Le pseudo twitch du joueur à supprimer
--
--------------------------------------------------------------------

function briocheDel(senderNickname, senderPlayer, twitchUsername)

    -- On récupère le joueur demandé
    local player = Player.get(twitchUsername)

    -- Il n'existe pas
    if player == nil then

        -- Erreur
        server:sendTwitch("Joueur " .. twitchUsername .. " non trouvé!")

    -- C'est l'envoyeur
    elseif senderPlayer:is(twitchUsername) then

        -- Erreur
        server:sendTwitch("Vous ne pouvez pas vous supprimer vous-même!")

    -- Sinon
    else

        -- On le supprime
        Player.remove(twitchUsername)

         -- On envoie un message à twitch
        server:sendTwitch("Joueur " .. twitchUsername .. " supprimé!")

    end

end

--------------------------------------------------------------------
--
-- Commande !brioche op <Pseudo Twitch>
--
-- senderNickname: Le pseudo de l'auteur de la commande
-- senderPlayer: L'objet Player associé à cet auteur (Ou nil)
-- twitchUsername: Le pseudo twitch du joueur à op
--
--------------------------------------------------------------------

function briocheOp(senderNickname, senderPlayer, username)

    -- On récupère le joueur demandé
    local player = Player.get(username)

    -- Si il n'existe pas
    if player == nil then

        -- Non trouvé
        server:sendTwitch("Le joueur " .. username .. " n'a pas été trouvé!")

    -- Sinon si le joueur est déjà admin
    elseif player:getAdmin() then

        -- On envoie un message à twitch
        server:sendTwitch("Le joueur " .. player:getTwitchUsername() .. " est déjà admin!")

    -- Sinon
    else

        -- On le passe admin
        player:setAdmin(true)

        -- On envoie un message à twitch
        server:sendTwitch("Le joueur " .. player:getTwitchUsername() .. " est maintenant admin!")

    end

end

--------------------------------------------------------------------
--
-- Commande !brioche deop <Pseudo Twitch>
--
-- senderNickname: Le pseudo de l'auteur de la commande
-- senderPlayer: L'objet Player associé à cet auteur (Ou nil)
-- twitchUsername: Le pseudo twitch du joueur à déop
--
--------------------------------------------------------------------

function briocheDeop(senderNickname, senderPlayer, twitchUsername)

    -- On récupère le joueur demandé
    local player = Player.get(twitchUsername)

    -- Si il n'existe pas
    if player == nil then

        -- Non trouvé
        server:sendTwitch("Le joueur " .. twitchUsername .. " n'a pas été trouvé!")

    -- Sinon si le joueur est l'envoyeur
    elseif senderPlayer:is(twitchUsername) then

        -- Pas possible
        server:sendTwitch("Vous ne pouvez pas vous déop vous même!")

    -- Sinon
    else

        -- On lui enlève son statut d'admin
        player:setAdmin(false)

        -- On envoie un message à twitch
        server:sendTwitch("Le joueur " .. player:getTwitchUsername() .. " n'est plus admin!")

    end

end

--------------------------------------------------------------------
--
-- Commande !brioche skin <Pseudo Twitch>
--
-- senderNickname: Le pseudo de l'auteur de la commande
-- senderPlayer: L'objet Player associé à cet auteur (Ou nil)
-- twitchUsername: Le pseudo twitch du joueur dont on veut le skin
--
--------------------------------------------------------------------

function briocheSkin(senderNickname, senderPlayer, twitchUsername)

    -- Variable temporaire pour le joueur
    local player = nil;

    -- Un argument
    if twitchUsername ~= nil then

        -- On récupère le joueur correspondant
        player = Player.get(twitchUsername)

    -- Aucun argument
    else

        -- On récupère le streamer actuel
        player = server:getCurrentStreamer()

    end

    -- Un pseudo twitch précisé mais le joueur n'est pas trouvé
    if twitchUsername ~= nil and player == nil then

        -- Erreur
        server:sendTwitch("Joueur non trouvé!")

	-- Pas de pseudo twitch précisé mais le joueur n'existe pas non plus
	elseif player == nil then

		-- Pas de streamer
		server:sendTwitch("Aucun streamer actuellement")

    -- Pas de skin
    elseif player:getOsuSkin() == "null" then

        -- Erreur
        server:sendTwitch("Le joueur " .. player:getTwitchUsername() .. " n'a pas défini de skin!")

    -- Tout est bon
    else

        -- On envoie le skin
        server:sendTwitch("Skin de " .. player:getTwitchUsername() .. ": " .. player:getOsuSkin())

    end

end

--------------------------------------------------------------------
--
-- Commande !brioche setskin <Pseudo Twitch>
--
-- senderNickname: Le pseudo de l'auteur de la commande
-- senderPlayer: L'objet Player associé à cet auteur (Ou nil)
-- osuSkin: Le nouveau skin du joueur
--
--------------------------------------------------------------------

function briocheSetSkin(senderNickname, senderPlayer, osuSkin)

    -- On change son skin
    senderPlayer:setOsuSkin(osuSkin)

    -- Et on indique la mise à jour
    server:sendTwitch("Skin de " .. senderPlayer:getTwitchUsername() .. " mis à jour!")

end

function adminBriocheSetSkin(senderNickname, senderPlayer, twitchUsername, osuSkin)

    -- Si on a deux arguments et non un seul
    if osuSkin ~= nil then

        -- On récupère le joueur donné
        local player = Player.get(twitchUsername)

        -- Si le joueur n'existe pas
        if player == nil then

            -- Erreur
            server:sendTwitch("Le joueur " .. twitchUsername .. " n'existe pas")

        -- Sinon
        else

            -- On change son skin
            player:setOsuSkin(osuSkin)

            -- Et on indique la mise à jour
            server:sendTwitch("Skin de " .. player:getTwitchUsername() .. " mis à jour!")

        end

    -- Sinon
    else

        -- On appelle l'autre commande
        briocheSetSkin(senderNickname, senderPlayer, twitchUsername)

    end

end

--------------------------------------------------------------------
--
-- Commande !brioche stream <Pseudo Twitch>
--
-- senderNickname: Le pseudo de l'auteur de la commande
-- senderPlayer: L'objet Player associé à cet auteur (Ou nil)
--
--------------------------------------------------------------------

function briocheStream(senderNickname, senderPlayer)

    -- On le définit en tant que streameur
    server:setCurrentStreamer(senderPlayer)

    -- On envoie un message à twitch
    server:sendTwitch("Streameur actuel: " .. senderPlayer:getTwitchUsername())

end

function adminBriocheStream(senderNickname, senderPlayer, twitchUsername)

    -- Si on a un pseudo twitch
    if twitchUsername ~= nil then

        -- On récupère le joueur
        local player = Player.get(twitchUsername)

        -- Si le joueur n'existe pas
        if player == nil then

            -- Erreur
            server:sendTwitch("Le joueur " .. twitchUsername .. " n'existe pas")

        -- Sinon
        else

            -- On le définit en tant que streameur
            server:setCurrentStreamer(player)

            -- On envoie un message à twitch
            server:sendTwitch("Streameur actuel: " .. player:getTwitchUsername())
        end

    -- Sinon
    else

        -- On appelle la fonction normale
        briocheStream(senderNickname, senderPlayer)

    end

end

--------------------------------------------------------------------
--
-- Commande !brioche stop
--
-- senderNickname: Le pseudo de l'auteur de la commande
-- senderPlayer: L'objet Player associé à cet auteur (Ou nil)
--
--------------------------------------------------------------------

function briocheStop(senderNickname, senderPlayer)

    -- On récupère le streamer actuel
    local player = server:getCurrentStreamer()

    -- Personne
    if player == nil then

        -- Erreur
        server:sendTwitch("Aucun streamer actuellement")

    -- Quelqu'un
    else

        -- Si le joueur est admin
        if senderPlayer:getAdmin() then

            -- On récupère son temps de stream
            local hours, minutes, seconds = Utils.getTime(server:streamTime())

            -- On envoie un petit message à Twitch
            server:sendTwitch("Stream de " .. player:getTwitchUsername() .. " arrêté après " .. hours .. " heure(s) " .. minutes .. " minute(s) " .. seconds .. " seconde(s)");

            -- On vire le streamer actuel
            server:setCurrentStreamer(nil)

        -- Sinon
        else

            -- Si le streamer actuel est le joueur qui a fait la commande
            if server:getCurrentStreamer():is(senderNickname) then

                -- On récupère son temps de stream
                local hours, minutes, seconds = Utils.getTime(server:streamTime())

                -- On envoie un petit message à Twitch
                server:sendTwitch("Stream de " .. player:getTwitchUsername() .. " arrêté après " .. hours .. " heure(s) " .. minutes .. " minute(s) " .. seconds .. " seconde(s)");

                -- On vire le streamer actuel
                server:setCurrentStreamer(nil)

            end

        end

    end

end

--------------------------------------------------------------------
--
-- Commande !brioche restart
--
-- senderNickname: Le pseudo de l'auteur de la commande
-- senderPlayer: L'objet Player associé à cet auteur (Ou nil)
--
--------------------------------------------------------------------

function briocheRestart(senderNickname, senderPlayer)

    -- On stoppe le serveur (Il sera relancé automatiquement)
    server:stop()

end

--------------------------------------------------------------------
--
-- Callback
--
--------------------------------------------------------------------

-- La table des commandes accessibles aux joueurs admins
local adminCommands =
{
    add = briocheAdd,
    del = briocheDel,
    edit = briocheEdit,
    op = briocheOp,
    deop = briocheDeop,
    skin = briocheSkin,
    setskin = adminBriocheSetSkin,
    stream = adminBriocheStream,
    stop = briocheStop,
    restart = briocheRestart
}

-- La table des commandes accessibles aux joueurs normaux
local playerCommands =
{
    skin = briocheSkin,
    setskin = briocheSetSkin,
    stream = briocheStream,
    stop = briocheStop
}

-- La table des commandes accessibles aux viewers
local viewersCommands =
{
    skin = briocheSkin
}

-- Fonction permettant d'appeller une commande présente dans la table donnée
local function callCommand(command, table, ...)

     -- On récupère le callback correspondant à cette commande dans notre table
    local callback = table[command]

    -- Si la commande n'existe pas
    if callback == nil then

        -- Erreur
        server:sendTwitch("La commande " .. command .. " n'existe pas")

    -- Sinon
    else

        -- On appelle le callback
        callback(...)

    end

end

-- Callback
function onCommand(sender, command, ...)

    -- On récupère le joueur qui a envoyé le message
    local player = Player.get(sender)

    -- Si le joueur existe
    if player ~= nil then

        -- Si il est admin
        if player:getAdmin() then

            -- On appelle la commande depuis la table d'admin
            callCommand(command, adminCommands, sender, player, ...)

        -- Sinon
        else

            -- On appelle la commande depuis la table des joueurs
            callCommand(command, playerCommands, sender, player, ...)

        end

    -- Sinon
    else

        -- On appelle la commande depuis la table des viewers
        callCommand(command, viewersCommands, sender, nil, ...)

    end

end
