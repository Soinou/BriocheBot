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
MaxArguments = 3

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

    -- Pas de joueur
    if player == nil then

        -- Erreur
        server:sendTwitch("Joueur non trouvé!")

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
    op = briocheOp,
    deop = briocheDeop,
    skin = briocheSkin,
    setskin = briocheSetSkin,
    stream = briocheStream,
    restart = briocheRestart
}

-- La table des commandes accessibles aux joueurs normaux
local playerCommands =
{
    skin = briocheSkin,
    setskin = briocheSetSkin,
    stream = briocheStream
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
